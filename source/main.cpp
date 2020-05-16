#include <iostream>
#include <stack>

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

    bool verdict = false;
    Formula initialFormula = Parser().parseDimacsFile(inputFileName);
    initialFormula.removeTautologies();
    Interpretation initialInterpretation;

    std::stack<Pair<Formula, Interpretation>> configurations;
    configurations.push(Pair<Formula, Interpretation>{
        ._1 = initialFormula,
        ._2 = initialInterpretation
    });

    // std::cout << "Removing tautologies" << std::endl;
    // formula.removeTautologies();
    // formula.printContents();

    // std::cout << "Unit propagation" << std::endl;
    // formula.propagateUnit();
    // formula.printContents();

    // std::cout << "Pure literals exclusion" << std::endl;
    // formula.excludePureLiterals();
    // formula.printContents();

    while (not configurations.empty()) {
        std::cout << "<configurations>" << std::endl;
        auto configuration = configurations.top();
        std::cout << configurations.size() << std::endl;
        configurations.pop();
        std::cout << configurations.size() << std::endl;
        std::cout << "</configurations>" << std::endl;

        auto formula = configuration._1;
        formula.applyInterpretation(configuration._2);

        formula.propagateUnit();
        formula.excludePureLiterals();

        if (formula.hasEmptyClause()) {
            break;
        }

        if (formula.getClauses().empty()) {
            verdict = true;
            break;
        }

        Pair<Formula, Interpretation> childConfig1, childConfig2;
        childConfig1._1 = Formula(formula);
        childConfig2._1 = Formula(formula);

        auto mapping = formula.pickRandomLiteralMapping();
        Interpretation childInterpretation1 = Interpretation(configuration._2), 
            childInterpretation2 = Interpretation(configuration._2);
        childInterpretation1.getMappings().push_back(mapping);
        auto inversedMapping = Pair<Literal, bool>({
            ._1 = mapping._1,
            ._2 = not mapping._2
        });
        childInterpretation2.getMappings().push_back(inversedMapping);

        childConfig1._2 = childInterpretation1;
        childConfig2._2 = childInterpretation2;

        configurations.push(childConfig1);
        configurations.push(childConfig2);
    }

    std::cout << "Verdict: " << (verdict? "SAT": "UNSAT") << std::endl;
    return 0;
}
