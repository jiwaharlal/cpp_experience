/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     LoggerInstance.hpp
 * @author   Maxim Bondarenko
 * @date     10.03.2016
 */

#pragma once

#include <string>

#include "../calculatorCommon/ILogger.hpp"

class LoggerInstance: public ILogger
{
public:
   static LoggerInstance* instance();
   virtual void log_info(const char* message);
   virtual void log_error(const char* message);

   void setHeader(const char* header);
private:
   LoggerInstance();

   std::string mHeader;
};

