#include <omnetpp.h>
#include <ctime>
#include <cmath>

#include "guidance_dispatcher.h"
#include "../misc/cohort.h"
#include "../people/person.h"

using namespace omnetpp;

void form_cohorts(GuidanceDispatcher* guidance_dispatcher, int ideal_cohort_size) {
    cModule *setting = guidance_dispatcher->getParentModule();
    int num_people = setting->getSubmoduleVectorSize("principal_agents");

    int num_cohorts = std::ceil(
        static_cast<float>(num_people) / ideal_cohort_size
    );

    setting->addSubmoduleVector("cohorts", num_cohorts);

    setting->getSubmodule("primary_facility")->addGateVector(
        "cohorts",
        cGate::Type::INOUT,
        num_cohorts
    );

    setting->getSubmodule("secondary_facility")->addGateVector(
        "cohorts",
        cGate::Type::INOUT,
        num_cohorts
    );

    setting->getSubmodule("tertiary_facility")->addGateVector(
        "cohorts",
        cGate::Type::INOUT,
        num_cohorts
    );

    int people_per_cohort = std::floor(
        static_cast<float>(num_people) / num_cohorts
    );
    int num_people_remaining = num_people % num_cohorts;

    int start_index = 0;

    for (int i = 0; i < num_cohorts; i++) {
        int num_people_per_cohort = i < num_people_remaining ? people_per_cohort + 1 : people_per_cohort;

        cModuleType *moduleType = cModuleType::get("covid19_guidance_simulator.misc.Cohort");

        Cohort *cohort = check_and_cast<Cohort *>(
            moduleType->create("cohorts", setting, i)
        );

        cohort->finalizeParameters();

        cohort->change_icon_size_and_color(
            cohort->par("size").stdstringValue(),
            cohort->par("color").stdstringValue()
        );

        cohort->setGateSize("people", num_people_per_cohort);

        cohort->gate("primary_facility$o")->connectTo(
            setting->getSubmodule("primary_facility")->gate("cohorts$i", i)
        );
        cohort->gate("primary_facility$i")->connectTo(
            setting->getSubmodule("primary_facility")->gate("cohorts$o", i)
        );

        cohort->gate("secondary_facility$o")->connectTo(
            setting->getSubmodule("secondary_facility")->gate("cohorts$i", i)
        );
        cohort->gate("secondary_facility$i")->connectTo(
            setting->getSubmodule("secondary_facility")->gate("cohorts$o", i)
        );

        cohort->gate("tertiary_facility$o")->connectTo(
            setting->getSubmodule("tertiary_facility")->gate("cohorts$i", i)
        );
        cohort->gate("tertiary_facility$i")->connectTo(
            setting->getSubmodule("tertiary_facility")->gate("cohorts$o", i)
        );

        for (int j = 0; j < num_people_per_cohort; j++) {
            cModule *person = setting->getSubmodule("principal_agents", j + start_index);
            person->addGate("cohort", cGate::Type::INOUT);

            cohort->gate("people$o", j)->connectTo(
                person->gate("cohort$i")
            );

            cohort->gate("people$i", j)->connectTo(
                person->gate("cohort$o")
            );
        }
        for (int j = start_index; j < start_index + num_people_per_cohort; j ++) {

        }

        cohort->buildInside();

        cohort->scheduleStart(simTime());

        start_index += num_people_per_cohort;
    }
}

void GuidanceDispatcher::initialize() {
    cMessage *msg = new cMessage("form_cohorts");
    scheduleAfter(days(2), msg);
}

void GuidanceDispatcher::handleMessage(cMessage *msg) {
    Person *agent = check_and_cast<Person *>(this->getParentModule()->getSubmodule("principal_agents", 0));
    agent->change_icon_color(Person::iconColors::YELLOW);
    cDisplayString display_string = agent->getDisplayString();
    EV_INFO << "test: " << this->par("test").intValue() << "\n";
    delete msg;
}

void GuidanceDispatcher::finish() {
    // finish
}
