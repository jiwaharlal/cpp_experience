#include <bits/stdc++.h>

#include "../self-balancing-tree/TAvlSet.hpp"

using namespace std;

vector<string> split_string(string);

// Complete the solve function below.
long solveOld(vector<int> arr)
{
    long result = 0l;
    int max_elt = 1;
    TAvlSet<int> prev_values;

    for (int i : arr)
    {
        //int limit = max_elt / i + 1;
        max_elt = std::max(max_elt, i);
        const int limit = max_elt / i;
        const auto limit_it = prev_values.upper_bound(limit);

        // <debug>
        for (auto it = prev_values.begin(); it != limit_it; ++it)
        {
            if (i * *it > max_elt)
            {
                std::cout << "pair " << i << ", " << *it << " is used but should not" << std::endl;
            }
            //else
            //{
                //std::cout << "pair " << i << ", " << *it << " is ok" << std::endl;
            //}
        }
        // </debug>

        int acceptable = prev_values.count_before(limit_it);
        result += acceptable;
        prev_values.insert(i);
    }

    return result;
}

using ArrIter = std::vector<int>::iterator;

template<class T, class Compare>
std::pair<T&, T&> minmax_non_const( T& a, T& b, Compare comp )
{
    return comp(b, a) ? std::pair<T&, T&>(b, a)
                      : std::pair<T&, T&>(a, b);
}

std::pair<long, TAvlSet<int>> solveIJ(ArrIter begin, ArrIter end)
{
    std::pair<long, TAvlSet<int>> result{0, {}};
    const auto dist = std::distance(begin, end);
    if (dist == 0)
    {
    }
    else if (dist == 1)
    {
        result.second.insert(*begin);
    }
    else if (dist == 2)
    {
        if (*begin == 1 || *std::next(begin) == 1)
        {
            result.first = 1;
        }
        result.second.insert(begin, end);
    }
    else
    {
        auto pivot_it = std::max_element(begin, end);
        // count pairs for pivot
        result.first += std::count(begin, pivot_it, 1) + std::count(std::next(pivot_it), end, 1);

        auto left_result = solveIJ(begin, pivot_it);
        auto right_result = solveIJ(std::next(pivot_it), end);
        result.first += left_result.first + right_result.first;

        auto short_long_set = minmax_non_const(
                left_result.second,
                right_result.second,
                [](const auto& lhs, const auto& rhs){ return lhs.size() < rhs.size(); });
        auto& short_set = short_long_set.first;
        auto& long_set = short_long_set.second;

        if (short_set.size() * static_cast<size_t>(std::log2(long_set.size())) > long_set.size())
        {
            // apply linear pass for larger set
            int long_acceptable = long_set.size();
            auto rit = long_set.rbegin();
            for (int val1 : short_set)
            {
                int inclusive_limit = *pivot_it / val1;
                for (; rit != long_set.rend() && inclusive_limit < *rit; ++rit, --long_acceptable);
                if (rit == long_set.rend())
                {
                    break;
                }
                result.first += long_acceptable;
            }
        }
        else
        {
            // apply upper_bound search for larger set
            for (int val1 : short_set)
            {
                int inclusive_limit = *pivot_it / val1;
                auto it = long_set.upper_bound(inclusive_limit);
                result.first += long_set.count_before(it);
            }
        }

        result.second.swap(long_set);
        result.second.insert(short_set.begin(), short_set.end());
        result.second.insert(*pivot_it);
    }

    return result;
}

long solve(vector<int> arr)
{
    return solveIJ(arr.begin(), arr.end()).first;
}

int main()
{
    int arr_count;
    cin >> arr_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split_string(arr_temp_temp);

    vector<int> arr(arr_count);

    for (int i = 0; i < arr_count; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    long result = solve(arr);

    std::cout << result << "\n";

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
