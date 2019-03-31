#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <limits>

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

bool operator >(const SubSum& lhs, const SubSum& rhs)
{
    return lhs.min > rhs.min;
}

// Complete the riddle function below.
vector<long> riddle(vector<long> arr) {
    // complete this function
    std::vector<SubSum> q;
    std::vector<long> result;
    result.reserve(arr.size());

    for (int i = 0, limit = arr.size(); i < limit; ++i)
    {
        q.push_back(SubSum{i, 1, arr[i]});
    }

    std::make_heap(q.begin(), q.end());
    std::pop_heap(q.begin(), q.end());
    result.push_back(q.back().min);

    for (int s = 2; s <= arr.size(); ++s)
    {
        while (true)
        {
            auto& candidate = q.back();

            int limit = candidate.start + s;
            if (limit > arr.size())
            {
                q.erase(std::prev(q.end()));
                std::pop_heap(q.begin(), q.end());
                continue;
            }

            for (int i = candidate.start + candidate.len; i < limit; ++i)
            {
                candidate.min = std::min(candidate.min, arr[i]);
            }
            candidate.len = s;

            if (candidate.min >= q.front().min)
            {
                result.push_back(candidate.min);
                break;
            }

            std::push_heap(q.begin(), q.end());
            std::pop_heap(q.begin(), q.end());
        }
    }

    return result;
}

vector<long> riddleSet(vector<long> arr)
{
    using SetType = std::multiset<SubSum, std::greater<SubSum>>;
    SetType q;
    std::vector<long> result;
    result.reserve(arr.size());

    for (int i = 0; i < arr.size(); ++i)
    {
        q.insert(SubSum{i, 1, arr[i]});
    }

    result.push_back(q.begin()->min);

    std::vector<SubSum> tmp;
    for (int s = 2; s <= arr.size(); ++s)
    {
        long max_value = std::numeric_limits<long>::min();
        for (SetType::iterator it = q.begin(); it != q.end(); ++it)
        {
            SubSum candidate = *it;
            int limit = candidate.start + s;
            if (limit > arr.size())
            {
                continue;
            }

            for (int i = candidate.start + candidate.len; i < limit; ++i)
            {
                candidate.min = std::min(candidate.min, arr[i]);
            }
            max_value = std::max(max_value, candidate.min);
            candidate.len = s;
            tmp.push_back(candidate);

            auto next_it = std::next(it);
            if (next_it == q.end() || candidate.min >= next_it->min)
            {
                result.push_back(max_value);
                q.erase(q.begin(), next_it);
                q.insert(tmp.begin(), tmp.end());
                tmp.clear();
                break;
            }
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
