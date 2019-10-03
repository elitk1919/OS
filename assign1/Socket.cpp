#include <sys/socket.h>
#include "Socket.h"
#include <string>

bool Socket::connected() {
	char trash = 't';
	if (recv(this->fd, &trash, 1, MSG_PEEK | MSG_DONTWAIT) == -1){
		if (errno == ENOTCONN) return false;
		else return true;
	}
}

Socket::Socket(int sock, sockaddr_in add){ //constructor for server socket accept
	this->fd = sock;
	this->addr = add;
}

Socket::Socket(const Socket& s){
	this->fd = s.fd;
	
}

Socket::Socket(char* host, int portno) {
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd < 0){ 
		throw std::runtime_error("Error opening socket");
	} else {
		memset(&this->addr, '0', sizeof(this->addr));
		this->addr.sin_family = AF_INET;
		this->addr.sin_port = htons(portno);
		struct hostent* server;
		server = gethostbyname(host);
		if (server == NULL) { 
			throw std::runtime_error("Error finding host");
		} 
		memcpy(&this->addr.sin_addr.s_addr, server->h_addr, server->h_length);
		if (connect(this->fd, (sockaddr *) &this->addr, sizeof(this->addr)) < 0){
			throw std::runtime_error("Error connecting to host");
		}
	}
}

int Socket::queue(){
	int trash = 0;
	return recv(this->fd, &trash, 1, MSG_PEEK | MSG_DONTWAIT);
}

int Socket::getfd(){
	return this->fd;
}

std::string Socket::getIP(){ // returns the client IP address 
	using namespace std;
	union convert{
		unsigned long addr;
		uint8_t nums[4];
	};
	union convert c;
	c.addr = this->addr.sin_addr.s_addr;
	return to_string(c.nums[0]) + "." + to_string(c.nums[1]) + "." + to_string(c.nums[2]) + "." + to_string(c.nums[3]);
}

std::string Socket::readstring(){
	char buf[1024];
	memset(buf, '\0', 1024);
	int res = recv(this->fd, buf, 1024, 0);
	if (res < 0) throw std::runtime_error("Error on read");
	std::string s = buf;
	return s;
}

void Socket::setstatus(status s){ //ends OOB status message
	int res = send(this->fd, &s, 1, MSG_OOB);
	if (res < 0) throw std::runtime_error("Error setting status");
}

Socket::status Socket::getstatus(){ //for recieving an OOB status message, can be send while the stream is full with priority
	unsigned char buf;
	int res = recv(this->fd, &buf, 1, MSG_OOB);
	if (res < 0) return Socket::NOSTATUS;
	return (Socket::status) buf;
}

Socket& Socket::operator<< (std::string s){ // for writing strings to the socket
	write(this->fd, s.c_str(), s.length());
	return *this;
}

Socket::~Socket(){
	close(this->fd);
}

