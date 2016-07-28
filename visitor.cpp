#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/mpl/list.hpp>
#include <typeinfo>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/int.hpp>
#include <boost/any.hpp>

typedef boost::mpl::list<int, double, std::string> tValueList;

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

template <typename T, typename = void>
class VisitorBase
{
public:
   virtual void operator ()(T& value) { visit(value); }
   virtual void visit(T& value) = 0; // { std::cout << typeid(value).name() << " " << value << " visited" << std::endl; }
};

template <typename TypeList>
class VisitorBase
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
>  : public VisitorBase<typename boost::mpl::front<TypeList>::type>
   , public VisitorBase<typename boost::mpl::pop_front<TypeList>::type>
{};

template <typename TypeList>
class VisitorBase
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
>  : public VisitorBase<typename boost::mpl::front<TypeList>::type>
{};

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
   void apply(VisitorBase<ListType>& v) { static_cast<VisitorBase<T>& >(v)(mValue); }

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

template <typename TypeList>
class PoliVisitor: public VisitorBase<TypeList>
{
   //void visit(int& i) { std::cout << "int " << i << " visited" << std::endl; }
   //void visit(double& d) { std::cout << "double " << d << " visted" << std::endl; }
   //void visit(std::string& s) { std::cout << "string " << s << " visited" << std::endl; }
   //void visit(bool& ) {}
};

class ConcreteVisitor: public PoliVisitor<tTypeList>
{
   void visit(int& i) { std::cout << "int " << i << " visited" << std::endl; }
   void visit(double& d) { std::cout << "double " << d << " visted" << std::endl; }
   void visit(std::string& s) { std::cout << "string " << s << " visited" << std::endl; }
   void visit(bool& ) {}
};

int main(int, char**)
{
   //PoliVisitor v;
   ConcreteVisitor v;
   PoliVisitor<tTypeList>* pv = &v;

   typedef ValueHolder<tTypeList> tVariant;
   typedef std::vector<ValueHolder<tTypeList> > tHoldersVector;
   tHoldersVector vs;
   //tVariant v1, v2;
   tVariant v1(1);
   tVariant v2(true);
   v1.put(10);
   v2.put(std::string("hello"));

   vs.push_back(v1);
   vs.push_back(v2);
   //vs.emplace_back(42);
   //vs.emplace_back(std::string("wow"));
   //vs.emplace_back('a');

   //vs.push_back(tVariant(10));
   //vs.push_back(tVariant(std::string("hello!"));
   //std::for_each(vs.begin(), vs.end(), [&](ValueHolder<tTypeList>& vh) { vh.apply(*pv); });

   BOOST_FOREACH(tHoldersVector::reference vh, vs)
   {
      vh.apply(*pv);
   }

   return 0;
}
