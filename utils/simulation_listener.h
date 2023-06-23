#include <omnetpp.h>

#include "../people/person.h"

using namespace omnetpp;

namespace utils {
class SimulationListener : public cISimulationLifecycleListener {
private:
    void set_submodule_display_strings(cModule *module);
public:
    inline SimulationListener() {
        cSimulation::getActiveSimulation()->addLifecycleListener(this);
    }
    inline ~SimulationListener() {
        cSimulation::getActiveSimulation()->removeLifecycleListener(this);
    }
    void lifecycleEvent(SimulationLifecycleEventType event_type, cObject * details);
    void set_agent_positions(Person::types agents_type);
};
}
