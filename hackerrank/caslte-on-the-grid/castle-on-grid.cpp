#include <array>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

using Pos = std::pair<int, int>;

Pos operator +(const Pos& lhs, const Pos& rhs)
{
    return Pos{lhs.first + rhs.first, lhs.second + rhs.second};
}

bool isValid(const Pos& p, const std::vector<std::string>& grid)
{
    return p.first >= 0 && p.second >= 0 && p.second < grid.size() && p.first < grid[0].size()
        && grid[p.second][p.first] == '.';
}

std::vector<Pos> getAvailabeMoves(std::vector<std::string>& grid, const Pos& p)
{
    std::vector<Pos> result;
    static std::array<Pos, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    for (const auto& d : directions)
    {
        for (auto new_pos = p + d; isValid(new_pos, grid); new_pos = new_pos + d)
        {
            result.push_back(new_pos);
        }
    }

    return result;
}

// Complete the minimumMoves function below.
int minimumMoves(vector<string> grid, int startX, int startY, int goalX, int goalY)
{
    Pos start(startY, startX);
    Pos goal(goalY, goalX);

    std::queue<Pos> q;
    std::vector<std::vector<bool>> visited(
            grid.size(),
            std::vector<bool>(grid[0].size(), false));
    std::vector<std::vector<bool>> discovered(
            grid.size(),
            std::vector<bool>(grid[0].size(), false));

    std::map<Pos, Pos> prev;
    q.push(start);
    discovered[start.second][start.first] = true;

    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();
        if (visited[cur.second][cur.first])
        {
            continue;
        }
        visited[cur.second][cur.first] = true;

        if (cur == goal)
        {
            int len = 0;
            while (cur != start)
            {
                cur = prev[cur];
                ++len;
            }
            return len;
        }

        auto moves = getAvailabeMoves(grid, cur);
        for (auto new_pos : moves)
        {
            if (discovered[new_pos.second][new_pos.first])
            {
                continue;
            }
            discovered[new_pos.second][new_pos.first] = true;
            q.push(new_pos);
            prev[new_pos] = cur;
        }
    }

    return std::numeric_limits<int>::max();
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> grid(n);

    for (int i = 0; i < n; i++) {
        string grid_item;
        getline(cin, grid_item);

        grid[i] = grid_item;
    }

    string startXStartY_temp;
    getline(cin, startXStartY_temp);

    vector<string> startXStartY = split_string(startXStartY_temp);

    int startX = stoi(startXStartY[0]);

    int startY = stoi(startXStartY[1]);

    int goalX = stoi(startXStartY[2]);

    int goalY = stoi(startXStartY[3]);

    int result = minimumMoves(grid, startX, startY, goalX, goalY);

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
