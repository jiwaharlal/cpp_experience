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

#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>
#include <signal.h>

#include "CInstanceSynchronizer.hpp"

namespace ip = boost::interprocess;

namespace
{
   class CUniqueInstanceImpl: public CInstanceSynchronizer::IUniqueInstance
   {
   public:
      CUniqueInstanceImpl(ip::shared_memory_object& shmo);
      virtual ~CUniqueInstanceImpl();
      virtual void setAttached(bool isAttached);
      virtual bool saveProcessId();

   private:
      bool mIsAttached;
      ip::shared_memory_object mShmo;
   };

   class CInstanceHandlerImpl: public CInstanceSynchronizer::IInstanceHandler
   {
   public:
      CInstanceHandlerImpl(int processId);
      //virtual ~CInstanceHandlerImpl();
      virtual bool stopInstance();
      virtual int getProcessId();

   private:
      int mProcessId;
   };

   CUniqueInstanceImpl::CUniqueInstanceImpl(ip::shared_memory_object& shmo)
      : mIsAttached(true)
   {
      mShmo.swap(shmo);
   }

   CUniqueInstanceImpl::~CUniqueInstanceImpl()
   {
      if ( mIsAttached )
      {
         ip::shared_memory_object::remove(mShmo.get_name());
      }
   }

   void CUniqueInstanceImpl::setAttached(bool isAttached)
   {
      mIsAttached = isAttached;
   }

   bool CUniqueInstanceImpl::saveProcessId()
   {
      try
      {
         mShmo.truncate( sizeof(int));
         ip::mapping_handle_t mappingHandle = mShmo.get_mapping_handle();
         int* pId = reinterpret_cast<int*>(mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, mappingHandle.handle, 0));
         if ( pId == MAP_FAILED )
         {
            std::cout << "Mapping error" << strerror(errno) << std::endl;
            return false;
         }
         *pId = getpid();
         if ( munmap(pId, sizeof(int)) != 0 )
         {
            std::cout << "Unmapping error " << strerror(errno) << std::endl;
            return false;
         }
      } 
      catch(ip::interprocess_exception& e)
      {
         // executable is already running
         std::cout << e.what() << std::endl;
         return false; 
      }

      return true;
   }

   CInstanceHandlerImpl::CInstanceHandlerImpl(int processId)
      : mProcessId(processId)
   {
   }

   bool CInstanceHandlerImpl::stopInstance()
   {
      if (kill(mProcessId, SIGINT) != 0)
      {
         std::cout << "Error sending interuption signal. " << strerror(errno) << std::endl;
         return false;
      }
      return true;
   }

   int CInstanceHandlerImpl::getProcessId()
   {
      return mProcessId;
   }
}

CInstanceSynchronizer::IUniqueInstance* CInstanceSynchronizer::createUnique(const char* instanceName)
{
   try
   {
      ip::shared_memory_object shmo(ip::create_only, instanceName, ip::read_write);
      return new CUniqueInstanceImpl(shmo);
   }
   catch (...)
   {
      return NULL;
   }
}

CInstanceSynchronizer::IInstanceHandler* CInstanceSynchronizer::getHandler(const char* instanceName)
{
   try 
   {
      ip::shared_memory_object newShm(ip::open_only, instanceName, ip::read_only);
      ip::offset_t size;
      newShm.get_size(size);
      if ( size != sizeof(int) )
      {
         std::cout << "Wrong size of shared momory object." << std::endl;
         return NULL;
      }
      ip::mapping_handle_t mappingHandle = newShm.get_mapping_handle();
      int* pId = reinterpret_cast<int*>(mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, mappingHandle.handle, 0));
      if ( pId == MAP_FAILED )
      {
         std::cout << "Mapping error" << std::endl;
         return NULL;
      }
      
      IInstanceHandler* handler = new CInstanceHandlerImpl(*pId);

      if ( munmap(pId, sizeof(int)) != 0 )
      {
         std::cout << "Unmapping error " << strerror(errno) << std::endl;
      }

      return handler;
   }
   catch(ip::interprocess_exception& e)
   {
      std::cout << e.what() << std::endl;
      return NULL;
   }
   catch(...)
   {
      std::cout << "Unknown error" << std::endl;
      return NULL;
   }
}

