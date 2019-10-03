CC=g++
CFLAGS=--std=c++11

server: serv_main.cpp blowfish.o util.o ServerSocket.o
	${CC} ${CFLAGS} serv_main.cpp blowfish.o ServerSocket.o util.o Socket.o -o server

client: cli_main.cpp blowfish.o util.o Socket.o
	${CC} ${CFLAGS} cli_main.cpp blowfish.o Socket.o util.o -o client

main: main.cpp blowfish.o
	${CC} ${CFLAGS} main.cpp blowfish.o -o main

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
