/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     CBoard.hpp
 * @author   Maxim Bondarenko
 * @date     29.07.2016
 */

#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/type_index.hpp>
#include <map>
#include <set>
#include <string>

#include "THandler.hpp"
#include "utils/ForEach.hpp"

namespace NFastActor
{

struct HandlerListBase {};

class CBoard : boost::noncopyable
{
public: // methods
   template <typename MessageType>
   void subscribe(THandler<MessageType>* handler);

   template <typename MessageType>
   void unsubscribe(THandler<MessageType>* handler);

   template <typename MessageTypeList, typename HandlerType>
   void subscribeList(HandlerType* handler);

   template <typename MessageTypeList, typename HandlerType>
   void unsubscribeList(HandlerType* handler);

   template <typename MessageType>
   void publish(MessageType message);

private: // types
   typedef boost::container::flat_map<boost::typeindex::type_index, boost::shared_ptr<HandlerListBase> > tHandlerMap;

private: // fields
   tHandlerMap mHandlerMap;
   boost::mutex mHandlersMutex;
};

namespace NBoardPrivate
{
   template <typename HandlerType>
   struct Subscriber
   {
      Subscriber(HandlerType* handler, CBoard* board)
         : mHandler(handler)
         , mBoard(board)
      {}

      template <typename MsgType>
      void operator ()()
      {
         mBoard->subscribe<MsgType>(static_cast<THandler<MsgType>*>(mHandler));
      }

      HandlerType* mHandler;
      CBoard* mBoard;
   };

   template <typename HandlerType>
   struct Unsubscriber
   {
      Unsubscriber(HandlerType* handler, CBoard* board)
         : mHandler(handler)
         , mBoard(board)
      {}

      template <typename MsgType>
      void operator ()()
      {
         mBoard->unsubscribe<MsgType>(static_cast<THandler<MsgType>*>(mHandler));
      }

      HandlerType* mHandler;
      CBoard* mBoard;
   };
} // NBoardPrivate

template <typename MessageTypeList, typename HandlerType>
void CBoard::subscribeList(HandlerType* handler)
{
   using NBoardPrivate::Subscriber;
   forEach<MessageTypeList>(Subscriber<HandlerType>(handler, this));
}

template <typename MessageTypeList, typename HandlerType>
void CBoard::unsubscribeList(HandlerType* handler)
{
   using NBoardPrivate::Unsubscriber;
   forEach<MessageTypeList>(Unsubscriber<HandlerType>(handler, this));
}

template <typename MessageType>
struct THandlerList: public HandlerListBase
{
   typedef boost::container::flat_set<THandler<MessageType>* > tHandlerSet;

   tHandlerSet mHandlers;
};

template <typename MessageType>
void CBoard::subscribe(THandler<MessageType>* handler)
{
   typedef typename THandlerList<MessageType>::tHandlerSet tHandlerSet;

   boost::lock_guard<boost::mutex> lk(mHandlersMutex);

   boost::shared_ptr<HandlerListBase>& hl = mHandlerMap[boost::typeindex::type_id<MessageType>()];
   if (!hl)
   {
      hl.reset(new THandlerList<MessageType>());
   }

   tHandlerSet& handlers = static_cast<THandlerList<MessageType>* >(hl.get())->mHandlers;
   handlers.insert(handler);
}

template <typename MessageType>
void CBoard::unsubscribe(THandler<MessageType>* handler)
{
   typedef typename THandlerList<MessageType>::tHandlerSet tHandlerSet;

   boost::lock_guard<boost::mutex> lk(mHandlersMutex);

   tHandlerMap::iterator it = mHandlerMap.find(boost::typeindex::type_id<MessageType>());
   if (it == mHandlerMap.end())
   {
      return;
   }

   tHandlerSet& handlers = static_cast<THandlerList<MessageType>* >(it->second.get())->mHandlers;
   handlers.erase(handler);
}

template <typename MessageType>
void CBoard::publish(MessageType message)
{
   typedef typename THandlerList<MessageType>::tHandlerSet tHandlerSet;

   boost::lock_guard<boost::mutex> lk(mHandlersMutex);

   tHandlerMap::iterator it = mHandlerMap.find(boost::typeindex::type_id<MessageType>());
   if (it == mHandlerMap.end())
   {
      //AR_LOG_INFO << "No handler for message of type "
               //<< boost::typeindex::type_id<MessageType>().pretty_name();
      return;
   }

   tHandlerSet& handlers = static_cast<THandlerList<MessageType>* >(it->second.get())->mHandlers;
   BOOST_FOREACH (typename tHandlerSet::reference handler, handlers)
   {
      handler->post(message);
   }
}

} // NFastActor
