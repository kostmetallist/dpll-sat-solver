# Simple SAT solver

## Description
This repository contains programmatic solution for the final assignment of Model Checking CMC MSU course. The goal is to acquire practical skills in the field of automated theorem proving. Requirements: 
* solution must be implemented in C++ programming language;
* main algorithm ([DPLL](https://ru.wikipedia.org/wiki/DPLL)) should be implemented in a non-recursive way;
* input for the solution uses [DIMACS](https://fairmut3x.wordpress.com/2011/07/29/cnf-conjunctive-normal-form-dimacs-format-explained/) format.

## Launching
`./run.sh <DIMACS input file>`

## Test data
Samples of DIMACS files are located under `/data` and divided into simple ones, intended for separate DPLL stages verification, and real-world ones, gathered from the [repository](https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html) of Scientific Computing Department of Florida State University.
