#include <boost/variant.hpp>
#include <boost/mpl/for_each.hpp>
#include <queue>

#include "THandlerBase.hpp"

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
   typedef typename boost::make_variant_over<typename wrap_list_with_ptr<TypeList>::type>::type type;
};

//template <class TypeList>
//struct shared_ptr_variant
//{
   //typedef typename boost::make_variant_over
   //<
      //typename boost::mpl::transform
      //<
         //TypeList,
         //add_shared_ptr<boost::mpl::_1>
      //>::type
   //>::type type;
//};

//template <typename TypeList>
//class CActor: public IHandler<typename boost::mpl::transform<TypeList, add_shared_ptr<boost::mpl::_1>::type>::type>;

template <typename MsgTypeList>
class TActor : public THandlerBase<typename wrap_list_with_ptr<MsgTypeList>::type>
{
public:
   CActor(CBoard& board) : mBoard(board) {}

   void run()
   {
      mIsStop = false;
      mThread = boost::thread(boost::bind(&CActor::threadFunction, this));
   }

   void stop()
   {
      mIsStop = true;
      mCondition.notify_one();
   }

   virtual void post(const boost::any& anyMsg)
   {
      boost::mpl::for_each<MsgTypeList>(MsgPusher(mMsgQueue, anyMsg));
   }

private:
   typedef typename shared_ptr_variant<MsgTypeList>::type tMsgVariant;
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

private:
   tMsgQueue mMsgQueue;
};

