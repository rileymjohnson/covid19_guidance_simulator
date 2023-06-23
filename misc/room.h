#ifndef ROOM_H_
#define ROOM_H_

#include <omnetpp.h>
#include "../special/base.h"

using namespace omnetpp;

class Room : public Base {
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void handleParameterChange(const char * parameter_name);
        virtual void finish();
    public:
        enum class sizes { SMALL, MEDIUM, LARGE };
        int size_enum_to_int(Room::sizes size);
};

Define_Module(Room);

#endif /* ROOM_H_ */
