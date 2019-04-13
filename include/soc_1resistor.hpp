
#include "pathn.hpp"
#include "integrator_rk4.hpp"


#ifndef PLNR_SOC_1RESISTOR
#define PLNR_SOC_1RESISTOR

namespace plnr
{
    class soc_1resistor : public first_derivative {

    private:
        pathn  curr_load;           // current load (instantaneos power consumption in function of time t)

        double volt_internal;       // internal battery voltage
        double resistance_internal; // internal resistance of the battery
        double volt_stabilized;     // fixed value determined by the load of the system
        double q_c;                 // battery capacity

    public:
        soc_1resistor(pathn _curr_load, double _volt_internal, double _resistance_internal, double _volt_stabilized, double _q_c);

        ~soc_1resistor();

        virtual vectorn get_value(double t, vectorn q);
    };
}

#endif
