#ifndef EX2_TCPSERVER_H
#define EX2_TCPSERVER_H


#include <vector>
#include "Tcp.h"

using namespace std;

/**
 * Tcp server socket.
 */
class TcpServer : public Tcp {
private:
    int numClients;
    vector<int> *clientDescriptors;

public:
    TcpServer(uint16_t port_num, int numOfClients);

    ~TcpServer();

    int initialize();

    vector<int> *getClientDescriptors();
};


#endif //EX2_TCPSERVER_H
