//#include <boost/asio.hpp>
//#include <boost/signals2.hpp>
//#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/for_each.hpp>
//#include <boost/type_index.hpp>
#include <boost/variant.hpp>
#include <boost/variant/detail/over_sequence.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <iostream>
#include <boost/any.hpp>
#include <map>
#include <set>
#include <queue>
#include <boost/utility/enable_if.hpp>

template <typename TypeList>
class CActor;

//struct Storage
//{
   //template <class T>
   //void put(T& value);
//};

template <typename T, typename = void>
class IHandler
{
public:
   void operator ()(const T& msg) { on(msg); }
   virtual void on(const T&) = 0;
   void post(T& msg) { post(boost::any(msg)); }
private:
   virtual void post(const boost::any&) = 0;
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

struct FirstMessage{};
struct SecondMessage{};
struct ThirdMessage{};

template <typename MsgTypeList>
class CActor: public IHandler<MsgTypeList>
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

//typedef boost::mpl::vector<FirstMessage, SecondMessage, ThirdMessage> tMsgTypeList;
typedef boost::mpl::list<FirstMessage, SecondMessage> tMsgTypeList;
typedef boost::variant<boost::detail::variant::over_sequence<tMsgTypeList> > tMsgVariant;

class FirstSecondActor: public CActor<tMsgTypeList>
{
   void on(const FirstMessage&)
   {
      std::cout << "First message handled" << std::endl;
   }

   void on(const SecondMessage&) {}
   void on(const ThirdMessage&) {}
};

struct FirstParent
{
   virtual void on(int) = 0;
};

struct SecondParent
{
   virtual void on(int) = 0;
};

struct Printer : public FirstParent, public SecondParent
{
   void on(int i)
   {
      std::cout << i << std::endl;
   }
};

int main(int, char**)
{
   boost::shared_ptr<FirstSecondActor> actor(new FirstSecondActor());
   IHandler<FirstMessage>* firstHandler(actor.get());
   FirstMessage fm;
   firstHandler->on(fm);
   firstHandler->post(fm);

   std::cout << actor->mMsgQueue.size() << " messages in queue" << std::endl;

   std::cout << "Size of actor: " << sizeof(FirstSecondActor) << std::endl;

   //tMsgVariant v = FirstMessage();

   //boost::any msg = FirstMessage();
   //tMsgVariant mv = boost::any_cast<tMsgVariant>(msg);

   Printer p;
   FirstParent& fp = p;
   SecondParent& sp = p;
   fp.on(23);
   sp.on(32);

   std::cout << "Printer size: " << sizeof(Printer) << std::endl;
   std::cout << sizeof(char) << std::endl;
   //firstHandler->post(FirstMessage());

   //std::cout << boost::typeindex::type_id<FirstMessage>().raw_name() << std::endl;
   //std::cout << boost::typeindex::type_id<SecondMessage>().raw_name() << std::endl;
   //std::cout << boost::typeindex::type_id<IHandler<SecondMessage> >().pretty_name() << std::endl;

   return 0;
}
