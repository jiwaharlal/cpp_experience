#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>

typedef boost::shared_ptr<std::string> tStrPtr;

void f1(const tStrPtr& str)
{
   str->assign("hello");

   tStrPtr str2 = str;

   std::cout << "Is unique: " << std::boolalpha << str.unique() << "\n";
}

int main()
{
   tStrPtr s = boost::make_shared<std::string>("hi");
   boost::shared_ptr<const std::string> s2 = s;
   f1(s);

   s2->clear();

   std::cout << *s2 << "\n";

   return 0;
}
