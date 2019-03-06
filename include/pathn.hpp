#include <vector>
#include <string>

#include "vectorn.hpp"

#ifndef PLNR_PATHN_H
#define PLNR_PATHN_H

namespace plnr
{
    class pathn {

    private:
        int                         _length;
        std::vector<double>         _control;
        std::vector<vectorn>        _path;

    public:
        pathn(int __length, double* values);

        pathn(int __length);
        
        pathn(double value);

        pathn(const pathn& _pathn);

        pathn();

        ~pathn();
        
        const int length() const;

        const double get(int index) const;

        void set(int index, double value);

        const int path_length() const;

        vectorn path_get(int index);

        void path_add(vectorn point);

        double abs();

        pathn* copy();

        pathn& operator=(const pathn& _pathn);
        pathn operator+(const pathn& _pathn) const;
        pathn operator-(const pathn& _pathn) const;
        pathn operator*(const double value) const;
        pathn operator/(const double value) const;

        friend pathn operator*(const double lhs, const pathn& rhs) { return rhs.operator*(lhs); }
        friend pathn operator/(const double lhs, const pathn& rhs) { return rhs.operator/(lhs); } 
    };  
}

#endif