#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "TaxiCenter.h"
#include "../ThreadControl/ThreadManagement.h"


using namespace std;
using namespace boost;

/**
 * Constructor.
 * @param map pointer to city map.
 * @return TaxiCenter object.
 */
TaxiCenter::TaxiCenter(CityMap *map) : numOfDrivers(0), tcpServer(NULL),
                                       cityMap(map),
                                       clock(0) {
    pthread_mutex_init(&locker, 0);
}

/**
 * Destructor.
 */
TaxiCenter::~TaxiCenter() {
    // Delete drivers.
    delete tcpServer;
    // Wait for all the threads to end.
    cout<<"destructor taxiCenter\n";
    for (unsigned long i = 0; i < threads.size(); i++) {
        pthread_join(*threads[i], NULL);
        delete threads[i];
        cout << "delete driver thread\n";
    }
    // Delete cabs.
    for (map<int, Cab *>::iterator it = idToCab.begin();
         it != idToCab.end(); ++it) {
        it->second->resetNavigation();
        delete it->second;
    }
    // Delete rides.
    for (map<int, Ride *>::iterator it = idToRides.begin();
         it != idToRides.end(); ++it) {
        delete it->second;
    }
    for (unsigned long i = 0; i < rideThreads.size(); i++) {
        pthread_join(*rideThreads[i], NULL);
        delete rideThreads[i];
        cout << "delete ride thread\n";
    }
    delete cityMap;
    pthread_mutex_destroy(&locker);
}

/**
 * Create number of sockets, according to the number of drivers,
 *  with the given port, and initialize them.
 * @param numDrivers number of drivers.
 * @param port port number.
 */
void TaxiCenter::initialize(int numDrivers, uint16_t port) {
    // Create the main-server socket.
    numOfDrivers = numDrivers;
    tcpServer = new TcpServer(port, numDrivers);
    tcpServer->initialize();
    vector<int> *clients = tcpServer->getClientDescriptors();
    int i = 0, status;
    ClientThreadInfo *clientThreadInfo;
    pthread_t *p;
    // Start connection with each client
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    for (i = 0; i < numDrivers; ++i) {
        p = new pthread_t;
        threads.push_back(p);
        clientThreadInfo = new ClientThreadInfo;
        clientThreadInfo->taxiCenter = this;
        clientThreadInfo->socket = (*clients)[i];
        status = pthread_create(p, NULL, ThreadManagement::threadFunction,
                                (void *) clientThreadInfo);
        if (status) {
            cout << "ERROR" << endl;
        }
        globalInfo->addDriverToMap((*clients)[i]);
    }
}

/**
 * Add a cab to the taxi center.
 * @param cab pointer to cab.
 */
void TaxiCenter::addCab(Cab *cab) {
    idToCab.insert(std::pair<int, Cab *>(cab->getId(), cab));
}

/**
 * Add a ride to the taxi center.
 * @param Ride pointer to ride.
 */
void TaxiCenter::addRide(Ride *ride) {
    idToRides.insert(std::pair<int, Ride *>(ride->getId(), ride));
    rides.push_back(ride);
    pthread_t *rideThread = new pthread_t;
    PathCalcInfo *info = new PathCalcInfo;
    info->ride = ride;
    info->cityMap = cityMap;
    int status = pthread_create(rideThread, NULL,
                                ThreadManagement::produceNavigation,
                                (void *) info);
    if (status) {
        cout << "ERROR" << endl;
    }
    rideThreads.push_back(rideThread);
}

/**
 * Send the given ride to the driver with the given id.
 * @param driverId id number of the driver.
 * @param ride pointer to ride object.
 */
void TaxiCenter::sendRide(int driverSocket, Ride *ride) {
    // Serialize the ride.
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > stream(inserter);
    archive::binary_oarchive oa(stream);
    oa << ride;
    stream.flush();
    // Send the ride to the driver.
    tcpServer->sendData(serial_str, driverSocket);
    char buffer[16] = {0};
    tcpServer->receiveData(buffer, sizeof(buffer), driverSocket);
    if (strcmp(buffer, RECEIVED)) {
        cout << "ERROR, expected received-msg, got: " << buffer;
    }
    // Send a navigation-system of the given ride to the driver.
    sendNavigation(driverSocket, ride);
}

