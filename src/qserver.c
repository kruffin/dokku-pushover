#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <inttypes.h>

#define MSG_SIZE       4096

// This handler will be called when the queue 
// becomes non-empty.

void handler (int sig_num) {
    printf ("Received sig %d.\n", sig_num);
}
    
void main () {

  struct mq_attr attr, old_attr;   // To store queue attributes
  struct sigevent sigevent;        // For notification
  mqd_t mqdes, mqdes2;             // Message queue descriptors
  char buf[MSG_SIZE];              // A good-sized buffer
  unsigned int prio;               // Priority 
    
  // First we need to set up the attribute structure
  attr.mq_maxmsg = 300;
  attr.mq_msgsize = MSG_SIZE;
  attr.mq_flags = 0;

  // Open a queue with the attribute structure
  mqdes = mq_open ("/qtest", O_RDWR | O_CREAT, 
                   0664, &attr);
    
  // Get the attributes for the test queue
  mq_getattr (mqdes, &attr);
  printf ("%"PRIu64" messages are currently on the queue.\n", 
          attr.mq_curmsgs);

  if (attr.mq_curmsgs != 0) {
    
    // There are some messages on this queue....eat em
    
    // First set the queue to not block any calls    
    attr.mq_flags = O_NONBLOCK; //MQ_NONBLOCK;
    mq_setattr (mqdes, &attr, &old_attr);    
        
    // Now eat all of the messages
    while (mq_receive (mqdes, &buf[0], MSG_SIZE, &prio) != -1) 
      printf ("Received a message with priority %d: %s.\n", prio, buf);
            
    // The call failed.  Make sure errno is EAGAIN
    if (errno != EAGAIN) { 
      perror ("mq_receive()");
      _Exit (EXIT_FAILURE);
    }
        
    // Now restore the attributes
    mq_setattr (mqdes, &old_attr, 0);            
  }
    
  // We want to be notified when something is there 
  signal (SIGUSR1, handler);
  sigevent.sigev_signo = SIGUSR1;
    
  if (mq_notify (mqdes, &sigevent) == -1) {
    if (errno == EBUSY) 
      printf (
        "Another process has registered for notification.\n");
    _Exit (EXIT_FAILURE);
  }
    
  // Close all open message queue descriptors    
  mq_close (mqdes);
}
