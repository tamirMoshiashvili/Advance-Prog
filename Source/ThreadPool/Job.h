
#ifndef EX2_JOB_H
#define EX2_JOB_H


class Job {

public:
    Job();

    virtual ~Job();

    virtual void execute()=0;

};


#endif //EX2_JOB_H
