#include <chrono>
#include <functional>
#include <iostream>
#include <random>

#include <boost/bind.hpp>
#include <boost/coroutine2/all.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/tuple/tuple_io.hpp>

using boost::coroutines2::coroutine;

using ResultType = std::pair<bool, std::size_t>;

void findTens(coroutine<ResultType>::push_type& sink, const std::vector<int>& ints)
{
    for (size_t i = 0; i != ints.size(); i++)
    {
        if (ints[i] == 10)
        {
            sink(std::make_pair(false, i));
        }
    }

    sink(std::make_pair(true, ints.size()));
}

int main()
{
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> int_dist(4, 15);

    std::vector<int> values;

    for (int i = 0; i < 100; i++)
    {
        values.push_back(int_dist(re));
    }

    boost::copy(values, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<ResultType> results;
    coroutine<ResultType>::pull_type searchTens(boost::bind(findTens, _1, values));
    do {
        auto val_pair = searchTens.get();
        if (!val_pair.first)
        {
            results.push_back(val_pair);
            searchTens();
            continue;
        }
        break;
    } while (true);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time spent: " << (end - start).count() << std::endl;

    for (const auto& r : results)
    {
        std::cout << boost::tie(r.first, r.second) << "\n";
    }
    std::cout << "That's all" << std::endl;

    return 0;
}
