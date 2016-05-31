#include <iostream>
#include <stdint.h>
#include <typeinfo>

// c++11
//#include <chrono>
//#include <thread>

int main(int argc, char** argv)
{
   std::cout << typeid(34).name() << std::endl;
   std::cout << "Sizeof(34): " << sizeof(34) << " name: " << typeid(34).name() << std::endl;
   std::cout << "Sizeof(34L): " << sizeof(34L) << " name: " << typeid(34L).name() << std::endl;
   std::cout << "Sizeof(34LL): " << sizeof(34LL) << " name: " << typeid(34LL).name() << std::endl;
   std::cout << "Sizeof uintmax_t: " << sizeof(uintmax_t) << std::endl;

   int32_t timestamp = 0x3fffffff;
   std::cout << timestamp << std::endl;

   int64_t longTimestamp = timestamp * 0x1000L;
   std::cout << longTimestamp << std::endl;

   longTimestamp = static_cast<int64_t>(timestamp) * 0x1000;
   std::cout << longTimestamp << std::endl;

   // c++11
   //std::thread dummyThread([]
   //{
      //std::this_thread::sleep_for(std::chrono::milliseconds(200));
      //std::cout << "Hello from thread" << std::endl;
   //});

   //std::cout << "Waiting for thread to join..." << std::endl;
   //dummyThread.join();
   //std::cout << "Exit program." << std::endl;

   return 0;
}
