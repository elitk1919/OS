#include <sys/socket.h>
#include "Socket.h"
#include <string>
#include <fstream>
#ifdef DEBUG
#include <chrono>
#endif

bool Socket::connected() {
	char trash = 't';
	if (recv(this->fd, &trash, 1, MSG_PEEK | MSG_DONTWAIT) == -1){
		if (errno == ENOTCONN) return false;
		else return true;
	}
}

Socket::Socket(){}

Socket::Socket(int sock, sockaddr_in add){ //constructor for server socket accept
	this->fd = sock;
	this->addr = add;
}

Socket::Socket(const Socket& s){
	this->fd = s.fd;
	
}

Socket::Socket(char* host, int portno) {
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd < 0){ 
		throw std::runtime_error("Error opening socket");
	} else {
		memset(&this->addr, '0', sizeof(this->addr));
		this->addr.sin_family = AF_INET;
		this->addr.sin_port = htons(portno);
		struct hostent* server;
		server = gethostbyname(host);
		if (server == NULL) { 
			throw std::runtime_error("Error finding host");
		} 
		memcpy(&this->addr.sin_addr.s_addr, server->h_addr, server->h_length);
		if (connect(this->fd, (sockaddr *) &this->addr, sizeof(this->addr)) < 0){
			throw std::runtime_error("Error connecting to host");
		}
	}
}

int Socket::queue(){
	int trash = 0;
    ioctl(fd, FIONREAD, &trash);
	//return recv(this->fd, &trash, 1024, MSG_PEEK | MSG_DONTWAIT);
    return trash;
}

bool Socket::hasqueue() {
    return !this->queue() == 0;
}

void Socket::sendbytes(char* bytes, int size) {
    send(this->fd, &bytes, size, 0);
}

char* Socket::getbytes(char* rec, int size) {
    recv(this->fd, &rec, size, 0);
    return rec;
}

int Socket::getfd(){
	return this->fd;
}

std::string Socket::getIP(){ // returns the client IP address 
	using namespace std;
	union convert{
		unsigned long addr;
		uint8_t nums[4];
	};
	union convert c;
	c.addr = this->addr.sin_addr.s_addr;
	return to_string(c.nums[0]) + "." + to_string(c.nums[1]) + "." + to_string(c.nums[2]) + "." + to_string(c.nums[3]);
}

std::string Socket::readstring(){
	char buf[1024];
	memset(buf, '\0', 1024);
    char c = ' ';
    int i = 0;
	int res = recv(this->fd, buf, 1024, 0);
	if (res < 0) throw std::runtime_error("Error on read");
	std::string s = buf;
	return s;
}

void Socket::setstatus(status s){ //ends OOB status message
	int res = send(this->fd, &s, 1, MSG_OOB);
	if (res < 0) throw std::runtime_error("Error setting status");
}

Socket& Socket::operator<< (std::string s){ // for writing strings to the socket
	write(this->fd, s.c_str(), s.length());
    //write(this->fd, '\0', 1);
	return *this;
}

Socket& Socket::operator<< (binarystring s){ // for writing strings to the socket
	write(this->fd, s.data.c_str(), s.length);
    //write(this->fd, '\0', 1);
	return *this;
}

