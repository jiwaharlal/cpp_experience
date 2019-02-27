#include <iostream>

#include "printer2.hpp"

printer2::printer2()
{
    std::cout << "value in printer2: " << i << " changing to 20" << std::endl;
    i = 20;
}

void printer2::print()
{
    std::cout << "print() i = " << i << std::endl;

    throw std::runtime_error("Something went wrong");
}
