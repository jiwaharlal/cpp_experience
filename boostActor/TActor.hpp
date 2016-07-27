#include <boost/variant.hpp>
#include <boost/mpl/for_each.hpp>
#include <queue>

#include "THandlerBase.hpp"

template <typename MsgTypeList>
class TActor : public THandlerBase<MsgTypeList>
{
public:
   typedef boost::variant<boost::detail::variant::over_sequence<MsgTypeList> > tMsgVariant;
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
            mMsgQueue.push(tMsgVariant(boost::any_cast<MsgType>(mAnyMsg)));
         }
      }
      tMsgQueue& mMsgQueue;
      const boost::any& mAnyMsg;
   };

   virtual void post(const boost::any& anyMsg)
   {
      boost::mpl::for_each<MsgTypeList>(MsgPusher(mMsgQueue, anyMsg));
   }

   tMsgQueue mMsgQueue;
};

