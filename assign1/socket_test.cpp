#define DEBUG

#include "ServerSocket.h"
#include <iostream>
#include <fstream>
#include <string>

//#define WRITE_SIZE 256

using namespace std;

typedef struct f_pkt {
    int ind;
    char data[WRITE_SIZE];
    int total;
} f_pkt;
/*
bool sendFile(Socket ss, string fname) {
    ifstream inFile;
    inFile.open(fname, ios::binary);
    //string temp("");
    //string total("");
    inFile.seekg(0, ios::end);
    int fsize = inFile.tellg();
#ifdef DEBUG
    cout << "Sending file size " << fsize << endl;
#endif
    f_pkt init;
    init.total = fsize;
    ss.writebytes<f_pkt>(init);
    inFile.seekg(0, ios::beg);
    char* c_total = new char[fsize];
    inFile.read(c_total, fsize);
    int outof = fsize / WRITE_SIZE;  
    if (fsize % WRITE_SIZE != 0) outof++;
    f_pkt f;
    int sent = 0;
    for (int i = 0; i < fsize; i++) { 
        if (i % WRITE_SIZE == 0) {
            memset(&f, '\0', sizeof(f_pkt)); 
            f.ind = i / WRITE_SIZE;
            f.total = outof;
            int size = 0;
            if (fsize - i < WRITE_SIZE) {
                size = fsize % WRITE_SIZE;
            } else {
                size = WRITE_SIZE;
            }
            memcpy(&f.data, &c_total[i], size);
            //data = write.data();
#ifdef DEBUG
            cout << "Sending " << f.ind << "out of " << f.total << endl;
#endif
            ss.writebytes<f_pkt>(f);
            sent++;   
        }
        if (sent == 10){
            while (!ss.hasqueue()); 
            cout << ss.readstring(); //blocking after 10 packets
        }
    }
    cout << "out of loop" << endl;
    //ss << write;
    //ss << "FILE_TRANSFER_COMPLETE";
    cout << "waiting fot client...";
    //if (ss.readstring() == "TRANSFER_END_CONFIRM") return true;
    delete[] c_total;
}

bool readFile(Socket s, string outfile) {
    f_pkt init = s.readbytes<f_pkt>();
    int num_bytes = init.total;
#ifdef DEBUG
    cout << "num_bytes: " << num_bytes << endl;
#endif
    char* arr = new char[init.total];
    int arri = 0;
    int recieved = 0;
    f_pkt p;
    while(true) {
        if (s.hasqueue()) {
            p = s.readbytes<f_pkt>();
        } else { 
            s << "READY";
            while (!s.hasqueue());
            p = s.readbytes<f_pkt>();
        }
        //recieved++;
#ifdef DEBUG
        cout << p.ind << " out of "<< p.total << endl;
        cout << "writing to arr[" <<  p.ind * WRITE_SIZE << "]" << endl;
#endif
        for (int i = 0; i < strlen(p.data); i++) arr[(WRITE_SIZE * p.ind) + i] = p.data[i];
        arri += strlen(p.data);
        //cout << strlen(p.data);
        //cout << p.data << endl;
        if (p.ind == p.total - 1) break;
        memset(&p, '\0', sizeof(f_pkt));
    }
    ofstream out;
    out.open(outfile, ios::binary);
    //out.write((char*)&arr, num_bytes);
    out << arr;
    out.close();
    delete[] arr;
    return true;
}
*/
int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        struct sample {
            uint8_t i;
            uint8_t j;
            char arr[512];
        };
        cout << sizeof(sample);
    } else {
        string arg1 = argv[1];
        if (arg1 == "client") {
            string file = argv[3]; 
            Socket so(argv[2], 9420);
            cout << "Client socket opened" << endl;
            so.readFile(file);
        }
        if (arg1 == "server") {
            string file = argv[2];
            cout << "ServerSocket opened" << endl;
            ServerSocket ss(9420);
            Socket so = ss.acceptCli();
            cout << "sending file..." << endl;
            so.writeFile(file);
            cout << "file sent" << endl;

        }
    }
}

