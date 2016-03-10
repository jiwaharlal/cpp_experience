/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CalculatorFactory.cpp
 * @author   Maxim Bondarenko
 * @date     01.03.2016
 */

#include "CalculatorFactory.hpp"
#include "calculator.hpp"
#include "LoggerHolder.hpp"

// private
CalculatorFactory::CalculatorFactory()
{

}

ICalculator* CalculatorFactory::create()
{
   return new Calculator();
}

void CalculatorFactory::destroy(ICalculator* calculator)
{
   delete calculator;
}

extern "C" ICalculator* create()
{
   return CalculatorFactory::create();
}

extern "C" void destroy(ICalculator* calc)
{
   delete calc;
}

extern "C" ILogger* registerLogger(ILogger* logger)
{
   ILogger* prevLogger = LoggerHolder::logger();
   LoggerHolder::setLogger(logger);

   return prevLogger;
}
