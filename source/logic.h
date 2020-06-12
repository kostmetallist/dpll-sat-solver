#pragma once

#include <set>
#include <vector>


template <class F, class S> 
struct Pair {
    F _1;
    S _2;
};


class Literal {
private:
    int id;
    bool sign;

public:
    Literal();
    Literal(int id, bool sign=true);
    Literal(const Literal &prototype);
    bool operator==(const Literal &other) const;
    bool operator<(const Literal &other) const;
    const int getId() const;
    void setId(int id);
    const bool getSign() const;
    Literal getInversion() const;
    void setSign(bool sign);
    void printContents() const;
};


class Clause {
private:
    std::vector<Literal> literals;

    // TODO maybe move this along with printContents and with their 
    // Formula's counterparts to the Base class
    void removeLiteralsByIndices(const std::vector<int> indices);

public:
    Clause();
    Clause(const std::vector<Literal> &literals);
    Clause(const Clause &prototype);
    void addLiteral(const Literal &literal);
    const std::vector<Literal> &getLiterals() const;
    std::vector<Literal> &getLiterals();
    void printContents() const;
    void eliminateDublications();
    bool hasContraryLiterals() const;
    void removeLiteralEntries(Literal literal);
};


class Interpretation {
private:
    std::vector<Pair<Literal, bool>> mappings;

public:
    Interpretation();
    Interpretation(const std::vector<Pair<Literal, bool>> mappings);
    Interpretation(const Interpretation &prototype);
    const std::vector<Pair<Literal, bool>> &getMappings() const;
    std::vector<Pair<Literal, bool>> &getMappings();
    void printContents() const;
};


class Formula {
private:
    std::vector<Clause> clauses;

    void removeClausesByIndices(const std::vector<int> indices);
    std::vector<int> collectContraryClausesIndices() const;
    Literal searchForUnitClauseLiteral() const;
    std::vector<int> collectSpecificClausesIndices(const Literal part) const;
    std::set<Literal> gatherLiterals() const;

public:
    Formula();
    Formula(const Formula &prototype);
    void addClause(const Clause &clause);
    const std::vector<Clause> &getClauses() const;
    std::vector<Clause> &getClauses();
    void printContents() const;
    bool hasEmptyClause() const;
    void removeTautologies();
    void propagateUnit();
    void excludePureLiterals();
    void applyInterpretation(Interpretation interpretation);
    Pair<Literal, bool> pickRandomLiteralMapping();
    int getGeneralizedLiteralsNum();
};
