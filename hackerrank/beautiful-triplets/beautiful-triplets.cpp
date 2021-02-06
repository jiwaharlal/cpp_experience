#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the beautifulTriplets function below.
int beautifulTriplets(int d, vector<int> arr)
{
    std::map<int, int> values_count;
    for (const int val : arr)
    {
        const auto it = values_count.lower_bound(val);
        if (it->first != val)
        {
            values_count.emplace_hint(it, val, 1);
        }
        else
        {
            ++it->second;
        }
    }

    int triplets_count = 0;

    for (const auto value_count_pair : values_count)
    {
        const int value = value_count_pair.first;
        const auto second_it = values_count.find(value + d);
        if (second_it == values_count.end())
        {
            continue;
        }
        const auto third_it = values_count.find(value + d * 2);
        if (third_it == values_count.end())
        {
            continue;
        }
        triplets_count += value_count_pair.second * second_it->second * third_it->second;
    }

    return triplets_count;
}

int beautifulTripletsFast(int d, vector<int> arr)
{
    const auto max_el = *std::max_element(arr.begin(), arr.end());
    std::vector<int> values_count(max_el + 1, 0);
    for (const auto val : arr)
    {
        ++values_count[val];
    }
    int triplets_count = 0;

    for (int i = 0, limit = max_el - d * 2; i <= limit; ++i)
    {
        triplets_count += values_count[i] * values_count[i + d] * values_count[i + d * 2];
    }

    return triplets_count;
}

int main()
{
    string nd_temp;
    getline(cin, nd_temp);

    vector<string> nd = split_string(nd_temp);

    int n = stoi(nd[0]);

    int d = stoi(nd[1]);

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split_string(arr_temp_temp);

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    int result = beautifulTripletsFast(d, arr);

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
