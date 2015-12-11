OUTDIR="${PUSHOVER_ROOT:='.'}/bin"

build: ./src/qserver.c ./src/qclient.c
	gcc ./src/qserver.c -o server -lrt
	gcc ./src/qclient.c -o client -lrt
install: build
	mkdir -p ${OUTDIR}
	mv server ${OUTDIR}/.
	mv client ${OUTDIR}/.
	chmod +x ${OUTDIR}/server
	chmod +x ${OUTDIR}/client
clean:
	rm -f server
	rm -f client
	rm -f ${OUTDIR}/server
	rm -f ${OUTDIR}/client
test: ./src/test.c
	gcc ./src/test.c -o test -lrt
	#sh test
