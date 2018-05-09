#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <sys/time.h>

#include <sstream>
#include <iostream>
#include <vector>

inline std::vector<std::string> split(std::string endpoints){
    std::istringstream f(endpoints);
    std::string s;
    std::vector<std::string> strings;
    while (getline(f, s, ',')) {
        strings.push_back(s);
    }

    return strings;
}


