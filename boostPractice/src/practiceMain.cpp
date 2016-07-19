#define BOOST_THREAD_PROVIDES_FUTURE
#include <iostream>
#include <boost/chrono.hpp>
#include <boost/thread/future.hpp>
#include <typeinfo>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/range/algorithm.hpp>

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

struct FutureHolder
{
   virtual void addWaiter(boost::detail::future_waiter& waiter) = 0;
   virtual void visitWith(Visitor& visitor) = 0;
};

template <typename FutureType>
class VisitorImpl
{
   virtual void visit(FutureHolder& holder)
   {
      holder.visitWith(*this);
   }

   virtual void visit(FutureHolderImpl<FutureType>& holder)
   {
      process(holder.future);
   }

   virtual void process(FutureType& future) = 0;
};

template <typename FutureType>
struct FutureHolderImpl: public FutureHolder
{
   FutureHolderImpl(FutureType f) : future(f) {}
   virtual void addWaiter(boost::detail::future_waiter& waiter)
   {
      waiter.add(future);
   }

   void visitWith(VisitorImpl<FutureType>& visitor)
   {
      visitor.visit(*this);
   }

   FutureType future;
};

typedef boost::shared_ptr<FutureHolder> tFutureHolderPtr;

template <typename FutureType>
tFutureHolderPtr createHolder(FutureType f)
{
   return tFutureHolderPtr(new FutureHolderImpl<FutureType>(f));
}

void useFuture()
{
   boost::shared_future<int> fi = getInt().share();
   std::cout << fi.has_value() << std::endl;

   //boost::shared_future<std::string> fs = getString().share();
   auto fs = getString().share();

   //boost::fusion::vector<boost::future<int>, boost::future<std::string> > futures;
   //boost::waif_for_any(futures);
   boost::detail::future_waiter w;

   std::vector<tFutureHolderPtr> futures;
   futures.push_back(createHolder(fi));
   futures.push_back(createHolder(fs));
   //w.add(fi);
   //w.add(fs);

   //futures.push_back(
   //futures.emplace_back(fs);

   boost::for_each(futures, [&](tFutureHolderPtr& f){ f->addWaiter(w); });

   //futures.push_back(fi);
   //futures.push_back(fs);

   //boost::wait_for_any(fi, fs);
   //while (true)
   //{
      int index = w.wait();
      std::cout << index << "'th future reacted" << std::endl;

      if (fi.has_value() && fi.is_ready())
      {
         std::cout << fi.get() << std::endl;
      }
      if (fs.has_value() && fs.is_ready())
      {
         std::cout << fs.get() << std::endl;
         std::cout << fs.is_ready() << " " << fs.has_value() << std::endl;
         std::cout << fs.get() << std::endl;
      }
   //}

   //std::string prefix = " value = ";
   struct Printer
   {
      Printer(const std::string& p) : prefix(p) {}
      void operator ()(int i)
      {
         std::cout << prefix << i;
      }
      std::string prefix;
   };
   std::vector<int> ints = {1, 2, 3, 4, 56};
   boost::for_each(ints, Printer(" value = "));

   std::cout << "that's all" << std::endl;
   //std::cout << fi.get() << " " << fs.get() << std::endl;
   //std::cout << f.get() << std::endl;
   //fi.wait();
   //std::cout << fi.get() << std::endl;
}

struct DataProcessor
{
   boost::future<int> processData(int value)
   {
      if (mPromise)
      {
         //mPromise->set_value(0);
         mPromise->set_exception(std::exception());
      }
      mPromise.reset(new boost::promise<int>());
      boost::thread([=]()
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(400));
         mPromise->set_value(value * 2);
      });
      return mPromise->get_future();
   }
   boost::shared_ptr<boost::promise<int> > mPromise;
};

void useDataProcessor()
{
   DataProcessor dp;
   auto f1 = dp.processData(34).share();

   boost::this_thread::sleep(boost::posix_time::milliseconds(200));

   auto f2 = dp.processData(34).share();

   std::cout << f1.get() << " " << f2.get() << std::endl;
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

int main(int, char**)
{
   //useFuture();
   useDataProcessor();

   return 0;
}
