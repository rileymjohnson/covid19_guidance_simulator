package covid19_guidance_simulator.people;

import covid19_guidance_simulator.special.Base;

simple Person extends Base
{
    parameters:
        color = default("green");
        string race = default("White");
        string ethnicity = default("Non-Hispanic");
        int age = default(intuniform(1, 85));
        string sex = default("Male");

        bool has_pneumonia = default(false) @mutable;
        bool has_fever = default(false) @mutable;
        bool has_subjective_fever = default(false) @mutable;
        bool has_chills = default(false) @mutable;
        bool has_myalgia = default(false) @mutable;
        bool has_rhinorrhea = default(false) @mutable;
        bool has_sore_throat = default(false) @mutable;
        bool has_cough = default(false) @mutable;
        bool has_dyspnea = default(false) @mutable;
        bool has_nausea_vomiting = default(false) @mutable;
        bool has_headache = default(false) @mutable;
        bool has_abdominal_pain = default(false) @mutable;
        bool has_diarrhea = default(false) @mutable;

        bool is_disabled = default(false);
        bool is_an_h2a_employee = default(false);
        bool is_high_risk = default(false);
        bool is_immunocompromised = default(false);
        bool has_underlying_medical_condition = default(false);
        bool is_a_newborn = default(false);
        bool is_pregnant = default(false);
        bool is_a_racial_or_ethnic_minority = default(false);

		@display("p=0,0,x,0,0");
        @class(Person);
    gates:
        inout primary_facility;
        inout secondary_facility;
        inout tertiary_facility;
        inout dispatcher;
}
