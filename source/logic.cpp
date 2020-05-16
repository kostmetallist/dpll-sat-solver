#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <vector>

#include "logic.h"


Literal::Literal() {
    this->id = 0;
    this->sign = false;
}

Literal::Literal(int id, bool sign) {
    this->id = id;
    this->sign = sign;
}

Literal::Literal(const Literal &prototype) {
    this->id = prototype.getId();
    this->sign = prototype.getSign();
}

bool Literal::operator==(const Literal &other) const {
    return (this->id == other.id && this->sign == other.sign);
}

bool Literal::operator<(const Literal &other) const {
    return ((this->sign? this->id: -this->id) < 
            (other.sign? other.id: -other.id));
}

const int Literal::getId() const {
    return this->id;
}

void Literal::setId(int id) {
    this->id = id;
}

const bool Literal::getSign() const {
    return this->sign;
}

Literal Literal::getInversion() const {
    Literal result(*this);
    result.setSign(not result.getSign());
    return result;
}

void Literal::setSign(bool sign) {
    this->sign = sign;
}

void Literal::printContents() const {
    std::cout << (sign? "": "~") << id << std::endl;
}


void Clause::removeLiteralsByIndices(const std::vector<int> indices) {

    std::vector<int> ordered(indices.size());
    std::copy(indices.begin(), indices.end(), ordered.begin());
    std::sort(ordered.begin(), ordered.end(), std::greater<int>());

    for (unsigned int i = 0; i < ordered.size(); ++i) {
        literals.erase(literals.begin() + ordered[i]);
    }
}

Clause::Clause() {}

Clause::Clause(const std::vector<Literal> &literals) {
    for (unsigned int i = 0; i < literals.size(); ++i) {
        this->addLiteral(Literal(literals[i]));
    }
}

Clause::Clause(const Clause &prototype) {
    auto literalsSource = prototype.getLiterals();
    this->literals = std::vector<Literal>(literalsSource.size());
    for (unsigned int i = 0; i < literalsSource.size(); ++i) {
        this->literals[i] = Literal(literalsSource[i]);
    }
}

void Clause::addLiteral(const Literal &literal) {
    this->literals.push_back(literal);
}

const std::vector<Literal> &Clause::getLiterals() const {
    return this->literals;
}

std::vector<Literal> &Clause::getLiterals() {
    return this->literals;
}

void Clause::printContents() const {
    for (unsigned int i = 0; i < literals.size(); ++i) {
        std::cout << (literals[i].getSign() ? "": "~") << 
            literals[i].getId() << " ";
    }
    std::cout << std::endl;
}

void Clause::eliminateDublications() {
    std::set<Literal> withDublicates;
    for (unsigned int i = 0; i < literals.size(); ++i) {
        auto pivotLiteral = literals[i];
        for (unsigned int j = i+1; j < literals.size(); ++j) {
            auto current = literals[j];
            if (current == pivotLiteral) {
                withDublicates.insert(pivotLiteral);
                break;
            }
        }
    }
    for (auto iter = withDublicates.begin(); 
         iter != withDublicates.end(); 
         ++iter) {

        removeLiteralEntries(*iter);
        // restoring first occurrence of the literal
        literals.push_back(*iter);
    }
}

bool Clause::hasContraryLiterals() const {
    for (unsigned int i = 0; i < literals.size(); ++i) {
        auto pivotLiteral = literals[i];
        for (unsigned int j = i+1; j < literals.size(); ++j) {
            auto current = literals[j];
            if (current.getId() == pivotLiteral.getId() &&
                current.getSign() != pivotLiteral.getSign())

                return true;
        }
    }
    return false;
}

void Clause::removeLiteralEntries(Literal literal) {
    std::vector<int> matchedIndices;
    for (unsigned int i = 0; i < literals.size(); ++i) {
        if (literals[i] == literal) {
            matchedIndices.push_back(i);
        }
    }
    removeLiteralsByIndices(matchedIndices);
}


Interpretation::Interpretation() {}

Interpretation::Interpretation(
    const std::vector<Pair<Literal, bool>> mappings) {

    for (unsigned int i = 0; i < mappings.size(); ++i) {
        this->mappings.push_back(mappings[i]);
    }
}

Interpretation::Interpretation(const Interpretation &prototype) {
    auto sourceMappings = prototype.getMappings();
    for (unsigned int i = 0; i < sourceMappings.size(); ++i) {
        this->mappings.push_back(sourceMappings[i]);
    }
}

const std::vector<Pair<Literal, bool>> &Interpretation::getMappings() const {
    return this->mappings;
}

std::vector<Pair<Literal, bool>> &Interpretation::getMappings() {
    return this->mappings;
}


