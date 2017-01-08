#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

#define END          "end"
#define IS_AVAILABLE "Are you available?"
#define YES          "yes"
#define NO           "no"
#define GO           "go"
#define SEND_LOCATION "send location"
#define CLOSE        "close"

// Return values to each function if error happened.
#define CORRECT           0
#define ERROR_SOCKET      1
#define ERROR_BIND        2
#define ERROR_LISTEN      3
#define ERROR_CONNECT     4
#define ERROR_SEND        5
#define ERROR_RECEIVE     6
#define ERROR_ACCEPT      7
#define CONNECTION_CLOSED 8

#define IP "127.0.0.1"

class Socket {
protected:
    int socketDescriptor;
    string ip_address;
    int backLog;
    uint16_t port_number;

public:

    Socket();

    virtual ~Socket();

    /**
     * Initialize the Socket object and getting a socket descriptor.
     * @return number representing the return status.
     */
    virtual int initialize() = 0;

    /**
     * Sending the input data to the socket who connect to this socket.
     * @param data string representing the data to send.
     * @param sockDescriptor descriptor.
     * @return number representing the return status.
     */
    virtual int sendData(string data, int sockDescriptor = -1) = 0;

    /**
     * Getting data from the other socket and print the data.
     * @param buffer the place where the data will be stored.
     * @param size number.
     * @param sockDescriptor descriptor.
     * @return number.
     */
    virtual ssize_t receiveData(char *buffer,
                                size_t size, int sockDescriptor = -1) = 0;
};

#endif /* SOCKET_H_ */
