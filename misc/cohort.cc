#include <omnetpp.h>
#include "cohort.h"

void Cohort::initialize() {
    // initialize
}

void Cohort::handleMessage(cMessage *msg) {
    delete msg;
}

void Cohort::handleParameterChange(const char * parameter_name) {
    // hand parameter change
}

void Cohort::finish() {
    // finish
}
