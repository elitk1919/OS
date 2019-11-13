#include <string>
#ifndef UTIL_H
#include "util.h"
#endif
using namespace std;

util::timer::timer(){}
void util::timer::start() {
    this->starttime = std::chrono::high_resolution_clock::now();
}

double util::timer::stop() {
    this->stoptime = std::chrono::high_resolution_clock::now();
     return (double) std::chrono::duration_cast<std::chrono::microseconds>(stoptime - starttime).count() / 1000000;
}

std::vector<char> util::file2Vec(std::string filename) {
    ifstream in(filename, ios::binary | ios::app);
    in.seekg(0, ios::end);
    uint64_t size = in.tellg();
    in.seekg(0, ios::beg);
    std::cout << size << std::endl;
    char* data = new char[size];
    
    //v.reserve(size);
    //std::cout << "Allocated" << std::endl;
    in.read(data, size);
    //v.insert(v.begin(), istream_iterator<char>(in), istream_iterator<char>());
    in.close();
    vector<char> v;
    v.reserve(size);
    //for (int i = 0; i < size; i++) {
    //    v[i] = data[i];
    //}
    v.assign(data, data + size);
    delete[] data; 
    return v;
}

void util::vec2File(std::vector<char> out, std::string fname) {
    ofstream fout(fname, ios::binary);
    size_t size = out.size();
    //char* data = new char[size];
    //memcpy(&data, out.data(), size);
    //data = out.data();
    //r (int i = 0 ; i < out.size(); i++) {
    fout.write(&out[0], size);
    //
    //copy(out.begin(), out.end(), ostream_iterator<char>(fout));
    fout.close();
    //delete[] data;
}

std::string util::str2Hex(std::string s) {
    return util::str2Hex(s.data(), s.length());
}


std::vector<char> util::str2Vec(std::string s){
    vector<char> v;
    for (char& c : s)v.push_back(c);
    return v;
}

std::vector<char> util::long2Vec(unsigned long nonce) {
    vector<char> v;
    union {
        unsigned long l;
        char bits[sizeof(unsigned long)];
    };
    l = nonce;

    for (int i = 0; i < sizeof(unsigned long); i++)
        v.push_back(bits[i]);
    
    return v;
}

long util::vec2Long(vector<char> v) {
    union {
        long l;
        char bits[sizeof(long)];
    };

    //const char* vecdata = v.data();

    memcpy(&bits, v.data(), sizeof(long));
    return l;
}

std::string util::vec2Str(std::vector<char> v) {
    string s;
    for (char& c : v) s.push_back(c);
    return s;
}

std::string util::str2Hex(const char* input, size_t len) {

    static const char* const key = "0123456789ABCDEF";
    //size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(key[c >> 4]);
        output.push_back(key[c & 15]);
    }
    return output;
}

std::string util::hex2Str(const std::string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");\
        std::string output;
        output.reserve(len / 2);
        for (size_t i = 0; i < len; i += 2) {
            char a = input[i];
            const char* p = std::lower_bound(lut, lut + 16, a);
            if (*p != a) throw std::invalid_argument("not a hex digit");
            char b = input[i + 1];
            const char* q = std::lower_bound(lut, lut + 16, b);
            if (*q != b) throw std::invalid_argument("not a hex digit");
            output.push_back(((p - lut) << 4) | (q - lut));
        }
    return output;
}

unsigned long util::generateNonce() {
    srand(time(NULL));
    return rand();
}

long util::f(long nonce) {
    const long A = 48271;
    const long M = 2147483647;
    const long Q = M/A;
    const long R = M%A;

        static long state = 1;
        long t = A * (state % Q) - R * (state / Q);

        if (t > 0)
                state = t;
        else
                state = t + M;
        return (long)(((double) state/M)* nonce);
}
