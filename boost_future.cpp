#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/thread/future.hpp>
#include <iostream>

boost::future<int> getInt(const std::string&)
{
   boost::promise<int> pi;
   pi.set_value(42);
   return pi.get_future();
}

int main(int, char**)
{
   boost::future<int> f;

   std::cout << getInt("give me int").get() << std::endl;

   return 0;
}
