#include <fstream>
#include <iostream>

#include "parsing.h"
#include "datatypes.h"

#define LINE_FETCH_SIZE 128

// c A sample .cnf file.
// p cnf 3 2
// 1 -3 0
// 2 3 -1 0 

Formula Parser::parseDimacsFile(const char *filePath) {

    int literalsNum, clausesNum;
    char line[LINE_FETCH_SIZE];
    std::cout << "Opening " << filePath << std::endl;

    std::ifstream inputStream(filePath);
    inputStream.getline(line, LINE_FETCH_SIZE);
    std::cout << line << std::endl;


    inputStream.close();
    return Formula();
}    

