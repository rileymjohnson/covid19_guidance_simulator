package covid19_guidance_simulator.special;

import covid19_guidance_simulator.special.GuidanceDispatcher;
import covid19_guidance_simulator.special.InvisibleChannel;
import covid19_guidance_simulator.facilities.Facility;
import covid19_guidance_simulator.people.Person;
import covid19_guidance_simulator.special.Base;

module Setting extends Base
{
    parameters:
        color = default("yellow");
        string name = default("No Name");
        int num_principal_agents = default(intuniform(10, 30));
        int num_auxiliary_agents = default(intuniform(5, 15));
        @display("bgb=1500,660;bgg=100,2,grey95;bgu=km"); // correct ratio: 1098,660
        @figure[principal_agents_divider](type=line; points=435,0,435,660; lineWidth=2; lineStyle="dashed"; lineColor="#5A5A5A");
        @figure[auxillary_agents_divider](type=line; points=1310,0,1310,660; lineWidth=2; lineStyle="dashed"; lineColor="#5A5A5A");
        @figure[facilities_box](type=polyline; points=760,0,760,160,990,160,990,0; lineWidth=2; lineStyle="dashed"; lineColor="#5A5A5A");
    submodules:
        principal_agents[num_principal_agents]: Person {
            color = "orange";
            size = "vs";
        	//@display("p=25,25,m,12,35,35");
        };
        auxiliary_agents[num_auxiliary_agents]: Person {
            size = "vs";
        	//@display("p=1335,25,m,5,35,35");
        };
        primary_facility: Facility {
            facility_type = 0;
        	color = "red";
        	size = "l";
        	@display("p=875,40");
        };
        secondary_facility: Facility {
            facility_type = 1;
        	color = "dark_gray";
        	size = "l";
        	@display("p=800,40");
        };
        tertiary_facility: Facility {
            facility_type = 2;
        	color = "light_gray";
        	size = "l";
        	@display("p=950,40");
        };
        guidance_dispatcher: GuidanceDispatcher {
        	size = "vl";
        	@display("p=875,115");
        };
    connections:
        for i=0..sizeof(principal_agents)-1 {
            primary_facility.principal_agents[i] <--> InvisibleChannel <--> principal_agents[i].primary_facility;
            secondary_facility.principal_agents[i] <--> InvisibleChannel <--> principal_agents[i].secondary_facility;
            tertiary_facility.principal_agents[i] <--> InvisibleChannel <--> principal_agents[i].tertiary_facility;
            guidance_dispatcher.principal_agents[i] <--> InvisibleChannel <--> principal_agents[i].dispatcher;
        }
        for i=0..sizeof(auxiliary_agents)-1 {
        	primary_facility.auxiliary_agents[i] <--> InvisibleChannel <--> auxiliary_agents[i].primary_facility;
        	secondary_facility.auxiliary_agents[i] <--> InvisibleChannel <--> auxiliary_agents[i].secondary_facility;
        	tertiary_facility.auxiliary_agents[i] <--> InvisibleChannel <--> auxiliary_agents[i].tertiary_facility;
        	guidance_dispatcher.auxiliary_agents[i] <--> InvisibleChannel <--> auxiliary_agents[i].dispatcher;
        }
        guidance_dispatcher.primary_facility <--> InvisibleChannel <--> primary_facility.dispatcher;
        guidance_dispatcher.secondary_facility <--> InvisibleChannel <--> secondary_facility.dispatcher;
        guidance_dispatcher.tertiary_facility <--> InvisibleChannel <--> tertiary_facility.dispatcher;
}
