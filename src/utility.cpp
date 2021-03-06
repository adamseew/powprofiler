
#include "../include/utility.hpp"

#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <string>

using namespace plnr;

using std::vector;
using std::string;

vector<string> plnr::utility_split(const string& source, char token) {
    int             index;

    vector<string>  _source;

    string          __source(source);

    while (__source.size()) {

        index = __source.find(token);

        if (index != string::npos) {
            _source.push_back(__source.substr(0, index));
            __source = __source.substr(index + 1);
            if(__source.size() == 0)
                _source.push_back(__source);
        } else {
            _source.push_back(__source);
            __source = "";
        }
    }

    return _source;
}

const std::string plnr::utility_trim(const string& source) {
	auto start = source.begin();
	while (start != source.end() && std::isspace(*start)) {
		start++;
	}

	auto end = source.end();
	do {
		end--;
	} while (std::distance(start, end) > 0 && std::isspace(*end));

	return string(start, end + 1);
}

bool plnr::utility_is_number(const string& source) {
    return  !source.empty() && std::find_if(source.begin(), source.end(), [](char c) { 
        return !(std::isdigit(c) || c == '.');  
    }) == source.end();
}

bool plnr::utility_file_exists(const string& name) {

    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
