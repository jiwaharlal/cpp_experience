#include <iterator>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

struct Query
{
    int direction;
    int first;
    int last;
};

struct RangeInfo
{
    int begin;
    int map_begin;
    int size;
};

using Mapping = std::list<RangeInfo>;

void breakRangeInfo(Mapping& mapping, Mapping::iterator break_it, int position)
{
    int prev_size = position - break_it->begin;
    RangeInfo prev{break_it->begin, break_it->map_begin, prev_size};

    mapping.insert(break_it, prev);

    break_it->begin += prev_size;
    break_it->map_begin += prev_size;
    break_it->size -= prev_size;
}

void remap(std::list<RangeInfo>& mapping, const Query& query)
{
    int array_size = mapping.back().begin + mapping.back().size;
    if ((query.direction == 1 && query.first == 0)
            || (query.direction == 2 && query.last == array_size))
    {
        return;
    }

    auto end_it = mapping.end();
    if (query.last != array_size)
    {
        end_it = std::prev(std::find_if(
            mapping.begin(),
            mapping.end(),
            [&](const RangeInfo& r){ return r.begin > query.last; }));

        if (end_it->begin != query.last)
        {
            breakRangeInfo(mapping, end_it, query.last);
        }
    }

    auto begin_it = std::prev(std::find_if(
            mapping.begin(),
            mapping.end(),
            [&](const RangeInfo& r){ return r.begin > query.first; }));

    if (begin_it->begin != query.first)
    {
        breakRangeInfo(mapping, begin_it, query.first);
    }

    if (query.direction == 1)
    {
        // move to begin
        for (auto it = mapping.begin(); it != begin_it; ++it)
        {
            it->begin += query.last - query.first;
        }

        for (auto it = begin_it; it != end_it; ++it)
        {
            it->begin -= query.first;
        }

        mapping.splice(mapping.begin(), mapping, begin_it, end_it);
    }
    else
    {
        // move to end
        for (auto it = begin_it; it != end_it; ++it)
        {
            it->begin += array_size - query.last;
        }

        for (auto it = end_it; it != mapping.end(); ++it)
        {
            it->begin -= query.last - query.first;
        }

        mapping.splice(mapping.end(), mapping, begin_it, end_it);
    }
}

std::vector<int> reconstruct(const std::vector<int>& arr, const Mapping& mapping)
{
    std::vector<int> result(arr.size());
    for (const auto& r : mapping)
    {
        std::copy_n(arr.begin() + r.map_begin, r.size, result.begin() + r.begin);
    }
    return result;
}

std::vector<int> arrayQueriesRemap(std::vector<int> arr, std::vector<Query> queries)
{
    Mapping mapping = {{0, 0, static_cast<int>(arr.size())}};
    int map_size_limit = std::sqrt(arr.size());

    for (const auto& q : queries)
    {
        remap(mapping, q);

        if (mapping.size() >= map_size_limit)
        {
            arr = reconstruct(arr, mapping);
            mapping = {{0, 0, static_cast<int>(arr.size())}};
        }
    }

    return reconstruct(arr, mapping);
}

std::vector<int> arrayQueries(std::vector<int> v, std::vector<Query> queries)
{
    std::list<int> l{v.begin(), v.end()};
    for (const auto& query : queries)
    {
        int q = query.direction;
        int first = query.first;
        int last = query.last;

        if ((q == 1 && first == 0) || (q == 2 && last == l.size()))
        {
            continue;
        }

        int dist_end = l.size() - last;
        std::list<int>::iterator first_it, last_it;
        if (first < dist_end)
        {
            first_it = std::next(l.begin(), first);
            last_it = std::next(first_it, last - first);
        }
        else
        {
            last_it = std::prev(l.end(), dist_end);
            first_it = std::prev(last_it, last - first);
        }

        if (q == 1)
        {
            l.splice(l.begin(), l, first_it, last_it);
        }
        else
        {
            l.splice(l.end(), l, first_it, last_it);
        }
    }

    return {l.begin(), l.end()};
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int n, m;
    std::cin >> n >> m;

    std::vector<int> arr;
    for (int i = 0; i < n; ++i)
    {
        int v;
        std::cin >> v;
        arr.push_back(v);
    }

    std::vector<Query> queries;
    for (int i = 0; i < m; ++i)
    {
        Query q;
        std::cin >> q.direction >> q.first >> q.last;
        --q.first;
        queries.push_back(q);
    }

    arr = arrayQueries(arr, queries);

    std::cout << std::abs(arr.front() - arr.back()) << std::endl;
    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
