#include <iostream>
#include <iterator>
#include <vector>

struct Data
{
   Data(int v = 0)
      : value(v)
   {
      std::cout << "ctor " << v << std::endl;
   }

   Data(const Data& other)
      : value(other.value)
   {
      std::cout << "Copy ctor " << value << std::endl;
   }

   Data(Data&& other)
      : value(other.value)
   {
      other.value = 0;
      std::cout << "Move ctor " << value << std::endl;
   }

   const Data& operator =(const Data& other)
   {
      value = other.value;
      std::cout << "operator = " << value << std::endl;
      return *this;
   }

   int value;
};

std::ostream& operator <<(std::ostream& out, const Data& d)
{
   out << d.value;
   return out;
}

Data makeData(int i)
{
   return Data(i);
}

std::vector<Data> createData()
{
   std::vector<Data> result;
   int values[] = {1, 2, 3, 4};
   int valLen = sizeof(values) / sizeof(values[0]);
   result.reserve(valLen);
   for (int i = 0; i < valLen; i++)
   {
      result.push_back(makeData(i));
      //result.emplace_back(i);
   }

   return result;
}

int main(int, char**)
{
   std::vector<Data> v = createData();

   std::copy(v.begin(), v.end(), std::ostream_iterator<Data>(std::cout, " "));
   std::cout << std::endl;

   Data d1(32);
   Data d2(std::move(d1));

   std::cout << d1 << " " << d2 << std::endl;

   return 0;
}
