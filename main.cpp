#include <iostream>
#include <vector>
#include "datatypes.h"
#include "parsing.h"


int main(int argc, char **argv) {

    char *inputFileName;
    if (argc > 1) {
        inputFileName = argv[1];
    } else {
        std::cerr << "No DIMACS input file is provided. Aborting." << std::endl;
        return 1;
    }

    std::cout << inputFileName << std::endl;
    Literal foo(42), bar(64, false), baz(89);
    Clause clause;
    clause.addLiteral(foo);
    clause.addLiteral(bar);
    clause.addLiteral(baz);

    std::vector<Literal> literals = clause.getLiterals();
    for (unsigned int i = 0; i < literals.size(); ++i) {
        std::cout << (literals[i].getSign() ? "": "~") << 
            literals[i].getId() << " ";
    }
    std::cout << std::endl;

    literals[1].setId(66);

    // TODO change return type for getLiterals in order to retrieve actual objects
    literals = clause.getLiterals();
    for (unsigned int i = 0; i < literals.size(); ++i) {
        std::cout << (literals[i].getSign() ? "": "~") << 
            literals[i].getId() << " ";
    }
    std::cout << std::endl;

    return 0;
}
