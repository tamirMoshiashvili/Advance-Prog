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
    do {
        // Get input.
        cin >> mission;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Determine which mission needed to be executed.
        switch (mission) {
            case 1:
                // Connect with drivers.
                cin >> numDrivers;
                mainFlow.addDrivers(numDrivers, port, globalInfo);
                break;
            case 2:
                // Add new ride.
                mainFlow.addRide(InputManager::readRide());
                break;
            case 3:
                // Add new cab.
                mainFlow.addCab(InputManager::readCab(taxiCenter));
                //TODO: wait for all command 1,2,3 to execute before updating
                //TODO: commands to threads.
                break;
            case 4:
                // Ask for driver location.
                globalInfo->setAllDriversToNotFinish();
                cin >> driverId;
                globalInfo->updateCommand(mission, driverId);
                while (!globalInfo->areAllDriversFinishedCommand()) {
                }
                BOOST_LOG_TRIVIAL(debug) << "all drivers finish command" << endl;
                break;
            case 9:
                // Advance.
                globalInfo->setAllDriversToNotFinish();
                globalInfo->updateCommand(mission);
                while (!globalInfo->areAllDriversFinishedCommand()) {
                }
                mainFlow.advanceClock();
                BOOST_LOG_TRIVIAL(debug) << "all drivers finish command" << endl;
                break;
            default:
                // Invalid option.
                break;
        }
    } while (mission != 7);
    // Announce about end of program.
    globalInfo->setAllDriversToNotFinish();
    globalInfo->updateCommand(mission);
    pthread_exit(NULL);
}

