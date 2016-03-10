/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CDynamicLoaderException.hpp
 * @author   Maxim Bondarenko
 * @date     04.03.2016
 */

#pragma once

#include <exception>
#include <string>

class CDynamicLoaderException: public std::exception
{
public:
   CDynamicLoaderException(const char* libName, const char* reason);
   virtual ~CDynamicLoaderException() throw();

   virtual const char* what() const throw();

private:
   std::string mWhat;
};

