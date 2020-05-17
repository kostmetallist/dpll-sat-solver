#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stack>
#include "logic.h"
#include "parsing.h"


int main(int argc, char **argv) {

    char *inputFileName;
    if (argc > 1) {
        inputFileName = argv[1];
    } else {
        std::cerr << "No DIMACS input file is provided; aborting" << std::endl;
        return 1;
    }

    std::srand(std::time(NULL));
    bool verdict = false;
    Formula initialFormula = Parser().parseDimacsFile(inputFileName);
    initialFormula.removeTautologies();

    Interpretation initialInterpretation;
    std::stack<Pair<Formula, Interpretation>> configurations;
    configurations.push(Pair<Formula, Interpretation>{
        ._1 = initialFormula,
        ._2 = initialInterpretation
    });

    while (not configurations.empty()) {
        auto configuration = configurations.top();
        configurations.pop();

        auto formula = configuration._1;
        std::cout << "Applying interpretation:" << std::endl;
        configuration._2.printContents();
        formula.applyInterpretation(configuration._2);

        // std::cout << "Formula for call #" << callNumber << std::endl;
        // formula.printContents();
        // std::cout << "___" << std::endl;

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
