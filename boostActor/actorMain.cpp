//#include <boost/asio.hpp>
//#include <boost/signals2.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_index.hpp>
#include <boost/variant.hpp>
#include <boost/variant/detail/over_sequence.hpp>
#include <iostream>
#include <map>
#include <set>
#include <queue>

template <typename TypeList>
class CActor;

template <typename T>
class IHandler
{
public:
   void operator ()(const T& msg) { on(msg); }
   virtual void on(const T&) = 0;
};

struct FirstMessage{};
struct SecondMessage{};
struct ThirdMessage{};

struct HandlerListBase {};

template <typename MessageType>
struct HandlerList
{
   std::set<IHandler<MessageType> > mHandlers;
};

class CBoard
{
public:
   template <typename MessageType>
   void subscribe(IHandler<MessageType>& handler)
   {
      std::string typeName = boost::typeindex::type_id<FirstMessage>().raw_name();
      boost::shared_ptr<HandlerListBase> hl = mHandlerMap[typeName];
      if (!hl)
      {
         hl.reset(new HandlerList<MessageType>());
      }
      std::set<IHandler<MessageType> >& handlers = static_cast<HandlerList<MessageType> >(hl)->mHandlers;
      handlers.insert(handler);
   }

   //template <typename MessageType>
   //void unsubscribe(IHandler<MessageType>& handler);

   //template <typename MessageType>
   //void publish(boost::shared_ptr<MessageType> message);
private:
   std::map<int, std::string> stringMap;
   std::map<std::string, boost::shared_ptr<HandlerListBase> > mHandlerMap;
};

template <typename TypeList>
class CActor
{
public:
   CActor(CBoard& board);

   void run();
   void stop();

   template <typename MessageType>
   void post(boost::shared_ptr<MessageType> message)
   {
      mMessages.push(tMsgVariant(message));
      cw.notify_all();
   }

private:
   void threadFunction()
   {
      while (true)
      {
         boost::mutex m;
         boost::lock_guard<boost::mutex> lk(m);

         cw.wait(m);

         tMsgVariant msg = mMessages.pop();
         boost::apply_visitor(*this, msg);
      }
   }

private:
   typedef boost::variant<boost::detail::variant::over_sequence<TypeList> > tMsgVariant;
   typedef std::queue<tMsgVariant> tMsgQueue;

private:
   tMsgQueue mMessages;
   boost::condition_variable cw;
};

//class CMapDataEmitter: public CActor<boost::mpl::list<Position> >
//{
//};

typedef boost::mpl::vector<FirstMessage, SecondMessage> tMsgTypeList;

int main(int, char**)
{
   std::cout << boost::typeindex::type_id<FirstMessage>().raw_name() << std::endl;
   std::cout << boost::typeindex::type_id<SecondMessage>().raw_name() << std::endl;
   std::cout << boost::typeindex::type_id<IHandler<SecondMessage> >().pretty_name() << std::endl;

   return 0;
}
