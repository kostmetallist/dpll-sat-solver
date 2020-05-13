#pragma once

#include "datatypes.h"


class Parser {
public:
    Formula parseDimacsFile(const char *filePath);
};
