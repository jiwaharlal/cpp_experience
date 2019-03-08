#include <iostream>

class A
{
public:
    A() : m_i(0) {}
    A(int i) : m_i(i) {}
    A(A& a) : m_i(a.m_i) {}

    void p() { std::cout << m_i << '\n'; }
private:
    int m_i;
};

int main()
{
    A a1(10);
    A a2 = a1;
    a2.p();

    return 0;
}
