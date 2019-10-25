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

string createResponse(Blowfish a, Blowfish b, 
                                    string sess, string request, 
                                    unsigned long non) {
    union nonce_convert {
        unsigned long l;
        char dat[sizeof(unsigned long)];
    };

    vector<char> vec;
    vector<char> vec2; // used for B's encrypted message  
    //int index = 0;
    for (char& c : sess) {
        //cout << c;
        vec.push_back(c);
        vec2.push_back(c);
    }
    vec.push_back((char) REQ);
    
    vec2.push_back((char) ID);
    vec2.push_back('A');
    
    for (char& c : request) 
        vec.push_back(c);
    vec.push_back((char) NONCE);

    nonce_convert nc;
    nc.l = non;
    for (char& c : string(nc.dat)) 
        vec.push_back(c);
    vec.push_back((char) ENC);
    
    //ut << "Decrypted: " << decB << endl;
    string enc = vec2Str(a.Encrypt(vec));
    //cout << enc << endl;
    //string encA = "";
    //cout << "FUUUCK " << a.Decrypt_CBC(enc) << endl;
    //for (int i = 0; i < vec.size(); i++) 
    //    encA[i] = vec[i];

    //cout << encA + decB << endl;
    return enc;
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
    string idB_key = "notusedyet";
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
    //bfA.SetIV(nonce);
    //BLOWFISH bfS(str2Hex(sess_key));
    string resp = createResponse(bfA, bfB, sess_key, req, nonce);
    cout << resp;
    (*so) << resp;
    cout << "sent" << endl;
    //string message = so.readstring();
    //cout << message << endl;
    //long tempNonce = 5647892341;
    //long nonce = f(tempNonce); // THIS IS ONLY RETURNING 126952.....
    //typedef union convert {
    //    long l;
    //    char c[4];
    //}convert;
    //convert con;
    //con.l = nonce;
    //string strNonce(con.c);
    //cout << "N1 = " << strNonce << endl;
    //string encryptNonce = bf.Encrypt_CBC(strNonce, sizeof(long));
    //cout << "Encrypted Nonce: " << encryptNonce << endl;
    //so << encryptNonce;
    while(true);
}  
