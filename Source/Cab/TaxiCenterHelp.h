
#ifndef EX2_TAXICENTERHELP_H
#define EX2_TAXICENTERHELP_H

#include <map>
#include "../Socket/Tcp/TcpServer.h"
#include "../Basic/Point.h"
#include "Vehicle/Cab.h"
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <boost/archive/binary_iarchive.hpp>


extern map<int, int> g_descriptorToDriverId;
extern map<int, DriverInfo*> g_driverIdToInfo;

using namespace std;
using namespace boost;

struct SocketInfo {
    TcpServer* serverSocket;
    int socketDescriptor;
};

struct DriverInfo{
    bool isAvailable;
    Point location;
    int cabId;
};

struct ConnectionInfo{
    pthread_t* pthread;
    SocketInfo* socketInfo;
};

struct ParamToSendCab{
    SocketInfo* socketInfo;
    Cab* cab;
};

void* receiveDriverId(void* param);
void* sendCab(void* param);


#endif //EX2_TAXICENTERHELP_H
