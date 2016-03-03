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

#include <errno.h>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>

#include "CUniqueInstance.hpp"

namespace ip = boost::interprocess;

// private
CUniqueInstance::CUniqueInstance(int fd)
   : mPidFile(fd)
   , mIsAttached(true)
{
}

CUniqueInstance::~CUniqueInstance()
{
   if ( mIsAttached )
   {
      flock(mPidFile, LOCK_UN);
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
      if (lseek(mPidFile, 0, SEEK_SET) != 0)
      {
         std::cout << "Error positioning in pid file: " << strerror(errno) << std::endl;
         return false;
      }
      int pid = getpid();
      if (write(mPidFile, &pid, sizeof(pid)) != sizeof(pid))
      {
         std::cout << "Error saving process id to file." << std::endl;
         return false;
      }

      return true;
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

namespace 
{
   std::string pidFileName(const char* instanceName)
   {
      std::stringstream ss;
      ss << "/var/run/" << instanceName << ".pid";
      return ss.str();
   }
}

CUniqueInstance* CUniqueInstance::create(const char* instanceName)
{
   try
   {
      std::string fileName = pidFileName(instanceName);
      int fd = open(fileName.c_str(), O_CREAT | O_RDWR);
      if (fd == -1)
      {
         std::cout << "Error creating locking file " << fileName << " " << strerror(errno) << std::endl;
         return NULL;
      }
      if (flock(fd, LOCK_EX | LOCK_NB) != 0)
      {
         std::cout << "Failed to acquire lock on " << fileName << " " << strerror(errno) << std::endl;
         return NULL;
      }

      return new CUniqueInstance(fd);
   }
   catch (...)
   {
      std::cout << "Unknown error" << std::endl;
      return NULL;
   }
}

int CUniqueInstance::getProcessId(const char* instanceName)
{
   try 
   {
      std::string fileName = pidFileName(instanceName);
      int fd = open(fileName.c_str(), O_RDONLY);
      if (fd == -1)
      {
         std::cout << "Error opening locking file " << fileName << " " << strerror(errno) << std::endl;
         return -1;
      }
      int processId;
      if (read(fd, &processId, sizeof(processId)) != sizeof(processId))
      {
         std::cout << "Error reading process ID from " << fileName << " " << strerror(errno) << std::endl;
         return -1;
      }

      return processId;
   }
   catch(...)
   {
      std::cout << "Unknown error" << std::endl;
      return -1;
   }
}

