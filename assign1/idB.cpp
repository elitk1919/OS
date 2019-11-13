#define DEBUG
#include "ServerSocket.h"
#include "blowfish.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace util;

Socket s;
Blowfish priv;
Blowfish session;

string process(vector<char> v) {
    auth_pkt p;
    string temp;
    for (char &c : v) {
        if (c == (char) ID) {
            cout << temp << endl;
            return temp;
            temp = "";
        } else {
            temp.push_back(c ^ 0xFF);
        }
    }
}

void recieveFile(string fname) {
    vector<char> decrypted, out;
    uint64_t totalFileSize = s.readbytes<uint64_t>();
    s.writebytes<char>('c');
    uint64_t currentFileSize = 0;
    cout << "File transfer of size " << totalFileSize << endl;
    ofstream outfile(fname, ios::binary);
    timer t;
    t.start();
    while(currentFileSize < totalFileSize) {
        cout << "reading" << endl;
        out = s.readdata();
        decrypted = session.Decrypt(out);
        outfile.write(&decrypted[0], decrypted.size());
        currentFileSize += decrypted.size();
        cout << currentFileSize << endl;
        out.clear();
        out.shrink_to_fit();
        decrypted.clear();
        decrypted.shrink_to_fit();
    }
    outfile.close();
    cout << "Completed in " << t.stop() << "seconds" << endl;
}

int main(int argc, char* argv[]) {
    string privatekey;
    unsigned long nonce;
//#ifdef PROD
    cout << "Enter idB private key: ";
    getline(cin, privatekey);
    cout << "Enter Nonce: ";
    cin >> nonce;
    cin.ignore();
    cout << nonce << endl;
//#endif
    ServerSocket* ss = new ServerSocket(9421);
    s = ss->acceptCli();
    priv = Blowfish(str2Vec(privatekey));
    vector<char> data = s.readdata();
    vector<char> encoded = priv.Decrypt(data);
    string sessionkey = process(encoded);
    cout << "Session key: " << sessionkey << endl;
    session = Blowfish(str2Vec(sessionkey));
    vector<char> vecnonce = long2Vec(nonce); 
    s.writedata(session.Encrypt(vecnonce));
    long fNonce = f(nonce);
    while(!s.hasqueue());
    data = s.readdata();
    long recievedfNonce = vec2Long(session.Decrypt(data));
    if (recievedfNonce == fNonce) { 
        cout << "Session Authenticated" << endl;
    } else { 
        cout << "Authentication failed" << endl;
        return 0;
    }
    char c = ' ';
    while(c != (char) TERMINATE){
        c = s.readbytes<char>();
        if (c == FTP) {
            string fname;
            cout << "Output file name: ";
            getline(cin, fname);
            recieveFile(fname);
        } else if (c == MESSAGE) {
            vector<char> encMessage = s.readdata();
            cout << "Encrypted message: " << str2Hex(vec2Str(encMessage)) << endl;
            vector<char> decryptedMessage = session.Decrypt(encMessage);
            cout << "Messsage recieved: " << endl;
            cout << "\t" << str2Hex(vec2Str(decryptedMessage)) << " (hex)" << endl;
            cout << "\t" << vec2Str(decryptedMessage) << " (plaintext)" << endl;
        } else if (c == (char) TERMINATE) {
            s.shut();
            return 0;
        }
    }
}
