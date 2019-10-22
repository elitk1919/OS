CC=g++
CFLAGS=--std=c++11

server: serv_main.cpp blowfish.o util.o ServerSocket.o
	${CC} ${CFLAGS} serv_main.cpp blowfish.o ServerSocket.o util.o Socket.o -o server

client: cli_main.cpp blowfish.o util.o Socket.o
	${CC} ${CFLAGS} cli_main.cpp blowfish.o Socket.o util.o -o client

main: main.cpp blowfish.o
	${CC} ${CFLAGS} main.cpp blowfish.o -o main

socket_test: socket_test.cpp ServerSocket.o Socket.o
	${CC} ${CFLAGS} socket_test.cpp ServerSocket.o Socket.o -o socket_test

kdcServ: kdc_serv.cpp util.o ServerSocket.o blowfish.o
        ${CC} ${CFLAGS} kdc_serv.cpp ServerSocket.o util.o Socket.o blowfish.o -o kdcServ

kdcCli: kdc_cli.cpp Socket.o blowfish.o util.o
	${CC} ${CFLAGS} kdc_cli.cpp Socket.o util.o blowfish.o -o kdcCli

util.o: util.cpp util.h
	${CC} ${CFLAGS} -c util.cpp

blowfish.o: blowfish.cpp blowfish.h
	${CC} ${CFLAGS} -c blowfish.cpp

ServerSocket.o: ServerSocket.cpp ServerSocket.h Socket.o
	${CC} ${CFLAGS} -c ServerSocket.cpp Socket.o

Socket.o: Socket.cpp Socket.h
	${CC} ${CFLAGS} -c Socket.cpp

clean: 
	rm *.o 
