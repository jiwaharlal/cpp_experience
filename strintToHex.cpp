#include <sstream>
#include <iostream>
#include <string.h>
#include <iterator>
#include <iomanip>

std::string bufToHexString(const unsigned char* buf, int dataSize)
{
   std::stringstream ss;
   for (const unsigned char* bufEnd = buf + dataSize; buf != bufEnd; buf++)
   {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned short>(*buf) << " ";
   }

   return ss.str();
}

int main(int argc, char** argv)
{
   if ( argc == 1 )
   {
      std::cout << "Usage: " << argv[0] << " <string to convert>" << std::endl;
      return 1;
   }

   std::cout << bufToHexString(reinterpret_cast<const unsigned char*>(argv[1]), strlen(argv[1])) << std::endl;

   return 0;
}
