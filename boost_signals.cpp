#include <iostream>

#include <boost/signals2.hpp>

void hello()
{
    std::cout << "Hello, ";
}

struct world
{
    world(std::string str) : m_str(str) { std::cout << "Constructing " << m_str << std::endl; }
    ~world() { std::cout << "Destroying " << m_str << std::endl; m_str.clear(); }

    void operator ()()
    {
        std::cout << m_str << std::endl;
    }

    void printSqr(int value)
    {
        std::cout << value * value << std::endl;
    }

    std::string m_str;
};

int main()
{
    boost::signals2::signal<void ()> sig;
    sig.connect(&hello);

    {
        world w(" World ");
        sig.connect(std::ref(w));

        sig();
    }

    world new_w(" new world ");

    sig();

    //boost::signals2::signal<void(int)> int_sig;
    //int_sig
    //int_sig(23);

    return 0;
}
