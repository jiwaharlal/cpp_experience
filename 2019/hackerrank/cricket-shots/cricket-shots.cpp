#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the solve function below.
int solve(vector<vector<int>> shots, vector<vector<int>> players) {
    std::vector<int> shot_begins;
    shot_begins.reserve(shots.size());
    std::vector<int> shot_ends;
    shot_ends.reserve(shots.size());
    for (const auto& s : shots)
    {
        shot_begins.push_back(s[0]);
        shot_ends.push_back(s[1]);
    }
    std::sort(shot_begins.begin(), shot_begins.end());
    std::sort(shot_ends.begin(), shot_ends.end());

    int s = 0;
    for (const auto& p : players)
    {
        auto ends_before_it = std::lower_bound(shot_ends.begin(), shot_ends.end(), p[0]);
        int ends_before = std::distance(shot_ends.begin(), ends_before_it);
        auto begins_after_it = std::upper_bound(shot_begins.begin(), shot_begins.end(), p[1]);
        int begins_after = std::distance(begins_after_it, shot_begins.end());
        s += shots.size() - ends_before - begins_after;
    }

    return s;
}

int main()
{
    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<vector<int>> shots(n);
    for (int shots_row_itr = 0; shots_row_itr < n; shots_row_itr++) {
        shots[shots_row_itr].resize(2);

        for (int shots_column_itr = 0; shots_column_itr < 2; shots_column_itr++) {
            cin >> shots[shots_row_itr][shots_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<vector<int>> players(m);
    for (int players_row_itr = 0; players_row_itr < m; players_row_itr++) {
        players[players_row_itr].resize(2);

        for (int players_column_itr = 0; players_column_itr < 2; players_column_itr++) {
            cin >> players[players_row_itr][players_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = solve(shots, players);

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
