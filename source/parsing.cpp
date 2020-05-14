#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>

#include "parsing.h"
#include "datatypes.h"

#define LINE_FETCH_SIZE 256

// c A sample .cnf file.
// p cnf 3 2
// 1 -3 0
// 2 3 -1 0 

std::vector<char *> Parser::stringToTokens(const char *string) {

    char *strtokInput = new char[LINE_FETCH_SIZE],
        *reserved = NULL;
    strcpy(strtokInput, string);
    unsigned int tokenCount = 0;
    std::vector<char *> result;
    while (char *token = strtok(strtokInput, " \t")) {
        std::cout << "Token \"" << token << "\"" << std::endl;
        if (strtokInput) {
            reserved = strtokInput;
            strtokInput = NULL;
        }

        if (tokenCount == 0) {
            if (!strcmp(token, "c")) {
                break;

            } else if (!strcmp(token, "p") || strpbrk(token, "123456789")) {
                char *copy = new char[strlen(token)];
                strcpy(copy, token);
                result.push_back(copy);

            } else {
                std::cerr << "Parsing error detected. Stopping file reading." << 
                    std::endl;
                break;
            }

        } else {
            char *copy = new char[strlen(token)];
            strcpy(copy, token);
            result.push_back(copy);
        }

        ++tokenCount;
    }

    delete [] reserved;
    return result;
}


Formula Parser::parseDimacsFile(const char *filePath) {

    int literalsNum, clausesNum;
    char line[LINE_FETCH_SIZE];
    std::cout << "Opening " << filePath << std::endl;

    std::ifstream inputStream(filePath);

    unsigned int lineCount = 0;
    while (inputStream.getline(line, LINE_FETCH_SIZE)) {

        std::cout << "Processing \"" << line << "\"" << std::endl;

        // TODO delete `tokens` allocated memory
        auto tokens = stringToTokens(line);
        for (unsigned int i = 0; i < tokens.size(); ++i) {
            std::cout << tokens[i] << " ";
        }
        std::cout << std::endl;
        ++lineCount;
    }


    inputStream.close();
    std::cout << "Input file is closed: " << lineCount << " lines processed" 
        << std::endl;
    return Formula();
}    

