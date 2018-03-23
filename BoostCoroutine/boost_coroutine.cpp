#include <functional>
#include <iostream>
#include <random>

#include <boost/bind.hpp>
#include <boost/coroutine2/all.hpp>
#include <boost/range/algorithm.hpp>

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

class LinestringMatcher
{
    LinestringMatcher(RoadGraph graph, Box map_box, Linestring line, PolylinePosition pos);

    MatchList getMatchFrom(PolylinePosition pos);

    void updateRoads(RoadGraph graph, Box map_box);
};

void cooperative(coroutine<int>::push_type &sink, int i)
{
  int j = i;
  sink(++j);
  sink(++j);
  std::cout << "end\n";
}

int main()
{
  //using std::placeholders::_1;
  //coroutine<int>::pull_type source(boost::bind(cooperative, _1, 0));
  //std::cout << source.get() << '\n';
  //source();
  //std::cout << source.get() << '\n';
  //source();

    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> int_dist(4, 15);

    std::vector<int> values;

    for (int i = 0; i < 100; i++)
    {
        values.push_back(int_dist(re));
    }

    boost::copy(values, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    coroutine<ResultType>::pull_type searchTens(boost::bind(findTens, _1, values));
    do {
        auto val_pair = searchTens.get();
        if (!val_pair.first)
        {
            std::cout << "Found at position: " << val_pair.second << std::endl;
            searchTens();
            continue;
        }
        break;
    } while (true);

    std::cout << "That's all" << std::endl;

    return 0;
}
