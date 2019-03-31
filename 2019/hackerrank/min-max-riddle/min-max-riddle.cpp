#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

struct SubSum
{
    int start;
    int len;
    long min;
};

bool operator <(const SubSum& lhs, const SubSum& rhs)
{
    return lhs.min < rhs.min;
}

// Complete the riddle function below.
vector<long> riddle(vector<long> arr) {
    // complete this function
    std::vector<SubSum> q;
    std::vector<long> result;
    result.reserve(arr.size());
    result.push_back(*std::max_element(arr.begin(), arr.end()));

    for (int i = 0, limit = arr.size() - 1; i < limit; ++i)
    {
        q.push_back(SubSum{i, 1, arr[i]});
    }
    std::make_heap(q.begin(), q.end());

    for (int s = 2; s <= arr.size(); ++s)
    {
        while (true)
        {
            std::pop_heap(q.begin(), q.end());
            auto& candidate = q.back();

            int limit = candidate.start + s;
            if (limit > arr.size())
            {
                q.erase(std::prev(q.end()));
                continue;
            }

            for (int i = candidate.start + candidate.len; i < limit; ++i)
            {
                candidate.min = std::min(candidate.min, arr[i]);
            }

            if (candidate.min >= q.front().min)
            {
                result.push_back(candidate.min);
                std::push_heap(q.begin(), q.end());
                break;
            }

            std::push_heap(q.begin(), q.end());
        }
    }

    return result;
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split_string(arr_temp_temp);

    vector<long> arr(n);

    for (int i = 0; i < n; i++) {
        long arr_item = stol(arr_temp[i]);

        arr[i] = arr_item;
    }

    vector<long> res = riddle(arr);

    for (int i = 0; i < res.size(); i++) {
        std::cout << res[i];

        if (i != res.size() - 1) {
            std::cout << " ";
        }
    }

    std::cout << "\n";

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
