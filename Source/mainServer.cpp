#include <iostream>
#include <limits>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "Cab/TaxiCenter.h"
#include "Control/MainFlow.h"
#include "Input/InputManager.h"
#include "Socket/Tcp/TcpServer.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

static void operate(uint16_t port);


int main(int argc, char **argv) {
    string port = argv[1];
    TcpServer *s = new TcpServer((uint16_t) atoi(port.c_str()),1);
    s->initialize();
    vector<int> *c = s->getClientDescriptors();
    char buffer[32];
    for (int i = 0; i < c->size(); ++i) {
        s->sendData("hello", (*c)[i]);
        s->receiveData(buffer, sizeof(buffer),(*c)[i]);
        cout << buffer;
    }
    delete s;
    //operate((uint16_t) atoi(port.c_str()));
    return 0;
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
