#include <iostream>
#include <fstream>
#include <string>
#include "Socket.h"
#include "blowfish.h"
#include "util.h"

using namespace std;
using namespace util;

void setvars(Socket *s, Blowfish &priv, string& sess, string& req, unsigned long n) {
    string str = s->readstring();
    cout << str << endl;
    cout << vec2Str(priv.Decrypt(str2Vec(str))) << endl;
}

bool validateKDC(string message, string request, unsigned long oldnonce, unsigned long newnonce) {
    return message == request && oldnonce == newnonce;   
}

int main(int argc, char* argv[]) {
    string priv_key = "private"; 
    string sess_key = "";
    string request = "";

    Socket* kdc = new Socket(argv[1], 9421);
#ifdef PROD
    cout << "Enter idA private key: ";
    getline(cin, priv_key);
#endif
    Blowfish bf_private(str2Vec(priv_key));

    unsigned long nonce = rand();
    cout << nonce << endl;
    //bf_private.SetIV(nonce);
    string message = "Send session key for communication with idB";
    (*kdc) << message;
    kdc->writebytes<char>('\0');
    kdc->writebytes<unsigned long>(nonce);
    while(!kdc->hasqueue());
    unsigned long newnonce;
    setvars(kdc, bf_private, sess_key, request, newnonce);
    if(validateKDC(message, request, nonce, newnonce))
        cout << "Session key " << sess_key;
}
