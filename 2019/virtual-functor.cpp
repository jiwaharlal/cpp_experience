#include <vector>
#include <iostream>
#include <functional>
#include <memory>

class A
{
    virtual void foo() { std::cout << "A::foo()\n"; }
};

class B : public A
{
    void foo() override { std::cout << "B::foo()\n"; }
};

int main()
{
    std::vector<std::shared_ptr<A>> as;

    return 0;
}
