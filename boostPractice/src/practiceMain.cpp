#define BOOST_THREAD_PROVIDES_FUTURE
#include <iostream>
#include <boost/chrono.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/future.hpp>
#include <typeinfo>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>

#include "FutureExpectant.hpp"

enum EFutureId
{
   e_mapData,
   e_routeData,
   e_pos
};

template <class T>
EFutureId getUnifiedFuture()
{
}

boost::future<int> getInt()
{
   boost::shared_ptr<boost::promise<int> > pi(new boost::promise<int>());
   boost::thread([=]()
   {
      boost::this_thread::sleep(boost::posix_time::milliseconds(400));
      pi->set_value(42);
   });

   return pi->get_future();
}

boost::future<int> getInt2()
{
   return boost::make_future(356);
}

boost::future<std::string> getString()
{
   boost::shared_ptr<boost::promise<std::string> > ps(new boost::promise<std::string>());
   boost::thread([=]()
   {
      boost::this_thread::sleep(boost::posix_time::milliseconds(700));
      ps->set_value("hello");
   });
   return ps->get_future();
}

struct FutureHolder;

template <class FutureType>
struct FutureHolderImpl;

class Visitor
{
   virtual ~Visitor() {}
   virtual void visit(FutureHolder& holder) = 0;
};

//struct FutureHolder
//{
   //virtual void addWaiter(boost::detail::future_waiter& waiter) = 0;
   //virtual void visitWith(Visitor& visitor) = 0;
//};

//template <typename FutureType>
//class VisitorImpl
//{
   //virtual void visit(FutureHolder& holder)
   //{
      //holder.visitWith(*this);
   //}

   //virtual void visit(FutureHolderImpl<FutureType>& holder)
   //{
      //process(holder.future);
   //}

   //virtual void process(FutureType& future) = 0;
//};

//template <typename FutureType>
//struct FutureHolderImpl: public FutureHolder
//{
   //FutureHolderImpl(FutureType f) : future(f) {}
   //virtual void addWaiter(boost::detail::future_waiter& waiter)
   //{
      //waiter.add(future);
   //}

   //void visitWith(VisitorImpl<FutureType>& visitor)
   //{
      //visitor.visit(*this);
   //}

   //FutureType future;
//};

//typedef boost::shared_ptr<FutureHolder> tFutureHolderPtr;

//template <typename FutureType>
//tFutureHolderPtr createHolder(FutureType f)
//{
   //return tFutureHolderPtr(new FutureHolderImpl<FutureType>(f));
//}

void printContent(boost::shared_future<int> fi)
{
   if (fi.is_ready())
   {
      if (fi.has_value())
      {
         std::cout << "Future content: " << fi.get() << std::endl;
      }
      else if (fi.has_exception())
      {
         std::cout << "Exception occured" << std::endl;
         std::cout << fi.get_exception_ptr() << std::endl;
      }
      else
      {
         std::cout << "Future holds exception" << std::endl;
      }
   }
}

void useFuture()
{
   boost::shared_future<int> fi = getInt().share();
   std::cout << fi.has_value() << std::endl;

   auto fs = getString().share();

   boost::detail::future_waiter w;

   int index = w.wait();
   std::cout << index << "'th future reacted" << std::endl;
}

void useFutureExpectant()
{
   auto expectant = boost::make_shared<FutureExpectant<boost::shared_future<int> > >(&printContent);
   //FutureExpectant<boost::shared_future<int> > expectant(&printContent);
   std::cout << "expectant created" << std::endl;

   //boost::this_thread::sleep(boost::posix_time::milliseconds(100));
   expectant->addFuture(getInt2().share());
   //expectant->addFuture(getInt().share());
   //expectant->addFuture(getInt().share());
   //expectant->addFuture(getInt().share());
   //expectant->addFuture(getInt().share());
   //expectant->addFuture(getInt().share());
   std::cout << "future added" << std::endl;

   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   //delete expectant;
   expectant.reset();

   std::cout << "that's all" << std::endl;
}

struct DataProcessor
{
   DataProcessor()
      : mPromise(new boost::promise<int>())
   {}

   boost::future<int> processData(int value)
   {
      boost::thread([=]()
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
         //mPromise->set_value(value * 2);
         try
         {
            throw std::invalid_argument(boost::lexical_cast<std::string>(value) + " is very invalid");
         }
         catch (const std::exception& e)
         {
            mPromise->set_exception(boost::current_exception());
         }
      });
      return mPromise->get_future();
   }
   boost::shared_ptr<boost::promise<int> > mPromise;
};

void useDataProcessor()
{
   DataProcessor dp;
   auto f1 = dp.processData(34).share();

   //boost::this_thread::sleep(boost::posix_time::milliseconds(400));

   //boost::detail::set_wait_callback(f1, );
   f1.wait();

   //auto f2 = dp.processData(34).share();

   if (f1.is_ready())
   {
      try
      {
         std::cout << f1.get() << std::endl;
      }
      catch (const std::exception& e)
      {
         std::cout << "Exception occured " << e.what() << std::endl;
      }
   }
}

void useChrono()
{
   using namespace boost::chrono;

   std::cout << "At least it builds" << std::endl;
   std::cout << system_clock::now().time_since_epoch() << std::endl;
   std::cout << typeid(system_clock::now().time_since_epoch()).name() << std::endl;

   boost::chrono::microseconds ms(10);
   std::cout << sizeof(ms) << std::endl;

   boost::chrono::microseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
   std::cout << now << std::endl;
   now += minutes(200);
   int64_t i = now.count();
   std::cout << i << std::endl;
   std::cout << sizeof(const boost::chrono::microseconds&) << std::endl;

   boost::chrono::microseconds minus(-1);
   std::cout << minus << std::endl;
}

void intCallback(boost::promise<int>& i)
{
   std::cout << __FUNCTION__ << " " << i.get_future().get() << std::endl;
}

void getIntWithCallback()
{
   boost::promise<int> pi;
   pi.set_wait_callback(&intCallback);

   boost::this_thread::sleep(boost::posix_time::milliseconds(400));
   pi.set_value(10);
   boost::this_thread::sleep(boost::posix_time::milliseconds(400));
}

void useWaitCallback()
{
   getIntWithCallback();
}

int main(int, char**)
{
   useFutureExpectant();
   //useDataProcessor();
   //useWaitCallback();

   return 0;
}
