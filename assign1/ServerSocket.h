#include "Socket.h"

class ServerSocket{
	struct sockaddr_in addr;
	int fd;
public:
	ServerSocket(int portno);
	~ServerSocket();
	Socket acceptCli();
};
