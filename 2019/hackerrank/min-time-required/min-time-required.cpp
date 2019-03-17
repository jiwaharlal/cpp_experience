#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

vector<string> split_string(string);

// Complete the minTime function below.
long minTime(vector<long> machines, long goal)
{
    double rate = std::accumulate(
        machines.begin(),
        machines.end(),
        0.,
        [](double rate, long days){ return rate + 1. / static_cast<double>(days); });
    long min_days = static_cast<long>(goal / rate);

    long produced = std::accumulate(
            machines.begin(), machines.end(), 0l,
            [min_days](long sum, long days){ return sum + min_days / days; });

    if (produced == goal)
    {
        return min_days;
    }

    using LongPair = std::pair<long, long>;
    std::vector<LongPair> days_left;
    days_left.reserve(machines.size());
    std::transform(machines.begin(), machines.end(), std::back_inserter(days_left),
            [min_days](long days){ return std::make_pair(days - min_days % days, days); });

    std::make_heap(days_left.begin(), days_left.end(), std::greater<LongPair>());
    long add_days = 0l;
    while (produced < goal)
    {
        std::pop_heap(days_left.begin(), days_left.end(), std::greater<LongPair>());
        add_days = days_left.back().first;
        days_left.back().first += days_left.back().second;
        std::push_heap(days_left.begin(), days_left.end(), std::greater<LongPair>());

        ++produced;
    }

    return min_days + add_days;
}

int main()
{
    string nGoal_temp;
    getline(cin, nGoal_temp);

    vector<string> nGoal = split_string(nGoal_temp);

    int n = stoi(nGoal[0]);

    long goal = stol(nGoal[1]);

    string machines_temp_temp;
    getline(cin, machines_temp_temp);

    vector<string> machines_temp = split_string(machines_temp_temp);

    vector<long> machines(n);

    for (int i = 0; i < n; i++) {
        long machines_item = stol(machines_temp[i]);

        machines[i] = machines_item;
    }

    long ans = minTime(machines, goal);

    std::cout << ans << "\n";

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
