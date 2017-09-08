#include <memory>
#include <iostream>
#include <vector>

struct A
{
    A() { std::cout << "A()" << std::endl; }
    A(const A&) { std::cout << "A(copy)" << std::endl; }
    A(A&&) { std::cout << "A(move)" << std::endl; }
    ~A() { std::cout << "~A()" << std::endl; }
};

A createA()
{
    return A();
}

std::unique_ptr<A> createAPtr()
{
    std::unique_ptr<A> result(new A);

    return result;
}

std::vector<A> createList(bool is_first)
{
    if (is_first)
    {
        std::vector<A> result;
        result.emplace_back();

        return result;
    }
    else
    {
        std::vector<A> other_result;
        for (int32_t i = 0; i < 5; ++i)
        {
            other_result.emplace_back();
        }

        return other_result;
    }
}

int main()
{
    //auto a = createA();
    auto as = createList(true);

    std::cout << "A created" << std::endl;

    //auto a2 = std::move(a);
    auto as2 = std::move(as);

    std::cout << as.size() << std::endl;

    std::cout << "A moved" << std::endl;

    return 0;
}
