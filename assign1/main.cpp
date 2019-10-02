#include <iostream>
#include <string.h>
#include "blowfish.h"
using namespace std;
typedef unsigned char byte;
int main()
{
    BLOWFISH bf("testing");
    string asdf = "BlowwFIshhhhhhhhhhh!";
    asdf = bf.Encrypt_CBC(asdf);
    cout << "Encrypted: " << asdf << endl;
    asdf = bf.Decrypt_CBC(asdf);
    cout << "Decrypted: " << asdf;
    return 0;
}
