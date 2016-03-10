/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     LoggerInstance.cpp
 * @author   Maxim Bondarenko
 * @date     10.03.2016
 */

#include "LoggerInstance.hpp"

#include <iostream>
#include <string>

LoggerInstance::LoggerInstance()
   : mHeader("")
{

}

LoggerInstance* LoggerInstance::instance()
{
   static LoggerInstance myInstance;
   return &myInstance;
}

void LoggerInstance::log_info(const char* message)
{
   std::cout << mHeader << " Info: " << message << std::endl;
}

void LoggerInstance::log_error(const char* message)
{
   std::cout << mHeader << " Error: " << message << std::endl;
}

void LoggerInstance::setHeader(const char* header)
{
   mHeader = header;
}
