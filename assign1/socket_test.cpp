#include "ServerSocket.h"
#include <iostream>
#include <fstream>
#include <string>

#define WRITE_SIZE 256

using namespace std;

bool sendFile(Socket ss, string fname) {
    ifstream inFile;
    inFile.open(fname);
    string temp("");
    string total("");
    while (getline(inFile, temp)) {
        total += temp + '\n';
    }
    total.pop_back();
    ss << "FILE_TRANSFER_INIT";
    if (ss.readstring() != "FILE_TRANSFER_CONFIRM") return false;
    string write = "";
    for (int i = 0; i < total.size(); i++) { 
        write += total[i];
        if (i == WRITE_SIZE) { 
            ss << write;
            write = "";
        }
    }
    ss << write;
    ss << "FILE_TRANSFER_COMPLETE";
    if (ss.readstring() == "TRANSFER_END_CONFIRM") return true;
}

bool readFile(Socket s, string outfile) {
    if (s.readstring() != "FILE_TRANSFER_INIT") return false;
    else s << "FILE_TRANSFER_CONFIRM";
    string str = "";
    string f = "";
    while(str != "FILE_TRANSFER_COMPLETE") {
        str = s.readstring();
        cout << str;
        if (str != "FILE_TRANSFER_COMPLETE") f += str;
    }
    s << "TRANSFER_END_CONFIRM";
    //ofstream out;
    //out.open(outfile, ios::binary);
    cout << f << endl;
    //out.close();
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        string strr = "hello";
        if (strr != "Hello") {
            cout << "Incorrent syntax. specify client or server" << endl;
        }
        //sendFile("sample.txt");
    } else {
        string arg1 = argv[1];
        if (arg1 == "client") { 
            Socket so(argv[2], 9420);
            cout << "Client socket opened" << endl;
            readFile(so, "newFile.txt");
        }
        if (arg1 == "server") {
            cout << "ServerSocket opened" << endl;
            ServerSocket ss(9420);
            Socket so = ss.acceptCli();
            cout << "sending file..." << endl;
            sendFile(so, "decind.txt");
            cout << "file sent" << endl;
        }
    }
}
