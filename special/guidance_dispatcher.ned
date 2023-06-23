package covid19_guidance_simulator.special;

import covid19_guidance_simulator.special.Base;

simple GuidanceDispatcher extends Base
{
	parameters:
	    color = default("black");
	    volatile int test = intuniform(25, 125);
	    @class(GuidanceDispatcher);
	    @display("p=0,0");
	gates:
	    inout principal_agents[sizeof(parent.principal_agents)];
	    inout auxiliary_agents[sizeof(parent.auxiliary_agents)];
	    inout primary_facility;
	    inout secondary_facility;
	    inout tertiary_facility;
}
