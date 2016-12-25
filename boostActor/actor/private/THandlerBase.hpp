/**
 * Project        CVNAR
 * Copyright (C)  2010-2016
 * Company        Luxoft
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 *
 * @file     THandlerBase.hpp
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

#include "../THandler.hpp"

namespace NActor
{

namespace NActorPrivate
{

template <typename MsgType, typename = void>
class THandlerBase;

// case for typelist with size >= 2
template <typename TypeList>
class THandlerBase
<
   TypeList,
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
>  : public THandler<typename boost::mpl::front<TypeList>::type>
   , public THandlerBase<typename boost::mpl::pop_front<TypeList>::type>
{};

// case for typelist with size = 1
template <typename TypeList>
class THandlerBase
<
   TypeList,
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
>  : public THandler<typename boost::mpl::front<TypeList>::type>
{};

// case for empty typelist, just in case
template <typename TypeList>
class THandlerBase
<
   TypeList,
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

} // NActorPrivate

} // NActor
