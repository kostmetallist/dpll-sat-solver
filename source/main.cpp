#include <iostream>
#include <vector>

#include "parsing.h"
#include "datatypes.h"


int main(int argc, char **argv) {

    char *inputFileName;
    if (argc > 1) {
        inputFileName = argv[1];
    } else {
        std::cerr << "No DIMACS input file is provided. Aborting." << std::endl;
        return 1;
    }

    Formula formula = Parser().parseDimacsFile(inputFileName);
    auto clauses = formula.getClauses();

    for (unsigned int i = 0; i < clauses.size(); ++i) {
        auto literals = clauses[i].getLiterals();
        for (unsigned int j = 0; j < literals.size(); ++j) {
            std::cout << (literals[i].getSign() ? "": "~") << 
                literals[i].getId() << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
