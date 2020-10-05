#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the absolutePermutation function below.
vector<int> absolutePermutation(int n, int k) {
    std::vector<int> result(1, -1);
    if (k && ((k * 2 > n) || (n % k) || ((n / k) % 2)))
    {
        return result;
    }

    std::set<int> available_values;
    for (int i = 1; i <= n; ++i)
    {
        available_values.insert(i);
    }

    std::vector<int> permutation;

    auto try_append = [&](int value) -> bool
    {
        if (available_values.count(value))
        {
            available_values.erase(value);
            permutation.push_back(value);
            return true;
        }
        return false;
    };

    for (int val = k + 1, limit = std::min(n, k * 2); val <= limit; ++val)
    {
        try_append(val);
    }

    while (!available_values.empty())
    {
        int cur_pos = permutation.size() + 1;
        bool appended = false;

        for (int candidate : {cur_pos - k, cur_pos + k})
        {
            if (candidate > 0 && candidate <= n && try_append(candidate))
            {
                appended = true;
                break;
            }
        }

        if (appended)
        {
            continue;
        }

        while (permutation.size() > k)
        {
            --cur_pos;
            int last_popped = permutation.back();
            permutation.pop_back();
            available_values.insert(last_popped);
            if (last_popped < cur_pos && try_append(last_popped + k * 2))
            {
                break;
            }
        }

        if (permutation.size() <= k)
        {
            return result;
        }
    }

    return permutation;
}

int main()
{
    //ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string nk_temp;
        getline(cin, nk_temp);

        vector<string> nk = split_string(nk_temp);

        int n = stoi(nk[0]);

        int k = stoi(nk[1]);

        vector<int> result = absolutePermutation(n, k);

        for (int i = 0; i < result.size(); i++) {
            std::cout << result[i];

            if (i != result.size() - 1) {
                std::cout << " ";
            }
        }

        std::cout << "\n";
    }

    //fout.close();

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
