#include <string>
#include <iostream>
#include "blowfish.h"
#include "Socket.h"
#include "util.h"

using namespace std;
using namespace util;

int main(int argc, char* argv[]) {
	BLOWFISH bf(str2Hex(COMMON_KEY));
	Socket s(argv[1], 9420);
	cout << "Recieve encrypted";
	cout << bf.Decrypt_CBC(s.readstring()) << endl;
}
