#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <inttypes.h>
#include <string.h>

#define MSG_SIZE       4096

// This handler will be called when the queue 
// becomes non-empty.

void handler (int sig_num) {
    printf ("Received sig %d.\n", sig_num);
}
    
void main (int argc, char **argv) {

  if (argc != 2) {
    printf("The message to post must be passed.\n usage: %s <message>", argv[0]);
    _Exit(EXIT_FAILURE);
  }

  struct mq_attr attr, old_attr;   // To store queue attributes
  struct sigevent sigevent;        // For notification
  mqd_t mqdes;             // Message queue descriptors
  char buf[MSG_SIZE];              // A good-sized buffer
  unsigned int prio;               // Priority 
  mode_t mode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
  // First we need to set up the attribute structure
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;

  // Open a queue with the attribute structure
  mqdes = mq_open ("/qtest", O_RDWR | O_CREAT, 
                   mode, &attr);
    
  // Get the attributes for the test queue
  mq_getattr (mqdes, &attr);
  printf ("%"PRIu64" messages are currently on the queue.\n", 
          attr.mq_curmsgs);

  // We want to be notified when something is there 
  signal (SIGUSR1, handler);
  sigevent.sigev_signo = SIGUSR1;
  
  // Write the message
  if (mq_send(mqdes, argv[1], strlen(argv[1]), 0) == -1) {
    perror("mqsend() failed.");
  }
  
  // Close all open message queue descriptors    
  mq_close (mqdes);
}
