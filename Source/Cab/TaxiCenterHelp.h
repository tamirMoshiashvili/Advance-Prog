
#ifndef EX2_TAXICENTERHELP_H
#define EX2_TAXICENTERHELP_H

#include <map>
#include "../Socket/Tcp/TcpServer.h"
#include "../Basic/Point.h"


extern map<int, DriverInfo> g_driverIdToInfo;

struct SocketInfo {
    TcpServer* serverSocket;
    int socketDescriptor;
};

struct DriverInfo{
    bool isAvailable;
    Point location;
    int socketDescriptor;
    int cabId;
};

struct ConnectionInfo{
    pthread_t* pthread;
    int socketDescriptor;
};

void* receiveDriverId(void* param);


#endif //EX2_TAXICENTERHELP_H
