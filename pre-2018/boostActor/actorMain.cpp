#include <boost/shared_ptr.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/variant.hpp>
#include <boost/variant/detail/over_sequence.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <iostream>
#include <boost/any.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <map>
#include <set>
#include <queue>
#include <boost/utility/enable_if.hpp>
#include <boost/container/vector.hpp>

#define FAST_ACTOR

#ifdef FAST_ACTOR

#include "fastActor/TActor.hpp"
#include "fastActor/CBoard.hpp"

using namespace NFastActor;

#else

#include "actor/TActor.hpp"
#include "actor/CBoard.hpp"

using namespace NActor;

#endif


struct FirstMessage
{
   FirstMessage(int i = 0) : mI(i) {}
   int mI;
};

typedef boost::shared_ptr<FirstMessage> tFirstMessagePtr;

struct SecondMessage
{
   SecondMessage(int i = 0) : mI(i) {}
   int mI;
};

typedef boost::shared_ptr<SecondMessage> tSecondMessagePtr;

typedef boost::mpl::list<FirstMessage, SecondMessage> tPublicMsgTypeList;

typedef boost::mpl::list<std::string> tPrivateMsgTypeList;

typedef boost::mpl::copy
<
   tPublicMsgTypeList,
   boost::mpl::back_inserter
   <
      boost::mpl::copy
      <
         tPrivateMsgTypeList,
         boost::mpl::back_inserter<boost::mpl::vector<> >
      >::type
   >
>::type tMsgTypeList;

//typedef boost::variant<boost::detail::variant::over_sequence<tMsgTypeList> > tMsgVariant;

class BadException: public std::exception
{
   virtual const char* what() const _GLIBCXX_USE_NOEXCEPT { return "Bad exception"; }
};

class FirstSecondActor: public TActor<tPublicMsgTypeList, tPrivateMsgTypeList>
{
public:
   FirstSecondActor(CBoard& board)
      : TActor(board)
      , mResult(0)
   {
      board.subscribe<std::string>(this);
   }

protected:

#ifdef FAST_ACTOR
   virtual void handle(const FirstMessage& m)
#else
   virtual void operator()(const FirstMessage& m)
#endif
   {
      mResult += m.mI;
      //std::cout << "First message handled" << std::endl;
   }

#ifdef FAST_ACTOR
   virtual void handle(const SecondMessage& m)
#else
   virtual void operator()(const SecondMessage& m)
#endif
   {
      mResult += m.mI;
      //std::cout << "SecondMessage handled" << std::endl;
   }

#ifdef FAST_ACTOR
   virtual void handle(const std::string&)
#else
   virtual void operator()(const std::string&)
#endif
   {
      std::cout << "Result : " << mResult << std::endl;
      std::cout << "std::string handled" << std::endl;
      //throw BadException();
      stopFromInside();
   }

//#ifdef FAST_ACTOR
   //virtual void handle(const boost::container::vector<int>& v)
//#else
   //virtual void operator()(const boost::container::vector<int>& v)
//#endif
   //{
      //mResult += v.size();
   //}

   long long mResult;
};

int main(int, char**)
try
{
   using boost::posix_time::microsec_clock;

   boost::shared_ptr<CBoard> board(new CBoard);
   boost::shared_ptr<FirstSecondActor> actor(new FirstSecondActor(*board));

   boost::shared_future<std::string> actorTerminationFuture = actor->start();

   boost::this_thread::sleep(boost::posix_time::milliseconds(200));

   std::cout << "posting message" << std::endl;

   tFirstMessagePtr fm(new FirstMessage);
   boost::shared_ptr<int> intMsg(new int(16));
   //actor->post(fm);
   //actor->post(intMsg);

   boost::posix_time::ptime startTime = microsec_clock::local_time();

   board->publish(*fm);
   board->publish(*intMsg);

   int total;

   for (int i = 0; i < 1000000; i++)
   {
      //tFirstMessagePtr first = boost::make_shared<FirstMessage>(i);
      //tSecondMessagePtr second = boost::make_shared<SecondMessage>(i);

      board->publish(FirstMessage(i));
      board->publish(SecondMessage(i));

      //board->publish(boost::container::vector<int>(i % 1000, 10));

      total += i;
   }

   boost::posix_time::ptime endPublishTime = microsec_clock::local_time();

   board->publish(std::string("stop"));
   board->publish(SecondMessage());

   //actor->post(std::string("hello"));
   //board->publish(intMsg);

   //boost::this_thread::sleep(boost::posix_time::milliseconds(20000000));
   //actor->stop();

   boost::shared_future<std::string>* futureIt = boost::wait_for_any(&actorTerminationFuture, &actorTerminationFuture);

   std::cout << "Actor terminated with message : " << futureIt->get() << std::endl;

   boost::posix_time::ptime terminationTime = microsec_clock::local_time();

   std::cout << "Start to end publish, microsec: " << (endPublishTime - startTime).total_microseconds() << std::endl;
   std::cout << "End publish to termination: " << (terminationTime - endPublishTime).total_microseconds() << std::endl;

   return 0;
}
catch (const std::exception& e)
{
   std::cout << "Program terminated with exception : " << e.what() << std::endl;
   return 1;
}
catch (...)
{
   std::cout << "Program terminated with unknown exception" << std::endl;
   return 1;
}
