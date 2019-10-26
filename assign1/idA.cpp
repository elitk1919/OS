#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include "Socket.h"
#include "blowfish.h"
#include "util.h"

using namespace std;
using namespace util;

auth_pkt* setvars(Socket* s, Blowfish &priv) {
    auth_pkt* p = new auth_pkt;
    union convert {
        uint64_t l;
        char bytes[sizeof(unsigned long)];
    };

    vector<char> encoded = s->readdata(); 
    vector<char> v_decrypted = priv.Decrypt(encoded);
    vector<char> temp;
    int i = 0;
    bool appendEncoded = false; // a flag for appending the encoded message
    for (char& c : v_decrypted) {
        if (c == (char) REQ) {
            cout << "setting session key" << endl;
            const char* sesskey = vec2Str(temp).c_str(); 
            strcpy(p->sessionkey, sesskey);
            temp.clear();
        } else if (c == (char) NONCE) {
            const char* requ = vec2Str(temp).c_str();
            strcpy(p->request, requ);
            temp.clear();
        } else if (c == (char) ENC) {
            convert con;
            cout << "uninit: " << con.l << endl;
            temp.push_back(ENC);
            for (int i = 0; i < sizeof(unsigned long); i++) {
                if (temp[i] == (char) ENC) break;
                con.bytes[i] = temp[i];
                cout << "I: " << i << " " << temp[i] << endl;
            }
            memcpy(&p->nonce ,&con.l, sizeof(uint64_t));
            temp.clear();
            appendEncoded = true;

        } else {
            if(appendEncoded) p->encryptedkey.push_back(c ^ 0xFF);
            temp.push_back(c ^ 0xFF);
        }
    }
    return p;
}

bool validateKDC(auth_pkt recieved, auth_pkt &actual) {
    if(recieved.nonce == actual.nonce) {
#ifdef DEBUG
        cout << "KDC authenticated" << endl;
#endif
        return true;
    } else {
#ifdef DEBUG
        cout << "expected: " << actual.nonce << endl;
        cout << "recieved: " << recieved.nonce << endl;
#endif
        return false;
    }
}

int main(int argc, char* argv[]) {
    
    auth_pkt actual;

    string priv_key = "private"; 
    string sess_key = "";
    string request = "";

    Socket* kdc = new Socket(argv[1], 9421);
#ifdef PROD
    cout << "Enter idA private key: ";
    getline(cin, priv_key);
#endif
    Blowfish bf_private(str2Vec(priv_key));
    unsigned long nonce = generateNonce();
    cout << "Nonce: " << nonce << endl;
    string message = "Send session key for communication with idB";
    strcpy(actual.request, message.c_str());
    actual.nonce = nonce;
    (*kdc) << message;
    kdc->writebytes<char>('\0');
    kdc->writebytes<unsigned long>(nonce);
    while(!kdc->hasqueue());
    auth_pkt* recieved = setvars(kdc, bf_private);
    if (validateKDC((*recieved), actual)) {
        cout << "\n\n\t\trecieved session: " << recieved->sessionkey << endl;
        sess_key = recieved->sessionkey;
    } else {
        exit(1);
    }
    delete kdc;
    
    Blowfish ses(str2Vec(sess_key));
    Socket* ftp = new Socket(argv[2], 9421);
    ftp->writedata(recieved->encryptedkey);
    long fNonce = f(vec2Long(ses.Decrypt(ftp->readdata())));
    ftp->writedata(ses.Encrypt(long2Vec(fNonce)));
    cout << "serializing file..." << endl;
    vector<char> v = file2Vec("/tmp/1G");
    cout << "done" << endl << "Encrypting" << endl;
    vector<char> encv = ses.Encrypt(v);
    cout << "done" << endl;
    ftp->writedata(encv); 
    delete recieved;
}
