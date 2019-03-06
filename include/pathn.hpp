
#include <vector>
#include <string>

#include "vectorn.hpp"

#ifndef PLNR_PATHN_H
#define PLNR_PATHN_H

namespace plnr
{
    class pathn {

    private:
        std::vector<vectorn>        path;

    public:
        pathn(int _length, vectorn* values);
        
        pathn(vectorn value);

        pathn(const pathn& _pathn);

        pathn();

        ~pathn();
        
        const int length() const;

        vectorn get(int index);

        void set(int index, double value);

        const int length() const;

        void add(vectorn point);

        double abs();

        pathn* copy();
    };  
}

#endif
