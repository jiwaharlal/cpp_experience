#pragma once

#include <boost/atomic.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/thread.hpp>
#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <queue>

#include "THandlerBase.hpp"
#include "TVariant.hpp"

class CBoard;

template <typename T>
struct add_shared_ptr
{
   typedef boost::shared_ptr<T> type;
};

template <typename TypeList>
struct wrap_list_with_ptr
{
   typedef typename boost::mpl::transform<TypeList, add_shared_ptr<boost::mpl::_1> >::type type;
};

template <typename TypeList>
struct shared_ptr_variant
{
   typedef TVariant<typename wrap_list_with_ptr<TypeList>::type> type;
};

template <typename MsgTypeList>
class TActor : public THandlerBase<typename wrap_list_with_ptr<MsgTypeList>::type>
{
   typedef typename wrap_list_with_ptr<MsgTypeList>::type MsgPtrList;
   typedef typename shared_ptr_variant<MsgTypeList>::type tMsgVariant;

public:
   typedef void result_type;

public:
   TActor(boost::shared_ptr<CBoard> board) : mBoard(board) {}

   void run()
   {
      mIsStop = false;
      mThread = boost::thread(boost::bind(&TActor::threadFunction, this));
   }

   void stop()
   {
      mIsStop = true;
      mCondition.notify_one();
      mThread.join();
   }

   //virtual void post(const tMsgVariant& msg)
   virtual void post(const boost::any& anyMsg)
   {
      boost::mpl::for_each<MsgPtrList>(MsgPusher(mMsgQueue, anyMsg));
      mCondition.notify_one();
      //mMsgQueue.push(msg);
   }

private:
   //typedef typename boost::make_variant_over<MsgTypeList>::type tMsgVariant;
   typedef std::queue<tMsgVariant> tMsgQueue;

   struct MsgPusher
   {
      MsgPusher(tMsgQueue& msgQueue, const boost::any& anyMsg)
         : mMsgQueue(msgQueue)
         , mAnyMsg(anyMsg)
      {};

      template <typename MsgType>
      void operator ()(MsgType&)
      {
         if (mAnyMsg.type() == typeid(MsgType))
         {
            std::cout << "pushing message to queue" << std::endl;
            mMsgQueue.push(tMsgVariant(boost::any_cast<MsgType>(mAnyMsg)));
         }
      }

      tMsgQueue& mMsgQueue;
      const boost::any& mAnyMsg;
   };

private:
   void threadFunction()
   {
      while (true)
      {
         boost::mutex m;
         boost::unique_lock<boost::mutex> lk(m);

         std::cout << "waiting" << std::endl;
         mCondition.wait(lk);
         std::cout << "notified" << std::endl;

         if (mIsStop)
         {
            std::cout << "stopping" << std::endl;
            break;
         }

         while (!mMsgQueue.empty())
         {
            std::cout << "extracting message from queue" << std::endl;
            tMsgVariant msg = mMsgQueue.front();
            mMsgQueue.pop();
            //boost::apply_visitor(*this, msg);
            msg.apply(*this);
         }
      }
   }

private:
   tMsgQueue mMsgQueue;
   boost::atomic<bool> mIsStop;
   boost::thread mThread;
   boost::condition_variable mCondition;
protected:
   boost::shared_ptr<CBoard> mBoard;
};

