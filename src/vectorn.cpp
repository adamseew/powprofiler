
#include "../include/vectorn.hpp"
#include <iostream>
#include <cassert>
#include <math.h>

using namespace plnr;

using std::vector;
using std::swap;
using std::invalid_argument;

void vectorn::_vectorn(int __length, double* values, vector<vector<vectorn_flags>> __flags) {
    assert(__flags.size() == __length);
    _length = __length;
    for (int i = 0; i < length(); i++) {
        _vector.push_back(values[i]);
        _flags.push_back(__flags.at(i));
    }
}

vectorn::vectorn(int __length, double* values, vector<vector<vectorn_flags>> __flags) {
    _vectorn(__length, values, __flags);
}

vectorn::vectorn(int __length, double* values) {
    vector<vector<vectorn_flags>> __flags(__length, vector<vectorn_flags>(1, vectorn_flags::unflagged));
    _vectorn(__length, values, __flags);
    vector<vector<vectorn_flags>>().swap(__flags);
} 

vectorn::vectorn(int __length, vector<vector<vectorn_flags>> __flags) { 
    double* values = new double[__length] {0.0};
    _vectorn(__length, values, __flags);
    delete [] values;
}

vectorn::vectorn(double value) : vectorn::vectorn(1, &value) { }

vectorn::vectorn(int __length) { 
    double* values = new double[__length] {0.0};
    vector<vector<vectorn_flags>> __flags(__length, vector<vectorn_flags>(1, vectorn_flags::unflagged));
    _vectorn(__length, values, __flags);
    delete [] values;
    vector<vector<vectorn_flags>>().swap(__flags);
}

vectorn::vectorn(const vectorn& _vectorn) {
    _length = _vectorn._length;
    _vector  = vector<double>(_vectorn._vector);
    _flags = vector<vector<vectorn_flags>>(_vectorn._flags);
}

vectorn::vectorn() : vectorn::vectorn(1) { }

vectorn::~vectorn() {
    vector<double>().swap(_vector);
    vector<vector<vectorn_flags>>().swap(_flags);
}

const int vectorn::length() const { return _length; }

const double vectorn::get(int index) const { 
    assert(index >= 0 && index < length());
    return _vector.at(index); 
}

const double vectorn::get(vectorn_flags flag) const { 
    for (int i = 0; i < length(); i++)
        for (int j = 0; j < _flags[i].size(); j++)
            if (_flags[i].at(j) == flag)
                return _vector.at(i);

    throw invalid_argument("no position in vector match the flag");
}

const int vectorn::get_index(vectorn_flags flag) const { 
    for (int i = 0; i < length(); i++)
        for (int j = 0; j < _flags[i].size(); j++)
            if (_flags[i].at(j) == flag)
                return i;

    throw invalid_argument("no position in vector match the flag");
}

vector<vectorn_flags> vectorn::get_flag(int index) {
    assert(index >= 0 && index < length());
    return _flags.at(index);
}

vector<vector<vectorn_flags> > vectorn::get_flags() {
    return _flags;
}

void vectorn::set(int index, double value) {
    assert(index >= 0 && index < length());
    _vector.at(index) = value; 
}

void vectorn::set(int index, double value, vectorn_flags flag) {
    set(     index, value);
    set_flag(index, flag );
}

void vectorn::set(int index, double value, vector<vectorn_flags> _flags) {
    set(     index, value );
    set_flag(index, _flags);
}


void vectorn::set_flag(int index, vectorn_flags flag) {
    vector<vectorn_flags> __flags;
    __flags.push_back(flag);

    if (!has_flag(index, flag))
        set_flag(index, __flags);
}

void vectorn::set_flag(int index, vector<vectorn_flags> __flags) {
    assert(index >= 0 && index < length());

    bool flags_has_instance;

    for (int i = 0; i < __flags.size(); i++) {
        if (__flags.at(i) == vectorn_flags::unflagged)
            continue;

        flags_has_instance = false;

        for (int j = 0; j < _flags.at(index).size(); j++)
            if (_flags.at(index).at(j) == __flags.at(i)) {
                flags_has_instance = true;
                break;
            }

        if (!flags_has_instance)
            if (_flags.at(index).at(0) == vectorn_flags::unflagged)
                _flags.at(index).at(0) = __flags.at(i);
            else
                _flags.at(index).push_back(__flags.at(i));
    }
}

bool vectorn::has_flag(int index, vectorn_flags flag) {
    for (int i = 0; i < _flags.at(index).size(); i++)
        if (_flags.at(index).at(i) == flag)
            return true;

    return false;
}

void vectorn::remove_flag(int index, vectorn_flags flag) {
    for (int i = 0; i < _flags.at(index).size(); i++)
        if (_flags.at(index).at(i) == flag)
            _flags.at(index).erase(_flags.at(index).begin() + i);

    if (_flags.at(index).size() == 0)
        _flags.at(index).push_back(vectorn_flags::unflagged);
}

vectorn* vectorn::inherit_flags(vectorn _vectorn) {
    for (int i = 0; i < _vectorn.length(); i++) {
        if (i >= length())
            break;

        set_flag(i, _vectorn.get_flag(i));
    }

    return this;
}


void vectorn::empty_flags() {
    int     i = 0;
    
    for (auto &__flags : _flags) {
        __flags.empty();
        set_flag(i++, vectorn_flags::unflagged);
    }
}

void vectorn::transform_flags(vectorn_flags flag) {
    int     __flag = 0;

    for (auto &__flags : _flags) {
        for (auto &_flag : __flags) {
            
            __flag = static_cast<int>(_flag) + static_cast<int>(flag);

            if (__flag >= LOWER_VECTORN_FLAGS_LIMIT && __flag < UPPER_VECTORN_FLAGS_LIMIT) 
                _flag = static_cast<vectorn_flags>(__flag);
        }
            
    }
}

double vectorn::abs() {
    double sum = 0;
    for (int i = 0; i < length(); i++)
        sum += pow(get(i), 2);
    return sqrt(sum);
}

vectorn* vectorn::copy() { return new vectorn(*this); }

vectorn& vectorn::operator=(const vectorn& _vectorn) {
    _length = _vectorn._length;
    _vector  = vector<double>(_vectorn._vector);
    _flags = vector<vector<vectorn_flags>>(_vectorn._flags);

    return *this;
}

vectorn vectorn::operator+(const vectorn& _vectorn) const {
    double* values = (double*)malloc(sizeof(double) * length());
    for (int i = 0; i < length(); i++)
        values[i] = this->get(i) + _vectorn.get(i);

    vectorn sum(length(), values);
    sum.inherit_flags(_vectorn);

    free(values);

    return sum;
}

vectorn vectorn::operator-(const vectorn& _vectorn) const { return this->operator+(_vectorn.operator*(-1.0)); }

vectorn vectorn::operator+(const double value) const {
    double* values = (double*)malloc(sizeof(double) * length());
    std::fill_n(values, length(), value);
    
    vectorn to_sum(length(), values);

    free(values);

    return to_sum + *this;
}

vectorn vectorn::operator-(const double value) const { return this->operator+(value*(-1.0)); }

vectorn vectorn::operator*(const double value) const {
    double* values = (double*)malloc(sizeof(double) * length());
    for (int i = 0; i < length(); i++)
        values[i] = value * this->get(i);

    vectorn product(length(), values);
    product.inherit_flags(*this);

    free(values);

    return product;
}

vectorn vectorn::operator/(const double value) const { return this->operator*(1.0 / value); }
