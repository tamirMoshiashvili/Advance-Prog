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
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    // Run the loop.
    int mission, driverId;
    int numDrivers = 0;
    do {
        // Get input.
        cin >> mission;
//        globalInfo->updateCommand(mission);
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Determine which mission need to be executed.
        switch (mission) {
            case 1:
                cin >> numDrivers;
                mainFlow.addDrivers(numDrivers, port, globalInfo);
                break;
            case 2:
                mainFlow.addRide(InputManager::readRide());
                break;
            case 3:
                mainFlow.addCab(InputManager::readCab(taxiCenter));
                break;
            case 4:
                globalInfo->setAllDriversToNotFinish();
                cout << "set all drivers to not finish command\n";
                cin >> driverId;
                globalInfo->updateCommand(mission, driverId);
                while (!globalInfo->areAllDriversFinishedCommand()) {
                }
                cout<<"all drivers finish command\n";
                break;
//            case 9:
//                mainFlow.operateTaxiCenter();
//                break;
            default:
                break;
        }
    } while (mission != 7);
    //TODO: wait for all threads to terminate.
    cout << "exit program\n";
}
