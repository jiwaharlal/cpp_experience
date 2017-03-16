#include <iostream>
#include <vector>

template <typename T>
class has_size {
private:
  typedef char Yes;
  typedef Yes No[2];

  template <typename U, U> struct really_has;

  template <typename C> static Yes& Test(really_has <size_t (C::*)() const, &C::size>*);

  // EDIT: and you can detect one of several overloads... by overloading :)
  //template <typename C> static Yes& Test(really_has <size_t (C::*)(), &C::size>*);

  template <typename> static No& Test(...);

public:
    static bool const value = sizeof(Test<T>(0)) == sizeof(Yes);
};

class A
{
public:
   double size() const { return 0; }
};

int main()
{
   std::cout << std::boolalpha
            << has_size<std::vector<int> >::value << std::endl
            << has_size<std::pair<int, double> >::value << std::endl
            << has_size<A>::value << std::endl;

   return 0;
}
