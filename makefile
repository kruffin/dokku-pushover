
build: ./src/qserver.c ./src/qclient.c
	gcc ./src/qserver.c -o server -lrt
	gcc ./src/qclient.c -o client -lrt
install: build
	mkdir -p ./bin
	mv server ./bin/.
	mv client ./bin/.
	chmod +x ./bin/server
	chmod +x ./bin/client
clean:
	rm -f server
	rm -f client
	rm -f ./bin/server
	rm -f ./bin/client
test: ./src/test.c
	gcc ./src/test.c -o test -lrt
	#sh test
