#include "blowfish.h"
#include "ServerSocket.h"
//#include "Socket.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;
using namespace util;


int main(int argc, char* argv[]) {
	BLOWFISH bf(str2Hex(argv[1]));
	ServerSocket ss(6969);
	string message = bf.Encrypt_CBC("hello there");
	Socket s = ss.acceptCli();
	s << message;
	
}
