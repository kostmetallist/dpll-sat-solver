#pragma once

#include <iostream>
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
    Literal() {
        this->id = 0;
        this->sign = false;
    }

    Literal(int id, bool sign=true) {
        this->id = id;
        this->sign = sign;
    }

    Literal(const Literal &prototype) {
        this->id = prototype.getId();
        this->sign = prototype.getSign();
    }

    const int getId() const {
        return this->id;
    }

    void setId(int id) {
        this->id = id;
    }

    const bool getSign() const {
        return this->sign;
    }

    void printContents() const {
        std::cout << (sign? "": "~") << id << std::endl;
    }
};


class Clause {
private:
    std::vector<Literal> literals;
public:
    Clause() {}

    Clause(const std::vector<Literal> &literals) {
        for (unsigned int i = 0; i < literals.size(); ++i) {
            this->addLiteral(Literal(literals[i]));
        }
    }

    void addLiteral(const Literal &literal) {
        this->literals.push_back(literal);
    }

    const std::vector<Literal> &getLiterals() const {
        return this->literals;
    }

    std::vector<Literal> &getLiterals() {
        return this->literals;
    }

    void printContents() const {
        for (unsigned int i = 0; i < literals.size(); ++i) {
            std::cout << (literals[i].getSign() ? "": "~") << 
                literals[i].getId() << " ";
        }
        std::cout << std::endl;
    }

    bool hasContraryLiterals() const {
        for (unsigned int i = 0; i < literals.size(); ++i) {
            Literal pivotLiteral = literals[i];
            for (unsigned int j = i+1; j < literals.size(); ++j) {
                Literal current = literals[j];
                if (current.getId() == pivotLiteral.getId() &&
                    current.getSign() != pivotLiteral.getSign())

                    return true;
            }
        }
        return false;
    }
};


class Formula {
private:
    std::vector<Clause> clauses;

    void removeClausesByIndices(const std::vector<int> indices) {
        for (unsigned int i = 0; i < indices.size(); ++i) {
            clauses[indices[i]] = clauses.back();
            clauses.pop_back();
        }
    }

    std::vector<int> collectContraryClausesIndices() const {
        std::vector<int> indices;
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            if (clauses[i].hasContraryLiterals()) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    Literal searchForUnitClauseLiteral() const {
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            auto literals = clauses[i].getLiterals();
            if (literals.size() == 1) 
                return literals[0];
        }

        return Literal(-1, false);
    }

    std::vector<int> collectClausesContainingLiteralIndices(
        const Literal sample) const {

        std::vector<int> indices;
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            auto literals = clauses[i].getLiterals();
            for (unsigned int j = 0; j < literals.size(); ++j) {
                if (literals[j].getId() == sample.getId() &&
                    literals[j].getSign() == sample.getSign()) {

                    indices.push_back(i);
                    break;
                }
            }
        }
        return indices;
    }



public:
    Formula() {
        this->clauses = std::vector<Clause>();
    }

    void addClause(const Clause &clause) {
        this->clauses.push_back(clause);
    }

    std::vector<Clause> &getClauses() {
        return this->clauses;
    }

    void printContents() const {
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            clauses[i].printContents();
        }
    }

    void removeTautologies() {
        removeClausesByIndices(collectContraryClausesIndices());
    }

    void propagateUnit() {
        auto unitLiteral = searchForUnitClauseLiteral();
        while (unitLiteral.getId() != -1) {
            auto clauseIndicesToDelete = 
                collectClausesContainingLiteralIndices(unitLiteral);
            removeClausesByIndices(clauseIndicesToDelete);
        }

        // TODO remove inverted unitLiteral from survived clauses
    }
};
