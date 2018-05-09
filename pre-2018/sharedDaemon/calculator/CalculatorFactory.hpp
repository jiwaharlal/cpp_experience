/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CalculatorFactory.hpp
 * @author   Maxim Bondarenko
 * @date     01.03.2016
 */

#pragma once

#include "../calculatorCommon/ICalculator.hpp"
//#include "ILogger.hpp"

class ILogger;

class CalculatorFactory
{
public:
   static ICalculator* create();
   static void destroy(ICalculator* calculator);
private:
   CalculatorFactory();

   static ILogger* mLogger;
};

