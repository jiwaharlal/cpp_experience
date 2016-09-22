#include <boost/lockfree/queue.hpp>
#include <iostream>

int main()
{
   boost::lockfree::queue<int> qi;
   qi.push(12);
   int i;
   qi.pop(i);

   std::cout << i << std::endl;

   return 0;
}
