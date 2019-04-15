
#include "../include/pathn.hpp"

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <math.h>

#include <sys/time.h>

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

pathn::pathn(const std::string& file) {
    ifstream                input_csv(file);

    int                     _length =       0,
                            __length,
                            row_length =    0;

    string                  line;

    vectorn*                negative =      new vectorn();

    vector<vectorn_flags>   flags;

    getline(input_csv, line);

    for (auto _flags : utility_split(line, "\t")) {
        if (flags.size() == 0)
            throw logic_error("bad format, header column for one of the row does not contain any flag. At least one flag for each row must be specified. Row index=" + std::to_string(_length));

        for (auto _flag : utility_split(_flags, "+"))
            flags.push_back(static_cast<vectorn_flags>(stoi(line)));

        negative->set(_length++, 0.0, flags);
    }

    while (getline(input_csv, line)) {
        vectorn*            point =         new vectorn(_length);
        ++row_length;

        __length = 0;

        for (auto column : utility_split(line, "\t"))
            point->set(__length, atof(line), flags.at(__length++));

        if (_length != __length)
            throw logic_error("bad format, at row=" + std::to_string(row_length) + " columns expected=" + std::to_string(_length) + " but parsed=" + std::to_string(__length));

        point->inherit_flags(negative);
        add(point);
    }

    delete _stringstream;
    delete __stringstream;
    delete negative;
}

pathn::pathn() { }

pathn::~pathn() {
    vector<vectorn>().swap(path);
}

void pathn::save() {
    struct timeval  _timeval;

    gettimeofday(&_timeval, NULL);
    save("profile_" + str(_timeval.tv_sec * 1000 + _timeval.tv_usec / 1000) + ".csv");
}

void pathn::save(const std::string& file) {
    int         i,
                j;

    ofstream    output_csv;

    string      header_column;

    output_csv.open(string(argv[1]).append(file));

    for (i = 0; i < get(0).length(); i++) {
        for (auto flag : get(0).get_flag(i)) {
            header_column << separator << flag.specific_detail;
            separator = "+";
        }
        header_column << "\t";
    }

    header_column.pop_back();
    header_column.pop_back();

    output_csv << header_column << endl;

    for (i = 0; i < length(); i++) {
        for (j = 0; j < get(0).length() - 1; j++)
            output_csv << get(i).get(j) << "\t";
        output_csv << get(i).get(j) << endl;
    }

    output_csv.close();
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
    _avg.inherit_flags(get(0));
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

std::vector<std::string> utility_split(std::string str, std::string token){
    int             index;

    vector<string>  _str;

    while (str.size()) {

        index = str.find(token);

        if (index != string::npos) {
            _str.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if(str.size() == 0)
                _str.push_back(str);
        } else {
            _str.push_back(str);
            str = "";
        }
    }

    return _str;
}
