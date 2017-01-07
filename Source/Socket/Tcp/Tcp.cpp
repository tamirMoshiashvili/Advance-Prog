#include <cstdint>
#include "Tcp.h"

/**
 * Constructor.
 * @param port_num port number.
 * @return Tcp object.
 */
Tcp::Tcp(uint16_t port_num) {
    port_number = port_num;
}

/**
 * Destructor.
 */
Tcp::~Tcp() {

}

/**
 * Initialize the socket.
 * @return int number representing the return status.
 */
int Tcp::initialize() {
    // Getting a socket descriptor and check if legal.
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0) {
        // Return an error represent error at this method.
        return ERROR_SOCKET;
    }
    // Return correct if there were no problem.
    return CORRECT;
}

/**
 * Send the given data.
 * @param data what we need to send.
 * @param sockDescriptor -1 if the current object is a client-tcp-socket,
 * otherwise it will be number which mention the client socket descriptor.
 * @return
 */
int Tcp::sendData(string data, int sockDescriptor = -1) {
    unsigned long data_len = data.length();
    const char *datas = data.c_str();
    // Check for socket descriptor, is server or client.
    if(sockDescriptor == -1){
        sockDescriptor = socketDescriptor;
    }
    ssize_t sent_bytes = send(sockDescriptor, datas, data_len, 0);
    if (sent_bytes < 0) {
        // Return an error represent error at this method.
        return ERROR_SEND;
    }
    // Return correct if there were no problem.
    return CORRECT;
}

/***********************************************************************
* function name: recive	`											   *
* The Input: none										               *
* The output: int number representing the return status	               *
* The Function operation: getting data from the other socket to,	   *
* enter it to the buffer and print the data							   *
***********************************************************************/
int Tcp::receiveData(char *buffer, int size) {
    int read_bytes = recv(this->isServer ? this->clientDescriptor
                                         : this->socketDescriptor, buffer, size,
                          0);
    //checking the errors
    if (read_bytes == 0) {
        return CONNECTION_CLOSED;
    } else if (read_bytes < 0) {
        //return an error represent error at this method
        return ERROR_RECIVE;
    } else {
        //prinrting the massege
//		cout<<buffer<<endl;
    }
    //return correct if there were no problem
    return read_bytes;
}
