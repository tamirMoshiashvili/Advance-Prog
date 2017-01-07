#ifndef UDP_H_
#define UDP_H_

#include "../Socket.h"

/**
 * Udp socket.
 */
class Udp : public Socket {
public:

    Udp(uint16_t port_num);

    virtual ~Udp();

    virtual int initialize();

    int sendData(string data, int sockDescriptor = -1);

    ssize_t receiveData(char *buffer, size_t size, int sockDescriptor = -1);
};

#endif /* UDP_H_ */
