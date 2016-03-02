/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CInstanceSynchronizer.hpp
 * @author   Maxim Bondarenko
 * @date     02.03.2016
 */

#pragma once

#include <boost/interprocess/shared_memory_object.hpp>

class CInstanceSynchronizer
{
public:
   class IUniqueInstance;
   static IUniqueInstance* createUnique(const char* instanceName);

   class IInstanceHandler;
   static IInstanceHandler* getHandler(const char* instanceName);

private:
   //CInstanceSynchronizer(boost::interprocess::shared_memory_object& shmo, bool isAttached);
   CInstanceSynchronizer();

   boost::interprocess::shared_memory_object mShmo;
   bool mIsAttached;
};

class CInstanceSynchronizer::IUniqueInstance
{
public:
   virtual ~IUniqueInstance() {}
   virtual void setAttached(bool isAttached) = 0;
   virtual bool saveProcessId() = 0;
};

class CInstanceSynchronizer::IInstanceHandler
{
public:
   virtual ~IInstanceHandler() {}
   virtual bool stopInstance() = 0;
   virtual int getProcessId() = 0;
};

