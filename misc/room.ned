package covid19_guidance_simulator.misc;

import covid19_guidance_simulator.special.Base;

simple Room extends Base
{
    parameters:
        int num_people;
        @class(Room);
    gates:
        inout people[num_people];
        inout facility;
}
