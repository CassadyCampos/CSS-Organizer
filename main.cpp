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
};

struct Ruling {
    ruleType type;
    std::vector<std::string> lines;
    Ruling(ruleType t, std::vector<std::string> l) : type(t) , lines(l) {}
};

void sortFile();
void readInStyleRules(std::fstream& fin);
void printRulings(std::fstream & fin);
void sortRulings();
bool compareFunction(std::string a, std::string b) {
    return a < b;
}
bool compareRule(Ruling a, Ruling b) { 
    return a.lines[1] < b.lines[1];
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
    std::vector<std::string> lines;
    ruleType type;


    while(getline(fin, line)) {
        while (line.length() == 0) {
            getline(fin, line);
        }

        //* The line is not blank - keep it
        std::vector<std::string> tokens;
        std::stringstream check1(line);
        std::string intermediate;


        //* Tokenizing by space ' ' 
        while (getline(check1, intermediate, ' ')) {
            tokens.push_back(intermediate);
        }

        // //* If first token belongs to class to has an id
        if (tokens.begin()[0] == "." || tokens.begin()[0] == "#") {
            tokens.begin()[0] == "." ? type = ruleType::Class : 
            type = ruleType::Id;
        } 

        //* We are at the end of the rule here
        if (tokens.begin()[0] == "}") {
            lines.push_back(line);
            Ruling rule(type, lines);
            lines.clear();
            rulings.push_back(rule);
        } else {
            //* If there is atleast 1 non whitespace character in the string
            if (line.find_first_not_of(' ') != std::string::npos) {
                lines.push_back(line);
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
        //* the +1/-1.
        std::sort(r.lines.begin() + 1, r.lines.end() - 1, compareFunction);

        for (std::string line : r.lines) {
            fout << line << std::endl;
        }
    }

    fout.close();
    //* Overwrite the file with our existing changes
    std::remove(filePath.c_str());
    std::rename(tempFile.c_str(), filePath.c_str());
}
