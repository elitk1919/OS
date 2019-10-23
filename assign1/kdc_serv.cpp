#include "ServerSocket.h"
#include "blowfish.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;
using namespace util;

// SERVER!!!

int main (int argc, char* argv[]) {
        BLOWFISH bf(str2Hex(COMMON_KEY));
        ServerSocket ss(9420);
        Socket so = ss.acceptCli();
        cout << "Request:" << endl;
        cout << "Ks for (IDb)" << endl;
        long tempNonce = 5647892341;
        long nonce = f(tempNonce); // THIS IS ONLY RETURNING 126952.....
        string strNonce = to_string(nonce);
        cout << "N1 = " << strNonce << endl;
        string encryptNonce = bf.Encrypt_CBC(strNonce, sizeof(long));
        cout << "Encrypted Nonce: " << encryptNonce << endl;
        so << encryptNonce;
}  
