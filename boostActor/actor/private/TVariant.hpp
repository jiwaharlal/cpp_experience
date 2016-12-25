/**
 *
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     TVariant.hpp
 * @author   Maxim Bondarenko
 * @date     29.07.2016
 */

#pragma once

#include <boost/mpl/list.hpp>
#include <boost/shared_ptr.hpp>

#include "THandlerBase.hpp"

namespace NActor
{

namespace NActorPrivate
{

template <typename TypeList>
struct TVariantBase
{
   virtual ~TVariantBase() {}
   virtual void apply(NActorPrivate::THandlerBase<TypeList>&) = 0;
};

template <typename T>
struct THanlderCaller
{
   static void call(THandler<T>& handler, const T& value)
   {
      handler(value);
   }
};

template <typename T, typename TypeList>
struct TVariantImpl: public TVariantBase<TypeList>
{
   TVariantImpl(const T& v)
      : mValue(v)
   {}

   void apply(NActorPrivate::THandlerBase<TypeList>& handler)
   {
      THanlderCaller<T>::call(static_cast<THandler<T>& >(handler), mValue);
   }

   T mValue;
};

template <class TypeList>
struct TVariant
{
public:
   template <typename T>
   TVariant(const T& v)
      : mHolder(new TVariantImpl<T, TypeList>(v))
   {}

   void apply(NActorPrivate::THandlerBase<TypeList>& handler)
   {
      mHolder->apply(handler);
   }

public:
   boost::shared_ptr<TVariantBase<TypeList> > mHolder;
};

} // NActorPrivate

} // NActor
