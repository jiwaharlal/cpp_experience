#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

// Complete the poisonousPlants function below.
int poisonousPlants(vector<int> p)
{
    std::list<int> plants{p.begin(), p.end()};

    std::vector<std::list<int>::iterator> to_die;
    std::vector<std::list<int>::iterator> to_check;
    to_check.reserve(plants.size());
    for (auto it = std::next(plants.begin()); it != plants.end(); ++it)
    {
        to_check.push_back(it);
    }

    int days = 0;

    while (!to_check.empty())
    {
        for (auto it : to_check)
        {
            if (*it > *std::prev(it))
            {
                to_die.push_back(it);
            }
        }

        if (to_die.empty())
        {
            break;
        }

        to_check.clear();
        for (int i = 0, limit = to_die.size() - 1; i < limit; ++i)
        {
            auto next_it = std::next(to_die[i]);
            if (next_it == to_die[i + 1])
            {
                continue;
            }
            to_check.push_back(next_it);
        }
        if (to_die.back() != plants.end() && std::next(to_die.back()) != plants.end())
        {
            to_check.push_back(std::next(to_die.back()));
        }

        ++days;
        for (auto it : to_die)
        {
            plants.erase(it);
        }
        to_die.clear();
    }

    return days;
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string p_temp_temp;
    getline(cin, p_temp_temp);

    vector<string> p_temp = split_string(p_temp_temp);

    vector<int> p(n);

    for (int i = 0; i < n; i++) {
        int p_item = stoi(p_temp[i]);

        p[i] = p_item;
    }

    int result = poisonousPlants(p);

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
