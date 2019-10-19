#include <iostream>
#include <fstream>
#include "blowfish.h"
#include <string>
#include "util.h"
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

using namespace std;
using namespace util;

template<typename InputIterator1, typename InputIterator2>
bool
range_equal(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2)
{
    while(first1 != last1 && first2 != last2)
    {
        if(*first1 != *first2) return false;
        ++first1;
        ++first2;
    }
    return (first1 == last1) && (first2 == last2);
}

bool compare_files(const std::string& filename1, const std::string& filename2)
{
    std::ifstream file1(filename1);
    std::ifstream file2(filename2);

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    std::istreambuf_iterator<char> end;

    return range_equal(begin1, end, begin2, end);
}

  int main(int argc, char* argv[]) {
    BLOWFISH bf(str2Hex(COMMON_KEY));
    
    ifstream fin("test", ios::binary);
    ofstream fout("test2", ios::binary);//for testing purpose, to see if the string is a right copy
    ostringstream ostrm;

     
    ostrm << fin.rdbuf();

    
    string data( bf.Encrypt_CBC(ostrm.str() ));
        
    data = bf.Decrypt_CBC(data);

    fout << data;

    bool result = compare_files("test", "test2");
    cout << result << endl;

    return 0;

}


