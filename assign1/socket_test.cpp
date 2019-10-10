#include "ServerSocket.h"
#include <iostream>
#include <string>

using namespace std;
int main(int arcg, char* argv[]) {
    string arg1 = argv[1];
    if (arg1 == "client") { 
        Socket so(argv[2], 9420);
        std::cout << so.readstring() << std::endl;
    }
    if (arg1 == "server") {
        std::cout << "here";
        ServerSocket ss(9420);
        Socket so = ss.acceptCli();
        so << "WAZZZZZZUPPPP";   
    }
}
