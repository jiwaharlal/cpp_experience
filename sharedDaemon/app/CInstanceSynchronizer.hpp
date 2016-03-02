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
   static CInstanceSynchronizer* tryOpen(const char* sharedObjectName);
   static CInstanceSynchronizer* tryCreate(const char* sharedObjectName);
   ~CInstanceSynchronizer();

   void setAttached(bool isAttached);
   bool stopInstance();
   void saveThisInstanceId();

   class IUniqueInstance
   {
   public:
      virtual void setAttached(bool isAttached);
      virtual bool saveProcessId();
   };

   static IUniqueInstance* createUnique(const char* instanceName);

   class IInstanceHandler
   {
   public:
      virtual bool stopInstance();
      virtual int processId();
   };

   static IInstanceHandler* getHandler(const char* instanceHane);

private:
   CInstanceSynchronizer(boost::interprocess::shared_memory_object& shmo, bool isAttached);

   boost::interprocess::shared_memory_object mShmo;
   bool mIsAttached;
};

