package covid19_guidance_simulator;

import covid19_guidance_simulator.geographies.County;
import covid19_guidance_simulator.special.Globals;
import covid19_guidance_simulator.special.Base;

network Covid19GuidanceSimulator extends Base
{
    parameters:
        string start_date = default("2020-01-01");
        string map_color_mode = default("dark");
        string short_name = default("No Name");
        string long_name = default("No Name");
        string map_name = default("new_york_city");
        int num_counties = default(intuniform(2, 5));

        @display("bgb=1098,660;bgi=images/maps/$map_color_mode/$map_name,s;bgg=100,2,grey95;bgu=km");
        @figure[counties_overlay](type=path; path="M 0 50 L 10 0 L 80 80 Z"; fillColor="red");
    submodules:
        globals: Globals {
            start_date_string = parent.start_date;
        }
        counties[num_counties]: County {
        	map_color_mode = parent.map_color_mode;
        };
}

