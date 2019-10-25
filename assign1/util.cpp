#include <string>
#ifndef UTIL_H
#include "util.h"
#endif
using namespace std;

std::string util::str2Hex(std::string s) {
    return util::str2Hex(s.data(), s.length());
}


std::vector<char> util::str2Vec(std::string s){
    vector<char> v;
    for (char& c : s)v.push_back(c);
    return v;
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
