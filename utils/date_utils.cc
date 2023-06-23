#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

#include "date_utils.h"

namespace utils {
std::tm parse_date_string(std::string date_string) {
    std::tm date;
    std::stringstream date_stream(date_string);
    date_stream >> std::get_time(&date, "%Y-%m-%d");
    return date;
}

std::string date_to_string(std::tm date) {
    std::stringstream date_string;
    date_string << std::put_time(&date, "%Y-%m-%d %H:%M");
    return date_string.str();
}

std::string date_to_pretty_string(std::tm date) {
    std::stringstream date_string;
    date_string << std::put_time(&date, "%B %d, %Y %H:%M (%A)");
    return date_string.str();
}
}
