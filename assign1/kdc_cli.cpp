#include<iostream>
#include"blowfish.h"
#include<string>
#include"Socket.h"
#include"util.h"

using namespace std;
using namespace util;

// CLIENT!!!

int main(int argc, char* argv[]) {
        BLOWFISH bf(str2Hex(COMMON_KEY));
        Socket s(argv[1], 9420);
        cout << "requesting Ks for (IDb)" << endl;
        string encryptNonce = s.readstring();
        cout << "Received encryptNonce: " << encryptNonce << endl;
        string strNonce = bf.Decrypt_CBC(encryptNonce);
        cout << "Decrypted nonce: " << strNonce << endl;
        // random key stuff
        int keyLength = 16;
        string firstKey = random_string(keyLength);
        string secondKey = random_string(keyLength);
        string sessionKey = random_string(keyLength);
        cout << "First Key: " << firstKey << endl;
        cout << "Second Key: " << secondKey << endl;
        cout << "Session Key: " << sessionKey << endl;
        // So I think as far as encrypting goes you can just use another 'BLOWFISH' like above and 
        // replace 'COMMON_KEY' with one of the randomly generated keys we just made.


}
