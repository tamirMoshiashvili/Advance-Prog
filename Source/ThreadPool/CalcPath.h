#ifndef EX2_CALCPATH_H
#define EX2_CALCPATH_H


#include "Job.h"
#include "../Ride/Ride.h"
#include "../Map/CityMap.h"

/**
 * A task that calculate path.
 */
class CalcPath : public Job {
private:
    Ride *ride;
    CityMap *cityMap;
    pthread_mutex_t *locker;

public:
    CalcPath(Ride* ride, CityMap *map, pthread_mutex_t* lock);

    ~CalcPath();

    void execute();
};


#endif //EX2_CALCPATH_H
