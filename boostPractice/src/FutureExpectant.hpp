#include <boost/thread/future.hpp>
#include <boost/foreach.hpp>
#include <vector>

template <class ValueType, class CallbackType>
class FutureExpectant
{
public:
   typedef boost::shared_future<ValueType> tFuture;

private:
   typedef std::vector<tFuture> tFutureList;

public:
   FutureExpectant(CallbackType callback);
   ~FutureExpectant();

   void addFuture(boost::shared_future<ValueType> f);

private:
   void threadFunction();

private:
   CallbackType mCallback;
   tFutureList mFutures;
   boost::shared_future<tFuture> mExpectedFuture;
   boost::promise<tFuture> mExpectedPromise;
   boost::thread mThread;
};

template <class ValueType, class CallbackType>
FutureExpectant<ValueType, CallbackType>* createExpectant(CallbackType callback)
{
   return new FutureExpectant<ValueType, CallbackType>(callback);
}

template <class ValueType, class CallbackType>
FutureExpectant<ValueType, CallbackType>::FutureExpectant(CallbackType callback)
   : mCallback(callback)
{
   mExpectedPromise = boost::promise<tFuture>();
   mExpectedFuture = mExpectedPromise.get_future().share();
   mThread = boost::thread(boost::bind(&FutureExpectant::threadFunction, this));
}

template <class ValueType, class CallbackType>
FutureExpectant<ValueType, CallbackType>::~FutureExpectant()
{
   mExpectedPromise.set_exception(std::bad_exception());
}

template <class ValueType, class CallbackType>
void FutureExpectant<ValueType, CallbackType>::addFuture(boost::shared_future<ValueType> f)
{
   std::cout << "setting value to promise" << std::endl;
   mExpectedPromise.set_value(f);
}

template <class ValueType, class CallbackType>
void FutureExpectant<ValueType, CallbackType>::threadFunction()
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
         std::cout << "Som of awaited futures was set" << std::endl;

         typename tFutureList::iterator futureIt = mFutures.begin() + futureIndex;
         mCallback(mFutures[futureIndex]);
         mFutures.erase(mFutures.begin() + futureIndex);
      }
   }
}
