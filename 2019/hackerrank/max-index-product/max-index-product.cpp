#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the solve function below.
std::int64_t solve(vector<int> arr) {
    std::int64_t max_product = 0;

    for (int i = 1; i < arr.size() - 1; ++i)
    {
        std::int64_t left = [&]()
        {
            for (int j = i - 1; j >= 0; --j)
                if (arr[j] > arr[i])
                    return j + 1;
            return 0;
        }();
        std::int64_t right = [&]()
        {
            for (int k = i + 1; k < arr.size(); ++k)
                if (arr[k] > arr[i])
                    return k + 1;
            return 0;
        }();
        max_product = std::max(max_product, left * right);
    }

    return max_product;
}

std::vector<int> findLastGreaterIndexes(const std::vector<int>& arr)
{
    std::set<std::pair<int, int>> left;
    left.emplace(arr[0], 0);
    std::vector<int> result(arr.size(), 0);

    for (int i = 1; i < arr.size(); ++i)
    {
        auto it = left.upper_bound(std::make_pair(arr[i], arr.size()));
        if (it != left.end())
        {
            result[i] = it->second + 1;
        }
        left.erase(left.begin(), it);
        left.emplace(arr[i], i);
    }

    return result;
}

std::int64_t solveFast(const std::vector<int>& arr)
{
    std::vector<int> arr_r;
    arr_r.reserve(arr.size());
    std::copy(arr.rbegin(), arr.rend(), std::back_inserter(arr_r));

    auto left_idxs = findLastGreaterIndexes(arr);
    auto right_idxs_r = findLastGreaterIndexes(arr_r);
    std::vector<int> right_idxs;
    right_idxs.reserve(arr.size());
    std::transform(right_idxs_r.rbegin(), right_idxs_r.rend(), std::back_inserter(right_idxs), [&](int v){ return v == 0 ? 0 : arr.size() - v + 1; });

    std::int64_t max_product = 0;
    for (int i = 0; i < arr.size(); ++i)
    {
        if (right_idxs[i] != 0 && left_idxs[i] != 0)
        {
            max_product = std::max(max_product, static_cast<std::int64_t>(left_idxs[i]) * static_cast<std::int64_t>(right_idxs[i]));
        }
    }

    return max_product;
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

    for (int arr_itr = 0; arr_itr < arr_count; arr_itr++) {
        int arr_item = stoi(arr_temp[arr_itr]);

        arr[arr_itr] = arr_item;
    }

    std::int64_t result = solveFast(arr);
    //std::int64_t result = solve(arr);

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
