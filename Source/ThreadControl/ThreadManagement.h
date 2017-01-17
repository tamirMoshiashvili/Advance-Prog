
#ifndef EX2_THREADMANAGEMENT_H
#define EX2_THREADMANAGEMENT_H

#include "../Cab/TaxiCenter.h"

/**
 * Contains information about a client.
 */
struct ClientThreadInfo {
    GlobalInfo *globalInfo;
    TaxiCenter *taxiCenter;
    int socket;
    pthread_mutex_t *map_insertion_locker;
};

/**
 * Class which hold a static-function which will be in use for every thread
 * that need to contact with a client.
 */
class ThreadManagement {
public:
    static void *threadFunction(void *param);
};


#endif //EX2_THREADMANAGEMENT_H
