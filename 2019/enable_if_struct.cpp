#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

template <typename T, typename = void>
struct A
{
    T val;
};

template <typename T>
struct A<T, typename std::enable_if<std::is_same<T, int>::value>::type>
{
    std::vector<T> v;
};

int main()
{
    A<int> a_int;
    a_int.v.push_back(2);
    std::cout << a_int.v.front() << std::endl;

    A<std::string> a_str;
    a_str.val = "hello";
    std::cout << a_str.val << std::endl;

    return 0;
}
