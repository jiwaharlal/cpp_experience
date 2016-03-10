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

class ILogger;

class LoggerHolder
{
public:
   static ILogger* logger();
   static void setLogger(ILogger* newLogger);
private:
   LoggerHolder();
   static ILogger* mLogger;
};

