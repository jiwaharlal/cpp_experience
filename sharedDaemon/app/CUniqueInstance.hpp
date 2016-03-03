/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CUniqueInstance.hpp
 * @author   Maxim Bondarenko
 * @date     02.03.2016
 */

#pragma once

#include <boost/interprocess/shared_memory_object.hpp>

class CUniqueInstance
{
public:
   ~CUniqueInstance();
   static CUniqueInstance* create(const char* instanceName);
   void setAttached(bool isAttached);
   bool saveProcessId();
   static bool stopInstance(const char* instanceName);
   static int getProcessId(const char* instanceName);

private:
   CUniqueInstance(boost::interprocess::shared_memory_object& shmo);
   
private:
   bool mIsAttached;
   boost::interprocess::shared_memory_object mShmo;
};

