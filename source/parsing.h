#pragma once

#include <vector>
#include "datatypes.h"


class Parser {
private:
    std::vector<char *> stringToTokens(const char *string);
public:
    Formula parseDimacsFile(const char *filePath);
};
