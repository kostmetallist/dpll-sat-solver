#pragma once

#include <vector>
#include "logic.h"


class Parser {
private:
    Pair<std::vector<char *>, bool> stringToTokens(const char *string);
public:
    Formula parseDimacsFile(const char *filePath);
};
