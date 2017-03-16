#include <boost/container/flat_set.hpp>
#include <boost/container/set.hpp>
#include <boost/container/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/move/move.hpp>
#include <boost/move/core.hpp>
#include <boost/range/algorithm.hpp>
#include <string>
#include <iostream>
#include <iterator>
#include <set>

class A
{
public:
   A(const std::string& name)
      : mName(name)
   {
      std::cout << "A::A " << name << "\n";
   }

   A(const A& other)
      : mName(other.mName)
   {
      std::cout << "A::A(copy) " << mName << "\n";
   }

   ~A()
   {
      std::cout << "A::~A name: " << mName << "\n";
   }

   bool operator <(const A& other) const
   {
      return mName < other.mName;
   }

   std::string mName;
};

//typedef boost::container::flat_set<A> tSetA;
typedef boost::container::set<A> tSetA;
//typedef std::set<A> tSetA;

//BOOST_RV_REF(tSetA) getNames()
tSetA getNames()
{
   tSetA result;
   //result.reserve(5);

   for (int i = 0; i < 5; ++i)
   {
      result.emplace(boost::lexical_cast<std::string>(i));
      //result.insert(A(boost::lexical_cast<std::string>(i).c_str()));
   }

   std::cout << "getNames returns value\n";

   return result;
}

class SetPrinter
{
public:
   explicit SetPrinter(BOOST_RV_REF(tSetA) aRange)
      : mRange(boost::move(aRange))
   {
      std::cout << "Construct SetPrinter with rvalue" << std::endl;
   }

   //explicit SetPrinter(tSetA&& aRange)
   //{
      //std::cout << "Construct SetPrinter with rvalue" << std::endl;
   //}

   explicit SetPrinter(const tSetA& aRange)
      : mRange(aRange)
   {
      std::cout << "Construct with const ref" << std::endl;
   }

   void print()
   {
      boost::transform(
            mRange,
            std::ostream_iterator<const std::string&>(std::cout, "\n"),
            boost::bind(&A::mName, _1));
   }

   tSetA mRange;
};

int main()
{
   tSetA names = getNames();
   SetPrinter printer(boost::move(names));
   //SetPrinter printer(names);

   //std::cout << "Size of names: " << names.size() << std::endl;

   //boost::container::vector<A> va;
   //va.emplace_back("hi");

   //SetPrinter printer(getNames());

   printer.print();

   return 0;
}