void Formula::removeClausesByIndices(const std::vector<int> indices) {

    std::vector<int> ordered(indices.size());
    std::copy(indices.begin(), indices.end(), ordered.begin());
    std::sort(ordered.begin(), ordered.end(), std::greater<int>());
    for (unsigned int i = 0; i < ordered.size(); ++i) {
        clauses.erase(clauses.begin() + ordered[i]);
    }
}

std::vector<int> Formula::collectContraryClausesIndices() const {
    std::vector<int> indices;
    for (unsigned int i = 0; i < clauses.size(); ++i) {
        if (clauses[i].hasContraryLiterals()) {

            indices.push_back(i);
        }
    }
    return indices;
}

Literal Formula::searchForUnitClauseLiteral() const {
    for (unsigned int i = 0; i < clauses.size(); ++i) {
        auto literals = clauses[i].getLiterals();
        if (literals.size() == 1) 
            return literals[0];
    }
    return Literal(-1, false);
}

std::vector<int> Formula::collectSpecificClausesIndices(
    const Literal part) const {

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

std::set<Literal> Formula::gatherLiterals() const {
    std::set<Literal> result;
    for (unsigned int i = 0; i < clauses.size(); ++i) {
        auto literals = clauses[i].getLiterals();
        for (unsigned int j = 0; j < literals.size(); ++j) {
            result.insert(literals[j]);
        }
    }
    return result;
}

Formula::Formula() {}

Formula::Formula(const Formula &prototype) {
    auto sourceClauses = prototype.getClauses();
    this->clauses = std::vector<Clause>(sourceClauses.size());
    for (unsigned int i = 0; i < sourceClauses.size(); ++i) {
        this->clauses[i] = Clause(sourceClauses[i]);
    }
}

void Formula::addClause(const Clause &clause) {
    this->clauses.push_back(clause);
}

const std::vector<Clause> &Formula::getClauses() const {
    return this->clauses;
}

std::vector<Clause> &Formula::getClauses() {
    return this->clauses;
}

void Formula::printContents() const {
    for (unsigned int i = 0; i < clauses.size(); ++i) {
        clauses[i].printContents();
    }
}

bool Formula::hasEmptyClause() const {
    for (unsigned int i = 0; i < clauses.size(); ++i) {
        if (clauses[i].getLiterals().empty())
            return true;
    }
    return false;
}

void Formula::removeTautologies() {
    removeClausesByIndices(collectContraryClausesIndices());
    for (unsigned int i = 0; i < clauses.size(); ++i) {
        clauses[i].eliminateDublications();
    }
}

void Formula::propagateUnit() {
    auto unitLiteral = searchForUnitClauseLiteral();
    while (unitLiteral.getId() != -1) {
        auto clauseIndicesToDelete = collectSpecificClausesIndices(unitLiteral);
        removeClausesByIndices(clauseIndicesToDelete);

        auto clausesIndicesWithInverse = collectSpecificClausesIndices(
            unitLiteral.getInversion());

        for (unsigned int i = 0; i < clausesIndicesWithInverse.size(); ++i) {
            clauses[i].removeLiteralEntries(unitLiteral);
        }

        unitLiteral = searchForUnitClauseLiteral();
    }
}

void Formula::excludePureLiterals() {
    auto literals = gatherLiterals();
    for (auto iter = literals.begin(); iter != literals.end(); ++iter) {
        Literal inversion = (*iter).getInversion();
        // if not found
        if (literals.find(inversion) == literals.end()) {
            auto clauseIndicesToDelete = 
                collectSpecificClausesIndices(*iter);
            removeClausesByIndices(clauseIndicesToDelete);
        }
    }
}

void Formula::applyInterpretation(Interpretation interpretation) {
    auto mappings = interpretation.getMappings();
    for (unsigned int i = 0; i < mappings.size(); ++i) {
        Literal target = Literal(mappings[i]._1);
        target.setSign(target.getSign() == mappings[i]._2);

        auto clauseIndicesToDelete = 
            collectSpecificClausesIndices(target);
        removeClausesByIndices(clauseIndicesToDelete);

        Literal inversion = target.getInversion();
        auto clauseIndicesToReduce = collectSpecificClausesIndices(inversion);
        for (unsigned int j = 0; j < clauseIndicesToReduce.size(); ++j) {
            clauses[clauseIndicesToReduce[j]].removeLiteralEntries(inversion);
        }
    }
}

Pair<Literal, bool> Formula::pickRandomLiteralMapping() {

    std::srand(std::time(NULL));
    int clauseIndex = std::rand() % clauses.size();
    auto literals = clauses[clauseIndex].getLiterals();
    int literalIndex = std::rand() % literals.size();

    auto result = Pair<Literal, bool>({
        ._1 = literals[literalIndex],
        ._2 = bool(std::rand() % 2)
    });
    return result;
}
