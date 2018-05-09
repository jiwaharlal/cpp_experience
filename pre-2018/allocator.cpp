#include <vector>
#include <iostream>

template <int Value>
struct ZeroOrOne
{
   enum { value = 1 };
};

template <>
struct ZeroOrOne<0>
{
   enum { value = 0 };
};

template <int Value, int Base>
struct RoundUpTo
{
   enum { value = (Value / Base + ZeroOrOne<Value % Base>::value) * Base };
};

template <int Value>
struct RoundUpToLog2
{
   enum { value = RoundUpToLog2<Value / 2>::value * 2 };
};

template <>
struct RoundUpToLog2<1>
{
   enum { value = 1 };
};

template <int Size>
struct SizedAllocator
{
   enum { cellSize = Size };
};

template <typename T>
//struct SimpleAllocator: public SizedAllocator<RoundUpTo<sizeof(T), 16>::value>
struct SimpleAllocator: public SizedAllocator<RoundUpToLog2<sizeof(T)>::value>
{};

int main()
{
   std::cout << "Size of vector: " << sizeof(std::vector<int>) << std::endl;
   std::cout << "Cell size for vector: " << SimpleAllocator<std::vector<int> >::cellSize << std::endl;

   return 0;
}
