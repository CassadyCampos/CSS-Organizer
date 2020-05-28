#include <Ruling.hpp>

bool compareProperties(Property a, Property b) {
    return a.getName() < b.getName();
}

std::string Ruling::getName() {
    return name;
};

std::vector<Property> Ruling::getProperties() {
    return properties; 
}

void Ruling::print(std::ofstream & fout) {
    std::string tempFile = "temp.txt";

    fout << name << "{" << std::endl;
    
    for (Property p : properties) {
        fout << p.getName() << ": ";
        
        for (unsigned int i = 0; i < p.getValues().size(); i++) {
            if (i < p.getValues().size() - 1){ 
                fout << p.getValues()[i] << " ";
            } else {
                fout << p.getValues()[i] << ";" << std::endl;
            }
        }
    }
    fout << "}" << std::endl;
    fout << std::endl;
}

void Ruling::sortProperties() {
    //* Because we only want to sort the contents of the rule here,
    //* we want to exclude the first and last lines of the rule. Hence
    // * the +1/-1.
    std::sort(properties.begin(), properties.end() - 1, compareProperties); 
}