//******/******/******/******/******/******////******/******/******/******/******/******/******//
//*                                                                                           *//
//*    This script when given a .css file while organize the file to help developers          *//
//*    organize their stylesheets.                                                            *//
//*                                                                                           *//
//*     Cassady Campos 2020                                                                   *//
//******/******/******/******/******/******////******/******/******/******/******/******/******//

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

void sortFile();
void readInStyleRules(const std::ofstream &fin);

std::string filePath;

enum ruleType {Id, Class};

struct Ruling {
    ruleType type;
    int numberOfLines;
    std::vector<std::string> lines;
    Ruling(ruleType t, int n, std::vector<std::string> l) : type(t), numberOfLines(n),
    lines(l) {}
};

int main() {


    //Lets ask for the css file
    std::cout << "Please enter the file path" << std::endl;
    std::cin >> filePath;

    std::ofstream fin;

    fin.open(filePath);

    readInStyleRules(fin);
    

    if (!fin.is_open()) {
        std::cout << "Couldn't open file." << std::endl;
        return 0;

    };

    std::cout << "File reached and opened successfully." << std::endl;


    return 0;
}

void readInStyleRules(const std::ofstream &fin) {
    if (!fin.is_open()) {
        std::cout << "In function couldnt open" << std::endl;
        return;
    };

    std::cout << "In function opened successfully" << std::endl;
}

void sortFile() {

}