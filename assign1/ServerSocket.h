#include "Socket.h"

class ServerSocket{
	struct sockaddr_in addr;
	int fd;
	bool tcp;
public:
	ServerSocket(int portno, proto p);
	~ServerSocket();
	Socket acceptCli();
};
