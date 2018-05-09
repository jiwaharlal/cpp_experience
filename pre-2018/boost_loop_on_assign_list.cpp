#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>

template <class T>
struct RangeCreator: public std::vector<T>
{
   RangeCreator<T> operator ()(const T& newValue)
   {
      std::vector<T>::push_back(newValue);
      return *this;
   }
};

template <class T>
RangeCreator<T> createRange(const T& value)
{
   return RangeCreator<T>()(value);
}

int main()
{
   BOOST_FOREACH (const std::string str, createRange<std::string>("onw")("two")("three"))
   {
      std::cout << str << std::endl;
   }

   return 0;
}
