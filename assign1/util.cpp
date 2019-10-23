#include <string>
#include "util.h"
using namespace std;
string util::str2Hex(string input) {

    static const char* const key = "0123456789ABCDEF";
    size_t len = input.length();

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
