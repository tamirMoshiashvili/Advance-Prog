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

    void operateTaxiCenter(uint16_t port, CityMap *map);
};


#endif //EX2_MAINFLOW_H
