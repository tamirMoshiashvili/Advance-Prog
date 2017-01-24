#ifndef EX2_THREADPOOL_H
#define EX2_THREADPOOL_H

#include <queue>
#include <sys/types.h>
#include "Job.h"

using namespace std;

static void *doJobs(void *param);

class ThreadPool {
private:
    queue<Job *> jobs;
    int numThreads;
    pthread_t *threads;
    pthread_mutex_t locker;
    bool stop;

public:
    ThreadPool(int numThreads);

    ~ThreadPool();

    void addJob(Job *job);

    void executeJobs();

    void terminate();

};


#endif //EX2_THREADPOOL_H
