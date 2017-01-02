#include "Client.h"
#include "../Socket/Udp/UdpClient.h"
#include "../Cab/Vehicle/LuxuryCab.h"
#include "../Cab/Vehicle/StandardCab.h"
#include "../Ride/Navigation/BFS.h"
#include "../Basic/Block/MatrixBlock.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_GUID(MatrixBlock, "matrix_block");
BOOST_CLASS_EXPORT_GUID(BFS, "bfs");
BOOST_CLASS_EXPORT_GUID(StandardCab, "standard_cab");
BOOST_CLASS_EXPORT_GUID(LuxuryCab, "luxury_cab");

using namespace std;
using namespace boost;

/**
 * Constructor.
 * @param idNum id number.
 * @param ageVal age of the driver.
 * @param status marital status.
 * @param years years of experience.
 * @param cabID id number of the driver's cab.
 * @param ip_addr ip address.
 * @param port port number.
 * @return Client object.
 */
Client::Client(int idNum, int ageVal, MaritalStatus status, int years,
               int cabID, const string &ip_addr, uint16_t port)

        : Driver(idNum, ageVal, status, years, cabID) {

    // Handle the socket.
    socket = new UdpClient(ip_addr, port);
    socket->initialize();
    // Connect with server.
    sendIds();
    connectToCab();
}

/**
 * Destructor.
 */
Client::~Client() {
    delete socket;
}

/**
 * Sending the input data to the socket who connect to this socket.
 * @param data string representing the data to send.
 * @return number representing the return status.
 */
int Client::sendData(string data) {
    return socket->sendData(data);
}

/**
 * Getting data from the other socket and print the data.
 * @param buffer the place where the data will be stored.
 * @param size number.
 * @return
 */
ssize_t Client::receiveData(char *buffer, size_t size) {
    return socket->receiveData(buffer, size);
}

/**
 * Send the server the id of the client and the id of its cab.
 * Create a string in the format of "number,number".
 */
void Client::sendIds() {
    // Create the string that represents the ids.
    string str = to_string(getId());
    str.append(",");
    str.append(to_string(getCabId()));
    // Send to the server.
    sendData(str);
}

/**
 * Get a cab from the server and set it as cab.
 */
void Client::connectToCab() {
    // Get a cab from the server.
    char buffer[1024];
    receiveData(buffer, sizeof(buffer));
    // De-serialize the cab.
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
    archive::binary_iarchive ia(s);
    Cab *cab = NULL;
    ia >> cab;
    // Set the given cab to the client.
    setCab(cab);
}

/**
 * Operate according to server's messages.
 */
void Client::operate() {
    // Get message from server.
    char buffer[64];
    while (true) {
        // Get a message from the server.
        receiveData(buffer, sizeof(buffer));
        if (!strcmp(buffer, END)) {
            // End-message has been sent, end of the client.
            break;
        } else if (!strcmp(buffer, IS_AVAILABLE)) {
            // Server asked if the driver is available, send yes.
            sendData(YES);
            // Server asks for availability only when ride need to be handled,
            // since we are available, take the ride.
            handleRide();
        } else if (!strcmp(buffer, SEND_LOCATION)) {
            // Server asked for the location, send the location of the driver.
            sendLocationToServer();
        }
        // reset the buffer.
        memset(buffer, 0, 64);
    }
}

/**
 * Get a ride and drive.
 */
void Client::handleRide() {
    // Get a ride from the server and add the needed listeners.
    getRideFromServer();
    // Handle the navigation-process.
    char buffer[64];
    // Get message from server.
    receiveData(buffer, sizeof(buffer));
    if (!strcmp(buffer, SEND_LOCATION)) {
        // Server asked for location, send location.
        sendLocationToServer();
        // Handle the navigation.
        handleNavigation();
        // Start the loop which means the driver is in the middle of a ride.
        drive();
    }
}

/**
 * Get a ride from server.
 */
void Client::getRideFromServer() {
    // Get ride from server.
    char buffer[4096];
    receiveData(buffer, sizeof(buffer));
    // De-serialize the ride.
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<iostreams::basic_array_source<char> > s1(device);
    archive::binary_iarchive ia(s1);
    Ride *ride = NULL;
    ia >> ride;
    // Add listeners to the client according to the ride.
    addListeners(ride);
}

/**
 * Send the location of the client to the server.
 */
void Client::sendLocationToServer() {
    // Serialize the location of the client.
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > stream(inserter);
    archive::binary_oarchive oa(stream);
    Point location = getLocation();
    oa << location;
    stream.flush();
    // Send it to server.
    sendData(serial_str);
}

/**
 * Get and set the navigation.
 */
void Client::handleNavigation() {
    // Get navigation from server.
    char buffer[2048];
    receiveData(buffer, sizeof(buffer));
    // De-serialize the navigation-path.
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<iostreams::basic_array_source<char> > stream(device);
    archive::binary_iarchive ia(stream);
    deque<int> oppositePath;
    ia >> oppositePath;
    // Create path of blocks from the path of ids.
    deque<Block *> *path = new deque<Block *>();
    while (!oppositePath.empty()) {
        // Get the block with the id that on the top of the stack.
        Block *block =
                getCab()->getLocationDetector()->getBlock(oppositePath.back());
        path->push_back(block);
        oppositePath.pop_back();
    }
    // Create new navigation out of this path.
    Navigation *navigation = new BFS(path);
    // Take a ride.
    takeRide(navigation);
}

/**
 * Client is in the middle of a ride.
 */
void Client::drive() {
    char buffer[64];
    while (!getCab()->isArrivedToDestination()) {
        // Driver is in the middle of a ride.
        // Get message from server.
        receiveData(buffer, sizeof(buffer));
        if (!strcmp(buffer, END)) {
            // End-message sent, end of client.
            break;
        } else if (!strcmp(buffer, IS_AVAILABLE)) {
            // Server asked for availability, Send no.
            sendData(NO);
        } else if (!strcmp(buffer, GO)) {
            // Driver got a sign to continue the ride.
            moveOneStep();
        } else if (!strcmp(buffer, SEND_LOCATION)) {
            // Server asked for location, send the location of the driver.
            sendLocationToServer();
        }
        // reset the buffer.
        memset(buffer, 0, 64);
    }
}
