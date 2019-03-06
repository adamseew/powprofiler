#include "../include/pathn.hpp"

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <math.h>

using namespace std;
using namespace plnr;

using std::vector;
using std::swap;

pathn::pathn(int __length, double *values) {
    _length = __length;
    for (int i = 0; i < length(); i++)
        _control.push_back(values[i]);
}

pathn::pathn(double value) : pathn::pathn(1, &value) { }

pathn::pathn(int __length) {
    _length = __length;
    for (int i = 0; i < length(); i++)
        _control.push_back(0.0);
}

pathn::pathn(const pathn& _pathn) {
    _length = _pathn._length;
    _control = _pathn._control;
    _path = _pathn._path;
}

pathn::pathn() : pathn::pathn(1) { }

pathn::~pathn() {
    vector<double>().swap(_control);
}

const int pathn::length() const {
    return _length;
}

const double pathn::get(int index) const {
    assert(index >= 0 && index < length());
    return _control.at(index);
}

void pathn::set(int index, double value) {
    assert(index >= 0 && index < length());
    _control.at(index) = value;
}

const int pathn::path_length() const { return _path.size(); }

vectorn pathn::path_get(int index) {
    assert(index >= 0 && index < _path.size());
    return _path.at(index);
}

void pathn::path_add(vectorn point) { _path.push_back(point); }

double pathn::abs() {
    double sum = 0;
    for (int i = 0; i < length(); i++)
        sum += pow(get(i), 2);
    return sqrt(sum);
}

pathn* pathn::copy() { return new pathn(*this); }

pathn& pathn::operator=(const pathn& _pathn) {
    _length = _pathn._length;
    _path  = _pathn._path;

    return *this;
}

pathn pathn::operator+(const pathn& _pathn) const {
    double* values = (double*)malloc(sizeof(double) * length());
    for (int i = 0; i < length(); i++)
        values[i] = this->get(i) + _pathn.get(i);

    pathn sum(length(), values);

    delete values;

    return sum;
}

pathn pathn::operator-(const pathn& _pathn) const { return this->operator+(_pathn.operator*(-1.0)); }

pathn pathn::operator*(const double value) const {
    double* values = (double*)malloc(sizeof(double) * length());
    for (int i = 0; i < length(); i++)
        values[i] = value * this->get(i);

    pathn product(length(), values);

    delete values;

    return product;
}

pathn pathn::operator/(const double value) const { return this->operator*(1.0 / value); }
