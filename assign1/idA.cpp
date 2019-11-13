#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include "Socket.h"
#include "blowfish.h"
#include "util.h"
#include "filegen.h"

using namespace std;
using namespace util;

Socket kdc;
Socket ftp;
Blowfish priv;
Blowfish session;

auth_pkt* setvars(vector<char> encoded) {
    auth_pkt* p = new auth_pkt;
    
    union convert { // a union used for serializing the long
        uint64_t l;
        char bytes[sizeof(unsigned long)];
    };

    vector<char> v_decrypted = priv.Decrypt(encoded);
    vector<char> temp;
    int i = 0;
    bool appendEncoded = false; // a flag for appending the encoded message
    for (char& c : v_decrypted) {
        if (c == (char) REQ) {
            cout << "setting session key" << endl;
            const char* sesskey = vec2Str(temp).c_str(); 
            strcpy(p->sessionkey, sesskey);
            temp.clear(); //vector<char>();
        } else if (c == (char) NONCE) {
            const char* requ = vec2Str(temp).c_str();
            strcpy(p->request, requ);
            temp.clear();
        } else if (c == (char) ENC) {
            convert con;
            memset(&con.l, 0, sizeof(uint64_t));
            cout << "uninit: " << con.l << endl;
            temp.push_back(ENC);
            for (int i = 0; i < sizeof(unsigned long); i++) {
                if (temp[i] == (char) ENC) break;
                con.bytes[i] = temp[i];
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
//#ifdef DEBUG
        cout << "KDC authenticated" << endl;
//#endif
        return true;
    } else {
//#ifdef DEBUG
        cout << "expected: " << actual.nonce << endl;
        cout << "recieved: " << recieved.nonce << endl;
//#endif
        return false;
    }
}

bool fileExist(string fname) {
    ifstream in(fname);
    return !in.fail();
}

void fileTransfer(string fname)  {
    ftp.writebytes<char>((char) FTP);
    timer encryption, send, total; 
    cout << "file transfer initiated" << endl;
    filegen fg(fname);
    fg.setChunkSize(0x3E800000);
    vector<char> v, encrypted;
    ftp.writebytes<uint64_t>((uint64_t)fg.getFileSize());
    while(!ftp.hasqueue());
    ftp.readbytes<char>();
    total.start();
    while (fg) {
        vector<char> v = fg();
        vector<char> encrypted = session.Encrypt(v);
        ftp.writedata(encrypted);
        encrypted.clear();
        encrypted.shrink_to_fit();
    }
    cout << "File transfer complete:  " << total.stop() << " seconds" << endl;
}

bool validateresponse(string r){
    if (r.length() == 1 ){
        return true;
    }
    return false;
}

void writeMessage() {
    string s;
    cout << "Enter string S: ";
    getline(cin, s);
    cout << "S converted to hex: " << str2Hex(s) << endl;
    cout << "Sending to " << ftp.getIP() << endl;
    ftp.writebytes<char>((char) MESSAGE);
    vector<char> v = str2Vec(s);
    vector<char> encrypted = session.Encrypt(v);
    ftp.writedata(encrypted);
}

int main(int argc, char* argv[]) {
    
    auth_pkt actual;

    string priv_key = "private"; 
    string sess_key = "";
    string request = "";
    string message = "Request session key for communication wirh IDb";
    kdc = Socket(argv[1], 9421);
    unsigned long nonce;
//#ifdef PROD
    cout << "Enter idA private key: ";
    getline(cin, priv_key);
    cout << "Enter Nonce: ";
    cin >> nonce;
    cin.ignore();
//#endif
    priv = Blowfish(str2Vec(priv_key));
    //unsigned long nonce = generateNonce();
    cout << "Nonce: " << nonce << endl;
    strcpy(actual.request, message.c_str());
    actual.nonce = nonce;
    kdc << message;
    kdc.writebytes<char>('\0');
    kdc.writebytes<unsigned long>(nonce);
    while(!kdc.hasqueue());
    vector<char> kdcResponse = kdc.readdata();
    auth_pkt* recieved = setvars(kdcResponse);
    if (validateKDC((*recieved), actual)) {
        cout << "recieved session key: " << recieved->sessionkey << endl;
        sess_key = recieved->sessionkey;
    } else {
        exit(1);
    }
    kdc.shut();
    session = Blowfish(str2Vec(sess_key));
    cout << "Copying FTP" << endl;
    ftp = Socket(argv[2], 9421);
    ftp.writedata(recieved->encryptedkey);
    vector<char> data = ftp.readdata();
    long fNonce = f(vec2Long(session.Decrypt(data)));
    vector<char> lovec = long2Vec(fNonce);
    ftp.writedata(session.Encrypt(lovec));
    delete recieved;
    cout << "Secure connection established." << endl;
    while(true) {
        cout << "Select an option: \n\t1: send a message\n\t2: send a file\n\t3: exit\n";
        string response;
        getline(cin, response);
        while (!validateresponse(response)){
            cout << "Invalid option, please pick again:" << endl;
            getline(cin, response);
        }
        if (response == "2") {
            string fname;
            cout << "File to send: ";
            getline(cin, fname);
            while(!fileExist(fname)) {
                cout << "File doesn't exist, pick again" << endl;
                getline(cin, fname);
            }
            fileTransfer(fname);
        } else if (response == "1") {
            writeMessage();
        } else if (response == "3") {
            ftp.writebytes<char>((char) TERMINATE);
            return 0;
        } else {
            cout << "Invalid" << endl; 
        }
    }
}
