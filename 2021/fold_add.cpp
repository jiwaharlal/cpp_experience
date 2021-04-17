#include <iostream>

//template <typename T>
//struct Val {
    //operator (const T& v) {
        //if constexpr (std::is_same<T, int>::value)
            //return v;
        //return 0;
    //}
//};

//template <typename T>
//int val(const T& v)
//{
    //if constexpr (std::is_same<T, int>::value)
        //return v;
    //return 0;
//}

template <typename ... Args>
int sum(Args ... args)
{
    auto val = []<typename T>(const T& v) {
        if constexpr (std::is_same<T, int>::value)
            return v;
        return 0;
    };
    return (val(args) + ... + 0);
}

int main()
{
    std::cout << sum(1, 2, "hello", 3) << std::endl;
    return 0;
}
