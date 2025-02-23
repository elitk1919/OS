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
/*
 * This function crafts the response to idA's request for a session key,
 * using enums defined in util.h to append portions of the message with 
 * status chars. To differentiate from other values equal with these chars,
 * every character is masked, except for the status chars.
 */
vector<char> createResponse(Blowfish a, Blowfish b, 
                                    string sess, string request, 
                                    unsigned long non) {
    union nonce_convert {
        uint64_t l;
        char dat[sizeof(unsigned long)];
    };

    vector<char> vec;
    vector<char> vec2; // used for B's encrypted message  
    for (char& c : sess) {
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
    for (int i = 0; i < sizeof(unsigned long); i++) {
        if (nc.dat[i] == '\0') {
             break;
        }
        vec.push_back(nc.dat[i] ^ 0XFF);
    }
    vec.push_back((char) ENC);
    for (char& c : vec2) {
        vec.push_back(c ^ 0xFF);   
    }
    return a.Encrypt(vec);
}

int main (int argc, char* argv[]) {
    ServerSocket ss(9421);
    Socket* so = new Socket(ss.acceptCli());
//#ifdef PROD
    string sess_key, idA_key, idB_key;
    cout << "Enter session key: ";
    getline(cin, sess_key);
    cout << "Enter idA key: ";
    getline(cin, idA_key);
    cout << "enter idB key: ";
    getline(cin, idB_key);
//#else
    //string sess_key = "session";
    //string idA_key = "private";
    //string idB_key = "bsprivatekey";
//#endif
    while(!so->hasqueue());
    string req = parseRequest(so);
    unsigned long nonce = so->readbytes<unsigned long>();
    cout << "Recieved from idA :" << endl;
    cout << "\t" << req << endl;
    cout << "\tnonce: " << nonce << endl;
    Blowfish bfA(str2Vec(idA_key));
    Blowfish bfB(str2Vec(idB_key));
    vector<char> resp = createResponse(bfA, bfB, sess_key, req, nonce);
    so->writedata(resp);
    cout << "sent" << endl;
}  
