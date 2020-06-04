#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

std::int64_t permutations(std::int64_t collection_size, std::int64_t group_size)
{
    if (collection_size < group_size)
    {
        return 0;
    }

    std::int64_t result = 1;
    for (std::int64_t i = collection_size - group_size + 1; i <= collection_size; ++i)
    {
        result *= i;
    }

    return result;
}

// Complete the solve function below.
std::int64_t solve(vector<int> arr) {
    std::vector<std::pair<int, int>> prev;
    std::int64_t result = 0;

    for (int i = 0; i != arr.size(); )
    {
        int v = arr[i];

        if (prev.empty() || prev.back().first > v)
        {
            prev.emplace_back(v, 1);
            ++i;
            continue;
        }

        if (prev.back().first == v)
        {
            ++prev.back().second;
            ++i;
        }
        else if (prev.back().first < v)
        {
            result += permutations(prev.back().second, 2);
            prev.pop_back();
        }
    }

    while (!prev.empty())
    {
        result += permutations(prev.back().second, 2);
        prev.pop_back();
    }

    return result;
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

    std::int64_t result = solve(arr);

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
