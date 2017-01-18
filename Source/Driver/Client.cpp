#include "Client.h"
#include "../Cab/Vehicle/LuxuryCab.h"
#include "../Cab/Vehicle/StandardCab.h"
#include "../Ride/Navigation/BFS.h"
#include "../Basic/Block/MatrixBlock.h"
#include "../Socket/Tcp/TcpClient.h"
#include "../Ride/Navigation/Navigation.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/export.hpp>
#include <boost/log/trivial.hpp>

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

    BOOST_LOG_TRIVIAL(debug) << "start CTOR of client with id: " << idNum;
    // Handle the socket.
    socket = new TcpClient(ip_addr, port);
    socket->initialize();
    // Connect with server.
    sendIds();
    connectToCab();
    BOOST_LOG_TRIVIAL(debug) << "END CTOR of client with id: " << idNum << endl;
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
    stringstream out;
    out << getId() << "," << getCabId();
    string str = out.str();
    // Send to the server.
    BOOST_LOG_TRIVIAL(debug) << "Send IDs, ";
    sendData(str);
}

/**
 * Get a cab from the server and set it as cab.
 */
void Client::connectToCab() {
    BOOST_LOG_TRIVIAL(debug) << "start connect-to-cab of client with id: " << getId();
    // Get a cab from the server.
    char buffer[1024] = {0};
    BOOST_LOG_TRIVIAL(debug) << "Wait for cab with client: " << getId();
    receiveData(buffer, sizeof(buffer));
    BOOST_LOG_TRIVIAL(debug) << "Wait for cab with client: " << getId() << endl;
    // De-serialize the cab.
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
    archive::binary_iarchive ia(s);
    Cab *cab = NULL;
    ia >> cab;
    // Set the given cab to the client.
    setCab(cab);
    BOOST_LOG_TRIVIAL(debug) << "driver with the id: " << this->getId() << " got the cab" << endl;
}

/**
 * Operate according to server's messages.
 */
