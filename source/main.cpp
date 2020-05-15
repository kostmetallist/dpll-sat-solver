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
    formula.printContents();

    std::cout << "Contrary clauses indices found: " << std::endl;
    std::vector<int> contraryClausesIndices = formula.getContraryClausesIndices();
    for (unsigned int i = 0; i < contraryClausesIndices.size(); ++i) {
        std::cout << contraryClausesIndices[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
