
#include "../include/utility.hpp"
#include "../include/pathn.hpp"

#include <sys/stat.h>
#include <sys/time.h>
#include <iostream>
#include <iterator>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <math.h>

using namespace plnr;

using std::logic_error;
using std::to_string;
using std::string;
using std::vector;
using std::swap;
using std::endl;

pathn::pathn(int _length, vectorn* values) {
    for (int i = 0; i < _length; i++)
        add(values[i]);
}

pathn::pathn(vectorn value) : pathn::pathn(1, &value) { }

pathn::pathn(const pathn& _pathn) {
    path = _pathn.path;
}

pathn::pathn(const string& file) {
    std::ifstream                   input_csv(file);

    int                             _length =       0,
                                    __length,
                                    row_length =    0;

    string                          line;

    vectorn*                        negative;
    vectorn*                        point;

    vector<vectorn_flags>*          _flags;
    vector<vector<vectorn_flags>>   flags;

    getline(input_csv, line);

    for (auto __flags : utility_split(line, ',')) {
        if (__flags.size() == 0)
            throw logic_error("bad format, header column for one of the row does not contain any flag. At least one flag for each row must be specified. Row index=" + to_string(_length));

        _flags = new vector<vectorn_flags>();

        for (auto flag : utility_split(__flags, '+'))
            _flags->push_back(static_cast<vectorn_flags>(stoi(flag)));

        flags.push_back(*_flags);

        vector<vectorn_flags>().swap(*_flags);
        delete _flags;

        ++_length;
    }

    negative = new vectorn(_length, flags);

    while (getline(input_csv, line)) {
        point = new vectorn(*negative);
        ++row_length;

        __length = 0;

        for (auto column : utility_split(line, ','))
            point->set(__length++, atof(column.c_str()));

        if (_length != __length)
            throw logic_error("bad format, at row=" + to_string(row_length) + " column length expected=" + to_string(_length) + " but parsed=" + to_string(__length));

        add(*point);
        delete point;
    }

    input_csv.close();

    delete negative;

    return;
}

pathn::pathn() { }

pathn::~pathn() {
    vector<vectorn>().swap(path);
}

string pathn::save(config* _config, const component& _component) {

    int     i;

    string  file_name;

    // creating the directory where to save pathn

    if (mkdir(_config->get_directory().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        if(errno != EEXIST) 
            throw std::runtime_error("error creating the directory " + _config->get_directory() + ". " + strerror(errno));
    }

    file_name = _config->get_directory() + "/" + _component.name + ".csv";

    for (i = 2; utility_file_exists(file_name); i++)
        file_name = _config->get_directory() + "/" + _component.name + "_" + to_string(i) + ".csv";

    return save(file_name);
}   

string pathn::save() {
    struct timeval  _timeval;

    gettimeofday(&_timeval, NULL);
    return save("profile_" + to_string(_timeval.tv_sec * 1000 + _timeval.tv_usec / 1000) + ".csv");
}

string pathn::save(const string& file) {
    int             i,
                    j;

    std::ofstream   output_csv;

    string          separator;

    output_csv.open(file);

    for (i = 0; i < rows(); i++) {
        separator = "";
        for (auto flag : path.at(0).get_flag(i)) {
            output_csv << separator << static_cast<int>(flag);
            separator = "+";
        }

        if (i < rows() - 1)
            output_csv << ",";
    }
    output_csv << endl;

    for (i = 0; i < columns(); i++) {
        for (j = 0; j < rows() - 1; j++)
            output_csv << get(i).get(j) << ",";
        output_csv << get(i).get(j) << endl;
    }

    output_csv.close();

    return file;
}

const int pathn::columns() const { return path.size(); }

const int pathn::rows() const { 
    assert (path.size() > 0);
    
    return path.at(0).length(); 
}

const vectorn pathn::get(int index) const {
    assert(index >= 0 && index < columns());
    return path.at(index);
}

void pathn::set(int index, vectorn value) {
    assert(index >= 0 && index < columns());
    path.at(index) = value;
}

void pathn::add(vectorn point) { 
    if (path.size() > 0 && point.length() != rows())
        throw std::invalid_argument("points must have same size");
        
    path.push_back(point); 
}

void pathn::approximate() {
    approximate(1);
}

void pathn::approximate(int degree) {
    assert(degree >= 1);

    if (degree != 1)
        approximate(degree - 1);

    int         i;

    vector<vectorn> new_path;

    for (i = 1; i < columns(); i++) {
        new_path.push_back(path.at(i - 1));
        new_path.push_back((path.at(i - 1) + path.at(i)) / 2);
    }
    new_path.push_back(path.at(i - 1));

    vector<vectorn>().swap(path);
    path = new_path;
}

vectorn pathn::abs() {
    vectorn _abs(rows());
    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < columns(); j++)
            _abs.set(i, _abs.get(i) + pow(get(j).get(i), 2));
        _abs.set(i, sqrt(_abs.get(i)));
    }
    return _abs;
}

vectorn pathn::avg() {
    vectorn _avg(rows());
    _avg.inherit_flags(get(0));
    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < columns(); j++)
            _avg.set(i, _avg.get(i) + get(j).get(i));
        _avg.set(i, _avg.get(i) / columns());
    }
    return _avg;
}

vectorn pathn::sum() {
    vectorn _sum(rows());
    _sum.inherit_flags(get(0));
    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < columns(); j++)
            _sum.set(i, _sum.get(i) + get(j).get(i));
    }
    return _sum;
}

pathn* pathn::copy() { return new pathn(*this); }

pathn& pathn::operator=(const pathn& _pathn) {
    path  = _pathn.path;

    return *this;
}

pathn pathn::operator*(const double value) const {
    pathn product;

    for (int i = 0; i < columns(); i++)
        product.add(value * this->get(i));

    return product;
}

pathn pathn::operator/(const double value) const { return this->operator*(1.0 / value); }
