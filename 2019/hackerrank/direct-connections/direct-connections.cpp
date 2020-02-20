#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

static const std::int64_t divisor = 1000000007;

// Complete the solve function below.
int solve(vector<int> positions, vector<int> populations)
{
    std::vector<std::pair<int, int>> cities(positions.size());
    for (int i = 0; i < cities.size(); ++i)
    {
        cities[i] = std::make_pair(positions[i], populations[i]);
    }
    std::sort(cities.begin(), cities.end(), [](const auto& lhs, const auto& rhs){ return lhs.second < rhs.second; });

    std::int64_t sum = 0;

    for (auto it = std::prev(cities.end()); it != cities.begin(); --it)
    {
        std::int64_t sum_distance = std::accumulate(
                cities.begin(),
                it,
                std::int64_t{0},
                [&](auto sum, auto city){ return sum + std::abs(city.first - it->first); });

        sum_distance %= divisor;
        sum += sum_distance * it->second % divisor;
    }

    return sum % divisor;
}

std::vector<int> readArray(int arr_count)
{
    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split_string(arr_temp_temp);

    vector<int> arr(arr_count);

    for (int arr_itr = 0; arr_itr < arr_count; arr_itr++) {
        int arr_item = stoi(arr_temp[arr_itr]);

        arr[arr_itr] = arr_item;
    }

    return arr;
}

int main()
{
    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int arr_count;
        cin >> arr_count;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        auto positions = readArray(arr_count);
        auto populations = readArray(arr_count);

        int result = solve(positions, populations);

        std::cout << result << "\n";
    }

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
