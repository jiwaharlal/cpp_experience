#include <boost/make_shared.hpp>
#include <boost/move/make_unique.hpp>
#include <boost/move/unique_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/typeof/std/utility.hpp>

int main()
{
   std::string longName = "boost::shared_ptr";
   //longName = "shared_ptr";
   //boost::iterator_range<char*> result = boost::find_last(longName, ":");
   //BOOST_AUTO(result, boost::find_last(longName, ":"));
   //if (result.end() != longName.end())
   //{
      //std::string shortName(result.end(), longName.end());
      //std::cout << "Short: " << shortName << std::endl;
   //}

   size_t lastColumnPos = longName.find_last_of(":");
   std::string shortName(
         lastColumnPos == std::string::npos ?
         longName :
         longName.substr(lastColumnPos + 1));
   //std::string shortName(longName.substr(lastColumnPos + 1));
   std::cout << shortName << "\n";


   boost::movelib::unique_ptr<int> pi = boost::movelib::make_unique<int>(10);

   boost::movelib::unique_ptr<int> pi2 = boost::move(pi);
   //boost::shared_ptr<int> pi2 = boost::move(pi);

   std::cout << *pi2 << "\n";
   std::cout << *pi << "\n";

   return 0;
}
