//******/******/******/******/******/******////******/******/******/******/******/******/******//
//*                                                                                           *//
//*    This script when given a .css file will organize the file alphabetically               *//
//*    to better help organization.                                                           *//
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


void sortFile();
void readInStyleRules(std::fstream& fin);
void printRulings();


std::string filePath;
std::string tempFile = "temp.txt";

enum ruleType {Id = 0, Class};

struct Ruling {
    ruleType type;
    // int numberOfLines;
    std::vector<std::string> lines;
    Ruling(ruleType t, std::vector<std::string> l) : type(t) , lines(l) {}
};

std::vector<Ruling> rulings;

int main() {


    //Lets ask for the css file
    std::cout << "Please enter the file path" << std::endl;
    std::cin >> filePath;

    std::fstream fin;


    fin.open(filePath);

    // readInStyleRules(fin, lines);
    readInStyleRules(fin);

    std::cout << "After function" << std::endl;

    printRulings();


    fin.close();
    

    return 0;
}

void readInStyleRules(std::fstream& fin) {
    //* Check if file is still opened
    if (!fin.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    };

    std::cout << "In function opened successfully" << std::endl;
    std::string line;
    std::vector<std::string> lines;

    //* Create file for writing out
    std::ofstream fout;
    fout.open(tempFile, std::ios::out);

    ruleType type;


    while(getline(fin, line)) {
        
        //* The line is blank - remove it
        
        // line.find_first_not_of(' ') != std::string::npos
        bool res = line.find_first_not_of(' ') == std::string::npos;
        // bool res = line.length() == 0;
        std::cout << "Comparing: :" << line << ": res:"  << res <<  std::endl;
        while (line.length() == 0) {
            // if (fin.eof()) break;
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

        // //* print out tokens
        // for (int i = 0; i < tokens.size(); i++) {
        //     std::cout << tokens[i] << " " << std::endl;
        // } 

        //* We are at the end of the rule here
        if (tokens.begin()[0] == "}") {
            lines.push_back(line);
            Ruling rule(type, lines);
            lines.clear();
            rulings.push_back(rule);
            // break;
        } else {
            //* If there is atleast 1 non whitespace character in the string
            if (line.find_first_not_of(' ') != std::string::npos) {
                lines.push_back(line);
            }
        }
        
        //*****************************************************************//
        //*****************************************************************//
        //******************// Print back out our line //******************//
        //*****************************************************************//
        //*****************************************************************//
        
        //* If there is atleast 1 non whitespace character in the string
        if (line.find_first_not_of(' ') != std::string::npos) {
            fout << line << std::endl;
        }

    }

    fout.close();
    //* Overwrite the file with our existing changes
    std::remove(filePath.c_str());
    std::rename(tempFile.c_str(), filePath.c_str());
}

void sortFile() {

}

void printRulings() {
    for (Ruling r : rulings) {
        std::cout << "type: " << r.type << std::endl;
        for (std::string s : r.lines) {
            std::cout << s  << std::endl;
        }
        std::cout << "****rule done****" << std::endl;
    }
    std::cout << "done " << std::endl;
}