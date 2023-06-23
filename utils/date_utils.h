#include <ctime>
#include <string>

namespace utils {
std::tm parse_date_string(std::string date_string);
std::string date_to_string(std::tm date);
std::string date_to_pretty_string(std::tm date);

enum class Day {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

enum class Time {
    MORNING,
    AFTERNOON,
    EVENING,
    MIDNIGHT
};

enum class MonthTime {
    FIRST_OF_MONTH,
    LAST_OF_MONTH
};

enum class WeekTime {
    FIRST_WEEK_OF_MONTH,
    SECOND_WEEK_OF_MONTH,
    THIRD_WEEK_OF_MONTH,
    FOURTH_WEEK_OF_MONTH
};

enum class TimeUnit {
    SECONDS,
    MINUTES,
    HOURS,
    DAYS
};
}
