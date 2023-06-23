#include <iomanip>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <omnetpp.h>

#include "base.h"
#include "globals.h"

using namespace omnetpp;

std::string Base::size_to_string(Base::iconSizes size) {
    std::string size_string;

    switch(size) {
    case Base::iconSizes::VS:
        size_string = "vs";
        break;
    case Base::iconSizes::S:
        size_string = "s";
        break;
    case Base::iconSizes::N:
        size_string = "n";
        break;
    case Base::iconSizes::L:
        size_string = "l";
        break;
    case Base::iconSizes::VL:
        size_string = "vl";
        break;
    case Base::iconSizes::MAX:
        size_string = "512x512";
        break;
    }

    return size_string;
}

std::string Base::color_to_string(Base::iconColors color) {
    std::string color_string;

    switch(color) {
    case Base::iconColors::DARK_GRAY:
        color_string = "dark_gray";
        break;
    case Base::iconColors::LIGHT_GRAY:
        color_string = "light_gray";
        break;
    case Base::iconColors::BLACK:
        color_string = "black";
        break;
    case Base::iconColors::BLUE:
        color_string = "blue";
        break;
    case Base::iconColors::GREEN:
        color_string = "green";
        break;
    case Base::iconColors::ORANGE:
        color_string = "orange";
        break;
    case Base::iconColors::PINK:
        color_string = "pink";
        break;
    case Base::iconColors::PURPLE:
        color_string = "purple";
        break;
    case Base::iconColors::RED:
        color_string = "red";
        break;
    case Base::iconColors::YELLOW:
        color_string = "yellow";
        break;
    }

    return color_string;
}

cModule * Base::get_globals() {
    return getSystemModule()->getSubmodule("globals");
}

int Base::get_start_day_offset() {
    cModule * globals = get_globals();
    return check_and_cast<Globals *>(globals)->start_day_offset;
}

tm Base::get_start_date() {
    cModule * globals = get_globals();
    return check_and_cast<Globals *>(globals)->start_date;
}

tm Base::get_end_date() {
    std::tm start_date = get_start_date();
    start_date.tm_sec += get_simulation_length_seconds();
    std::mktime(&start_date);

    return start_date;
}

tm Base::get_date() {
    std::tm date = get_start_date();
    date.tm_sec += simTime().raw();
    std::mktime(&date);

    return date;
}

std::string Base::get_date_string() {
    std::tm date = get_date();
    std::stringstream date_string;
    date_string << std::put_time(&date, "%Y-%m-%d %H:%M");
    return date_string.str();
}

std::string Base::get_pretty_date_string() {
    std::tm date = get_date();
    std::stringstream date_string;
    date_string << std::put_time(&date, "%B %d, %Y %H:%M (%A)");
    return date_string.str();
}

std::string Base::get_day_of_week() {
    std::tm date = get_date();
    std::stringstream date_string;
    date_string << std::put_time(&date, "%A");

    return date_string.str();
}

int64_t Base::seconds(int n) {
    return simTime().getScale() * n;
}

int64_t Base::minutes(int n) {
    return simTime().getScale() * n * 60;
}

int64_t Base::hours(int n) {
    return simTime().getScale() * 3600; // 60 * 60
}

int64_t Base::days(int n) {
    return simTime().getScale() * n * 86400; // 60 * 60 * 24
}

int64_t Base::weeks(int n) {
    return simTime().getScale() * n * 604800; // 60 * 60 * 24 * 7
}

int64_t Base::get_simulation_length_seconds() {
    cConfiguration * config = getEnvir()->getConfig();
   const char * time_limit_string = config->getConfigValue("sim-time-limit");
   simtime_t time_limit = SimTime::parse(time_limit_string);

   return time_limit.inUnit(SimTimeUnit::SIMTIME_S);
}

void Base::log_info(std::string info) {
    EV_INFO << "[" << get_date_string() << "] " << info << endl;
}

void Base::log_info(char * info) {
    EV_INFO << "[" << get_date_string() << "] " << info << endl;
}

void Base::on_parameter_change(std::string par_name) {
    EV_INFO << "Parameter changed: " << par_name << "\n";
}

void Base::on_event(cMessage * msg) {
    EV_INFO << "On event!\n";
}

void Base::on_message(cMessage * msg) {
    EV_INFO << "On message!\n";
}

void Base::handleParameterChange(const char * parname) {
    if (strcmp(parname, "config") != 0) {
        std::string par_name(parname);
        on_parameter_change(par_name);
    }
}

void Base::handleMessage(cMessage * msg) {
    if (msg->isSelfMessage()) {
        on_event(msg);
    } else {
        on_message(msg);
    }
}

std::string Base::get_package() {
    return getSimulation()->getNedPackageForFolder(
        getComponentType()->getSourceFileDirectory()
    );
}

std::string Base::get_package_stub() {
    std::string package = get_package();

    std::string root_package = getSimulation()->getNedPackageForFolder(
        getSystemModule()->getComponentType()->getSourceFileDirectory()
    );

    if (package.rfind(root_package, 0) == 0 && package.size() > root_package.size()) {
        package = package.substr(root_package.size() + 1);
    }

    std::string::size_type pos = 0;

    while (true) {
        pos = package.find(".", pos);

        if (pos == std::string::npos) {
            break;
        }

        package.replace(pos, 1, "/");
    }

    return package;
}

std::string Base::get_asset_name() {
    std::string file_path(
        getComponentType()->getSourceFileName()
    );

    std::string::size_type name_pos = file_path.rfind("/");

    if (name_pos == std::string::npos) {
        name_pos = file_path.rfind("\\");

        if (name_pos == std::string::npos) {
            name_pos = -1;
        }
    }

    std::string::size_type dot_pos = file_path.rfind(".");

    if (dot_pos == std::string::npos) {
        return std::string(file_path.begin() + name_pos + 1, file_path.end());
    }

    return std::string(file_path.begin() + name_pos + 1, file_path.begin() + dot_pos);
}

void Base::change_icon_size(Base::iconSizes size) {
    std::string size_string = Base::size_to_string(size);

    change_icon_size_and_color(
        size_string,
        par("color").stdstringValue()
    );
}

void Base::change_icon_size(std::string size) {
    change_icon_size_and_color(
        size,
        par("color").stdstringValue()
    );
}

void Base::change_icon_color(Base::iconColors color) {
    std::string color_string = Base::color_to_string(color);

    change_icon_size_and_color(
        par("size").stdstringValue(),
        color_string
    );
}

void Base::change_icon_color(std::string color) {
    change_icon_size_and_color(
        par("size").stdstringValue(),
        color
    );
}

void Base::change_icon_size_and_color(Base::iconSizes size, Base::iconColors color) {
    std::string size_string = Base::size_to_string(size);
    std::string color_string = Base::Base::color_to_string(color);

    change_icon_size_and_color(size_string, color_string);
}

void Base::change_icon_size_and_color(std::string size, std::string color) {
    std::stringstream new_icon_name;

    new_icon_name
        << "icons/"
        << get_package_stub()
        << "/"
        << get_asset_name()
        << "/"
        << size
        << "/"
        << color;

    std::string new_icon_string = new_icon_name.str();

    getDisplayString().setTagArg("i", 0, new_icon_string.c_str());
}
