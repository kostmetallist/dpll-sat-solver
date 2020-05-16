#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
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

    bool operator==(const Literal &other) const {
        return (this->id == other.id && this->sign == other.sign);
    }

    bool operator<(const Literal &other) const {
        return ((this->sign? this->id: -this->id) < 
                (other.sign? other.id: -other.id));
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

    Literal getInversion() const {
        Literal result(*this);
        result.setSign(not result.getSign());
        return result;
    }

    void setSign(bool sign) {
        this->sign = sign;
    }

    void printContents() const {
        std::cout << (sign? "": "~") << id << std::endl;
    }
};


class Clause {
private:
    std::vector<Literal> literals;

    // TODO maybe move this along with printContents and with their 
    // Formula's counterparts to the Base class
    void removeLiteralsByIndices(const std::vector<int> indices) {

        std::vector<int> ordered(indices.size());
        std::copy(indices.begin(), indices.end(), ordered.begin());
        std::sort(ordered.begin(), ordered.end(), std::greater<int>());

        for (unsigned int i = 0; i < ordered.size(); ++i) {
            literals.erase(literals.begin() + ordered[i]);
        }
    }

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

    void removeLiteralEntries(Literal literal) {

        std::vector<int> matchedIndices;
        for (unsigned int i = 0; i < literals.size(); ++i) {
            if (literals[i] == literal) {
                matchedIndices.push_back(i);
            }
        }
        removeLiteralsByIndices(matchedIndices);
    }
};


class Formula {
private:
    std::vector<Clause> clauses;

    void removeClausesByIndices(const std::vector<int> indices) {

        std::vector<int> ordered(indices.size());
        std::copy(indices.begin(), indices.end(), ordered.begin());
        std::sort(ordered.begin(), ordered.end(), std::greater<int>());

        for (unsigned int i = 0; i < ordered.size(); ++i) {
            clauses.erase(clauses.begin() + ordered[i]);
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

    std::vector<int> collectSpecificClausesIndices(const Literal part) const {
        std::vector<int> indices;
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            auto literals = clauses[i].getLiterals();
            for (unsigned int j = 0; j < literals.size(); ++j) {
                if (literals[j] == part) {

                    indices.push_back(i);
                    break;
                }
            }
        }

        return indices;
    }

    std::set<Literal> gatherLiterals() const {
        std::set<Literal> result;
        for (unsigned int i = 0; i < clauses.size(); ++i) {
            auto literals = clauses[i].getLiterals();
            for (unsigned int j = 0; j < literals.size(); ++j) {
                result.insert(literals[j]);
            }
        }
        return result;
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
                collectSpecificClausesIndices(unitLiteral);
            removeClausesByIndices(clauseIndicesToDelete);

            auto clausesIndicesWithInverse = collectSpecificClausesIndices(
                unitLiteral.getInversion());

            for (unsigned int i = 0; 
                 i < clausesIndicesWithInverse.size(); 
                 ++i) {

                clauses[i].removeLiteralEntries(unitLiteral);
            }

            unitLiteral = searchForUnitClauseLiteral();
        }
    }

    void excludePureLiterals() {
        auto literals = gatherLiterals();
        for (auto iter = literals.begin(); iter != literals.end(); ++iter) {
            Literal inversion = (*iter).getInversion();
            // if not found
            if (literals.find(inversion) == literals.end()) {
                auto clauseIndicesToDelete = 
                    collectSpecificClausesIndices(*iter);

                std::cout << "current: ";
                (*iter).printContents();
                std::cout << "its inversion: ";
                inversion.printContents();
                std::cout << "___" << std::endl;
                for (unsigned int i = 0; i < clauseIndicesToDelete.size(); ++i) {
                    std::cout << clauseIndicesToDelete[i] << " ";
                }
                std::cout << "___" << std::endl;

                removeClausesByIndices(clauseIndicesToDelete);
            }
        }
    }
};
