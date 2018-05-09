/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     TestCalculator.cpp
 * @author   Maxim Bondarenko
 * @date     01.03.2016
 */

#include "TestCalculator.hpp"

int TestCalculator::add(int lhs, int rhs)
{
   return 42;
}

extern "C" ICalculator* create()
{
   return new TestCalculator();
}

extern "C" void destroy(ICalculator* calc)
{
   delete calc;
}
