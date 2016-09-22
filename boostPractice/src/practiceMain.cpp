#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/function.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/thread/future.hpp>
#include <iostream>
#include <stdexcept>
#include <tr1/shared_ptr.h>
#include <typeinfo>

#include "FutureExpectant.hpp"

struct Gps
{
   double lat;
   double lon;
};

template <typename T>
struct SharedPtrHolder
{
   typedef boost::shared_ptr<T> tPtr;

   SharedPtrHolder(tPtr ptr) : mPtr(ptr) {}
   tPtr& get() { return mPtr; }

   tPtr mPtr;
};

typedef std::tr1::shared_ptr<Gps> tGpsPtr;
typedef boost::future<tGpsPtr> tGpsFuture;
typedef boost::shared_future<Gps> tGpsSharedFuture;

boost::future<int> getInt()
{
   typedef boost::shared_ptr<boost::promise<int> > tIntPromisePtr;
   tIntPromisePtr pi(new boost::promise<int>());
   //struct Functor
   //{
      //Functor(tIntPromisePtr& pi) : mPi(pi) {}
      //void operator ()()
      //{
         //boost::this_thread::sleep(boost::posix_time::milliseconds(400));
         //mPi->set_value(42);
      //}

      //tIntPromisePtr& mPi;
   //};
   //Functor f(pi);
   //boost::thread t(boost::bind(&Functor::operator(), f));

   return pi->get_future();
}

tGpsFuture getGps()
{
   tGpsPtr gps(new Gps());
   return boost::make_ready_future(gps);
}

tGpsSharedFuture getSharedGps()
{
   return boost::make_shared_future(Gps());
}

int main(int, char**)
{
   boost::future<int> fi = getInt();
   if (fi.is_ready())
   {
      std::cout << fi.get() << std::endl;
   }

   return 0;
}
