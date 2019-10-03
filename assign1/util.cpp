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
