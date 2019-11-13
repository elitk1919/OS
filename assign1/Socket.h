//#define DEBUG
#define WRITE_SIZE 8192

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <cstdint>
#include <vector>
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
	
    struct binarystring {
        std::string data;
        int length;
    };

	typedef enum status { //Constant socket socket status 
		FTP_INIT,
        FTP_CLOSE,
		WAIT, 
		READY
	} status;
	

	Socket();
	Socket(int sock, sockaddr_in add);
	Socket(const Socket&);
	Socket(char* host, int portno);
	~Socket();
	std::string getIP();
	std::string readstring();
	Socket& operator<< (std::string s);
    Socket& operator<< (binarystring);
	//status getstatus();
	bool connected();
	int getfd();
	void setstatus(status s);
	int queue(); //returns the priority queue size (OOB bits in buffer)
    bool hasqueue();
    void writedata(std::vector<char>);
    std::vector<char> readdata();
    void shut();
    void sendbytes(char*, int);
    char* getbytes(char*, int);

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
		int res = send(this->fd, &data, sizeof(T) * num, 0);
		//else res = send(this->fd, (void*)data, num * sizeof(T), 0); // if n is greater than 1 than data is already a pointer
		if (res < 0) return false;
		else return true;
	}

	template <typename T>
	T readbytes(int num = 1){
		//std::cout << "Attempting to read " << sizeof(T) * n_elements << " bytes" << std::endl;
		T data;
		int res = recv(this->fd, &data, sizeof(T)*num, 0);
		if (res < 0) std::cout << "Error on read" << std::endl;
		return data;
	}

};
