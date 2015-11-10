
build: qserver.c
	gcc qserver.c -o qserver -lrt
	gcc qclient.c -o qclient -lrt
clean:
	rm -f qserver
	rm -f qclient
