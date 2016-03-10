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

#ifdef ENABLE_LOG

#include <stdlib.h>

ILogger* LoggerHolder::mLogger = NULL;

LoggerHolder::LoggerHolder()
{

}

ILogger* LoggerHolder::getLogger()
{
   return mLogger;
}

void LoggerHolder::setLogger(ILogger* newLogger)
{
   mLogger = newLogger;
}

extern "C" ILogger* registerLogger(ILogger* logger)
{
   ILogger* prevLogger = LoggerHolder::getLogger();
   LoggerHolder::setLogger(logger);

   return prevLogger;
}

#endif // ENABLE_LOG
