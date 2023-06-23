#include "globals.h"

#include <omnetpp.h>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <string>

void Globals::initialize() {
    start_date.tm_isdst = -1;
    start_date.tm_min = 0;
    start_date.tm_hour = 0;

    std::string start_date_string = par("start_date_string").stdstringValue();
    std::stringstream start_date_stream = std::stringstream(start_date_string);
    start_date_stream >> std::get_time(&start_date, "%Y-%m-%d");

    start_date.tm_sec = simTime().inUnit(SimTimeUnit::SIMTIME_S);
    std::mktime(&start_date);

    // std::tm indexes days from 1-7. The "+6" subtracts 1 so that it
    // is indexed from 0 and then adds 7 so that the value will always
    // be positive, which ensures that the mode gives the correct result
    start_day_offset = (start_date.tm_wday + 6) % 7;

    simulation_listener = new utils::SimulationListener();
}

void Globals::handleMessage(cMessage *msg) {
    delete msg;
}

void Globals::finish() {
    delete simulation_listener;
}
