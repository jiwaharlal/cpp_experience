#include <chrono>
#include <iostream>
#include <vector>

struct A
{
    explicit A() { std::cout << "A constructor" << std::endl; }
    A(const A&) { std::cout << "A copy constructor" << std::endl; }
    A(A&&) { std::cout << "A move constructor" << std::endl; }

    int value = 3;
};

A getA()
{
    A a;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    std::cout << timestamp << std::endl;

    if (timestamp % 2 == 0)
    {
        a.value = 2;
        return a;
    }

    a.value = 1;
    return a;
}

struct B
{
    std::vector<A> as;
};

B getB()
{
    B result;
    result.as.push_back(A());

    std::cout << "constructed" << std::endl;

    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    std::cout << timestamp << std::endl;

    if (timestamp % 2 == 0)
    {
        return result;
    }

    B other_result;
    other_result.as.push_back(A());

    return other_result;
};

int main()
{
    //auto a = getA();
    //std::cout << a.value << std::endl;
    //(void) a;

    auto b = getB();
    std::cout << b.as.front().value << std::endl;
    (void) b;

    return 0;
}
