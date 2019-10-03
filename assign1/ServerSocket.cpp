#include "ServerSocket.h"

ServerSocket::ServerSocket(int portno) {
	memset(&this->addr, '0', sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->addr.sin_port = htons(portno);
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(this->fd, (sockaddr *) &this->addr, sizeof(this->addr)) < 0) {
		throw std::runtime_error("Error binding to port. Maybe it's in use");		
	}
}
ServerSocket::~ServerSocket(){
	close(this->fd);
}

Socket ServerSocket::acceptCli() {

	listen(this->fd, 1);
	struct sockaddr_in cli;
	int cliInt;
	socklen_t clilen = sizeof(cli);
	cliInt = accept(this->fd, (struct sockaddr *) &cli, &clilen);
	if (cliInt < 0) throw std::runtime_error("Error excepting client");
	else return Socket(cliInt, cli); 	
}
