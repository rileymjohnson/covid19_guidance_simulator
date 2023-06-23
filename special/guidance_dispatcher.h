#ifndef GUIDANCE_DISPATCHER_H_
#define GUIDANCE_DISPATCHER_H_

#include <omnetpp.h>

#include "base.h"

using namespace omnetpp;

class GuidanceDispatcher : public Base {
    protected:
        int num_people;
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
};

Define_Module(GuidanceDispatcher)

#endif /* GUIDANCE_DISPATCHER_H_ */
