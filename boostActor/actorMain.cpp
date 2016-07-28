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

typedef boost::mpl::list<FirstMessage, SecondMessage> tMsgTypeList;
typedef boost::variant<boost::detail::variant::over_sequence<tMsgTypeList> > tMsgVariant;

class FirstSecondActor: public TActor<tMsgTypeList>
{
   virtual void on(boost::shared_ptr<FirstMessage>)
   {
      std::cout << "First message handled" << std::endl;
   }

   virtual void on(boost::shared_ptr<SecondMessage>)
   {
   }

   //void on(const SecondMessage&) {}
   //void on(const ThirdMessage&) {}
};

int main(int, char**)
{
   boost::shared_ptr<FirstSecondActor> actor(new FirstSecondActor());
   tFirstMessagePtr fm(new FirstMessage);
   actor->post(fm);

   //std::cout << actor->mMsgQueue.size() << " messages in queue" << std::endl;

   //std::cout << "Size of actor: " << sizeof(FirstSecondActor) << std::endl;

   return 0;
}
