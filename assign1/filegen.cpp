#include "filegen.h"
filegen::filegen(std::string fname) {
    this->fname = fname;
    fsize = getFileSize();
    numchunks = fsize / chunksize;
    rem = fsize % chunksize;
    if (rem) numchunks++;
    index = 0;
}
    
std::vector<char> filegen::operator() (){
    std::ifstream in(fname, std::ios::binary);
    uint64_t current = index * chunksize;
    if (++index == numchunks && rem != 0) chunksize = rem;
    char* chunk = new char[chunksize];
    in.seekg(current);
    in.read(chunk, chunksize);
    in.close();
    std::vector<char> v;
    v.reserve(chunksize);
    v.assign(chunk, chunk + chunksize);
    delete[] chunk;
    return v;
}

filegen::operator bool() const {
    return index < numchunks;
}

void filegen::setChunkSize(uint64_t size) {
    this->chunksize = size;
    this->numchunks = fsize / chunksize;
    this->rem = fsize % chunksize;
    if (rem) numchunks++;
}

uint64_t filegen::getFileSize() {
    std::ifstream temp(fname, std::ios::binary);
    temp.seekg(0, std::ios::end);
    uint64_t size = temp.tellg();
    temp.close();
    return size;
}
