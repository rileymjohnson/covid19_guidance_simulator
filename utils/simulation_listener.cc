#include <omnetpp.h>

#include "simulation_listener.h"
#include "../special/base.h"

using namespace omnetpp;

namespace utils {
void SimulationListener::set_submodule_display_strings(cModule *module) {
   for (cModule::SubmoduleIterator it(module); !it.end(); it++) {
       cModule *submodule = *it;

       if (submodule->hasPar("size") && submodule->hasPar("color")) {
           check_and_cast<Base *>(submodule)->change_icon_size_and_color(
               submodule->par("size").stdstringValue(),
               submodule->par("color").stdstringValue()
           );
       }

       set_submodule_display_strings(submodule);
   }
}

void SimulationListener::set_agent_positions(Person::types agents_type) {
    const char * agents_type_name;

    if (agents_type == Person::types::PRINCIPAL_AGENT) {
        agents_type_name = "principal_agents";
    } else if (agents_type == Person::types::AUXILIARY_AGENT) {
        agents_type_name = "auxiliary_agents";
    } else {
        throw cRuntimeError("Not a valid agent type");
    }
    int num_counties = this->getSimulation()->getSystemModule()->getSubmoduleVectorSize("counties");

    for (int i = 0; i < num_counties; i++) {
        cModule *county = this->getSimulation()->getSystemModule()->getSubmodule("counties", i);
        int num_settings = county->getSubmoduleVectorSize("settings");

        for (int j = 0; j < num_settings; j++) {
            cModule *setting = county->getSubmodule("settings", j);
            int num_agents = setting->getSubmoduleVectorSize(agents_type_name);

            for (int k = 0; k < num_agents; k++) {
                Person *agent = check_and_cast<Person *>(
                    setting->getSubmodule(agents_type_name, k)
                );
                cDisplayString display_string = agent->getDisplayString();
                display_string.setTagArg("p", 0, 200);
                display_string.setTagArg("p", 1, 200);
            }
        }
    }
}

void SimulationListener::lifecycleEvent(SimulationLifecycleEventType event_type, cObject * details) {
    if (event_type == SimulationLifecycleEventType::LF_POST_NETWORK_INITIALIZE) {
        EV_INFO << "The simulation has started!" << "\n";
        set_submodule_display_strings(
            getSimulation()->getSystemModule()
        );
        //set_agent_positions(Person::types::PRINCIPAL_AGENT);
    }
}
}
