#include <tuple>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/tuple.hpp>
#include <iostream>

struct ResultPrinter
{
    template <class T>
    void operator ()(const T& functor) const
    {
        std::cout << functor() << std::endl;
    }
};

int main()
{
    auto t = boost::fusion::make_tuple(
            [](){ return 4; },
            [](){ return "Hello"; });

    boost::fusion::for_each(t, ResultPrinter());

    return 0;
}
