#include <cstdlib>
#include <iostream>

struct B
{
    B() { std::cout << "B::B()" << std::endl; }
    B(const B&) { std::cout << "B::B(const B&)" << std::endl; }
    ~B() { std::cout << "B::~B()" << std::endl; }
};

class A
{
public:
    A()
    {
        std::cout << "construct" << std::endl;
    }

    A(const A& other)
    {
        std::cout << "copy construct" << std::endl;
    }

    ~A()
    {
        std::cout << "destruct" << std::endl;
    }

    const A& operator =(const A&)
    {
        std::cout << "operator =" << std::endl;
    }

    void print()
    {
        std::cout << "val: " << v << std::endl;
    }

private:
    B b;
    int v;
};

int main()
{
    A* a = new A;

    char buf[sizeof(A)];
    a->~A();

    //std::cout << "hopefully destructed" << std::endl;

    //{
        ////A a2;

        //*a = A();
    //}

    //a->print();

    //std::cout << "assigned again" << std::endl;

    std::free(a);

    return 0;
}
