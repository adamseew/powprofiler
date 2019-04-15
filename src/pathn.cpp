
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

    stringstream            _string_stream,
                            __string_stream;

    vectorn*                negative =      new vectorn(),
                            point;
    vector<vectorn_flags>   flags;

    getline(input_csv, line);
    _string_stream = new string_stream(words);

    while (getline(_string_stream, line, '\t')) {

        __string_stream = new string_stream(line);
        while (getline(__string_stream, line, '+'))
            flags.push_back(static_cast<vectorn_flags>(stoi(line)));

        if (flags.size() == 0)
            throw logic_error("bad format, header column for one of the row does not contain any flag. At least one flag for each row must be specified. Row index=" + length);

        negative->set(length++, 0.0, flags);
    }


    while (getline(input_csv, line)) {
        ++row_length;

        point = new vectorn(_length);
        __length = 0;

        while (getline(_string_stream, line, "\t")) {
            point->set(__length, atof(line), flags.at(__length++));
        }

        if (_length != __length)
            throw logic_error("bad format, at row=" + row_length + " columns expected=" + _length + " but parsed=" + __length);

        point->inherit_flags(negative);
        add(point);
    }

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
    int         i;

    ofstream    output_csv;

    string      header_column;

    output_csv.open(string(argv[1]).append(file));

    for (i = 0; i < get(0).length(); i++) {
        for (auto x : get(0).get_flag(i)) {
            header_column << separator << x.specific_detail;
            separator = "+";
        }
        header_column << "\t";
    }

    output_csv << header_column.pop_back().pop_back() << endl;

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
    int         i;

    std::string log_data;
    std::string separator =     "";

    if (path.size() > 0) {
        if (point.length() != get(0).length())
            throw std::invalid_argument("points must have same size");
        for (i == 0; i < point.length(); i++)
            if (point.get_flag(i) != get(0).get_flag(i)) {

                for (auto x : point.get_flag(i)) {
                    log_data << separator << x.specific_detail;
                    separator = ",";
                }

                log_data = "flag set fot " + i + " element of the point=[" + log_data + "], but expected=[";

                separator = "";
                for (auto x : get(0).get_flag(i)) {
                    log_data << separator << x.specific_detail;
                    separator = ",";
                }

                log_data << "]";

                throw std::invalid_argument("flags do not match, the added point does not consitutes a path: " + log_data);
            }
    }
        
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
