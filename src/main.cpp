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
#include <ctype.h>
#include <sstream>
#include <filesystem>

#include <Ruling.hpp>
#include <Property.hpp>

namespace fs = std::filesystem;

void sortFile();
void readInStyleRules(std::fstream& fin);
void printRulings();
void sortRulings();

bool compareRule(Ruling a, Ruling b) { 
    //* Check if element so we can put those first
    if (std::isalpha(a.getName()[0]) && !(std::isalpha(b.getName()[0]))) {
        return true;
    }
    return a.getName() < b.getName();
}

std::string directory;
std::string filePath = "";
std::string tempFile = "temp.txt";

std::vector<std::string> stylesheets = {"css", "scss"};

std::vector<Ruling> rulings;

int main() {

    //* Get the directory 
    std::cout << "Please enter the directory" << std::endl;
    std::cin >> directory;
    fs::create_directories(directory);

    for(auto& p: fs::recursive_directory_iterator(directory)) {
        rulings.clear();

        //* We need to make sure it is a stylesheet
        //* TODO - We can extend this to other stylesheets such as scss
        std::string path = p.path();
        std::string fileExt = path.substr(path.find_last_of(".")+ 1);
        std::cout << path << ". . . . . . . . . . . . . . ";

        if (std::find(stylesheets.begin(), stylesheets.end(), fileExt) != stylesheets.end()) {
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
    std::vector<Property> properties;
    std::string ruleName;
    bool isReading = false;

    while(!fin.eof()) {
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

        //* Try to start reading rule
        if (isReading == false) {
            isReading = true;
            ruleName = tokens[0];

            //* skip to next line basically
            getline(fin, line);
            while(line.length() == 0) {
                if (fin.eof()) return;
                getline(fin, line);
            }
        } 

        //* We are at the end of the rule here
        if (line == "}") {
            Ruling rule(ruleName, properties);
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
        r.sortProperties();
        r.print(fout);
    }

    fout.close();
    //* Overwrite the file with our existing changes
    std::remove(filePath.c_str());
    std::rename(tempFile.c_str(), filePath.c_str());
}
