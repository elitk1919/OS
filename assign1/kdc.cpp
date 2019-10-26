#include "ServerSocket.h"
#include "blowfish.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;
using namespace util;

// SERVER!!!

string parseRequest(Socket* s) {
    string request = "";
    char c = ' ';
    while (c != '\0') {
        c = s->readbytes<char>();
        request += c;
    }
    return request;
}

vector<char> createResponse(Blowfish a, Blowfish b, 
                                    string sess, string request, 
                                    unsigned long non) {
    union nonce_convert {
        uint64_t l;
        char dat[sizeof(unsigned long)];
    };

    vector<char> vec;
    vector<char> vec2; // used for B's encrypted message  
    //int index = 0;
    for (char& c : sess) {
        //cout << c;
        c = c ^ 0xFF;
        vec.push_back(c);
        vec2.push_back(c);
    }
    vec.push_back((char) REQ);
    
    vec2.push_back((char) ID);
    vec2.push_back('A' ^ 0xFF);

    vec2 = b.Encrypt(vec2);
    
    for (char& c : request) { 
        c = c ^ 0xFF;
        vec.push_back(c);
    }
    vec.push_back((char) NONCE);

    nonce_convert nc;
    nc.l = (uint64_t) non;
    cout << "here" << non << " " << nc.l << endl;
    for (int i = 0; i < sizeof(unsigned long); i++) {
        if (nc.dat[i] == '\0') {
             break;
        }
        vec.push_back(nc.dat[i] ^ 0XFF);
        cout << "I: " << i << " " << nc.dat[i] << endl;
        
    }
    vec.push_back((char) ENC);
    for (char& c : vec2) {
        vec.push_back(c ^ 0xFF);   
    }
    //ut << "Decrypted: " << decB << endl;
    return a.Encrypt(vec);
    //cout << enc << endl;
    //string encA = "";
    //cout << "FUUUCK " << a.Decrypt_CBC(enc) << endl;
    //for (int i = 0; i < vec.size(); i++) 
    //    encA[i] = vec[i];

    //cout << enc.length() << endl;
    //return enc;
}

int main (int argc, char* argv[]) {
    ServerSocket ss(9421);
    Socket* so = new Socket(ss.acceptCli());
#ifdef PROD
    string sess_key, idA_key, idB_key;
    cout << "Enter session key: ";
    getline(cin, sess_key);
    cout << "Enter idA key: ";
    getline(cin, idA_key);
    cout << "enter idB key: ";
    getline(cin, idB_key);
#else
    string sess_key = "session";
    string idA_key = "private";
    string idB_key = "bsprivatekey";
#endif
    while(!so->hasqueue());
    string req = parseRequest(so);
    //string req = "empty";
    unsigned long nonce = so->readbytes<unsigned long>();
    //unsigned long nonce = 0;
    cout << "Recieved from idA :" << endl;
    cout << "\t" << req << endl;
    cout << "\tnonce: " << nonce << endl;
    Blowfish bfA(str2Vec(idA_key));
    Blowfish bfB(str2Vec(idB_key));
    vector<char> resp = createResponse(bfA, bfB, sess_key, req, nonce);
    so->writedata(resp);
    cout << "sent" << endl;
}  
