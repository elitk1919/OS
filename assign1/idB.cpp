#include "ServerSocket.h"
#include "blowfish.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace util;

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
    //return temp;
}

int main(int argc, char* argv[]) {
    string privatekey = "bsprivatekey";
#ifdef PROD
    cout << "Emter idB private key: "
    getline(con, privatekey);
#endif
    ServerSocket* ss = new ServerSocket(9421);
    Socket s = ss->acceptCli();
    Blowfish priv(str2Vec(privatekey));
    vector<char> encoded = priv.Decrypt(s.readdata());
    string sessionkey = process(encoded);
    cout << "\n\n\t\tSession: " << sessionkey << endl;
    Blowfish sess(str2Vec(sessionkey));
    unsigned long nonce = generateNonce();
    s.writedata(sess.Encrypt(long2Vec(nonce)));
    long fNonce = f(nonce);
    while(!s.hasqueue());
    long recievedfNonce = vec2Long(sess.Decrypt(s.readdata()));
    if (recievedfNonce == fNonce) cout << "authenticated" << endl;
    //while(!s.hasqueue());
    vector<char> out = sess.Decrypt(s.readdata());
    //bf_private.SetIV(nonce);
    vec2File(out, "/tmp/1g");
}
