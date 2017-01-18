#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

#define END           "end"
#define IS_AVAILABLE  "Are you available?"
#define YES           "yes"
#define NO            "no"
#define GO            "go"
#define SEND_LOCATION "send location"
#define RECEIVED      "received msg"

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

    virtual int initialize() = 0;

    virtual int sendData(string data, int sockDescriptor = -1) = 0;

    virtual ssize_t receiveData(char *buffer,
                                size_t size, int sockDescriptor = -1) = 0;
};

#endif /* SOCKET_H_ */
