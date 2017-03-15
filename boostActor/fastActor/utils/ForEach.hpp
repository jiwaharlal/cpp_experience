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
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector.hpp>

namespace NFastActor
{

// declaration

template <typename TypeList, typename FunctorType>
void forEach(FunctorType functor);

template <typename TypeList1, typename TypeList2>
struct JoinLists
{
   typedef typename boost::mpl::copy
                     <
                        TypeList1,
                        boost::mpl::back_inserter
                        <
                           typename boost::mpl::copy
                           <
                              TypeList2,
                              boost::mpl::back_inserter<boost::mpl::vector<> >
                           >::type
                        >
                     >::type type;
};

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
         mFunctor.template operator()<T>();
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

} // NFastActor
