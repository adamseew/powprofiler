
#include "vectorn.hpp"

#ifndef PLNR_POWER_LOGGER_H
#define PLNR_POWER_LOGGER_H

namespace plnr 
{
    class power_logger {
    
    public:
        virtual ~power_logger() { }

        virtual vectorn get_value(double x, vectorn y) = 0;
    };
}

#endif