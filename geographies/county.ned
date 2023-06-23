package covid19_guidance_simulator.geographies;

import covid19_guidance_simulator.special.Setting;
import covid19_guidance_simulator.geographies.Geography;

module County extends Geography {
	parameters:
	    string map_name = default("unknown");
	    string map_color_mode = default("dark");
	    string short_name = default("No Name");
	    string long_name = default("No Name");
	    int population = default(intuniform(1000, 100000));
	    int num_settings = default(intuniform(3, 20));
	    @display("bgb=915,550;bgi=images/maps/$map_color_mode/$map_name,s;bgg=100,2,grey95;bgu=km");
	submodules:
	    settings[num_settings]: Setting;
}
