#ifndef EX2_MAINFLOW_H
#define EX2_MAINFLOW_H


#include "../Cab/TaxiCenter.h"
#include <string>

using namespace std;

/**
 * Represents the manager of the program.
 */
class MainFlow {
private:
    TaxiCenter *taxiCenter;

public:
    MainFlow(TaxiCenter *center);

    ~MainFlow();

    void addDrivers(int numDrivers, uint16_t port);

    void addCab(Cab *cab);

    void addRide(Ride *ride);

    Point requestDriverLocation(int id);

    void operateTaxiCenter();
};


#endif //EX2_MAINFLOW_H
