#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

int main(int argc, char** argv)
{
   if (argc != 2)
   {
      std::cout << "Usage: boost_tokenizer <string to tokenize>" << std::endl;
      return 0;
   }

   std::string line(argv[1]);
   boost::char_separator<char> sep(",");
   typedef boost::tokenizer<boost::char_separator<char> > tTokenizer;
   tTokenizer tok(line, sep);

   std::copy(tok.begin(), tok.end(), std::ostream_iterator<tTokenizer::value_type>(std::cout, "\n"));

   return 0;
}
