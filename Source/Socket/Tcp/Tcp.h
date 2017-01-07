#ifndef TCP_H_
#define TCP_H_


#include "../Socket.h"

/**
 * Tcp socket.
 */
class Tcp : public Socket {
public:

    Tcp(uint16_t port_num);

    virtual ~Tcp();

    virtual int initialize();

    int sendData(string data, int sockDescriptor = -1);

    ssize_t receiveData(char *buffer, size_t size, int sockDescriptor = -1);
};

#endif /* TCP_H_ */
