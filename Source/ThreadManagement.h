
#ifndef EX2_THREADMANAGEMENT_H
#define EX2_THREADMANAGEMENT_H

#include "Cab/TaxiCenter.h"

struct ClientThreadInfo {
    GlobalInfo *globalInfo;
    TaxiCenter *taxiCenter;
    int socket;
    pthread_mutex_t* map_insertion_locker;
    pthread_mutex_t* map_itearation_locker;
};

class ThreadManagement {

public:
    ThreadManagement();

    static void *threadFunction(void *param);

    ~ThreadManagement();
};


#endif //EX2_THREADMANAGEMENT_H
