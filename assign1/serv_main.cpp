#include "blowfish.h"
#include "ServerSocket.h"
//#include "Socket.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;
using namespace util;


int main(int argc, char* argv[]) {
	BLOWFISH bf(str2Hex(COMMON_KEY));
	ServerSocket ss(6969);
	cout << "Enter string S:" << endl;
	string s;
	s << cin;
	cin.ignore();
	string hex = str2Hex(s);
	string message = bf.Encrypt_CBC(hex);
	cout << "S encrypted:" << endl;	
	Socket s = ss.acceptCli();
	s << message;
	
}
