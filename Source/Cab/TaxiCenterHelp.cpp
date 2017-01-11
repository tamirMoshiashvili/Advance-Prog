
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
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
    DriverInfo *driverInfo = new DriverInfo;
    driverInfo->cabId = cabId;
    g_driverIdToInfo.insert(pair<int, DriverInfo*>(driverId, driverInfo));
    g_descriptorToDriverId.insert(pair<int, int>(socketDescriptor, driverId));
}


void *sendCab(void *param) {
    ParamToSendCab *paramToSendCab = (ParamToSendCab *) param;
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > s2(inserter);
    archive::binary_oarchive oa(s2);
    oa << paramToSendCab->cab;
    s2.flush();
    // Send the cab of the driver.
    paramToSendCab->socketInfo->serverSocket->
            sendData(serial_str, paramToSendCab->socketInfo->socketDescriptor);
}