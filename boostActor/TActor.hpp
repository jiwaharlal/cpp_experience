/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     TActor.hpp
 * @author   Maxim Bondarenko
 * @date     29.07.2016
 */

#pragma once

#include <boost/atomic.hpp>
#include <boost/container/deque.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <queue>

#include "THandlerBase.hpp"
#include "TVariant.hpp"
#include "CBoard.hpp"
#include "Wrap.hpp"
//#include "navigation/utils/ForEach.hpp"

class CBoard;

template <typename MsgTypeList>
class TActor : boost::noncopyable
               , public THandlerBase<MsgTypeList>
{
public: // types
   typedef TVariant<MsgTypeList> tMsgVariant;

public: // methods
   TActor(CBoard* board);

   void start();

   void stop();

   virtual void post(const boost::any& anyMsg);

   CBoard* board() const;

private: // types
   typedef std::queue<tMsgVariant, boost::container::deque<tMsgVariant> > tMsgQueue;
   struct MsgPusher;

private: // methods
   void threadFunction();

private: // fields
   tMsgQueue mMsgQueue;
   boost::mutex mMsgsMutex;
   boost::atomic<bool> mIsStop;
   boost::thread mThread;
   boost::condition_variable mCondition;

protected: // fields
   CBoard* mBoard;
};

template <typename MsgTypeList>
struct TActor<MsgTypeList>::MsgPusher
{
   MsgPusher(tMsgQueue& msgQueue, const boost::any& anyMsg)
      : mMsgQueue(msgQueue)
      , mAnyMsg(anyMsg)
   {}

   template <typename MsgType>
   void operator ()()
   {
      if (mAnyMsg.type() == typeid(MsgType))
      {
         mMsgQueue.push(tMsgVariant(boost::any_cast<MsgType>(mAnyMsg)));
      }
   }

   tMsgQueue& mMsgQueue;
   const boost::any& mAnyMsg;
};

template <typename MsgTypeList>
TActor<MsgTypeList>::TActor(CBoard* board)
   : mBoard(board)
{
   //board->subscribeList<MsgTypeList>(this);
}

template <typename MsgTypeList>
void TActor<MsgTypeList>::start()
{
   mIsStop = false;
   mThread = boost::thread(boost::bind(&TActor::threadFunction, this));
}

template <typename MsgTypeList>
void TActor<MsgTypeList>::stop()
{
   static const boost::chrono::seconds sJoinDuration(1);

   mIsStop = true;
   mCondition.notify_one();
   if (!mThread.try_join_for(sJoinDuration))
   {
      mThread.interrupt();
   }
}

template <typename MsgTypeList>
void TActor<MsgTypeList>::post(const boost::any& anyMsg)
{
   {
      boost::lock_guard<boost::mutex> lk(mMsgsMutex);
      //forEach<MsgTypeList>(MsgPusher(mMsgQueue, anyMsg));
      boost::mpl::for_each<MsgTypeList, Wrap<boost::mpl::placeholders::_1> >(FunctorWrapper<MsgPusher>(mMsgQueue, anyMsg));
   }
   mCondition.notify_one();
}

template <typename MsgTypeList>
CBoard* TActor<MsgTypeList>::board() const
{
   return mBoard;
}

template <typename MsgTypeList>
void TActor<MsgTypeList>::threadFunction()
{
   while (!mIsStop)
   {
      {
         boost::mutex m;
         boost::unique_lock<boost::mutex> lk(m);

         mCondition.wait(lk);
      }

      if (mIsStop)
      {
         break;
      }

      typedef boost::unique_lock<boost::mutex> tLock;
      tLock lk;
      while ((lk = tLock(mMsgsMutex)).owns_lock() && !mMsgQueue.empty() && !mIsStop)
      {
         tMsgVariant msg = mMsgQueue.front();
         mMsgQueue.pop();
         lk.release()->unlock();

         msg.apply(*this);
      }
   }
}
