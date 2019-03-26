#include <iostream>
#include <limits>

template<typename T> constexpr
T const& max(T const& a, T const& b) {
  return a > b ? a : b;
}

template <long I>
struct bits_in_val
{
    enum { value = 1 + bits_in_val<I / 2>::value };
};

template <>
struct bits_in_val<0>
{
    enum { value = 0 };
};

template <typename T>
struct bits_in
{
    enum { value = max(
            static_cast<T>(bits_in_val<std::numeric_limits<T>::max()>::value),
            static_cast<T>(bits_in_val<std::numeric_limits<T>::min()>::value)) };
};

int main()
{
    //std::cout << std::numeric_limits<std::intmax_t>::max() << std::endl;
    //std::cout << sizeof(std::intmax_t) << std::endl;

    std::cout << (-4 / 8) << std::endl;

    std::cout << bits_in<char>::value << std::endl;

    return 0;
}
