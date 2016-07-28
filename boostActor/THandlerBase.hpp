#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/int.hpp>
#include <boost/any.hpp>

template <typename T, typename = void>
class THandlerBase
{
public:
   void operator ()(T msg) { on(msg); }
   virtual void on(T) = 0;
   void post(T msg) { post(boost::any(msg)); }
private:
   virtual void post(const boost::any&) = 0;
};

template <typename T>
class THandlerBase
<
   T,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<T>,
         boost::mpl::greater_equal
         <
            boost::mpl::size<T>,
            boost::mpl::int_<2>
         >
      >
   >::type
>  : public THandlerBase<typename boost::mpl::front<T>::type>
   , public THandlerBase<typename boost::mpl::pop_front<T>::type>
{};

template <typename T>
class THandlerBase
<
   T,
   typename boost::enable_if
   <
      boost::mpl::and_
      <
         boost::mpl::is_sequence<T>,
         boost::mpl::equal_to
         <
            boost::mpl::size<T>,
            boost::mpl::int_<1>
         >
      >
   >::type
>  : public THandlerBase<typename boost::mpl::front<T>::type>
{};
