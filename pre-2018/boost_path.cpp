#include <boost/filesystem.hpp>
#include <boost/assign.hpp>
#include <iostream>
#include <vector>

int main()
{
   //boost::filesystem::path p("/home/mgbondarenko/Documents/puppet-theater.pdf");

   std::cout << std::boolalpha;
   boost::filesystem::path p;
   std::vector<std::string> paths = boost::assign::list_of
      ("/home/mgbondarenko/Documents/puppet-theater.pdf")
      ("some-filename.txt");

   for (auto& pathStr : paths)
   {
      p = pathStr;
      std::cout << "Path : " << pathStr << "\n";
      std::cout << "Filename : " << p.filename() << "\n";
      std::cout << "Stem : " << p.stem() << "\n";
      std::cout << "Extension : " << p.extension() << "\n";
      std::cout << "Has parent path: " << p.parent_path() << "\n";
   }
   std::cout.flush();

   std::cout << boost::filesystem::path("~/Documents/gps_configs").replace_extension(".txt") << std::endl;

   return 0;
}
