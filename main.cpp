#include <iostream>
#include "types.h"
#include "parsing.h"


int main(int argc, char **argv) {

    Literal foo(42);
    std::cout << foo.getId() << std::endl;
    return 0;
}
