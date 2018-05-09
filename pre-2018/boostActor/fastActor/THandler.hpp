/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     THandler.hpp
 * @author   Maxim Bondarenko
 * @date     28.09.2016
 */

#pragma once

#include <boost/any.hpp>
#include <boost/function.hpp>

namespace NFastActor
{

template <typename MsgType>
class THandler
{
public: // methods
   virtual ~THandler() {}

   virtual void post(const MsgType& msg) = 0;

// It should be protected but compiler cannot handle the code in this case
//protected: // methods
   virtual void handle(const MsgType& msg) = 0;
};

} // NFastActor
