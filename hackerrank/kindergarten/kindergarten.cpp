#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

int segmentsSolve(const std::vector<int>& t)
{
    std::vector<int> segments(t.size(), 0);
    int n = t.size();

    for (int i = 0; i < t.size(); ++i)
    {
        if (t[i] == 0)
        {
            ++segments[0];
            continue;
        }

        if (i >= t[i])
        {

            ++segments[0];
            --segments[i - t[i] + 1];
        }

        if (t[i] < n)
        {
            ++segments[i + 1];
            if (t[i] > i)
            {
                --segments[i + n - t[i] + 1];
            }
        }
    }

    int cur = 0;
    int best_index = n;
    int max_value = 0;

    for (int i = 0; i < segments.size(); ++i)
    {
        cur += segments[i];
        if (cur > max_value)
        {
            best_index = i;
            max_value = cur;
        }
    }

    return best_index + 1;
}

int bruteSolve(const std::vector<int>& t)
{
    std::pair<int, int> least_late{t.size(), 0};

    for (int start = 0; start < t.size(); ++start)
    {
        int lated = 0;
        int limit = 0;
        int i = start;
        for ( ; i < t.size(); ++i, ++limit)
        {
            if (t[i] > limit)
            {
                ++lated;
            }
        }
        for (i = 0; i < start; ++i, ++limit)
        {
            if (t[i] > limit)
            {
                ++lated;
            }
        }

        least_late = std::min(least_late, std::make_pair(lated, start));
    }

    return least_late.second + 1;
}

/*
 * Complete the solve function below.
 */
int solve(vector<int> t) {
    /*
     * Return the ID
     */
    return segmentsSolve(t);
}

int main()
{
    int t_count;
    cin >> t_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string t_temp_temp;
    getline(cin, t_temp_temp);

    vector<string> t_temp = split_string(t_temp_temp);

    vector<int> t(t_count);

    for (int t_itr = 0; t_itr < t_count; t_itr++) {
        int t_item = stoi(t_temp[t_itr]);

        t[t_itr] = t_item;
    }

    int id = solve(t);

    std::cout << id << "\n";

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
