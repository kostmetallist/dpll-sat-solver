#pragma once

#include <vector>
#include "datatypes.h"


class Parser {
private:
    Pair<std::vector<char *>, bool> stringToTokens(const char *string);
public:
    Formula parseDimacsFile(const char *filePath);
};
