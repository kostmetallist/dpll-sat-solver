#pragma once

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
};


class Clause {
private:
    std::vector<Literal> literals;
public:
    Clause() {
        this->literals = std::vector<Literal>();
    }

    Clause(const std::vector<Literal> &literals) {
        this->literals = std::vector<Literal>(literals.size());
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
