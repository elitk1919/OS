#ifndef UTIL_H
#define UTIL_H
#define COMMON_KEY "commonkeyforencryption"
#include <algorithm>
#include <stdexcept>
#include <vector>
namespace util {
    enum bf_stat { // enum to hold append marks for blowfish
        REQ,
        NONCE,
        ENC,
        KEY,
        ID
    };
    
    std::string vec2Str(std::vector<char>);
    std::vector<char> str2Vec(std::string);
    std::string str2Hex(std::string);
    std::string hex2Str(const std::string&);
    std::string str2Hex(const char*, size_t);
    long f(long);
};
#endif
