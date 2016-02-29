#include "ICalculator.hpp"

#pragma once

class Calculator: public ICalculator
{
public:
   virtual int add(int lhs, int rhs);
};
