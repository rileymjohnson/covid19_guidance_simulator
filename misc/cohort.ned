package covid19_guidance_simulator.misc;

import covid19_guidance_simulator.special.Base;

simple Cohort extends Base
{
    parameters:
        @display("p=50,79;r=90");
        @class(Cohort);
    gates:
        inout people[];
        inout primary_facility;
        inout secondary_facility;
        inout tertiary_facility;
}
