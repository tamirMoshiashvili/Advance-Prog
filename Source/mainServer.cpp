#include <iostream>
#include <limits>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "Cab/TaxiCenter.h"
#include "Control/MainFlow.h"
#include "Input/InputManager.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <pthread.h>

using namespace std;
using namespace boost;

struct Param {
    TcpServer *tcp;
    int socket;
};

struct Param1 {
    Param param;
    string str;
};

static void operate(uint16_t port);

void *receiveSignal(void *p);

void *sendString(void *p);

int main(int argc, char **argv) {
    pthread_t t1, t2;
    string port = argv[1];
    TcpServer *tcpServer = new TcpServer((uint16_t) atoi(port.c_str()), 2);
    tcpServer->initialize();
    vector<int> *clients = tcpServer->getClientDescriptors();
    Param p;
    p.tcp = tcpServer;
    p.socket = (*clients)[0];
    int status = pthread_create(&t1, NULL, receiveSignal, (void *) &p);
    if (status) {
        cout << "ERROR\n";
    }
    pthread_join(t1, NULL);
    // send hello to first client.
    Param1 p1;
    p1.param = p;
    p1.str = "hello first client\n";
    status = pthread_create(&t1, NULL, sendString, (void *) &p1);
    if (status) {
        cout << "ERROR\n";
    }
    Param p2;
    p2.tcp = tcpServer;
    p2.socket = (*clients)[1];
    Param1 param2;
    param2.param = p2;
    param2.str = "hello second client\n";
    status = pthread_create(&t2, NULL, sendString, (void *) &param2);
    if (status) {
        cout << "ERROR\n";
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    delete tcpServer;
//    operate((uint16_t) atoi(port.c_str()));
    return 0;
}

void *receiveSignal(void *p) {
    Param *param = (Param *) p;
    char buffer[64] = {0};
    cout << "waiting for message\n";
    param->tcp->receiveData(buffer, sizeof(buffer), param->socket);
    cout << "message received\n";
    string str(buffer);
    cout << str;
}

void *sendString(void *p) {
    Param1 *param1 = (Param1 *) p;
    param1->param.tcp->sendData(param1->str, param1->param.socket);
}

/**
 * Read input from user and operate according to the missions.
 * @param port port number.
 */
static void operate(uint16_t port) {
    // Create objects.
    CityMap *map = InputManager::readCityMap();
    TaxiCenter *taxiCenter = new TaxiCenter(map);
    MainFlow mainFlow(taxiCenter);
    // Run the loop.
    int mission;
    int numDrivers = 0;
    do {
        // Get input.
        cin >> mission;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Determine which mission need to be executed.
        switch (mission) {
            case 1:
                cin >> numDrivers;
                mainFlow.addDrivers(numDrivers, port);
                break;
            case 2:
                mainFlow.addRide(InputManager::readRide());
                break;
            case 3:
                mainFlow.addCab(InputManager::readCab(taxiCenter));
                break;
            case 4:
                cout << mainFlow.requestDriverLocation(
                        InputManager::readNumber());
                break;
            case 9:
                mainFlow.operateTaxiCenter();
                break;
            default:
                break;
        }
    } while (mission != 7);
}
