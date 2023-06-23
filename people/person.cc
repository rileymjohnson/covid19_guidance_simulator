#include <omnetpp.h>
#include "person.h"

void Person::initialize() {
    // initialize
    //cMessage * msg = new cMessage("test");
    //scheduleAfter(weeks(2), msg);
}

void Person::handleMessage(cMessage *msg) {
    par("has_chills").setBoolValue(true);
    log_info("handling message");
    delete msg;
}

void Person::handleParameterChange(const char * parameter_name) {
    EV_INFO << parameter_name << "\n";
}

void Person::finish() {
    // finish
}
