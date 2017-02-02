#include <boost/coroutine/all.hpp>
#include <functional>
#include <iostream>
#include <boost/bind.hpp>

using boost::coroutines::coroutine;

void cooperative(coroutine<int>::push_type &sink, int i)
{
  int j = i;
  sink(++j);
  sink(++j);
  std::cout << "end\n";
}

int main()
{
  //using std::placeholders::_1;
  coroutine<int>::pull_type source(boost::bind(cooperative, _1, 0));
  std::cout << source.get() << '\n';
  source();
  std::cout << source.get() << '\n';
  source();
}
