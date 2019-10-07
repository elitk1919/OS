#include "ServerSocket.h"

ServerSocket::ServerSocket(int portno, proto p) {
	
	memset(&this->addr, '0', sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->addr.sin_port = htons(portno);
	if (p == TCP) {
		this->fd = socket(AF_INET, SOCK_STREAM, 0);
		this->tcp = true;
	} else {
		this->fd = socket(AF_INET, SOCK_DGRAM, 0);
		this->tcp = false;
	}
	if (bind(this->fd, (sockaddr *) &this->addr, sizeof(this->addr)) < 0) {
		throw std::runtime_error("Error binding to port. Maybe it's in use");		
	}
}
ServerSocket::~ServerSocket(){
	close(this->fd);
}

Socket ServerSocket::acceptCli() {
	struct sockaddr_in cli;
	int cliInt;
	socklen_t clilen = sizeof(cli);
	if (this->tcp){
		listen(this->fd, 1);
		clilen = sizeof(cli);
		cliInt = accept(this->fd, (struct sockaddr *) &cli, &clilen);
		if (cliInt < 0) throw std::runtime_error("Error accepting client");
		else return Socket(cliInt, cli);
	} else { // UDP
		uint16_t p_size; //Client will send preferred packet size 

		cliInt = recvfrom(this->fd, &p_size, sizeof(uint16_t), 
			MSG_WAITALL, (struct sockaddr*) &cli, &clilen
		);
		std::cout << p_size << std::endl;
		const char* confirm = "confirm";

		sendto(this->fd, confirm, 
			7, MSG_CONFIRM, 
			(struct sockaddr*) &cli, sizeof(cli)
		);

		Socket udp_sock(this->fd, cli);
		udp_sock.udp_p_size = p_size;
		std::cout << udp_sock.getIP() << std::endl;
		return udp_sock;
	} 	
}


