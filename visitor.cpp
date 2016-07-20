#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/mpl/list.hpp>
#include <typeinfo>

typedef boost::mpl::list<int, double, std::string> tValueList;

template <typename T>
class VisitorBase
{
public:
   virtual void visit(T& value) { std::cout << typeid(value).name() << " " << value << " visited" << std::endl; }
};

template <typename FirstType, typename ... Types>
class VisitorBase<boost::mpl::list<FirstType, Types...> > : public VisitorBase<FirstType>
                                                          , public VisitorBase<boost::mpl::list<Types...> >
{
};

template <typename T>
class VisitorBase<boost::mpl::list<T> > : public VisitorBase<T>
{
};

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

class PoliVisitor: public VisitorBase<tTypeList>
{
   void visit(int& i) { std::cout << "int " << i << " visited" << std::endl; }
   void visit(double& d) { std::cout << "double " << d << " visted" << std::endl; }
   void visit(std::string& s) { std::cout << "string " << s << " visited" << std::endl; }
   void visit(bool& ) {}
};

int main(int, char**)
{
   PoliVisitor v;

   typedef ValueHolder<tTypeList> tVariant;
   std::vector<ValueHolder<tTypeList> > vs;
   //tVariant v1, v2;
   tVariant v1(1);
   tVariant v2(true);
   v1.put(10);
   v2.put(std::string("hello"));

   vs.push_back(v1);
   vs.push_back(v2);
   vs.emplace_back(42);
   vs.emplace_back(std::string("wow"));
   //vs.emplace_back('a');

   //vs.push_back(tVariant(10));
   //vs.push_back(tVariant(std::string("hello!"));
   std::for_each(vs.begin(), vs.end(), [&](ValueHolder<tTypeList>& vh) { vh.apply(v); });

   return 0;
}
