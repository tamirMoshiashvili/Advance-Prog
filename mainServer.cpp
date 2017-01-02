#include <iostream>
#include <limits>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "Source/Cab/TaxiCenter.h"
#include "Source/Control/MainFlow.h"
#include "Source/Input/InputManager.h"
#include "Source/Ride/Navigation/BFS.h"
#include "Source/Basic/Block/MatrixBlock.h"
#include "Source/Cab/Vehicle/StandardCab.h"
#include "Source/Cab/Vehicle/LuxuryCab.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>

using namespace std;
using namespace boost;

static void operate(uint16_t port);

// hello tamir


int main(int argc, char **argv) {
    string port = argv[1];
    operate((uint16_t) atoi(port.c_str()));
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
