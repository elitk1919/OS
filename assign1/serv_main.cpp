#include "blowfish.h"
#include "ServerSocket.h"
//#include "Socket.h"
#include <iostream>
#include <string>

using namespace std;

string str2Hex(string input) {

    static const char* const key = "0123456789ABCDEF";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(key[c >> 4]);
        output.push_back(key[c & 15]);
    }
    return output;
}


int main(int argc, char* argv[]) {
	BLOWFISH bf(str2Hex(argv[1]));
	ServerSocket ss(6969);
	string message = bf.Encrypt_CBC("hello there");
	Socket s = ss.acceptCli();
	s << message;
	
}
