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
#include <map>
#include <set>
#include <queue>
#include <boost/utility/enable_if.hpp>

#include "TActor.hpp"
#include "THandlerBase.hpp"
#include "CBoard.hpp"

struct FirstMessage{};
typedef boost::shared_ptr<FirstMessage> tFirstMessagePtr;

struct SecondMessage{};
typedef boost::shared_ptr<SecondMessage> tSecondMessagePtr;

struct ThirdMessage{};

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

class FirstSecondActor: public TActor<tMsgTypeList>
{
public:
   FirstSecondActor(CBoard* board)
      : TActor(board)
   {
      //board->subscribe(static_cast<THandlerBase<boost::shared_ptr<FirstMessage> >*>(this));
      board->subscribe<FirstMessage>(this);
   }

private:
   virtual void operator ()(FirstMessage)
   {
      std::cout << "First message handled" << std::endl;
   }

   virtual void operator ()(SecondMessage)
   {
      std::cout << "SecondMessage handled" << std::endl;
   }

   virtual void operator ()(std::string)
   {
      std::cout << "std::string handled" << std::endl;
   }
};

int main(int, char**)
{
   boost::shared_ptr<CBoard> board(new CBoard);
   boost::shared_ptr<FirstSecondActor> actor(new FirstSecondActor(board.get()));

   actor->start();

   boost::this_thread::sleep(boost::posix_time::milliseconds(200));

   std::cout << "posting message" << std::endl;

   tFirstMessagePtr fm(new FirstMessage);
   boost::shared_ptr<int> intMsg(new int(16));
   //actor->post(fm);
   //actor->post(intMsg);

   board->publish(*fm);
   board->publish(intMsg);
   //board->publish(intMsg);

   boost::this_thread::sleep(boost::posix_time::milliseconds(200));
   actor->stop();
   //boost::this_thread::sleep(boost::posix_time::milliseconds(200));
   //std::cout << actor->mMsgQueue.size() << " messages in queue" << std::endl;

   //std::cout << "Size of actor: " << sizeof(FirstSecondActor) << std::endl;

   return 0;
}
