
#include "../include/soc_1resistor.hpp"

#include <math.h>

using namespace plnr;

soc_1resistor::soc_1resistor(pathn _curr_load, double _volt_internal, double _resistance_internal, double _volt_stabilized, double _q_c) {
    curr_load = _curr_load;
    volt_internal = _volt_internal;
    resistance_internal = _resistance_internal;
    volt_stabilized = _volt_stabilized;
    q_c = _q_c;
}

soc_1resistor::~soc_1resistor() { }

vectorn soc_1resistor::get_value(double t, vectorn q) {

    vectorn dq(q.length());
    dq.inherit_flags(q);

    dq.set(0, ((-1) * volt_internal +
        sqrt(pow(volt_internal, 2) - 4 * resistance_internal *
        volt_stabilized * curr_load.get(t).get(0))) /
        (2 * resistance_internal * q_c)
    );

    return dq;

}
