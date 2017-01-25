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
#include <boost/log/trivial.hpp>

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
    CityMap *map = InputManager::readCityMap();
    TaxiCenter *taxiCenter = new TaxiCenter(map);
    MainFlow mainFlow(taxiCenter);
    // Create the object that will hold the mutual info for all drivers.
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    // Run the loop.
    int mission, driverId;
    int numDrivers = 0;
    Ride *ride;
    Cab *cab;
    do {
        // Get input.
        string in;
        getline(cin, in);
        stringstream s(in);
        s >> mission;
        if (s.fail() || !s.eof()) {
            // Invalid command.
            BOOST_LOG_TRIVIAL(debug) << "Invalid command";
            mission = 0;
        }
        cin.clear();
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
                ride = InputManager::readRide(map);
                if (ride != NULL) {
                    mainFlow.addRide(ride);
                } else {
                    cout << "-1\n";
                }
                break;
            case 3:
                // Add new cab.
                cab = InputManager::readCab();
                if (cab != NULL) {
                    mainFlow.addCab(cab);
                } else {
                    cout << "-1\n";
                }
                break;
            case 4:
                // Ask for driver location.
                cin >> driverId;
                if (GlobalInfo::getInstance()->doesDriverExist(driverId)) {
                    globalInfo->updateCommand(mission, driverId);
                    while (!globalInfo->areAllDriversFinishedCommand()) {
                    }
                } else {
                    // Driver does not exist, Invalid input.
                    BOOST_LOG_TRIVIAL(debug) << "Invalid driver id";
                    cout << "-1\n";
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
