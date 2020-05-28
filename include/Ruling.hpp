#ifndef RULING_HPP
#define RULING_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sstream>

#include "Property.hpp";

class Ruling {

    public:
        Ruling(std::string n, std::vector<Property> p) : name(n), properties(p) {};
        std::string getName();
        std::vector<Property> getProperties();
        void print(std::ofstream & fout);

    private: 
        std::vector<Property> properties;
        std::string name;

};

#endif