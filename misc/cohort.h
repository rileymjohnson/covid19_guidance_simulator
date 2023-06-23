#ifndef COHORT_H_
#define COHORT_H_

#include <omnetpp.h>
#include "../special/base.h"

using namespace omnetpp;

class Cohort : public Base {
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void handleParameterChange(const char * parameter_name);
        virtual void finish();
};

Define_Module(Cohort);

#endif /* COHORT_H_ */
