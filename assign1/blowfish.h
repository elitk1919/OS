#ifndef BLOWFISH_INCLUDED
#define BLOWFISH_INCLUDED

#include <string>

//headers needed for the CSPRNG
#ifdef _WIN32
    #include <Windows.h>
    #include <Wincrypt.h>
#else
    #include <fstream> //for reading from /dev/urandom on *nix systems
#endif

typedef unsigned char byte;

class BLOWFISH{

    //Although there is no successful cryptanalysis of the 16 round version, a higher number of rounds generally means more security.
    //STANDARD: 16
    //MAXIMUM: 256
    //**MUST be an EVEN number**
    #define ROUNDS 16
    public:
        BLOWFISH(std::string hexKey);
        BLOWFISH(byte* cipherKey, int keylength);

        //TODO: string encryption functions -> base64
        std::string Encrypt_CBC(std::string data, int length);
        byte* Encrypt_CBC(byte* data, int length, int* newlength);
        byte* Encrypt_ECB(byte* data, int length, int* newlength);
        void Encrypt_Block(byte* block, int offset = 0);

        std::string Decrypt_CBC(std::string data);
        byte* Decrypt_CBC(byte* data, int length, int* newlength);
        byte* Decrypt_ECB(byte* data, int length, int* newlength);
        void Decrypt_Block(byte* block, int offset = 0);

        void SetRandomIV();
        void SetIV(byte* newIV);
        byte* GetIV();
        bool IvSet;

    protected:
        void SetupKey(byte* cipherKey, int length);
        void encipher();
        void decipher();
        unsigned int round(unsigned int a, unsigned int b, unsigned int n);
        void setblock(byte* block, int offset);
        void getblock(byte* block, int offset);
        static unsigned int p[];
        static unsigned int s0[];
        static unsigned int s1[];
        static unsigned int s2[];
        static unsigned int s3[];

        unsigned int xl_par;
        unsigned int xr_par;

        byte IV[8];

        byte* Crypt_ECB(byte* data, int length, int* newlength, void (BLOWFISH::*CryptBlock)(byte*, int offset), bool decrypt);
        byte* Crypt_CBC(byte* data, int length, int* newlength, void (BLOWFISH::*CryptBlock)(byte*, int offset), bool decrypt);
        byte* padData(byte* data, int length, int* paddedLength, bool decrypt, bool IvSpace);
        int findPaddingEnd(byte* data, int length);
        int hex2dec(char hex);
        std::string byteToHex(unsigned char x);
};
#endif
