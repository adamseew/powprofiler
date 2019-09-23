
#include <vector>
#include <string>

#include "vectorn.hpp"
#include "config.hpp"

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

        std::string save(config* _config, const component& _component);

        const int columns() const;

        const int rows() const;

        const vectorn get(int index) const;

        void set(int index, vectorn value);

        void add(vectorn point);

        void approximate();

        void approximate(int degree);

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
