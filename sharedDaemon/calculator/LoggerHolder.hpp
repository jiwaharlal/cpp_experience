/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     LoggerHolder.hpp
 * @author   Maxim Bondarenko
 * @date     10.03.2016
 */

#pragma once

#define ENABLE_LOG

#ifdef ENABLE_LOG

#include <sstream>
#include <string>

#define LOG_INFO(message) \
   if (LoggerHolder::getLogger()) \
   { \
      std::stringstream ss; \
      ss << message; \
      LoggerHolder::getLogger()->log_info(ss.str().c_str()); \
   }

#define LOG_ERROR(message) \
   if (LoggerHolder::getLogger()) \
   { \
      std::stringstream ss; \
      ss << message; \
      LoggerHolder::getLogger()->log_error(ss.str().c_str()); \
   }

class ILogger;

class LoggerHolder
{
public:
   static ILogger* getLogger();
   static void setLogger(ILogger* newLogger);
private:
   LoggerHolder();
   static ILogger* mLogger;
};

#else // ENABLE_LOG

#define LOG_INFO(message)
#define LOG_ERROR(message)

#endif // ENABLE_LOG

