CC=g++
CFLAGS=--std=c++11

server: serv_main.cpp blowfish.o util.o ServerSocket.o
	${CC} ${CFLAGS} serv_main.cpp blowfish.o ServerSocket.o util.o Socket.o -o server

client: cli_main.cpp blowfish.o util.o Socket.o
	${CC} ${CFLAGS} cli_main.cpp blowfish.o Socket.o util.o -o client

ft_server: ft_serv.cpp ServerSocket.o Socket.o
	${CC} ${CFLAGS} ft_serv.cpp ServerSocker.o Socket.o -o ft_server	

ft_client: ft_cli.cpp Socket.o
	${CC} ${CFLAGS} ft_cli.cpp Socket.o -o ft_client

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
