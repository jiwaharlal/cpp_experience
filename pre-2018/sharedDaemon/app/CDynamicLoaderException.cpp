/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CDynamicLoaderException.cpp
 * @author   Maxim Bondarenko
 * @date     04.03.2016
 */

#include "CDynamicLoaderException.hpp"

#include <sstream>
#include <string.h>

CDynamicLoaderException::CDynamicLoaderException(const char* libName, const char* reason)
{
   try
   {
      std::stringstream ss;
      ss << "Error loading: " << libName << " Reason: " << reason;
      mWhat = ss.str();
   }
   catch (...)
   {
   }
}

CDynamicLoaderException::~CDynamicLoaderException() throw()
{
}

const char* CDynamicLoaderException::what() const throw()
{
   return mWhat.c_str();
}

