//******/******/******/******/******/******////******/******/******/******/******/******/******//
//*                                                                                           *//
//*    This script when given a directory will organize stylesheets within the file           *//
//*    to better help developers with organization.                                           *//
//*                                                                                           *//
//*     Cassady Campos 2020                                                                   *//
//******/******/******/******/******/******////******/******/******/******/******/******/******//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include <experimental/filesystem>

#include <filesystem>

namespace fs = std::filesystem;

enum ruleType {Id = 0, Class};

struct Property {
    std::string property;
    std::vector<std::string> values;
    Property (std::string p, std::vector<std::string> v) : property(p), values(v) {}
};

struct Ruling {
    ruleType type;
    // std::vector<std::string> lines;
    std::string name;
    std::vector<Property> properties;
    // Ruling(ruleType t, std::vector<std::string> l) : type(t) , lines(l) {}
    Ruling(ruleType t, std::string n, std::vector<Property> p) : type(t) , name(n), properties(p) {}

};



void sortFile();
void readInStyleRules(std::fstream& fin);
void printRulings();
void sortRulings();
std::string ruleType_ToString(ruleType rt);

bool compareProperties(Property a, Property b) {
    return a.property < b.property;
}
bool compareRule(Ruling a, Ruling b) { 
    return a.name < b.name;
}

std::string directory;
std::string filePath = "";
std::string tempFile = "temp.txt";
const std::string CSS = "css";


std::vector<Ruling> rulings;

int main() {

    //* Get the directory 
    std::cout << "Please enter the directory" << std::endl;
    std::cin >> directory;

    fs::create_directories(directory);
    for(auto& p: fs::recursive_directory_iterator(directory)) {
        rulings.clear();

        //* We need to make sure it is a css file
        //* TODO - We can extend this to other stylesheets such as scss
        std::string path = p.path();
        std::string fileExt = path.substr(path.find_last_of(".")+ 1);
        std::cout << path << ". . . . . . . . . . . . . . ";
        if (fileExt == CSS) {
            std::fstream fin;
            filePath = p.path();
            fin.open(filePath);
            readInStyleRules(fin);
            printRulings();
            fin.close();
            std::cout << "done" << std::endl;
        } else {
            std::cout << "skipped" << std::endl;
        }
    }
    
    return 0;
}

void readInStyleRules(std::fstream& fin) {

    //* Check if file is still opened
    if (!fin.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    };

    std::string line;
    // std::vector<std::string> lines;
    std::vector<Property> properties;
    ruleType type;
    std::string ruleName;
    bool isReading = false;


    while(!fin.eof()) {
    // while(getline(fin, line)) {
        getline(fin, line);
        while (line.length() == 0) {
            if (fin.eof()) return;
            getline(fin, line);
        }

        //* The line is not blank - keep it
        std::vector<std::string> tokens;
        std::stringstream check1(line);
        std::string intermediate;

        //* Tokenizing by space ' ' 
        if (isReading) { 
            while(getline(check1, intermediate, ' ')) {
                tokens.push_back(intermediate);
            } 
        } else {
            while (getline(check1, intermediate, '{')) {
                tokens.push_back(intermediate);
            }
        }

        // if (tokens.begin()[0][0] == '.' || tokens.begin()[0][0] == '#') {

        //* Try to start reading rule
        if (isReading == false) {
            isReading = true;
            // std::cout << tokens[0] << std::endl;
            // ruleName = tokens.begin()[0].substr(tokens.begin()[0].find('.')+1);
            ruleName = tokens[0];
            // std::cout 
            type = ruleType::Class;
            // tokens.begin()[0] == "." ? type = ruleType::Class : 
            // type = ruleType::Id;

            //* skip to next line basically
            getline(fin, line);
            while(line.length() == 0) {
                if (fin.eof()) return;
                getline(fin, line);
            }
        } 

        //* We are at the end of the rule here
        if (line == "}") {
            Ruling rule(type, ruleName, properties);
            properties.clear();
            rulings.push_back(rule);
            isReading = false;
        } else {
            //* If there is atleast 1 non whitespace character in the string
            if (line.find_first_not_of(' ') != std::string::npos) {
                //* Split line up so we can turn it into a property
                std::vector<std::string> propValues;

                std::string propName = line.substr(0, line.find(':'));
                std::stringstream ss(line.substr(line.find(':') + 1, line.length() - propName.length() - 2));
                std::string propertyValue;

                while (ss >> propertyValue) { 
                    propValues.push_back(propertyValue);
                }
                Property prop(propName, propValues);
                properties.push_back(prop);
            }
        }
    }
}

void printRulings() {
    std::ofstream fout;
    fout.open(tempFile, std::ios::out);

    //* Check if file is still opened
    if (!fout.is_open()) {
        std::cout << "Could not open temp file" << std::endl;
        return;
    };

    std::sort(rulings.begin(), rulings.end(), compareRule);

    for (Ruling r : rulings) {
        //* Because we only want to sort the contents of the rule here,
        //* we want to exclude the first and last lines of the rule. Hence
        // * the +1/-1.
        std::sort(r.properties.begin(), r.properties.end() - 1, compareProperties);
        // r.print();
        // std::string ruleType = ruleType_ToString(r.type);
        // fout << r.name << " { " << std::endl; 

        // for (Property p : r.properties) {
        //     fout << p.property << ": ";

        //     for (unsigned int i = 0; i < p.values.size(); i++) {
        //         if (i < p.values.size() - 1) {
        //             fout << p.values[i] << " ";
        //         } else {
        //             fout << p.values[i] << ";" << std::endl;
        //         }
        //     }
        // }
        // fout << "}" << std::endl;
        // fout << std::endl;
    }

    fout.close();
    //* Overwrite the file with our existing changes
    std::remove(filePath.c_str());
    std::rename(tempFile.c_str(), filePath.c_str());
}

std::string ruleType_ToString(ruleType rt) {
    if (rt == 0) { 
        return ".";
    } else if (rt == 1) {
        return "#";
    }
    std::cerr << "Error, unknown ruleType: " << rt << std::endl;

    return "Error";
}