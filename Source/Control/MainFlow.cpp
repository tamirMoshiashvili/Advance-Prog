#include <iostream>
#include <sstream>
#include "MainFlow.h"
#include "../Input/InputManager.h"

using namespace std;

/**
 * Constructor.
 * @param center pointer to taxi center.
 * @return MainFlow object.
 */
MainFlow::MainFlow(TaxiCenter *center) : taxiCenter(center) {

}

/**
 * Destructor.
 */
MainFlow::~MainFlow() {
    delete taxiCenter;
}

/**
 * Read input from user and operate according to the missions.
 * @param port port number.
 * @param map city map.
 */
void MainFlow::operateTaxiCenter(uint16_t port, CityMap *map) {
    // Create the object that will hold the mutual info for all drivers.
    GlobalInfo *globalInfo = GlobalInfo::getInstance();
    // Run the loop.
    int mission, driverId;
    int numDrivers = 0;
    do {
        // Get input.
        string in;
        getline(cin, in);
        stringstream s(in);
        s >> mission;
        if (s.fail() || !s.eof()) {
            // Invalid command.
            mission = 0;
            cin.clear();
        }
        // Determine which mission needed to be executed.
        switch (mission) {
            case 1:
            {
                // Connect with drivers.
                string num;
                getline(cin, num);
                stringstream s2(num);
                s2 >> numDrivers;
                if (s2.fail() || !s2.eof()) {
                    // Invalid number of drivers.
                    cout << "-1\n";
                    cin.clear();
                } else {
                    // Add drivers.
                    taxiCenter->initialize(numDrivers, port);
                    globalInfo->updateCommand(mission);
                    while (!globalInfo->areAllDriversFinishedCommand()) {
                    }
                }
                break;
            }
            case 2:
            {
                // Add new ride.
                Ride *ride = InputManager::readRide(map);
                if (ride != NULL) {
                    taxiCenter->addRide(ride);
                } else {
                    cin.clear();
                }
                break;
            }
            case 3:
            {
                // Add new cab.
                Cab *cab = InputManager::readCab();
                if (cab != NULL) {
                    taxiCenter->addCab(cab);
                } else {
                    cout << "-1\n";
                    cin.clear();
                }
                break;
            }
            case 4:
            {
                // Ask for driver location.
                string id;
                getline(cin, id);
                stringstream id_s(id);
                id_s >> driverId;
                if (id_s.fail() || !id_s.eof()) {
                    // Invalid driver id.
                    cout << "-1\n";
                    cin.clear();
                } else {
                    if (GlobalInfo::getInstance()->doesDriverExist(driverId)) {
                        globalInfo->updateCommand(mission, driverId);
                        while (!globalInfo->areAllDriversFinishedCommand()) {
                        }
                    } else {
                        // Driver does not exist, Invalid input.
                        cout << "-1\n";
                    }
                }
                break;
            }
            case 7:
                break;
            case 9:
                taxiCenter->advanceClock();
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
