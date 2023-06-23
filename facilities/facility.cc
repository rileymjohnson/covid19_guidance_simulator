#include <omnetpp.h>
#include <string>
#include <iterator>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <array>
#include <cmath>
#include <omnetpp.h>
#include <omnetpp/cexception.h>

#include "facility.h"

using namespace omnetpp;

int day_of_year_to_month(int day_of_year) {
    // These numbers are the number of days in each month cumulatively summed
    if (day_of_year < 31) return 0;
    if (day_of_year < 59) return 1;
    if (day_of_year < 90) return 2;
    if (day_of_year < 120) return 3;
    if (day_of_year < 151) return 4;
    if (day_of_year < 181) return 5;
    if (day_of_year < 212) return 6;
    if (day_of_year < 243) return 7;
    if (day_of_year < 273) return 8;
    if (day_of_year < 304) return 9;
    if (day_of_year < 334) return 10;
    if (day_of_year < 365) return 11;

    throw cRuntimeError("The day of year must be between 0 and 364");
}

bool day_of_year_is_first_day_of_month(int day_of_year) {
    return (
            day_of_year == 0 ||
            day_of_year == 31 ||
            day_of_year == 59 ||
            day_of_year == 90 ||
            day_of_year == 120 ||
            day_of_year == 151 ||
            day_of_year == 181 ||
            day_of_year == 212 ||
            day_of_year == 243 ||
            day_of_year == 273 ||
            day_of_year == 304 ||
            day_of_year == 334
    );
}

bool day_of_year_is_last_day_of_month(int day_of_year) {
    return (
            day_of_year == 30 ||
            day_of_year == 58 ||
            day_of_year == 89 ||
            day_of_year == 119 ||
            day_of_year == 150 ||
            day_of_year == 180 ||
            day_of_year == 211 ||
            day_of_year == 242 ||
            day_of_year == 272 ||
            day_of_year == 303 ||
            day_of_year == 333 ||
            day_of_year == 364
    );
}

std::string get_month_name(int month) {
    if (month == 0) return "January";
    if (month == 1) return "February";
    if (month == 2) return "March";
    if (month == 3) return "April";
    if (month == 4) return "May";
    if (month == 5) return "June";
    if (month == 6) return "July";
    if (month == 7) return "August";
    if (month == 8) return "September";
    if (month == 9) return "October";
    if (month == 10) return "November";
    if (month == 11) return "December";

    throw cRuntimeError("The month must be between 0 and 11");
}

std::string get_day_of_week_name(int day_of_week) {
    if (day_of_week == 0) return "Monday";
    if (day_of_week == 1) return "Tuesday";
    if (day_of_week == 2) return "Wednesday";
    if (day_of_week == 3) return "Thursday";
    if (day_of_week == 4) return "Friday";
    if (day_of_week == 5) return "Saturday";
    if (day_of_week == 6) return "Sunday";

    throw cRuntimeError("The day of week must be between 0 and 6");
}



void Facility::initialize() {
    cMessage * msg = new cMessage("test");
    scheduleAfter(days(0), msg);

    if (get_facility_type() == Facility::types::PRIMARY_FACILITY) {
        /*
        cRectangleFigure *small_room = new cRectangleFigure("small_room");
        small_room->setBounds(cFigure::Rectangle(5,5,40,40));
        small_room->setFilled(true);
        small_room->setFillColor("#C4A484");
        small_room->setZIndex(-1);
        getParentModule()->getCanvas()->addFigure(small_room);

        cRectangleFigure *medium_room = new cRectangleFigure("medium_room");
        medium_room->setBounds(cFigure::Rectangle(75,40,80,40));
        medium_room->setFilled(true);
        medium_room->setFillColor("#C4A484");
        medium_room->setZIndex(-1);
        getParentModule()->getCanvas()->addFigure(medium_room);
        */
    }
}

void Facility::handleMessage(cMessage *msg) {
    log_info("handling message");

    simtime_t sim_time = simTime();
    int day_of_year = div(sim_time, days(1)) % 365;
    int week_of_year = div(sim_time, days(7)) % 52;
    int day_of_week = (div(sim_time, days(1)) + get_start_day_offset()) % 7;
    int month_of_year = day_of_year_to_month(day_of_year);

    EV_INFO << "Day of year: " << day_of_year << "\n";
    EV_INFO << "Day of week: " << day_of_week << "\n";
    EV_INFO << "Week of year: " << week_of_year << "\n";
    EV_INFO << "Day of week name: " << get_day_of_week_name(day_of_week) << "\n";
    EV_INFO << "Month of year: " << month_of_year << "\n";
    EV_INFO << "Month of year name: " << get_month_name(month_of_year) << "\n";

    EV_INFO << "XXX " << week_of_year << " | " << day_of_week << " | " << day_of_year << "\n";

    if (day_of_week == 0) {
        change_icon_size_and_color(Facility::iconSizes::VS, Facility::iconColors::GREEN);
    } else if (day_of_week == 4) {
        change_icon_size_and_color(Facility::iconSizes::VL, Facility::iconColors::ORANGE);
    }

    scheduleAfter(days(1), msg->dup());

    delete msg;
}

void Facility::finish() {
    // finish
}

Facility::types Facility::get_facility_type() {
    switch(par("facility_type").intValue()) {
    case 0:
        return Facility::types::PRIMARY_FACILITY;
    case 1:
        return Facility::types::SECONDARY_FACILITY;
    case 2:
        return Facility::types::TERTIARY_FACILITY;
    }

    throw cRuntimeError("The facility type is not valid");
}

int Facility::num_principal_agents() {
    return this->gate("principal_agents$o", 0)->size();
}

int Facility::num_auxiliary_agents() {
    return this->gate("auxiliary_agents$o", 0)->size();
}
