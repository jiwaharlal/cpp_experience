#include <boost/thread/future.hpp>
#include <boost/foreach.hpp>
#include <vector>

template <class FutureType>
class FutureExpectant
{
public:
   typedef FutureType tFuture;
   typedef boost::function<void(const tFuture&)> tCallback;

private:
   typedef std::vector<tFuture> tFutureList;

public:
   FutureExpectant(const tCallback& callback);
   ~FutureExpectant();

   void addFuture(const tFuture& f);

private:
   void threadFunction();

private:
   tCallback mCallback;

   tFutureList mFutures;
   boost::shared_future<tFuture> mExpectedFuture;
   boost::promise<tFuture> mExpectedPromise;
   boost::thread mThread;
};

template <typename FutureType>
FutureExpectant<FutureType>::FutureExpectant(const tCallback& callback)
   : mCallback(callback)
{
   mExpectedPromise = boost::promise<tFuture>();
   mExpectedFuture = mExpectedPromise.get_future().share();
   mThread = boost::thread(boost::bind(&FutureExpectant::threadFunction, this));
}

template <typename FutureType>
FutureExpectant<FutureType>::~FutureExpectant()
{
   mExpectedPromise.set_exception(std::bad_exception());
}

template <typename FutureType>
void FutureExpectant<FutureType>::addFuture(const tFuture& f)
{
   std::cout << "setting value to promise" << std::endl;
   mExpectedPromise.set_value(f);
}

template <typename FutureType>
void FutureExpectant<FutureType>::threadFunction()
{
   while (true)
   {
      boost::detail::future_waiter w;
      BOOST_FOREACH(tFuture& f, mFutures)
      {
         w.add(f);
      }
      w.add(mExpectedFuture);

      std::cout << "now waiting for " << mFutures.size() + 1 << " futures" << std::endl;
      int futureIndex = w.wait();
      std::cout << "awaking on index " << futureIndex << std::endl;

      if (mExpectedFuture.has_exception())
      {
         std::cout << "Exception, terminating thread" << std::endl;
         break;
      }
      if (futureIndex == mFutures.size())
      {
         std::cout << "Extracting new future" << std::endl;

         mFutures.push_back(mExpectedFuture.get());
         mExpectedPromise = boost::promise<tFuture>();
         mExpectedFuture = mExpectedPromise.get_future().share();
      }
      else
      {
         std::cout << "Some of awaited futures was set" << std::endl;

         typename tFutureList::iterator futureIt = mFutures.begin() + futureIndex;
         mCallback(mFutures[futureIndex]);
         mFutures.erase(mFutures.begin() + futureIndex);
      }
   }
}
