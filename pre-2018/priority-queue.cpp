#include <boost/bind.hpp>
#include <functional>
#include <iostream>
#include <boost/range/algorithm.hpp>
#include <iterator>
#include <queue>
#include <cstdlib>

int main()
{
    std::priority_queue<
        int32_t,
        std::vector<int32_t>,
        std::function<bool(int32_t, int32_t)>
        > int_queue(boost::bind(std::less<int32_t>(), _2, _1));

    for (int i = 0; i < 20; i++)
    {
        int_queue.push(rand() % 100);
    }

    std::cout << "numbers: ";
    while (!int_queue.empty())
    {
        std::cout << int_queue.top() << ", ";
        int_queue.pop();
    }
    std::cout << std::endl;

    return 0;
}
