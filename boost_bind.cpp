#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>

void printIntPtr(boost::shared_ptr<int> i)
{
   std::cout << *i << std::endl;
}

boost::function<void(void)> getFunctor()
{
   boost::shared_ptr<int> pi(new int(12));
   boost::function<void(void)> result = boost::bind(&printIntPtr, pi);
   pi.reset(new int(34));

   return result;
}

int main()
{
   boost::function<void(void)> functor = getFunctor();
   boost::this_thread::sleep(boost::posix_time::milliseconds(400));

   functor();

   return 0;
}
