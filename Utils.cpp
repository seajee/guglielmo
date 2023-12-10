#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadFile(const std::string& filePath)
{
    std::string empty;
    std::ifstream fileReader(filePath);

    if (!fileReader.is_open()) {
        std::cerr << "ERROR: Could not read file " << filePath << std::endl;
        return empty;
    }

    std::stringstream content;
    content << fileReader.rdbuf();
    fileReader.close();

    return content.str();
}
