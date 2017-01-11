
#ifndef EX2_TAXICENTERHELP_H
#define EX2_TAXICENTERHELP_H

#include <map>
#include "../Socket/Tcp/TcpServer.h"
#include "../Basic/Point.h"

extern map<int,int> g_socketToDriver;

struct SocketInfo {
    TcpServer Serversocket;
    int socketDescriptor;
};

struct DriverInfo{
    bool isAvailable;
    Point location;
    int socketDescriptor;
};

struct ConnectionInfo{
    pthread_t* pthread;
    int socketDescriptor;
};

void* receiveDriverId(void*);


#endif //EX2_TAXICENTERHELP_H
