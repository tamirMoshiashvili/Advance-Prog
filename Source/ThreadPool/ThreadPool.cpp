#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <boost/log/trivial.hpp>
#include "ThreadPool.h"

/**
 * Execute all jobs.
 * @param param pointer to this class.
 * @return
 */
static void *doJobs(void *param) {
    ThreadPool *threadPool = (ThreadPool *) param;
    threadPool->executeJobs();
    return NULL;
}

/**
 * Contructor. Create threads as the number was given.
 * @param numThreads
 * @return
 */
ThreadPool::ThreadPool(int numThreads) : numThreads(numThreads), stop(false) {
    pthread_mutex_init(&locker, NULL);
    int status;
    threads = new pthread_t[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        status = pthread_create(threads+i, NULL, doJobs, this);
        if (status) {
            cout << "ERROR" << endl;
        }
    }
}

/**
 * Destructor.
 */
ThreadPool::~ThreadPool() {
    pthread_mutex_destroy(&locker);
    delete[] threads;
}

/**
 * Execute the jobs from the queue of jobs.
 */
void ThreadPool::executeJobs() {
    while (!stop) {
        pthread_mutex_lock(&locker);
        if (!jobs.empty()) {
            // Pop a job from the queue.
            Job *job = jobs.front();
            jobs.pop();
            pthread_mutex_unlock(&locker);
            job->execute();
            // After executing the job we can delete the job.
            delete job;
        } else {
            pthread_mutex_unlock(&locker);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

/**
 * Add job to the queue.
 * @param job
 */
void ThreadPool::addJob(Job *job) {
    jobs.push(job);
}

/**
 * Terminate the working threads.
 */
void ThreadPool::terminate() {
    stop = true;
    // Wait for them to finish their job before closing them.
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(*(threads+i), NULL);
    }
}
