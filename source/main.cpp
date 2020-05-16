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

    std::cout << "Removing tautologies" << std::endl;
    formula.removeTautologies();
    formula.printContents();

    std::cout << "Unit propagation" << std::endl;
    formula.propagateUnit();
    formula.printContents();

    std::cout << "Pure literals exclusion" << std::endl;
    formula.excludePureLiterals();
    formula.printContents();

    return 0;
}
