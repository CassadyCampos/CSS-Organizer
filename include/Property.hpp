#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sstream>

class Property {
    public:
        Property(std::string p, std::vector<std::string> v) : name(p), values(v) {}
        std::string getName();
        std::vector<std::string> getValues();

    private: 
        std::string name;
        std::vector<std::string> values;
};

#endif