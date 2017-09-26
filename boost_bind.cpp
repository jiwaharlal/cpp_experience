#include <boost/bind.hpp>
#include <boost/container/vector.hpp>
#include <boost/function.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/thread.hpp>
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
   //boost::this_thread::sleep(boost::posix_time::milliseconds(400));

   functor();

   std::function<void(const std::string&, const std::string)> print_words = [](const std::string& first, const std::string& second)
   {
       std::cout << first << " : " << second << std::endl;
   };

   print_words("hello", "world");

   auto print_second = boost::bind(print_words, "hello", _1);

   print_second("somebody");

   std::vector<int> v1 = {1, 2, 3, 4, 5};

   using IntVector = std::vector<int>;
   IntVector v2;
   auto inserter = [&v2](int i){ v2.push_back(i); };

   //auto push = boost::bind(&IntVector::push_back, v2, _1);
   boost::for_each(v1, inserter);

   std::cout << "v2:\n";
   boost::copy(v2, std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
