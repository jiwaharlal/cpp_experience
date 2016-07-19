#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>
#include <iostream>

struct Type1{};
struct Type2{};
struct Type3{};
struct Type4{};

void convert(const Type1&, Type2& dst)
{
   dst = Type2();
}

void convert(const Type3&, Type4& dst)
{
   dst = Type4();
}

//template <typename SrcType>
//struct Converter
//{
   //template <typename DstType>
   //DstType convert(const SrcType& src)
   //{
      //DstType result;
      //convert(src, result);
      //return result;
   //}
//};

template <typename DstType>
struct Converter
{
   template <typename SrcType>
   DstType operator ()(const SrcType& src)
   {
      DstType dst;
      convert(src, dst);
      return dst;
   }
};


int main(int, char**)
{
   boost::copy(boost::irange(0, 10), std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
