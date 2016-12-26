//#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/algorithm.hpp>
//#include <boost/thread/future.hpp>
#include <iostream>
#include <stdexcept>
//#include <tr1/shared_ptr.h>
#include <typeinfo>

//#include "FutureExpectant.hpp"

//struct Gps
//{
   //double lat;
   //double lon;
//};

//template <typename T>
//struct SharedPtrHolder
//{
   //typedef boost::shared_ptr<T> tPtr;

   //SharedPtrHolder(tPtr ptr) : mPtr(ptr) {}
   //tPtr& get() { return mPtr; }

   //tPtr mPtr;
//};

//typedef std::tr1::shared_ptr<Gps> tGpsPtr;
//typedef boost::future<tGpsPtr> tGpsFuture;
//typedef boost::shared_future<Gps> tGpsSharedFuture;

//boost::future<int> getInt()
//{
   //typedef boost::shared_ptr<boost::promise<int> > tIntPromisePtr;
   //tIntPromisePtr pi(new boost::promise<int>());

   //return pi->get_future();
//}

//tGpsFuture getGps()
//{
   //tGpsPtr gps(new Gps());
   //return boost::make_ready_future(gps);
//}

//tGpsSharedFuture getSharedGps()
//{
   //return boost::make_shared_future(Gps());
//}

int main(int, char**)
{
   //boost::future<int> fi = getInt();
   //if (fi.is_ready())
   //{
      //std::cout << fi.get() << std::endl;
   //}

   using namespace boost::filesystem;

   path p1("/home/mgbondarenko/Documents/map");
   path p2("/home/mgbondarenko/Documents/map.xml");

   std::string full1 = change_extension(p1, "zxml").string();
   std::string full2 = change_extension(p2, "zxml").string();

   std::cout << "path 1 " << full1 << std::endl;
   std::cout << "path 2 " << full2 << std::endl;

   return 0;
}
