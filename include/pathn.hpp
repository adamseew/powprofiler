
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

        pathn(const std::string& file);

        pathn();

        ~pathn();

        std::string save();

        std::string save(const std::string& file);

        const int columns() const;

        const int rows() const;

        const vectorn get(int index) const;

        void set(int index, vectorn value);

        void add(vectorn point);

        vectorn abs();

        vectorn avg();

        vectorn sum();

        pathn* copy();

        pathn operator*(const double value) const;
        pathn operator/(const double value) const;

        pathn& operator=(const pathn& _pathn);
    };  
}

#endif
