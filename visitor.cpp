#include <boost/foreach.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
//#include <boost/type_index.hpp>
#include <boost/variant.hpp>
#include <boost/variant/detail/over_sequence.hpp>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <typeinfo>
#include <vector>

//typedef boost::mpl::list<int, double, std::string> tValueList;
typedef boost::mpl::list<int> tValueList;

template <typename T, typename = void>
class VisitorBase
{
public:
   virtual void visit(T&) = 0;
};

template <typename T>
class VisitorBase
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
>  : public VisitorBase<typename boost::mpl::front<T>::type>
   , public VisitorBase<typename boost::mpl::pop_front<T>::type>
{};

template <typename T>
class VisitorBase
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
>  : public VisitorBase<typename boost::mpl::front<T>::type>
{};

//template <typename T>
//class VisitorBase
//{
//public:
   //virtual void visit(T& value) { std::cout << typeid(value).name() << " " << value << " visited" << std::endl; }
//};

//template <typename FirstType, typename ... Types>
//class VisitorBase<boost::mpl::list<FirstType, Types...> > : public VisitorBase<FirstType>
                                                          //, public VisitorBase<boost::mpl::list<Types...> >
//{
//};

//template <typename T>
//class VisitorBase<boost::mpl::list<T> > : public VisitorBase<T>
//{
//};




template <typename ListType>
struct ValueHolderBase
{
   virtual void apply(VisitorBase<ListType>&) = 0;
   virtual ~ValueHolderBase() {}
};

template <typename T, typename ListType>
struct ValueHolderImpl: public ValueHolderBase<ListType>
{
   ValueHolderImpl(const T& v) : mValue(v) {}
   T get() { return mValue; }
   void apply(VisitorBase<ListType>& v) { static_cast<VisitorBase<T>& >(v).visit(mValue); }

   T mValue;
};

template <class ListType>
struct ValueHolder
{
   template <typename T>
   ValueHolder(const T& v)
      : mHolder(new ValueHolderImpl<T, ListType>(v))
   {}

   template <typename T>
   void put(const T& value)
   {
      mHolder.reset(new ValueHolderImpl<T, ListType>(value));
   }

   boost::shared_ptr<ValueHolderBase<ListType> > mHolder;
   void apply(VisitorBase<ListType>& v) { mHolder->apply(v); }
};

typedef boost::mpl::list<int, double, std::string, bool> tTypeList;
//typedef boost::mpl::list<int> tTypeList;

class PoliVisitor: public VisitorBase<tTypeList>
{
   void visit(int& i) { std::cout << "int " << i << " visited" << std::endl; }
   void visit(double& d) { std::cout << "double " << d << " visted" << std::endl; }
   void visit(std::string& s) { std::cout << "string " << s << " visited" << std::endl; }
   void visit(bool& ) {}
};

template <typename T>
struct add_shared_ptr
{
   typedef boost::shared_ptr<T> type;
};

template<class Seq>
struct shared_ptr_list
{
   typedef typename boost::mpl::transform< Seq, add_shared_ptr<boost::mpl::_1> >::type type;
};

typedef shared_ptr_list<tTypeList>::type tPtrTypeList;

struct TypenamePrinter
{
   template <class T>
   void operator ()(T&)
   {
      std::cout << boost::typeindex::type_id<T>().pretty_name() << std::endl;
   }
};

typedef ValueHolder<tTypeList> tVariant;

struct Applicator
{
   Applicator(PoliVisitor& visitor) : mVisitor(visitor) {}
   void operator ()(tVariant& v) { v.apply(mVisitor); }
   PoliVisitor& mVisitor;
};

int main(int, char**)
{
   PoliVisitor v;

   boost::mpl::for_each<tPtrTypeList>(TypenamePrinter());

   std::vector<tVariant> vs;

   tVariant v1(1);
   tVariant v2(true);
   v1.put(10);
   v2.put(std::string("hello"));

   vs.push_back(v1);
   vs.push_back(v2);
   //vs.emplace_back(42);
   //vs.emplace_back(std::string("wow"));
   vs.push_back(tVariant(42));
   vs.push_back(tVariant(std::string("wow")));


   //std::for_each(vs.begin(), vs.end(), [&](ValueHolder<tTypeList>& vh) { vh.apply(v); });
   std::for_each(vs.begin(), vs.end(), Applicator(v));

   return 0;
}
