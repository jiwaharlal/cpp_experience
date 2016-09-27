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

#include "THandlerBase.hpp"

template <typename TypeList>
struct TVariantBase
{
   virtual ~TVariantBase() {}
   virtual void apply(THandlerBase<TypeList>&) = 0;
};

template <typename T, typename TypeList>
struct TVariantImpl: public TVariantBase<TypeList>
{
   TVariantImpl(const T& v)
      : mValue(v)
   {}

   T get()
   {
      return mValue;
   }

   void apply(THandlerBase<TypeList>& handler)
   {
      static_cast<THandler<T>& >(handler)(mValue);
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

   template <typename T>
   void put(const T& value)
   {
      mHolder.reset(new TVariantImpl<T, TypeList>(value));
   }

   void apply(THandlerBase<TypeList>& handler)
   {
      mHolder->apply(handler);
   }

public:
   boost::shared_ptr<TVariantBase<TypeList> > mHolder;
};
