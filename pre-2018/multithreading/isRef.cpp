#include <iostream>
#include <iterator>
#include <thread>
#include <vector>

struct Data
{
   Data(int i)
      : value(i)
   {
      std::cout << "ctor " << i << std::endl;
   }

   Data(const Data& other)
      : value(other.value)
   {
      std::cout << "copy constructor" << std::endl;
   }

   Data(Data&& other)
      : value(other.value)
   {
      other.value = 0;
      std::cout << "move constructor" << std::endl;
   }

   int value;
};

void updateData(Data& data)
{
   data.value += 10;
};

Data createData()
{
   Data d{3};
   return d;
}

std::vector<Data> createDataVector()
{
   std::vector<Data> result;
   result.reserve(3);
   int values[] = {1, 2, 3};
   for (auto& i: values)
   {
      result.emplace_back(i);
   }
   return result;
   //return std::vector<Data>{};
}

template <class T>
void printVector(std::vector<T>& v)
{
   std::vector<T> v2{std::move(v)};
   std::copy(v2.begin(), v2.end(), std::ostream_iterator<T>(std::cout, ", "));
}

std::ostream& operator <<(std::ostream& out, const Data& data)
{
   out << data.value;
   return out;
}

int main(int, char**)
{
   std::cout << "hc " << std::thread::hardware_concurrency() << std::endl;
   //Data d1 = {1};
   //Data d2 = {d1};
   //Data d3 = createData();
   //Data d3 = std::move(d1);

   //std::thread t(&updateData, std::ref(d3));
   //t.join();

   //std::cout << "Data " << d3.value << std::endl;

   auto v = createDataVector();
   printVector(v);
   //std::vector<Data> vd = std::move(createDataVector());
   //
   std::cout << "Size of v " << v.size() << std::endl;

   //std::copy(vd.begin(), vd.end(), std::ostream_iterator<Data>(std::cout, " "));
   //std::cout << std::endl;

   return 0;
}
