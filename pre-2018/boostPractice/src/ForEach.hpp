#include <boost/core/enable_if.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/size.hpp>

template <typename TargetType, typename FunctorType, typename = void>
struct ForEachStruct
{
   ForEachStruct(FunctorType functor)
   {
      TargetType* pTarget;
      functor(*pTarget);
   }
};

template <typename TypeList, typename FunctorType>
struct ForEachStruct
<
   TypeList,
   FunctorType,
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
>  : public ForEachStruct<typename boost::mpl::front<TypeList>::type, FunctorType>
   , public ForEachStruct<typename boost::mpl::pop_front<TypeList>::type, FunctorType>
{};

template <typename TypeList, typename FunctorType>
struct ForEachStruct
<
   TypeList,
   FunctorType,
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
>  : public ForEachStruct<typename boost::mpl::front<TypeList>::type, FunctorType>
{};

template <typename TypeList, typename FunctorType>
void forEach(FunctorType functor)
{
   ForEachStruct<TypeList, FunctorType>()(functor);
}
