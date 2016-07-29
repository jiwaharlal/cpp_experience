#include <boost/mpl/list.hpp>

#include "THandlerBase.hpp"

template <typename TypeList>
struct TVariantBase
{
   virtual void apply(THandlerBase<TypeList>&) = 0;
   virtual ~TVariantBase() {}
};

template <typename T, typename TypeList>
struct TVariantImpl: public TVariantBase<TypeList>
{
   TVariantImpl(const T& v) : mValue(v) {}
   T get() { return mValue; }
   void apply(THandlerBase<TypeList>& v) { static_cast<THandlerBase<T>& >(v)(mValue); }

   T mValue;
};

template <class TypeList>
struct TVariant
{
   template <typename T>
   TVariant(const T& v)
      : mHolder(new TVariantImpl<T, TypeList>(v))
   {}

   template <typename T>
   void put(const T& value)
   {
      mHolder.reset(new TVariantImpl<T, TypeList>(value));
   }

   boost::shared_ptr<TVariantBase<TypeList> > mHolder;
   void apply(THandlerBase<TypeList>& v) { mHolder->apply(v); }
};
