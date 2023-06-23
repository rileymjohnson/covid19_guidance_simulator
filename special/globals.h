#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <omnetpp.h>
#include <ctime>

#include "../utils/simulation_listener.h"

using namespace omnetpp;

class Globals : public cSimpleModule {
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
    public:
        std::tm start_date;
        int start_day_offset;
        utils::SimulationListener * simulation_listener;
};

Define_Module(Globals);

#endif /* GLOBALS_H_ */
