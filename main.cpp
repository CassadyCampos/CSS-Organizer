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

namespace fs = std::experimental::filesystem;

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
void printRulings(std::fstream & fin);
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
            printRulings(fin);
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


    while(getline(fin, line)) {
        while (line.length() == 0) {
            getline(fin, line);
        }

        //* The line is not blank - keep it
        std::vector<std::string> tokens;
        std::stringstream check1(line);
        std::string intermediate;
        std::string ruleName;
        bool readingRule = false;


        //* Tokenizing by space ' ' 
        while (getline(check1, intermediate, ' ')) {
            tokens.push_back(intermediate);
        }


        // //* If first token belongs to class to has an id
        // ruleName = tokens.begin()[0].substr(tokens.begin()[0].find('.')+1);


        if (tokens.begin()[0][0] == '.' || tokens.begin()[0][0] == '#') {
            
            ruleName = tokens.begin()[0].substr(tokens.begin()[0].find('.')+1);
            std::cout << "ruleName:" << ruleName << std::endl;
            tokens.begin()[0] == "." ? type = ruleType::Class : 
            type = ruleType::Id;

            // std::stringstream name(tokens.begin()[0]);

            // getline(name, ruleName, '.');


            continue; //* skip to next line basically
        } 

        //* We are at the end of the rule here
        if (line == "}") {
            // ruleName = tokens.begin()[0].substr(tokens.begin()[0].find('.')+1);

            // std::cout << "ruleName:" << ruleName << std::endl;
            Ruling rule(type, ruleName, properties);
            properties.clear();
            rulings.push_back(rule);
        } else {
            //* If there is atleast 1 non whitespace character in the string
            if (line.find_first_not_of(' ') != std::string::npos) {
                //* Split line up so we can turn it into a property
                std::vector<std::string> propValues;

                std::string propName = line.substr(0, line.find(':'));
                // std::cout << "propName: " << propName << std::endl;
                std::stringstream ss(line.substr(line.find(':') + 1, line.length() - propName.length() - 2));
                std::string propertyValue;
                while (ss >> propertyValue) { 
                    std::cout << propertyValue << std::endl;
                    propValues.push_back(propertyValue);
                }
                Property prop(propName, propValues);
                properties.push_back(prop);

                // std::cout << "length: " << line.length() - propName.length() - 2 << std::endl;
                // std::cout << line.substr(line.find(':') + 1, line.length() - propName.length() - 2) << std::endl;

            }
        }
    }
}

void printRulings(std::fstream & fin) {
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
        // fout << r.name << " {" << std::endl;
        std::string ruleType = ruleType_ToString(r.type);
        std::cout << "name: " << ruleType << r.name << std::endl;
        fout << ruleType << r.name << std::endl;
        std::sort(r.properties.begin(), r.properties.end() - 1, compareProperties);

        for (Property p : r.properties) {
            fout << p.property << ": ";
            for (std::string s : p.values) {
                fout << s << " "; 
            }
            fout << ";\n";
        }
        fout << "\n}\n";
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
}