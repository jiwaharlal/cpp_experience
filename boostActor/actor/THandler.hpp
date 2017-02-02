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

namespace NActor
{

namespace NActorPrivate
{
   template <typename MsgType>
   struct THanlderCaller;
}

template <typename MsgType>
class IPostman
{
   virtual void post(const MsgType& msg) = 0;
};

template <typename VariantType, typename MsgType>
class TPostmanBase: public IPostman<MsgType>
{
   typedef boost::function<void(VariantType)> tPostCallback;

   TPostmanBase(tPostCallback postCallback)
      : mPostCallback(postCallback)
   {}

   virtual void post(const MsgType& msg)
   {
      mPostCallback(msg);
   }

   tPostCallback mPostCallback;
};

template <typename MsgType>
class THandler
{
public: // methods
   virtual ~THandler() {}

   void post(const MsgType& msg)
   {
      post(boost::any(msg));
   }

protected: // methods
   virtual void operator ()(const MsgType& msg) = 0;
   virtual void post(const boost::any&) = 0;

   friend struct NActorPrivate::THanlderCaller<MsgType>;
};

} // NActor
