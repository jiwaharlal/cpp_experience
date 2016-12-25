/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     ForEach.hpp
 * @author   Maxim Bondarenko
 * @date     09.09.2016
 */

#pragma once

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>

namespace NActor
{

// declaration

template <typename TypeList, typename FunctorType>
void forEach(FunctorType functor);

// implementation

namespace NActorUtilsPrivate
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
         mFunctor.operator()<T>();
      }

      FunctorType mFunctor;
   };
} // NActorUtilsPrivate

template <typename TypeList, typename FunctorType>
void forEach(FunctorType functor)
{
   using namespace NActorUtilsPrivate;
   boost::mpl::for_each<TypeList, Wrap<boost::mpl::placeholders::_1> >(FunctorWrapper<FunctorType>(functor));
}

} // NActor
