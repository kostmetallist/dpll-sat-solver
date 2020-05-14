#include <cstdlib>
#include <cmath>
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

Pair<std::vector<char *>, bool> Parser::stringToTokens(const char *string) {

    char *strtokInput = new char[LINE_FETCH_SIZE],
        *reserved = NULL;
    strcpy(strtokInput, string);
    unsigned int tokenCount = 0;
    std::vector<char *> tokens;
    bool alarmState = false;
    while (char *token = strtok(strtokInput, " \t")) {

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
                tokens.push_back(copy);

            } else {
                alarmState = true;
                break;
            }

        } else {
            char *copy = new char[strlen(token)];
            strcpy(copy, token);
            tokens.push_back(copy);
        }

        ++tokenCount;
    }

    delete [] reserved;
    return Pair<std::vector<char *>, bool> {
        ._1 = tokens, 
        ._2 = alarmState
    };
}


Formula Parser::parseDimacsFile(const char *filePath) {

    int literalsNum = 0, clausesNum = 0, clausesProcessed = 0;
    char line[LINE_FETCH_SIZE];
    std::cout << "Opening " << filePath << std::endl;

    std::ifstream inputStream(filePath);
    unsigned int lineCount = 0;
    bool readingAborted = false,
        problemLineDetected = false;
    Formula result;

    while (!readingAborted && inputStream.getline(line, LINE_FETCH_SIZE)) {

        std::cout << "Processing \"" << line << "\"" << std::endl;
        auto tokenizeResult = stringToTokens(line);
        std::vector<char *> tokens = tokenizeResult._1;
        
        if (tokens.empty()) {
            if (tokenizeResult._2) {
                std::cerr << "Parsing error at line " << lineCount+1 << 
                    ": tokenization failed; aborting" << std::endl;
                readingAborted = true;

            } 
            // else, we're processing a comment line
            else { ++lineCount; }
            continue;
        }

        if (!strcmp(tokens[0], "p")) {
            if (problemLineDetected) {
                std::cerr << "Parsing error at line " << lineCount+1 << 
                    ": ambiguous problem definition; aborting" << std::endl;
                readingAborted = true;
            } 

            try {
                literalsNum = atoi(tokens[1]);
                clausesNum  = atoi(tokens[2]);
                problemLineDetected = true;

            } catch (std::out_of_range e) {
                std::cerr << "Parsing error at line " << lineCount+1 << 
                    ": " << e.what() << std::endl;
                readingAborted = true;
            }

        } else {

            std::vector<Literal> literals;
            for (unsigned int i = 0; i < tokens.size(); ++i) {
                int literalLabel = atoi(tokens[i]);
                std::cout << literalLabel << " ";

                if (!literalLabel) {
                    std::cout << "Null has been reached";
                    Clause clause(literals);
                    result.addClause(clause);
                }

                Literal literal(std::abs(literalLabel), literalLabel > 0);
                literals.push_back(literal);
            }
            std::cout << std::endl;
        }

        ++lineCount;
        for (unsigned int i = 0; i < tokens.size(); ++i) {
            delete [] tokens[i];
        }
    }


    inputStream.close();
    std::cout << "Input file is closed: " << lineCount << " lines processed" 
        << std::endl;
    return Formula();
}    

