
#include <string>
#include <vector>

#ifndef PLNR_UTILITY_H
#define PLNR_UTILITY_H

namespace plnr
{
    std::vector<std::string> utility_split(const std::string& source, char token);
    
    const std::string utility_trim(const std::string& source);

    bool utility_is_number(const std::string& source);
}

#endif
