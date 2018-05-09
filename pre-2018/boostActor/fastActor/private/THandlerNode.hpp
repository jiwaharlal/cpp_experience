/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     THandlerNode.hpp
 * @author   Maxim Bondarenko
 * @date     29.07.2016
 */

#pragma once

#include <boost/any.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/variant/variant.hpp>

#include "../THandler.hpp"

namespace NFastActor
{

namespace NActorPrivate
{

template <typename MsgType, typename VariantType>
class THandlerVariantAdapter: public THandler<MsgType>
{
   virtual void postVariant(const VariantType& msgVariant) = 0;

   virtual void post(const MsgType& msg)
   {
      postVariant(VariantType(msg));
   }
};

template <typename MsgType, typename VariantType, typename = void>
class THandlerNode;

// case for typelist with size >= 2
template <typename TypeList, typename VariantType>
class THandlerNode
<
   TypeList,
   VariantType,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<TypeList>,
         boost::mpl::greater_equal
         <
            boost::mpl::size<TypeList>,
            boost::mpl::int_<2>
         >
      >
   >::type
>  : public THandlerVariantAdapter<typename boost::mpl::front<TypeList>::type, VariantType>
   , public THandlerNode<typename boost::mpl::pop_front<TypeList>::type, VariantType>
{};

// case for typelist with size = 1
template <typename TypeList, typename VariantType>
class THandlerNode
<
   TypeList,
   VariantType,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<TypeList>,
         boost::mpl::equal_to
         <
            boost::mpl::size<TypeList>,
            boost::mpl::int_<1>
         >
      >
   >::type
>  : public THandlerVariantAdapter<typename boost::mpl::front<TypeList>::type, VariantType>
{};

// case for empty typelist, just in case
template <typename TypeList, typename VariantType>
class THandlerNode
<
   TypeList,
   VariantType,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<TypeList>,
         boost::mpl::empty<TypeList>
      >
   >::type
>
{};

template <typename TypeList>
class THandlerTree: public THandlerNode<TypeList, typename boost::make_variant_over<TypeList>::type>
{};

} // NActorPrivate

} // NFastActor
