#include <bits/stdc++.h>

using namespace std;

// first is row, second is col
std::vector<std::pair<int, int>> adj(const std::pair<int, int>& pos, const std::pair<int, int>& grid_size)
{
    std::vector<std::pair<int, int>> result;
    result.reserve(8);

    for (int row = std::max(pos.first - 1, 0),
                row_limit = std::min(grid_size.first, pos.first + 2);
            row < row_limit;
            ++row)
    {
        for (int col = std::max(pos.second - 1, 0),
                    col_limit = std::min(grid_size.second, pos.second + 2);
                col < col_limit;
                ++col)
        {
            if (row == pos.first && col == pos.second)
            {
                continue;
            }
            result.emplace_back(row, col);
        }
    }

    return result;
}

// Complete the maxRegion function below.
int maxRegion(vector<vector<int>> grid)
{
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
    const auto grid_size = std::make_pair(grid.size(), grid[0].size());

    int max_size = 0;

    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[0].size(); ++col)
        {
            if (visited[row][col])
            {
                continue;
            }

            visited[row][col] = true;
            if (grid[row][col] == 0)
            {
                continue;
            }

            int cur_size = 1;
            std::vector<std::pair<int, int>> stack;
            stack.emplace_back(row, col);

            while (!stack.empty())
            {
                auto cur = stack.back();
                stack.pop_back();

                for (auto next : adj(cur, grid_size))
                {
                    if (visited[next.first][next.second])
                    {
                        continue;
                    }
                    visited[next.first][next.second] = true;

                    if (grid[next.first][next.second] == 0)
                    {
                        continue;
                    }

                    ++cur_size;
                    stack.push_back(next);
                }
            }

            max_size = std::max(max_size, cur_size);
        }
    }

    return max_size;
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int m;
    cin >> m;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> grid(n);
    for (int i = 0; i < n; i++) {
        grid[i].resize(m);

        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int res = maxRegion(grid);

    std::cout << res << "\n";

    return 0;
}
