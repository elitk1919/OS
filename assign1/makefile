MAKEFLAGS += -r

CC=g++
CFLAGS=--std=c++11

util.o: util.cpp util.h
	${CC} ${CFLAGS} -c util.cpp

blowfish.o: blowfish.cpp blowfish.h
	${CC} ${CFLAGS} -c blowfish.cpp -fopenmp -O3

ServerSocket.o: ServerSocket.cpp ServerSocket.h Socket.o util.o
	${CC} ${CFLAGS} -c ServerSocket.cpp Socket.o

Socket.o: Socket.cpp Socket.h
	${CC} ${CFLAGS} -c Socket.cpp

all: kdc idA idB

kdc: kdc.cpp util.o ServerSocket.o Socket.cpp blowfish.o
	${CC} ${CFLAGS} -fopenmp kdc.cpp blowfish.o ServerSocket.o Socket.o util.o -o kdc

idA: idA.cpp Socket.o util.o blowfish.o filegen.o
	${CC} ${CFLAGS} -fopenmp idA.cpp Socket.o util.o blowfish.o filegen.o -o idA

idB: idB.cpp ServerSocket.o Socket.o util.o blowfish.o
	${CC} ${CFLAGS} -fopenmp idB.cpp ServerSocket.o Socket.o util.o blowfish.o -o idB 

filegen.o: filegen.cpp filegen.h
	${CC} ${CFLAGS} -c filegen.cpp

clean: 
	rm *.o

/tmp/2G:
	dd if=/dev/urandom of=/tmp/2G bs=500M count=4 iflag=fullblock

/tmp/4G:
	dd if=/dev/urandom of=/tmp/4G bs=500M count=8 iflag=fullblock

/tmp/8G:
	dd if=/dev/urandom of=/tmp/8G bs=500M count=16 iflag=fullblock

/tmp/16G:
	dd if=/dev/urandom of=/tmp/16G bs=500M count=32 iflag=fullblock

/tmp/32G:
	dd if=/dev/urandom of=/tmp/32G bs=500M count=64 iflag=fullblock

/tmp/64G:
	dd if=/dev/urandom of=/tmp/64G bs=500M count=128 iflag=fullblock

%g: /tmp/%G
	
randfiles: 4g 8g 16g 32g

rm%g:
	rm -f /tmp/$*G
	
rmrandfiles: rm4g rm8g rm16g rm32g	 
