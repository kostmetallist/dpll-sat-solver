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

    std::vector<int> getContraryClausesIndices() const {
        std::vector<int> indices;
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            if (clauses[i].hasContraryLiterals()) {
                indices.push_back(i);
            }
        }
        return indices;
    }
};
