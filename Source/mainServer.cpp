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

using namespace std;
using namespace boost;


static void operate(uint16_t port);

int main(int argc, char **argv) {
    string port = argv[1];
    operate((uint16_t) atoi(port.c_str()));
    delete GlobalInfo::getInstance();
    return 0;
}

/**
 * Read input from user and operate according to the missions.
 * @param port port number.
 */
static void operate(uint16_t port) {
    // Create objects.
    CityMap *map;
    do {
        map = InputManager::readCityMap();
    } while (map == NULL);
    TaxiCenter *taxiCenter = new TaxiCenter(map);
    MainFlow mainFlow(taxiCenter);
    // Create the object that will hold the mutual info for all drivers.
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    // Run the loop.
    int mission, driverId;
    int numDrivers = 0;
    do {
        // Get input.
        cin >> mission;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Determine which mission needed to be executed.
        switch (mission) {
            case 1:
                // Connect with drivers.
                cin >> numDrivers;
                mainFlow.addDrivers(numDrivers, port);
                globalInfo->updateCommand(mission);
                while (!globalInfo->areAllDriversFinishedCommand()) {
                }
                break;
            case 2:
                // Add new ride.
                Ride *ride = InputManager::readRide(map);
                if (ride != NULL) {
                    mainFlow.addRide(ride);
                }
                break;
            case 3:
                // Add new cab.
                Cab *cab = InputManager::readCab();
                if (cab != NULL) {
                    mainFlow.addCab(cab);
                }
                break;
            case 4:
                // Ask for driver location.
                cin >> driverId;
                //TODO: if driver id doesn't exist we should print -1?
                globalInfo->updateCommand(mission, driverId);
                while (!globalInfo->areAllDriversFinishedCommand()) {
                }
                break;
            case 7:
                break;
            case 9:
                mainFlow.advanceClock();
                // Wait for the flag.
                while (globalInfo->isFlagTurnOn()) {}
                // Advance.
                globalInfo->updateCommand(mission);
                while (!globalInfo->areAllDriversFinishedCommand()) {
                }
                break;
            default:
                // Invalid option.
                cout << "-1\n";
                break;
        }
    } while (mission != 7);
    // Announce about end of program.
    globalInfo->updateCommand(mission);
    pthread_exit(NULL);
}
