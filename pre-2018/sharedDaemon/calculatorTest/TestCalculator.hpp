/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     TestCalculator.hpp
 * @author   Maxim Bondarenko
 * @date     01.03.2016
 */

#pragma once

#include "../calculatorCommon/ICalculator.hpp"

class TestCalculator: public ICalculator
{
public:
   virtual int add(int lhs, int rhs);
};

