#include <sys/socket.h>
#include "Socket.h"
#include <string>
#include <fstream>

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
    ioctl(fd, FIONREAD, &trash);
	//return recv(this->fd, &trash, 1024, MSG_PEEK | MSG_DONTWAIT);
    return trash;
}

bool Socket::hasqueue() {
    return !this->queue() == 0;
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
    char c = ' ';
    int i = 0;
    //ile (c != '\0') {
	int res = recv(this->fd, buf, 1024, 0);
    //buf[i] = c;
	if (res < 0) throw std::runtime_error("Error on read");
    //i++; 
    //}
	std::string s = buf;
    //std::cout << s.size() << std::endl;
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
    //write(this->fd, '\0', 1);
	return *this;
}

bool Socket::writeFile(std::string fname) {
    enum sock_stat {
        RECV_READY,
        SEND_WRITING,
        SEND_WAITING
    };

    std::ifstream inFile;
    inFile.open(fname, std::ios::binary);
    inFile.seekg(0, std::ios::end); // go to end of file got byte count
    uint16_t fsize = inFile.tellg();
    inFile.seekg(0, std::ios::beg); // to back to beginning
    
    this->writebytes<uint16_t>(fsize);
    if (this->readbytes<uint8_t>() != RECV_READY)
        return false;
    
    char* bytes = new char[fsize];
    inFile.read(bytes, fsize);

    int outof = fsize / WRITE_SIZE;
    int sent = 0;
    if (fsize % WRITE_SIZE != 0) outof++;

    for (int i = 0; i < fsize; i++) {
        if (i % WRITE_SIZE == 0) {
            int size = 0;
            if (fsize - i < WRITE_SIZE) {
                size = fsize % WRITE_SIZE;
            } else {
                size = WRITE_SIZE;
            }
#ifdef DEBUG
            std::cout << "sending pkt" << sent << " size " << size << std::endl;
#endif
            char chunk[size];
            memcpy(&chunk, &bytes[i], size);
            std::cout << chunk << std::endl;
            send(this->fd, &chunk, size, 0);
            if (sent == 10) {
                while (!this->hasqueue());
                uint8_t cli_res = this->readbytes<uint8_t>();
            }
            sent++;

        }
        //while(!this->hasqueue());
        //uint8_t cli_resp = this->readbytes<uint8_t>();
    }

}

void Socket::readFile(std::string fname) {
    enum sock_stat {
        RECV_READY,
        SEND_WRITING,
        SEND_WAITING,
        TRANSFER_COMPLETE
    };

    uint16_t init = this->readbytes<uint16_t>();

    int fsize = init;

    uint8_t* arr = new uint8_t[fsize];
    
#ifdef DEBUG 
    std::cout << "Expecting " << fsize << std::endl; 
#endif
    this->writebytes<uint8_t>(RECV_READY);
#ifdef DEBUG
    std::cout << "handshake established" << std::endl;
#endif
    bool allsent = false;
    //int arri = 0;
    int bytes_recvd = 0;
    while(!allsent) {
        while(this->hasqueue()) {
            uint8_t c = 0;
            recv(this->fd, &c, 1, 0);
            std::cout << c << std::endl; 
            arr[bytes_recvd] = c;
            bytes_recvd++;
            //arri += strlen(p.data);
            if (bytes_recvd == fsize - 1) {
                allsent = true;
            }
        }
        std::cout << "sender has no queue" << std::endl;
        this->writebytes<uint8_t>(RECV_READY);
        while (!allsent && !this->hasqueue());
    }
    //this->writebytes<uint8_t>(TRANSFER_COMPLETE);
    std::cout << arr;
    std::cout << "all packets sent" << std::endl;
    std::ofstream out;
    out.open(fname, std::ios::binary);
    //out << arr;
    out.write((char*)arr, bytes_recvd);
    out.close();

}

Socket::~Socket(){
	close(this->fd);
}

