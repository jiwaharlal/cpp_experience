#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

// Complete the maximumSum function below.
long maximumSum(vector<long> a, long m) {
    std::vector<std::pair<long, int>> prefixes;
    prefixes.reserve(m);
    prefixes.emplace_back(a[0] % m, 0);

    for (int i = 1; i < a.size(); ++i)
    {
        long cur = (prefixes[i - 1].first + a[i]) % m;
        prefixes.emplace_back(cur, i);
    }

    std::sort(prefixes.begin(), prefixes.end());

    long min_diff = std::numeric_limits<long>::max();
    long max_sum = std::max_element(prefixes.begin(), prefixes.end())->first;
    for (int i = 1; i < prefixes.size(); ++i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (prefixes[j].second < prefixes[i].second)
            {
                continue;
            }
            long diff = prefixes[i].first - prefixes[j].first;

            if (diff != 0)
            {
                min_diff = std::min(min_diff, diff);
            }
        }
    }

    return std::max(max_sum, m - min_diff);
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string nm_temp;
        getline(cin, nm_temp);

        vector<string> nm = split_string(nm_temp);

        int n = stoi(nm[0]);

        long m = stol(nm[1]);

        string a_temp_temp;
        getline(cin, a_temp_temp);

        vector<string> a_temp = split_string(a_temp_temp);

        vector<long> a(n);

        for (int i = 0; i < n; i++) {
            long a_item = stol(a_temp[i]);

            a[i] = a_item;
        }

        long result = maximumSum(a, m);

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
