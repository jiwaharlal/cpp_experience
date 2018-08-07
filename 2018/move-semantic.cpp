#include <iostream>
#include <string>
#include <vector>

struct P
{
    P() { std::cout << "P::P()" << std::endl; }
    P(const P&) { std::cout << "P::P(const P&)" << std::endl; }
    P(P&&) { std::cout << "P::P(P&&)" << std::endl; }

    P& operator =(const P&)
    {
        std::cout << "P::operator =(const P&)" << std::endl;
        return *this;
    }

    P& operator =(P&&)
    {
        std::cout << "P::operator =(P&&)" << std::endl;
        return *this;
    }

    ~P() { std::cout << "P::~P()" << std::endl; }
};

struct A
{
    A() = default;
    //A(A&&) = default;
    A(const A&) = default;
    A(std::string&& str) : value(str) {}

    //A& operator =(const A&) = default;
    //A& operator =(A&&) = default;

    bool operator <(const A& other)
    {
        return value < other.value;
    }

    std::string value = "default value";
    P p;
};

std::ostream& operator <<(std::ostream& out, const A& a)
{
    return out << a.value;
}

int main()
{
    A a;
    A a2("hi");
    a2 = std::move(a);

    std::vector<A> as;
    as.reserve(4);
    as.push_back(std::move(a));
    as.emplace_back(std::move(a2));

    std::cout << "a: " << a << std::endl;
    std::cout << "a2: " << a2 << std::endl;


    return 0;
}
