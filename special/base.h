#ifndef BASE_H_
#define BASE_H_

#include <omnetpp.h>
#include <string>
#include <ctime>

using namespace omnetpp;

class Base : public cSimpleModule {
protected:
    virtual cModule * get_globals();
    virtual int get_start_day_offset();
    virtual std::tm get_start_date();
    virtual std::tm get_end_date();
    virtual std::tm get_date();
    virtual std::string get_date_string();
    virtual std::string get_pretty_date_string();
    virtual std::string get_day_of_week();
    virtual int64_t seconds(int n);
    virtual int64_t minutes(int n);
    virtual int64_t hours(int n);
    virtual int64_t days(int n);
    virtual int64_t weeks(int n);
    virtual int64_t get_simulation_length_seconds();
    virtual void log_info(std::string info);
    virtual void log_info(char * info);
    virtual void on_parameter_change(std::string par_name);
    virtual void handleParameterChange(const char *parname);
    virtual void handleMessage(cMessage * msg);
    virtual void on_event(cMessage * msg);
    virtual void on_message(cMessage * msg);
    std::string get_package();
    std::string get_package_stub();
    std::string get_asset_name();
public:
    enum class iconSizes { MAX, L, N, S, VL, VS };
    enum class iconColors { DARK_GRAY, LIGHT_GRAY, BLACK, BLUE, GREEN, ORANGE, PINK, PURPLE, RED, YELLOW };
    void change_icon_size(Base::iconSizes size);
    void change_icon_size(std::string size);
    void change_icon_color(Base::iconColors color);
    void change_icon_color(std::string color);
    void change_icon_size_and_color(Base::iconSizes size, Base::iconColors color);
    void change_icon_size_and_color(std::string size, std::string color);
    static std::string size_to_string(Base::iconSizes size);
    static std::string color_to_string(Base::iconColors color);
};

Define_Module(Base);

#endif /* BASE_H_ */
