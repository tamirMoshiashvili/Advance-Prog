#ifndef EX2_TCPCLIENT_H
#define EX2_TCPCLIENT_H


#include "Tcp.h"

class TcpClient : public Tcp {
public:
    TcpClient(uint16_t port_num);

    ~TcpClient();

    int initialize();
};


#endif //EX2_TCPCLIENT_H
