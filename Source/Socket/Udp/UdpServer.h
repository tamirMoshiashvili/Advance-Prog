#ifndef EX2_UDPSERVER_H
#define EX2_UDPSERVER_H


#include "Udp.h"

class UdpServer : public Udp {
public:

    UdpServer(uint16_t port_num);

    ~UdpServer();

    int initialize();
};


#endif //EX2_UDPSERVER_H
