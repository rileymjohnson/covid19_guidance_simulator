#ifndef FACILITY_H_
#define FACILITY_H_

#include <omnetpp.h>
#include "../special/base.h"

using namespace omnetpp;

class Facility : public Base {
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
    public:
        enum class types { PRIMARY_FACILITY, SECONDARY_FACILITY, TERTIARY_FACILITY };
        Facility::types get_facility_type();
        int num_principal_agents();
        int num_auxiliary_agents();
};

Define_Module(Facility);

#endif /* FACILITY_H_ */
