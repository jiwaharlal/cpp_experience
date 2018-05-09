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

#include <boost/move/move.hpp>
#include <boost/atomic.hpp>
#include <boost/variant.hpp>
#include <boost/container/deque.hpp>
//#include <boost/mpl/back_inserter.hpp>
//#include <boost/mpl/copy.hpp>
//#include <boost/mpl/vector.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <queue>
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/queue.hpp>

#include "CBoard.hpp"
#include "private/THandlerNode.hpp"
#include "utils/ForEach.hpp"

namespace NFastActor
{

template <typename PublicMsgTypeList, typename PrivateMsgTypeList = boost::mpl::list<> >
class TActor   : boost::noncopyable
               , public boost::static_visitor<void>
               , public NActorPrivate::THandlerNode
                 <
                     typename JoinLists<PublicMsgTypeList, PrivateMsgTypeList>::type,
                     typename boost::make_variant_over<
                        typename JoinLists<PublicMsgTypeList, PrivateMsgTypeList>::type>::type
                 >
{
private: // types
   //typedef boost::shared_future<std::string> tStringFuture;

   typedef typename JoinLists<PublicMsgTypeList, PrivateMsgTypeList>::type tMsgTypeList;

   //typedef TActor<PublicMsgTypeList, PrivateMsgTypeList> tActor;

private: // types
   typedef typename boost::make_variant_over<tMsgTypeList>::type tMsgVariant;
   typedef std::queue<tMsgVariant, boost::container::deque<tMsgVariant> > tMsgQueue;
   //typedef boost::lockfree::queue<tMsgVariant> tMsgQueue;
   //typedef boost::circular_buffer<tMsgVariant> tMsgQueue;

public: // methods
   TActor(CBoard& board);
   virtual ~TActor();

   boost::shared_future<std::string> start();

   void stop();

   virtual void postVariant(const tMsgVariant& msgVariant)
   {
      {
         boost::lock_guard<boost::mutex> lk(mMsgsMutex);
         mMsgQueue.push(msgVariant);
         //mMsgQueue.push_back(msgVariant);

         mMaxMessageCount = std::max(mMaxMessageCount, static_cast<int>(mMsgQueue.size()));
      }
      mCondition.notify_one();
   }

   CBoard& board() const;

   template <typename MessageType> void operator ()(const MessageType& msg)
   {
      static_cast<THandler<MessageType>*>(this)->handle(msg);
   }

protected: // methods
   void stopFromInside()
   {
      mIsStop = true;
      mCondition.notify_one();
   }

private: // methods
   void threadFunction();

private: // fields
   tMsgQueue mMsgQueue;
   boost::mutex mMsgsMutex;
   boost::atomic<bool> mIsStop;
   boost::thread mThread;
   boost::condition_variable mCondition;
   boost::promise<std::string> mTerminationPromise;

   int mMaxMessageCount;

protected: // fields
   CBoard& mBoard;
};

template <typename PublicMsgTypeList, typename PrivateMsgTypeList>
TActor<PublicMsgTypeList, PrivateMsgTypeList>::TActor(CBoard& board)
   : mBoard(board)
   , mMaxMessageCount(0)
   , mMsgQueue()
{}

template <typename PublicMsgTypeList, typename PrivateMsgTypeList>
TActor<PublicMsgTypeList, PrivateMsgTypeList>::~TActor()
{
   stop();
}

template <typename PublicMsgTypeList, typename PrivateMsgTypeList>
boost::shared_future<std::string> TActor<PublicMsgTypeList, PrivateMsgTypeList>::start()
{
   mIsStop = false;
   mThread = boost::thread(boost::bind(&TActor::threadFunction, this));
   return mTerminationPromise.get_future().share();
}

template <typename PublicMsgTypeList, typename PrivateMsgTypeList>
void TActor<PublicMsgTypeList, PrivateMsgTypeList>::stop()
{
   static const boost::chrono::seconds sJoinDuration(1);

   mIsStop = true;
   mCondition.notify_one();
   if (mThread.joinable() && !mThread.try_join_for(sJoinDuration))
   {
      mThread.interrupt();
   }
}

template <typename PublicMsgTypeList, typename PrivateMsgTypeList>
CBoard& TActor<PublicMsgTypeList, PrivateMsgTypeList>::board() const
{
   return mBoard;
}

template <typename PublicMsgTypeList, typename PrivateMsgTypeList>
void TActor<PublicMsgTypeList, PrivateMsgTypeList>::threadFunction()
try
{
   mBoard.subscribeList<PublicMsgTypeList>(this);

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
         //mMsgQueue.pop_front();
         lk.release()->unlock();

         boost::apply_visitor(*this, msg);
      }
   }

   mBoard.unsubscribeList<PublicMsgTypeList>(this);

   std::cout << "Max messages: " << mMaxMessageCount << "\n";

   mTerminationPromise.set_value("Correct completion");
}
catch (const std::exception& e)
{
   //AR_LOG_ERROR << "Exception caught in TActor::threadFunction: " << e.what();
   mTerminationPromise.set_exception(e);
}
catch (...)
{
   //AR_LOG_ERROR << "Unknown exception caught in TActor::threadFunction:";
   mTerminationPromise.set_exception(boost::current_exception());
}

} // NFastActor
