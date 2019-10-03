server: serv_main.cpp blowfish.o ServerSocket.o
	g++ serv_main.cpp blowfish.o ServerSocket.o Socket.o -o server

client: cli_main.cpp blowfish.o Socket.o
	g++ cli_main.cpp blowfish.o Socket.o -o client

main: main.cpp blowfish.o
	g++ main.cpp blowfish.o -o main

blowfish.o: blowfish.cpp blowfish.h
	g++ -c blowfish.cpp

ServerSocket.o: ServerSocket.cpp ServerSocket.h Socket.o
	g++ -c ServerSocket.cpp Socket.o

Socket.o: Socket.cpp Socket.h
	g++ -c Socket.cpp

clean: 
	rm *.o 
