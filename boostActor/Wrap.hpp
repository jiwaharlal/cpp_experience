#pragma once

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>

// declaration

template <typename TypeList, typename FunctorType>
void forEach(FunctorType functor);

// implementation

namespace NNaviUtilsPrivate
{
   template <class T>
   struct Wrap{};

   template <typename FunctorType>
   struct FunctorWrapper
   {
      FunctorWrapper(const FunctorType& functor)
         : mFunctor(functor)
      {}

      template <typename T>
      void operator ()(Wrap<T>&)
      {
         mFunctor.template operator()<T>();
      }

      FunctorType mFunctor;
   };
} // NNaviUtilsPrivate

template <typename TypeList, typename FunctorType>
void forEach(FunctorType functor)
{
   using namespace NNaviUtilsPrivate;
   boost::mpl::for_each<TypeList, Wrap<boost::mpl::placeholders::_1> >(FunctorWrapper<FunctorType>(functor));
}
