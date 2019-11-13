#ifndef UTIL_H
#define UTIL_H
#define COMMON_KEY "commonkeyforencryption"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <ctime>
#include <iterator> 
#include <iostream>
#include <cstring>
#include <chrono>

namespace util {

    struct timer {
        std::chrono::high_resolution_clock::time_point starttime;
        std::chrono::high_resolution_clock::time_point stoptime;
        timer();
        void start();
        double stop();    
    };

    enum bf_stat { // enum to hold append marks for blowfish
        REQ,
        NONCE,
        ENC,
        KEY,
        ID,
        FTP,
        MESSAGE,
        TERMINATE
    };

    struct auth_pkt {
        char sessionkey[100];
        char request[100];
        std::vector<char> encryptedkey;
        unsigned long nonce;
    };
    
    std::vector<char> file2Vec(std::string);
    void vec2File(std::vector<char>, std::string);
    std::string vec2Str(std::vector<char>);
    std::vector<char> str2Vec(std::string);
    std::string str2Hex(std::string);
    std::string hex2Str(const std::string&);
    std::string str2Hex(const char*, size_t);
    unsigned long generateNonce();
    long f(long);
    std::vector<char> long2Vec(unsigned long);
    long vec2Long(std::vector<char>);
};
#endif
