package covid19_guidance_simulator.facilities;

import covid19_guidance_simulator.special.Base;

simple Facility extends Base
{
    parameters:
        string name = default("No Name");
        int facility_type = default(0);
        //string test = "i=icons/facilities/facility/vl/red;bgb=800,500";
        @class(Facility);
        @display("i2=status/busy");
    gates:
        inout principal_agents[sizeof(parent.principal_agents)];
        inout auxiliary_agents[sizeof(parent.auxiliary_agents)];
        inout dispatcher;
}