/**
 * Send a navigation system of the given ride, to the driver with the given id.
 * @param driverId id number of the driver.
 * @param ride pointer to ride object.
 */
void TaxiCenter::sendNavigation(int driverSocket, Ride *ride) {
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    int rideId = ride->getId();
    while (!globalInfo->doesRideExist(rideId)) {}
    // Create opposite stack of blocks ids which represents the path.
    deque<string> *string_path = globalInfo->popPathOf(rideId);
    // Serialize the path.
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > stream(inserter);
    archive::binary_oarchive oa(stream);
    oa << string_path;
    stream.flush();
    // Send the path to the driver.
    tcpServer->sendData(serial_str, driverSocket);
    delete string_path;
}

/**
 * Assign the rides that need to be taken,
 * at the current time to driver if available.
 * @param driverSocket socket-descriptor of the driver.
 */
void TaxiCenter::makeDriverWork(int driverSocket) {
    char buffer[16] = {0};
    tcpServer->sendData(GO, driverSocket);
    tcpServer->receiveData(buffer, sizeof(buffer), driverSocket);
    Ride *ride = NULL;
    // Iterate over the rides list.
    pthread_mutex_lock(&locker);
    for (list<Ride *>::iterator it = rides.begin(); it != rides.end(); ++it) {
        if ((*it)->getStartTime() == clock) {
            // There is a ride we need to take right now,
            // Check if driver available.
            tcpServer->sendData(IS_AVAILABLE, driverSocket);
            // Get the driver's answer.
            tcpServer->receiveData(buffer, sizeof(buffer), driverSocket);
            if (!strcmp(buffer, YES)) {
                ride = *it;
                sendRide(driverSocket, ride);
                break;
            } else {
                // Driver isn't available, quit the iteration.
                break;
            }
        }
    }
    // Ride has been handled.
    if (ride != NULL) {
        rides.remove(ride);
    }
    pthread_mutex_unlock(&locker);
}

/**
 * Advance the clock by one.
 */
void TaxiCenter::advanceClock() {
    ++clock;
}

/**
 * Get driver's id and its cab id and send him his cab.
 * @param driverSocket driver's socket descriptor.
 */
void TaxiCenter::identifyDriver(int driverSocket) {
    char buffer[128];
    int driverId = 0, cabId = 0;
    // Wait for data (Ids) from a driver.
    tcpServer->receiveData(buffer, sizeof(buffer), driverSocket);
    // Parse the id of driver and its cab.
    string str(buffer);
    unsigned long j = str.find(",");
    driverId = atoi(str.substr(0, j).c_str());
    cabId = atoi(str.substr(j + 1, str.length()).c_str());
    // Add a driver and its socket descriptor to the map.
    pthread_mutex_lock(&locker);
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    globalInfo->addToSocketToIdMap(driverId, driverSocket);
    pthread_mutex_unlock(&locker);
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > s2(inserter);
    archive::binary_oarchive oa(s2);
    oa << idToCab.at(cabId);
    s2.flush();
    // send cab to driver
    tcpServer->sendData(serial_str, driverSocket);
}

/**
 * Ask the driver, with the given id, for his location.
 * @param driverId id number of the driver.
 * @return point which is the location of the driver on the map.
 */
Point TaxiCenter::askDriverLocation(int driverSocket) {
    // Send the driver a message,
    // which acknowledge him that he need to send his location to the server.
    tcpServer->sendData(SEND_LOCATION, driverSocket);
    Point driverLocation;
    // De-serialize the location.
    char buffer[128] = {0};
    tcpServer->receiveData(buffer, sizeof(buffer), driverSocket);
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<iostreams::basic_array_source<char> > stream(device);
    archive::binary_iarchive ia(stream);
    ia >> driverLocation;
    return driverLocation;
}
