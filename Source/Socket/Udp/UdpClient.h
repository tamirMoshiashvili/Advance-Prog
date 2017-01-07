#ifndef EX2_UDPCLIENT_H
#define EX2_UDPCLIENT_H


#include "Udp.h"

/**
 * Udp client socket.
 */
class UdpClient : public Udp {
public:

    UdpClient(std::string ip_address, uint16_t port_num);

    ~UdpClient();
};


#endif //EX2_UDPCLIENT_H
