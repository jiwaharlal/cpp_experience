/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     LoggerHolder.cpp
 * @author   Maxim Bondarenko
 * @date     10.03.2016
 */

#include "LoggerHolder.hpp"

#include <stdlib.h>

ILogger* LoggerHolder::mLogger = NULL;

LoggerHolder::LoggerHolder()
{

}

ILogger* LoggerHolder::logger()
{
   return mLogger;
}

void LoggerHolder::setLogger(ILogger* newLogger)
{
   mLogger = newLogger;
}
