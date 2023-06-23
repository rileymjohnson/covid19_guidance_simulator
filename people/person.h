#ifndef PERSON_H_
#define PERSON_H_

#include <omnetpp.h>
#include "../special/base.h"

using namespace omnetpp;

class Person : public Base {
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void handleParameterChange(const char * parameter_name);
        virtual void finish();
    public:
        enum class types {AUXILIARY_AGENT, PRINCIPAL_AGENT};
};

Define_Module(Person);

#endif /* PERSON_H_ */
