
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

pathn::pathn(int _length, vectorn* values) {
    for (int i = 0; i < _length; i++)
        add(values[i]);
}

pathn::pathn(vectorn value) : pathn::pathn(1, &value) { }

pathn::pathn(const pathn& _pathn) {
    path = _pathn.path;
}

pathn::pathn() { }

pathn::~pathn() {
    vector<vectorn>().swap(path);
}

const int pathn::length() const { return path.size(); }

vectorn pathn::get(int index) {
    assert(index >= 0 && index < length());
    return path.at(index);
}

void pathn::set(int index, vectorn value) {
    assert(index >= 0 && index < length());
    path.at(index) = value;
}

void pathn::add(vectorn point) { 
    if (path.size() > 0 && point.length() != get(0).length())
        throw std::invalid_argument("points must have same size");
        
    path.push_back(point); 
}

vectorn pathn::abs() {
    vectorn _abs(get(0).length());
    for (int i = 0; i < get(0).length(); i++) {
        for (int j = 0; j < length(); j++)
            _abs.set(i, _abs.get(i) + pow(get(j).get(i), 2));
        _abs.set(i, sqrt(_abs.get(i)));
    }
    return _abs;
}

vectorn pathn::avg() {
    vectorn _avg(get(0).length());
    for (int i = 0; i < get(0).length(); i++) {
        for (int j = 0; j < length(); j++)
            _avg.set(i, _avg.get(i) + get(j).get(i));
        _avg.set(i, _avg.get(i) / length());
    }
    return _avg;
}

pathn* pathn::copy() { return new pathn(*this); }

pathn& pathn::operator=(const pathn& _pathn) {
    path  = _pathn.path;

    return *this;
}
