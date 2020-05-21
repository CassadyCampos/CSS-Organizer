//******/******/******/******/******/******////******/******/******/******/******/******/******//
//*                                                                                           *//
//*    This script when given a .css file while organize the file to help developers          *//
//*    organize their stylesheets.                                                            *//
//*                                                                                           *//
//*     Cassady Campos 2020                                                                   *//
//******/******/******/******/******/******////******/******/******/******/******/******/******//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>


void sortFile();
void readInStyleRules(std::fstream& fin);

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
    for (Ruling r : rulings) { 
        std:: cout << "Rule Type: " << r.type << std::endl;
        std::cout << "Contents: " << std::endl;
        for (std::string line : r.lines) {
            std::cout << line << std::endl;
        }
    }

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
    std::ofstream fout;
    fout.open(tempFile, std::ios::out);


    while(getline(fin, line)) {
        
        //* The line is blank
        while (line.length() == 0) {
            getline(fin, line);
        }

        //* The line is not blank
        fout << line << std::endl;

    }
    fout.close();

    std::remove(filePath.c_str());
    std::rename(tempFile.c_str(), filePath.c_str());
}

void sortFile() {

}