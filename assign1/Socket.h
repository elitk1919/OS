#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <cstdint>
/**
 *	Generic TCP socket implementation
 *  Featuring:
 *	template typecasting on write and recieve
 *      OOB priority status messaging
 *      overloaded operator<< for string messages
**/
 
class Socket	{
	struct sockaddr_in addr;
	int fd;
public:
	
	typedef enum status: unsigned char { 
		NOSTATUS,
		WAIT, 
		READY
	} status;
	
	typedef struct packet {
		void *addr;
		int num;
		
	}packet;
	

	//Socket();
	Socket(int sock, sockaddr_in add);
	Socket(const Socket&);
	Socket(char* host, int portno);
	~Socket();
	std::string getIP();
	std::string readstring();
	Socket& operator<< (std::string s);
	status getstatus();
	bool connected();
	int getfd();
	void setstatus(status s);
	int queue(); //returns the priority queue size (OOB bits in buffer)

/**
 *                  Templates
 * These are implemented in the header so the methods that
 * use templates can still compile a specialized version of
 * the method to work with any datatype specified outside of 
 * this file. This allows for generic use.
 * Template implementation in a header file is
 * recomended in c++ dosumentation  
 *
**/
	
	template <typename T>
	bool writebytes(T data, int num = 1){ 
		//std::cout << "from write: " << data << std::endl;
		int res = send(this->fd, &data, sizeof(T), 0);
		//else res = send(this->fd, (void*)data, num * sizeof(T), 0); // if n is greater than 1 than data is already a pointer
		if (res < 0) return false;
		else return true;
	}

	template <typename T>
	T readbytes(int n_elements = 1){
		//std::cout << "Attempting to read " << sizeof(T) * n_elements << " bytes" << std::endl;
		T data;
		int res = recv(this->fd, &data, sizeof(T), 0);
		if (res < 0) std::cout << "Error on read" << std::endl;
		return data;
	}

};
