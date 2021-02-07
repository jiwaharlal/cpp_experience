#include <bits/stdc++.h>

#include "../self-balancing-tree/TAvlSet.hpp"

using namespace std;

vector<string> split_string(string);

// Complete the solve function below.
long solve(vector<int> arr)
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

std::pair<long, std::multiset<int>> solveIJ(ArrIter begin, ArrIter end)
{
    std::pair<long, std::multiset<int>> result{0, {}};
    auto pivot_it = std::max_element(begin, end);
    // count pairs for pivot
    result.first += std::count(begin, pivot_it, 1);
    result.first += std::count(std::next(pivot_it), end, 1);

    std::set<int> left_set(begin, pivot_it);
    std::set<int> right_set(std::next(pivot_it), end);

    auto& smaller_set = left_set.size() < right_set.size() ? left_set : right_set;
    auto& larger_set = left_set.size() < right_set.size() ? right_set : left_set;

    if (smaller_set.empty())
    {
    }
    else if (smaller_set.size() * static_cast<size_t>(std::log2(larger_set.size())) > larger_set.size())
    {
        // apply linear pass for larger set
    }
    else
    {
        // apply upper_bound search for larger set
    }
}

long solveIJ(vector<int> arr)
{

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