void Socket::writedata(std::vector<char> data) {
    uint64_t fsize = data.size();
#ifdef DEBUG
    std::cout << fsize << std::endl;
#endif
    char* bytes = data.data();//new char[fsize]
#ifdef DEBUG
    auto start = std::chrono::high_resolution_clock::now(); 
#endif
    this->writebytes<uint8_t>(FTP_INIT); // send protocol init message
    if (this->readbytes<uint8_t>() != FTP_INIT) { //check for handshake
#ifdef DEBUG
        std::cout << "Exiting now" << std::endl; //return false on bad handshake
#endif
        return;
    } else {
        //std::cout << "Handshake complete" << std::endl;
    }    
    this->writebytes<uint64_t>(fsize); //write file size to client 
    uint8_t cli_res = this->readbytes<uint8_t>(); //wait for response
    int outof = fsize / WRITE_SIZE;

    int sent = 0;
    if (fsize % WRITE_SIZE != 0) outof++;

#ifdef DEBUG
    std::cout << "Will send " << outof << "packets" << std::endl;
#endif
    for (int i = 0; i < fsize; i++) { //iterate through file size 
        if (i % WRITE_SIZE == 0) {
            /* 
             * if iterator is divisible by file size start 
             * calculating the size of the message.
             * If there is less than WRITE_SIZE left 
             * in the file, set message size to the 
             * amount left in the file 
             */
            int size = 0;
            if (fsize - i < WRITE_SIZE) {
                size = fsize % WRITE_SIZE;
            } else {
                size = WRITE_SIZE;
            }
#ifdef DEBUG
    //        std::cout << "sending pkt " << sent << " size " << size << std::endl;
    //        std::cout << "outof " << outof << std::endl;
#endif
            char chunk[size]; //initialize an array to previously set size var
            memcpy(&chunk, &bytes[i], size); //copy data array at i into chunk
            send(this->fd, &chunk, size, 0); // send over socket
            if (sent == 10) { 
                /*
                 * if 10 packets have been sent wait for reciever to
                 * signal for more packets to prevent socket buffer 
                 * from overflowing 
                 */
#ifdef DEBUG
                std::cout << "waiting for ready..";
#endif
                while (!this->hasqueue()); //block until sender sends
                uint8_t cli_res = this->readbytes<uint8_t>(); // read sent, could be used for error checking
#ifdef DEBUG
                std::cout << "ready recieved" << std::endl;
#endif

            }
            sent++;

        }
        //delete[] bytes; // free byte array
    }
#ifdef DEBUG
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "File transfer complete in "; 
    double time = (double) std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double elapsed = time / 1000000;
    std::cout << elapsed << "seconds" <<std::endl;
#endif
    while(this->readbytes<uint8_t>() != FTP_CLOSE); 
    /*
     * wait for FTP_CLOSE message. This stops the program
     * from potentially exiting early, closing the socket
     */
    //delete[] bytes;
}

std::vector<char> Socket::readdata() {
    
    if (this->readbytes<uint8_t>() != FTP_INIT) {
 //       std::cout << "ahhhhh" << std::endl;
        return std::vector<char>(); //return false on bad handshake
    }
    else this->writebytes<uint8_t>(FTP_INIT);//write proto back st sender

    uint64_t fsize = this->readbytes<uint64_t>(); // read size and send server confirmation
    this->writebytes<uint8_t>(READY);

    char* arr = new char[fsize]; //array to store revieved bytes
    // I changed to to uint8, it's the same size as a char and it works
    // but now I'm scared to change it back. If it aint broke dont fix it
    //               ¯\_(ツ)_/¯
    //                    |
    //                    |
    //                   / \ 
    
#ifdef DEBUG 
    std::cout << "Expecting " << fsize << std::endl; 
    std::cout << "handshake established" << std::endl;
#endif
    bool allsent = false;
    int bytes_recvd = 0; // initialize counter for recieved bytes
    while(!allsent) { // until every byte is sent
        while(this->hasqueue()) { // while reader has data to read
            char chunk[WRITE_SIZE]; // create char array of WRITE_SIZE
            int numRead = recv(this->fd, &chunk, WRITE_SIZE, MSG_DONTWAIT); //read into char array
            /*
             * If there are less than WRITE_SIZE bytes to read,
             * MSG_DONTWAIT will tell the socket to only read what is 
             * available to read. numRead will be the amount of bytes read
             */
#ifdef DEBUG
            std::cout << numRead << " bytes read" << std::endl;
#endif
            if (numRead != -1) {
                /*
                 * if recv returned no error, copy the bytes 
                 * read into output array.
                 * If we want to possibly catch bugs 
                 * we can add an else but it seems pretty
                 * stable at the moment
                 */ 
                memcpy(&arr[bytes_recvd], &chunk, numRead);
                bytes_recvd += numRead; // add numRead to total bytes recieved
            }
            if (bytes_recvd >= fsize - 1) { // if all bytes have been recieved, kill outer loop
                allsent = true;
            }
        }
#ifdef DEBUG
        std::cout << "sender has no queue\nsending ready" << std::endl;
        std::cout << bytes_recvd << " out of " << fsize << std::endl;
#endif  
        this->writebytes<uint8_t>(READY);
#ifdef DEBUG
        std::cout << "ready sent" << std::endl;
#endif
        while (!allsent && !this->hasqueue());
#ifdef DEBUG
        std::cout << "Sender has queue" << std::endl;
#endif
    }
    this->writebytes<uint8_t>(FTP_CLOSE);
    std::vector<char> v;
    v.reserve(bytes_recvd);
    //for (int i = 0; i < bytes_recvd; i++) 
    //    v[i] = arr[i];
    v.assign(arr, arr + bytes_recvd);
    delete[] arr;
    return v;
}

void Socket::shut() {
    close(this->fd);
}

Socket::~Socket() {}