void Client::operate() {
    BOOST_LOG_TRIVIAL(debug) << "start Operate of client with id: " << getId();
    // Get message from server.
    char buffer[64] = {0};
    while (true) {
        // Get a message from the server.
        BOOST_LOG_TRIVIAL(debug) << "Wait for msg with client: " << getId();
        receiveData(buffer, sizeof(buffer));
        BOOST_LOG_TRIVIAL(debug) << "Receive for msg with client: " << getId() << endl;
        if (!strcmp(buffer, END)) {
            // End-message has been sent, end of the client.
            break;
        } else if (!strcmp(buffer, IS_AVAILABLE)) {
            // Server asked if the driver is available, send yes.
            BOOST_LOG_TRIVIAL(debug) << buffer;
            BOOST_LOG_TRIVIAL(debug) << "Sent YES-response, ";
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
    BOOST_LOG_TRIVIAL(debug) << "END Operate of client with id: " << getId() << endl;
}

/**
 * Get a ride and drive.
 */
void Client::handleRide() {
    BOOST_LOG_TRIVIAL(debug) << "start handle-ride of client with id: " << getId();
    // Get a ride from the server and add the needed listeners.
    getRideFromServer();
    // Handle the navigation-process.
    char buffer[64] = {0};
    // Get message from server.
    BOOST_LOG_TRIVIAL(debug) << "Wait for msg of snd-location with client: " << getId();
    receiveData(buffer, sizeof(buffer));
    BOOST_LOG_TRIVIAL(debug) << "Receive for msg of snd-location with client: " << getId();
    if (!strcmp(buffer, SEND_LOCATION)) {
        // Server asked for location, send location.
        sendLocationToServer();
        // Handle the navigation.
        handleNavigation();
        // Start the loop which means the driver is in the middle of a ride.
        drive();
    }
    BOOST_LOG_TRIVIAL(debug) << "END handle-ride of client with id: " << getId() << endl;
}

/**
 * Get a ride from server.
 */
void Client::getRideFromServer() {
    BOOST_LOG_TRIVIAL(debug) << "start get-ride of client with id: " << getId();
    // Get ride from server.
    char buffer[4096] = {0};
    BOOST_LOG_TRIVIAL(debug) << "Wait for msg of snd-location with client: " << getId();
    receiveData(buffer, sizeof(buffer));
    BOOST_LOG_TRIVIAL(debug) << "Receive for msg of snd-location with client: " << getId();
    // De-serialize the ride.
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<iostreams::basic_array_source<char> > s1(device);
    archive::binary_iarchive ia(s1);
    Ride *ride = NULL;
    ia >> ride;
    BOOST_LOG_TRIVIAL(debug) << "driver got the ride with id: " << ride->getId();
    // Add listeners to the client according to the ride.
    addListeners(ride);
    BOOST_LOG_TRIVIAL(debug) << "END get-ride of client with id: " << getId() << endl;
}

/**
 * Send the location of the client to the server.
 */
void Client::sendLocationToServer() {
    BOOST_LOG_TRIVIAL(debug) << "start send-location of client with id: " << getId();
    // Serialize the location of the client.
    string serial_str;
    iostreams::back_insert_device<string> inserter(serial_str);
    iostreams::stream<iostreams::back_insert_device<string> > stream(inserter);
    archive::binary_oarchive oa(stream);
    Point location = getLocation();
    oa << location;
    stream.flush();
    // Send it to server.
    BOOST_LOG_TRIVIAL(debug) << "Sent location, ";
    sendData(serial_str);
    BOOST_LOG_TRIVIAL(debug) << "END send-location of client with id: " << getId() << endl;
}

/**
 * Get and set the navigation.
 */
void Client::handleNavigation() {
    BOOST_LOG_TRIVIAL(debug) << "start handle-navigation of client with id: " << getId();
    // Get navigation from server.
    char buffer[65536] = {0};
    BOOST_LOG_TRIVIAL(debug) << "Wait for msg of snd-location with client: " << getId();
    receiveData(buffer, sizeof(buffer));
    BOOST_LOG_TRIVIAL(debug) << "Receive for msg of snd-location with client: " << getId();
    // De-serialize the navigation-path.
    iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    iostreams::stream<iostreams::basic_array_source<char> > stream(device);
    archive::binary_iarchive ia(stream);
    deque<string> *string_path;
    ia >> string_path;
    // Create new navigation out of this path.
    Navigation *navigation = new Navigation(string_path);
    // Take a ride.
    takeRide(navigation);
    delete string_path;
    BOOST_LOG_TRIVIAL(debug) << "END handle-navigation of client with id: " << getId() << endl;
}

/**
 * Client is in the middle of a ride.
 */
void Client::drive() {
    BOOST_LOG_TRIVIAL(debug) << "start DRIVE of client with id: " << getId();
    char buffer[64] = {0};
    while (!getCab()->isArrivedToDestination()) {
        // Driver is in the middle of a ride.
        // Get message from server.
        BOOST_LOG_TRIVIAL(debug) << "WAIT for data with id: " << getId();
        receiveData(buffer, sizeof(buffer));
        BOOST_LOG_TRIVIAL(debug) << "RECEIVE for data with id: " << getId();
        if (!strcmp(buffer, END)) {
            // End-message sent, end of client.
            break;
        } else if (!strcmp(buffer, IS_AVAILABLE)) {
            // Server asked for availability, Send no.
            BOOST_LOG_TRIVIAL(debug) << "Sent NO-response, ";
            sendData(NO);
        } else if (!strcmp(buffer, GO)) {
            // Driver got a sign to continue the ride.
            BOOST_LOG_TRIVIAL(debug) << "driver should move one step";
            moveOneStep();
        } else if (!strcmp(buffer, SEND_LOCATION)) {
            // Server asked for location, send the location of the driver.
            sendLocationToServer();
        }
        // reset the buffer.
        memset(buffer, 0, 64);
    }
    BOOST_LOG_TRIVIAL(debug) << "END DRIVE of client with id: " << getId() << endl;
}
