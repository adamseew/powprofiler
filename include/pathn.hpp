
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

        void save();

        void save(const std::string& file);

        void load();
        
        const int length() const;

        vectorn get(int index);

        void set(int index, vectorn value);

        void add(vectorn point);

        vectorn abs();

        vectorn avg();

        pathn* copy();

        pathn& operator=(const pathn& _pathn);
    };  
}

#endif
