
#include "../include/utility.hpp"

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

const std::string& plnr::utility_trim(const string& source) {
	auto start = source.begin();
	while (start != source.end() && std::isspace(*start)) {
		start++;
	}

	auto end = source.end();
	do {
		end--;
	} while (std::distance(start, end) > 0 && std::isspace(*end));

	return *new string(start, end + 1);
}