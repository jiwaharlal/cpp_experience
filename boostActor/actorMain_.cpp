//#include <boost/asio.hpp>
//#include <boost/signals2.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_index.hpp>
#include <boost/variant.hpp>
#include <boost/variant/detail/over_sequence.hpp>
#include <iostream>
#include <map>
#include <set>
#include <queue>

template <typename TypeList>
class CActor;

template <typename T, typename = void>
class IHandler
{
public:
   void operator ()(T msg) { on(msg); }
   virtual void on(T) = 0;
   virtual void post(T) = 0;
};

template <typename T>
class IHandler
<
   T,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<T>,
         boost::mpl::greater_equal
         <
            boost::mpl::size<T>,
            boost::mpl::int_<2>
         >
      >
   >::type
>  : public IHandler<typename boost::mpl::front<T>::type>
   , public IHandler<typename boost::mpl::pop_front<T>::type>
{};

template <typename T>
class IHandler
<
   T,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<T>,
         boost::mpl::equal_to
         <
            boost::mpl::size<T>,
            boost::mpl::int_<1>
         >
      >
   >::type
>  : public IHandler<typename boost::mpl::front<T>::type>
{};

struct HandlerListBase {};

template <typename MessageType>
struct HandlerList: public HandlerListBase
{
   std::set<IHandler<MessageType> > mHandlers;
};

class CBoard
{
public:
   template <typename MessageType>
   void subscribe(IHandler<MessageType>& handler)
   {
      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();
      boost::shared_ptr<HandlerListBase> hl = mHandlerMap[typeName];
      if (!hl)
      {
         hl.reset(new HandlerList<MessageType>());
      }
      std::set<IHandler<MessageType> >& handlers = static_cast<HandlerList<MessageType>* >(hl.get())->mHandlers;
      handlers.insert(handler);
   }

   template <typename MessageType>
   void unsubscribe(IHandler<MessageType>& handler);

   template <typename MessageType>
   void publish(MessageType message)
   {
      typedef std::set<IHandler<MessageType> > tHandlerSet;
      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();
      boost::shared_ptr<HandlerListBase> hl = mHandlerMap[typeName];
      if (!hl)
      {
         hl.reset(new HandlerList<MessageType>());
      }
      std::set<IHandler<MessageType> >& handlers = static_cast<HandlerList<MessageType>* >(hl.get())->mHandlers;
      BOOST_FOREACH(typename tHandlerSet::reference handler, handlers)
      {
         handler.post(message);
      }
   }
private:
   typedef std::map<std::string, boost::shared_ptr<HandlerListBase> > tHandlerMap;
   tHandlerMap mHandlerMap;
};

template <typename T>
struct add_shared_ptr
{
   typedef boost::shared_ptr<T> type;
};

template<class Seq>
struct shared_ptr_variant
{
    typedef typename boost::make_variant_over<
            typename boost::mpl::transform<
                Seq, add_shared_ptr<boost::mpl::_1>
            >::type
        >::type type;
};

template <typename TypeList>
class CActor: public IHandler<typename boost::mpl::transform<TypeList, add_shared_ptr<boost::mpl::_1>::type>::type>
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

   template <typename MessageType>
   void post(boost::shared_ptr<MessageType> message)
   {
      mMessages.push(tMsgVariant(message));
      mCondition.notify_one();
   }

private:
   void threadFunction()
   {
      while (true)
      {
         boost::mutex m;
         boost::unique_lock<boost::mutex> lk(m);

         mCondition.wait(lk);
         if (mIsStop)
         {
            break;
         }

         tMsgVariant msg = mMessages.pop();
         boost::apply_visitor(*this, msg);
      }
   }

private:
   typedef typename shared_ptr_variant<TypeList>::type tMsgVariant;
   typedef std::queue<tMsgVariant> tMsgQueue;

private:
   tMsgQueue mMessages;
   boost::condition_variable mCondition;
   CBoard& mBoard;
   boost::thread mThread;
   boost::atomic<bool> mIsStop;
};

struct FirstMessage{};
struct SecondMessage{};
struct ThirdMessage{};

typedef boost::mpl::vector<FirstMessage, SecondMessage> tMsgTypeList;

class CMapDataEmitter: public CActor<boost::mpl::list<FirstMessage> >
{
   typedef CActor<boost::mpl::list<FirstMessage> > tParent;
public:
   CMapDataEmitter(CBoard& board) : tParent(board) {}

   virtual void on(boost::shared_ptr<FirstMessage>)
   {
   }

   virtual void on(boost::shared_ptr<SecondMessage>)
   {
   }
};

int main(int, char**)
{
   CBoard board;
   CMapDataEmitter dataEmitter(board);
   board.publish(boost::shared_ptr<FirstMessage>(new FirstMessage()));

   //std::cout << boost::typeindex::type_id<FirstMessage>().raw_name() << std::endl;
   //std::cout << boost::typeindex::type_id<SecondMessage>().raw_name() << std::endl;
   //std::cout << boost::typeindex::type_id<IHandler<SecondMessage> >().pretty_name() << std::endl;



   return 0;
}
