#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

/*      filegen.h
 * Header for filegen.cpp and filegen object.
 * Filegen object is a solution to reading large
 * files on limited-memory systems. It is based 
 * on a 'generator' object from python. 
 * A single filegen object is good for one reading of
 * a single large file. 
 * a filegen instance will read a file and store an the 
 * content in a character vector, sized by the configurable
 * 'chunksize' variable (2 GB by default). 
 * Overriding of operator bool and operator() allows for this 
 * class to return portions of files. The class also keeps track 
 * of where the last track ended, and the total bytes of the file.
 *
 * This class also closes the file and reopens it between processing 
 * chunks, thus it is not completely thread safe.
 */
class filegen {
    
    std::string fname;
    uint64_t chunksize = 2097152000; //2GBs
    int index;
    int numchunks;
    uint64_t fsize;
    uint64_t rem;
    

public:
    void setChunkSize(uint64_t);
    uint64_t getFileSize();
    filegen(std::string);
    std::vector<char> operator()();
    operator bool() const;
};
