#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stack>
#include <set>

#include <gmp.h>
#include <omp.h>

#include "logic.h"
#include "parsing.h"

// delay in seconds to display progress summary
#define  DELTA_TICK 1
#define  MAX_INT_DIGIT_NUMBER 10000


typedef enum {
    UNSAT = 0,
    SAT
} VERDICT;


int main(int argc, char **argv) {

    char *inputFileName;
    if (argc > 1) {
        inputFileName = argv[1];
    } else {
        std::cerr << "No DIMACS input file is provided; aborting" << std::endl;
        return 1;
    }

    std::cout.precision(2);
    double startTime = omp_get_wtime();
    double prevTick = startTime, currTick = startTime;

    time_t seed = std::time(NULL);
    std::srand(seed);
    VERDICT verdict = UNSAT;
    Formula initialFormula = Parser().parseDimacsFile(inputFileName);
    initialFormula.removeTautologies();

    int generalizedLiteralsNum = initialFormula.getGeneralizedLiteralsNum();
    std::cout << "Generalized literals number: " << 
        generalizedLiteralsNum << std::endl;

    mpz_t totalBranchesToExamine, examinedBranches;
    mpz_init2(totalBranchesToExamine, MAX_INT_DIGIT_NUMBER);
    mpz_init2(examinedBranches, MAX_INT_DIGIT_NUMBER);

    mpz_t base; mpz_init(base);
    mpz_set_str(base, "2", 10);
    mpz_pow_ui(totalBranchesToExamine, base, generalizedLiteralsNum);
    mpz_sub_ui(totalBranchesToExamine, totalBranchesToExamine, 1);

    mpf_t progressDenominator; mpf_init(progressDenominator);
    mpf_set_z(progressDenominator, totalBranchesToExamine);

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
        formula.applyInterpretation(configuration._2);

        formula.propagateUnit();
        formula.excludePureLiterals();

        if (formula.hasEmptyClause()) {

            mpz_t branchesToExclude;
            mpz_init2(branchesToExclude, MAX_INT_DIGIT_NUMBER);
            mpz_pow_ui(branchesToExclude, base, 
                formula.getGeneralizedLiteralsNum());
            mpz_sub_ui(branchesToExclude, branchesToExclude, 1);
            mpz_add(examinedBranches, examinedBranches, branchesToExclude);
            continue;

        } else if (formula.getClauses().empty()) {
            verdict = SAT;
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

        currTick = omp_get_wtime();
        if (currTick - prevTick > (double) DELTA_TICK) {

            mpf_t progressNumerator, progressRatio;
            mpf_init(progressNumerator); mpf_init(progressRatio);

            mpf_set_z(progressNumerator, examinedBranches);
            mpf_div(progressRatio, progressNumerator, progressDenominator);
            double ratioToDisplay = mpf_get_d(progressRatio);

            std::cout << "Progress: " << ratioToDisplay*100 << "%" << std::endl;
            prevTick = currTick;
        }
    }

    std::cout << "Completed in " << (omp_get_wtime()-startTime)*1000 << 
        " ms" << std::endl;
    std::cout << "Verdict: " << (verdict? "SAT": "UNSAT") << std::endl;
    return 0;
}
