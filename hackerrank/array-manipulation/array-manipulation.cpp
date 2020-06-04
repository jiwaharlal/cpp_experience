#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

// Complete the arrayManipulation function below.
enum class BorderSide
{
    left,
    right
};

using Border = std::pair<int, BorderSide>;

void printArray(const std::map<Border, int>& borders)
{
    int i = 0;
    int value = 0;
    for (const auto& b : borders)
    {
        while (i < b.first.first)
        {
            std::cout << value << " ";
            ++i;
        }

        if (b.first.second == BorderSide::left)
        {
            value += b.second;
        }
        else
        {
            value -= b.second;
        }
    }

    std::cout << std::endl;
}

std::map<Border, int> createBorders(const vector<vector<int>>& queries)
{
    std::map<Border, int> borders;
    for (const auto& query : queries)
    {
        auto left = std::make_pair(query[0], BorderSide::left);
        auto left_it = borders.find(left);
        if (left_it == borders.end())
        {
            borders[left] = query[2];
        }
        else
        {
            left_it->second += query[2];
        }

        auto right = std::make_pair(query[1] + 1, BorderSide::right);
        auto right_it = borders.find(right);
        if (right_it == borders.end())
        {
            borders[right] = query[2];
        }
        else
        {
            right_it->second += query[2];
        }

        //printArray(borders);
    }

    return borders;
}

long arrayManipulation(int n, vector<vector<int>> queries)
{
    (void) n;

    auto borders = createBorders(queries);

    long long max_value = 0;
    long long cur_value = 0;
    auto prev_index = 0;
    for (const auto& border_pair : borders)
    {
        const auto& border = border_pair.first;
        auto increment = border_pair.second;

        if (prev_index != border.first)
        {
            max_value = std::max(max_value, cur_value);
            prev_index = border.first;
        }

        if (border.second == BorderSide::left)
        {
            cur_value += increment;
        }
        else
        {
            cur_value -= increment;
        }
    }

    return max_value;
}

int main()
{
    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<vector<int>> queries(m);
    for (int i = 0; i < m; i++) {
        queries[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> queries[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    long result = arrayManipulation(n, queries);

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
