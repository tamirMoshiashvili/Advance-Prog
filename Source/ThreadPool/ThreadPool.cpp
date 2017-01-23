//
// Created by eden on 21/01/17.
//

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <boost/log/trivial.hpp>
#include "ThreadPool.h"

static void *doJobs(void *param) {
    ThreadPool *threadPool = (ThreadPool *) param;
    threadPool->executeJobs();
    return NULL;
}

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


ThreadPool::~ThreadPool() {
    pthread_mutex_destroy(&locker);
    delete[] threads;
}


void ThreadPool::executeJobs() {
    while (!stop) {
        pthread_mutex_lock(&locker);
        if (!jobs.empty()) {
            BOOST_LOG_TRIVIAL(debug) <<"about to execute job";
            Job *job = jobs.front();
            jobs.pop();
            pthread_mutex_unlock(&locker);
            job->execute();
            BOOST_LOG_TRIVIAL(debug) <<"finish executing the job";
            delete job;
        } else {
            pthread_mutex_unlock(&locker);
            sleep(1);
        }
    }
    pthread_exit(NULL);
}


void ThreadPool::addJob(Job *job) {
    jobs.push(job);
}


void ThreadPool::terminate() {
    stop = true;
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(*(threads+i), NULL);
    }
}
