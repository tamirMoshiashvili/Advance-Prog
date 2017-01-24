
#ifndef EX2_JOB_H
#define EX2_JOB_H

/**
 * A task that can be executed.
 */
class Job {

public:
    Job();

    virtual ~Job();

    virtual void execute()=0;

};


#endif //EX2_JOB_H
