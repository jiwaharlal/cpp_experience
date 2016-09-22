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

#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/type_index.hpp>
#include <iostream>
#include <map>
#include <set>
#include <string>

//#include "framework/logger/Log.hpp"
#include "THandlerBase.hpp"
//#include "navigation/utils/ForEach.hpp"

struct HandlerListBase {};

class CBoard : boost::noncopyable
{
public: // methods
   template <typename MessageType>
   void subscribe(THandler<MessageType>* handler);

   //template <typename MessageTypeList, typename HandlerType>
   //void subscribeList(HandlerType* handler);

   template <typename MessageType>
   void unsubscribe(THandler<MessageType>& handler);

   template <typename MessageType>
   void publish(MessageType message);

private: // types
   typedef std::map<boost::typeindex::type_index, boost::shared_ptr<HandlerListBase> > tHandlerMap;
   template <typename HandlerType> struct Subscriber;

private: // fields
   tHandlerMap mHandlerMap;
   boost::mutex mHandlersMutex;
};

template <typename MessageType>
struct THandlerList: public HandlerListBase
{
   typedef std::set<THandler<MessageType>* > tHandlerSet;

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

//template <typename HandlerType>
//struct CBoard::Subscriber
//{
   //Subscriber(HandlerType* handler, CBoard* board)
      //: mHandler(handler)
      //, mBoard(board)
   //{}

   //template <typename T>
   //void oprator ()(Wrap<T>&)
   //{
      //operator ()<T>();
   //}

   //template <typename MsgType>
   //void operator ()()
   //{
      //mBoard->subscribe<MsgType>(static_cast<THandler<MsgType>*>(mHandler));
   //}

   //HandlerType* mHandler;
   //CBoard* mBoard;
//};

//template <typename MessageTypeList, typename HandlerType>
//void CBoard::subscribeList(HandlerType* handler)
//{
   //forEach<MessageTypeList>(Subscriber<HandlerType>(handler, this));
//}

template <typename MessageType>
void CBoard::unsubscribe(THandler<MessageType>& handler)
{
   typedef typename THandlerList<MessageType>::tHandlerSet tHandlerSet;

   boost::lock_guard<boost::mutex> lk(mHandlersMutex);

   tHandlerMap::iterator it = mHandlerMap.find(boost::typeindex::type_id<MessageType>());
   if (it == mHandlerMap.end())
   {
      return;
   }

   tHandlerSet& handlers = static_cast<THandlerList<MessageType>* >(it->second)->mHandlers;
   handlers->erase(handler);
}

template <typename MessageType>
void CBoard::publish(MessageType message)
{
   typedef typename THandlerList<MessageType>::tHandlerSet tHandlerSet;

   boost::lock_guard<boost::mutex> lk(mHandlersMutex);

   tHandlerMap::iterator it = mHandlerMap.find(boost::typeindex::type_id<MessageType>());
   if (it == mHandlerMap.end())
   {
      std::cout << "No handler for message of type "
               << boost::typeindex::type_id<MessageType>().pretty_name() << std::endl;
      return;
   }

   tHandlerSet& handlers = static_cast<THandlerList<MessageType>* >(it->second.get())->mHandlers;
   BOOST_FOREACH (typename tHandlerSet::value_type handler, handlers)
   {
      handler->post(message);
   }
}
