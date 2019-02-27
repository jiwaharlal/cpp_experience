#include <iostream>

#include "printer.hpp"

printer::printer()
{
    std::cout << "Initial value: " << i << ", changing to 10" << std::endl;
    i = 10;
}
