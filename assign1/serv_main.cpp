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
	ServerSocket ss(9420);
	Socket so = ss.acceptCli();
	cout << "Enter string S:" << endl;
	string s;
	std::getline(cin , s);
	//string hex = str2Hex(s);
	string message = bf.Encrypt_CBC(s);
	cout << "S encrypted:" << endl << message << endl;
	so << message;
	
}
