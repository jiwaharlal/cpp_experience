#pragma once

#include "../calculatorCommon/ICalculator.hpp"

class Calculator: public ICalculator
{
public:
   virtual int add(int lhs, int rhs);
};
