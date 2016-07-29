#pragma once

#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/int.hpp>
#include <boost/any.hpp>

struct PostmanBase
{
};

template <typename MsgType, typename = void>
class THandlerBase
{
public:
   virtual void operator ()(MsgType msg) = 0;
   void post(MsgType msg) { post(boost::any(msg)); }
private:
   virtual void post(const boost::any&) = 0;
};

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
>  : public THandlerBase<typename boost::mpl::front<TypeList>::type>
   , public THandlerBase<typename boost::mpl::pop_front<TypeList>::type>
{};

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
>  : public THandlerBase<typename boost::mpl::front<TypeList>::type>
{};
