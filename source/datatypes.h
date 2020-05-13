#pragma once

#include <vector>


class Literal {
private:
    int id;
    bool sign;

public: 
    Literal(int id, bool sign=true) {
        this->id = id;
        this->sign = sign;
    }

    int getId() {
        return this->id;
    }

    void setId(int id) {
        this->id = id;
    }

    bool getSign() {
        return this->sign;
    }
};


class Clause {
private:
    std::vector<Literal> literals;
public:
    Clause() {
        this->literals = std::vector<Literal>();
    }

    void addLiteral(const Literal &literal) {
        this->literals.push_back(literal);
    }

    std::vector<Literal> &getLiterals() {
        return this->literals;
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
};