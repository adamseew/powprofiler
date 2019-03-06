
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
        path.push_back(values[i]);
}

pathn::pathn(vectorn value) : pathn::pathn(1, &value) { }

pathn::pathn(const pathn& _pathn) {
    path = _pathn.path;
}

pathn::pathn() { }

pathn::~pathn() {
    vector<vectorn>().swap(path);
}

const int pathn::length() const {
    return path.size();
}

vectorn pathn::get(int index) {
    assert(index >= 0 && index < length());
    return path.at(index);
}

void pathn::set(int index, double value) {
    assert(index >= 0 && index < length());
    path.at(index) = value;
}

const int pathn::length() const { return path.size(); }

vectorn pathn::get(int index) {
    assert(index >= 0 && index < path.size());
    return path.at(index);
}

void pathn::add(vectorn point) { path.push_back(point); }

double pathn::abs() {
    double sum = 0;
    for (int i = 0; i < length(); i++) {
        for (int j = 0; j < get(i).length(); j++)
            sum += pow(get(i).get(j), 2);
    }
    return sqrt(sum);
}

pathn* pathn::copy() { return new pathn(*this); }

pathn& pathn::operator=(const pathn& _pathn) {
    path  = _pathn.path;

    return *this;
}
