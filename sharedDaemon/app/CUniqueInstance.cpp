/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CUniqueInstance.cpp
 * @author   Maxim Bondarenko
 * @date     02.03.2016
 */

#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>
#include <signal.h>

#include "CUniqueInstance.hpp"

namespace ip = boost::interprocess;

// private
CUniqueInstance::CUniqueInstance(ip::shared_memory_object& shmo)
   : mIsAttached(true)
{
   mShmo.swap(shmo);
}

CUniqueInstance::~CUniqueInstance()
{
   if ( mIsAttached )
   {
      ip::shared_memory_object::remove(mShmo.get_name());
   }
}

void CUniqueInstance::setAttached(bool isAttached)
{
   mIsAttached = isAttached;
}

bool CUniqueInstance::saveProcessId()
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

      return true;
   } 
   catch(ip::interprocess_exception& e)
   {
      // executable is already running
      std::cout << e.what() << std::endl;
   }
   catch (...)
   {
      std::cout << "Unknown error while saving process ID." << std::endl;
   }

   return false;
}

bool CUniqueInstance::stopInstance(const char* instanceName)
{
   int processId = getProcessId(instanceName);
   if (processId == -1)
   {
      std::cout << "Seems like the instance with name " << instanceName << " does not exist." << std::endl;
      return false;
   }
   if (kill(processId, SIGINT) != 0)
   {
      std::cout << "Error sending interuption signal. " << strerror(errno) << std::endl;
      return false;
   }
   return true;
}

CUniqueInstance* CUniqueInstance::create(const char* instanceName)
{
   try
   {
      ip::shared_memory_object shmo(ip::create_only, instanceName, ip::read_write);
      return new CUniqueInstance(shmo);
   }
   catch (...)
   {
      return NULL;
   }
}

int CUniqueInstance::getProcessId(const char* instanceName)
{
   try 
   {
      ip::shared_memory_object newShm(ip::open_only, instanceName, ip::read_only);
      ip::offset_t size;
      newShm.get_size(size);
      if ( size != sizeof(int) )
      {
         std::cout << "Wrong size of shared momory object." << std::endl;
         return -1;
      }
      ip::mapping_handle_t mappingHandle = newShm.get_mapping_handle();
      int* pId = reinterpret_cast<int*>(mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, mappingHandle.handle, 0));
      if ( pId == MAP_FAILED )
      {
         std::cout << "Mapping error" << std::endl;
         return -1;
      }
      
      int id = *pId;

      if ( munmap(pId, sizeof(int)) != 0 )
      {
         std::cout << "Unmapping error " << strerror(errno) << std::endl;
      }

      return id;
   }
   catch(ip::interprocess_exception& e)
   {
      std::cout << e.what() << std::endl;
   }
   catch(...)
   {
      std::cout << "Unknown error" << std::endl;
   }

   return -1;
}

