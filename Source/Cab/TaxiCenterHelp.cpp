
#include "TaxiCenterHelp.h"

void *receiveDriverId(void *param) {
    // we'll get as parameters the server socket and the specific socket descriptor.
    SocketInfo *socketInfo = (SocketInfo *) param;
    char buffer[128];
    int driverId = 0, cabId = 0;
    int socketDescriptor = socketInfo->socketDescriptor;
    // Wait for data (Ids) from a driver.
    socketInfo->serverSocket->receiveData(buffer, sizeof(buffer),
                                          socketDescriptor);
    // Parse the id of driver and its cab.
    string str(buffer);
    unsigned long j = str.find(",");
    driverId = atoi(str.substr(0, j).c_str());
    cabId = atoi(str.substr(j + 1, str.length()).c_str());
    // Add a driver and its socket descriptor to the map.
    DriverInfo driverInfo;
    driverInfo.socketDescriptor = socketDescriptor;
    driverInfo.cabId = cabId;
    g_driverIdToInfo.insert(pair<int, DriverInfo>(driverId, driverInfo));
}