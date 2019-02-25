#include <thread>

class A
{
public:
    explicit A() = default;

    // Compiles:
    //A(const A &a) = default;

     //Doesn't compile:
     explicit A(const A &a) = default;
};

void f(A a)
{
}

int main()
{
    A a;
    std::thread t(f, a);

    A a2(a);

    return 0;
}
