#include "ServerSocket.h"
#include "blowfish.h"
#include <iostream>
#include <string>
#include "util.h"

using namespace std;
using namespace util;

// SERVER!!!

string parseRequest(Socket s) {
    string request = "";
    char c = ' ';
    while (c != '\0') {
        c = s.readbytes<char>();
        request += c;
    }
    return request;
}

string createResponse(BLOWFISH a, BLOWFISH b, string request, unsigned long non) {
    char* arr = 
}

int main (int argc, char* argv[]) {
    ServerSocket ss(9421);
    Socket so = ss.acceptCli();
    string sess_key, idA_key, idB_key;
    cout << "Enter session key: ";
    getline(cin, sess_key);
    cout << "Enter idA key: ";
    getline(cin, idA_key);
    cout << "enter idB key: ";
    getline(cin, idB_key);
    string req = parseRequest(so);
    unsigned long nonce = so.readbytes<unsigned long>();
    cout << "Recieved from idA :" << endl;
    cout << "\t" << req << endl;
    cout << "\tnonce: " << nonce;
    BLOWFISH bfA(str2Hex(idA_key));
    BLOWFISH bfB(str2Hex(idB_key));
    //cout << "Request: ";
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
}  
