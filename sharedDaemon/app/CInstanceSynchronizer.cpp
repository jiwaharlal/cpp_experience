/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CInstanceSynchronizer.cpp
 * @author   Maxim Bondarenko
 * @date     02.03.2016
 */

#include "CInstanceSynchronizer.hpp"

namespace ip = boost::interprocess;

// private
CInstanceSynchronizer::CInstanceSynchronizer(ip::shared_memory_object& shmo, bool isAttached)
   : mIsAttached(isAttached)
{
   mShmo.swap(shmo);
}

CInstanceSynchronizer::~CInstanceSynchronizer()
{
   ip::mode_t mode = mShmo.get_mode();
   if (isAttached)
   {
      ip::shared_memory_object::remove(mShmo.get_name());
   }
}

CInstanceSynchronizer* CInstanceSynchronizer::tryOpen(const char* sharedObjectName)
{
   try 
   {
      ip::shared_memory_object shmo(ip::open_only, sharedObjectName, ip::read_only);
      return new CInstanceSynchronizer(shmo, false);
   }
   catch (...)
   {
      return NULL;
   }
}

CInstanceSynchronizer* CInstanceSynchronizer::tryCreate(const char* sharedObjectName)
{
   try
   {
      ip::shared_memory_object shmo(ip::create_only, sharedObjectName, ip::read_write);
      return new CInstanceSynchronizer(shmo, true);
   }
   catch (...)
   {
      return NULL;
   }
}

void CInstanceSynchronizer::setAttached(bool isAttached)
{
   mIsAttached = isAttached;
}

bool CInstanceSynchronizer::stopInstance()
{
}
