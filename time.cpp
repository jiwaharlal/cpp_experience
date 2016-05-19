#include <time.h>
#include <errno.h>
#include <string.h>
#include <iostream>

int main(int, char**)
{
   timespec time;
   if (clock_gettime(CLOCK_REALTIME, &time) != 0)
   {
      std::cout << "Error getting time: " << strerror(errno) << std::endl;
   }
   else
   {
      std::cout << "Time: " << time.tv_sec << " sec " << time.tv_nsec << " nsec" << std::endl;
   }

   return 0;
}
