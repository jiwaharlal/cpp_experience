#include <algorithm>
#include <set>
#include <iostream>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class IntStorage
{
public:
   IntStorage(int value)
      : mValue(value)
   {}

   int get() const
   {
      return mValue;
   }

   bool operator <(const IntStorage& other) const
   {
      return get() < other.get();
   }
private:
   int mValue;
};

template <class T>
std::ostream& operator <<(std::ostream& out, const boost::shared_ptr<T>& ptr)
{
   out << *ptr;
   return out;
}

std::ostream& operator <<(std::ostream& out, const IntStorage& storage)
{
   out << storage.get();
   return out;
}

namespace
{
   template <class T>
   struct SameObject
   {
      SameObject(T* ptr) : mPtr(ptr) {}
      bool operator()(const boost::shared_ptr<T>& shared) const { return shared.get() == mPtr; }
      T* mPtr;
   };
}

int main(int argc, char** argv)
{
   (void) argc;
   (void) argv;

   std::set<boost::shared_ptr<int> > ints;
   std::set<IntStorage> storages;
   for (int i = 0; i < 10; i++)
   {
      ints.insert(boost::shared_ptr<int>(new int(i)));
      storages.insert(i);
   }

   int* someInt = new int(42);
   
   std::ostream_iterator<boost::shared_ptr<int> > out_it (std::cout, ", ");
   std::copy(ints.begin(), ints.end(), out_it);
   std::cout << std::endl;
   std::ostream_iterator<IntStorage> s_out_it(std::cout, ", ");
   std::copy(storages.begin(), storages.end(), s_out_it);
   //std::for_each( ints.begin(), ints.end(), boost::bind(&std::ostream::operator<<, std::cout) );
   std::cout << std::endl;

   //ints.erase(ints.end());
   std::remove_if(ints.begin(), ints.end(), SameObject<int>(someInt));

   //ints.erase( ints.find( 5 ) );

   //std::copy(ints.begin(), ints.end(), out_it);
   //std::cout << std::endl;

   return 0;
}
