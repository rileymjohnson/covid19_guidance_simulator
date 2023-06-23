#include <omnetpp.h>
#include "room.h"

using namespace omnetpp;

void Room::initialize() {
    // initialize
}

void Room::handleMessage(cMessage *msg) {
    delete msg;
}

void Room::handleParameterChange(const char * parameter_name) {
    // hand parameter change
}

void Room::finish() {
    // finish
}

int Room::size_enum_to_int(Room::sizes size) {
    switch (size) {
    case Room::sizes::SMALL:
        return 1;
    case Room::sizes::MEDIUM:
        return 2;
    case Room::sizes::LARGE:
        return 3;
    }

    throw cRuntimeError("Invalid room size");
}
