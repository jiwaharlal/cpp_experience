#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the bomberMan function below.

//void boom_4(std::vector<std::vector<int>>& g, int row, int col)
//{
    //g[row][col] = 0;
    //if (row != 0)
    //{
        //g[row - 1][col] = 0;
    //}
    //if (col != 0)
    //{
        //g[row][col - 1] = 0;
    //}
    //if (row + 1 != g.size())
    //{
        //g[row + 1][col] = 0;
    //}
    //if (col + 1 != g[0].size())
    //{
        //g[row][col + 1] = 0;
    //}
//}

//void boom_8(std::vector<std::vector<int>>& g, int row, int col)
//{
    //boom_4(g, row, col);
    //if (row != 0 && col != 0)
    //{
        //g[row - 1][col - 1] = 0;
    //}
    //if (row != 0 && col + 1 != g[0].size())
    //{
        //g[row - 1][col + 1] = 0;
    //}
    //if (row + 1 != g.size() && col != 0)
    //{
        //g[row + 1][col - 1] = 0;
    //}
    //if (row + 1 != g.size() && col + 1 != g[0].size())
    //{
        //g[row + 1][col + 1] = 0;
    //}
//}

//std::vector<std::vector<int>> tick_boom(const std::vector<std::vector<int>>& grid)
//{
    //std::vector<std::vector<int>> result(grid.size(), std::vector<int>(grid[0].size(), 0));

    //for (int row = 0; row != grid.size(); ++row)
    //{
        //for (int col = 0; col != grid[0].size(); ++col)
        //{
            //result[row][col] = std::max(grid[row][col] - 1, 0);
        //}
    //}

    //for (int row = 0; row != grid.size(); ++row)
    //{
        //for (int col = 0; col != grid[0].size(); ++col)
        //{
            //if (grid[row][col] == 1)
            //{
                //boom_4(result, row, col);
            //}
        //}
    //}

    //return result;
//}

void boom_cell_inplace(int& cell)
{
    if (cell == 1)
    {
        return;
    }
    cell = 0;
}

void boom_4_inplace(std::vector<std::vector<int>>& grid, int row, int col)
{
    if (row != 0)
    {
        boom_cell_inplace(grid[row - 1][col]);
    }
    if (col != 0)
    {
        boom_cell_inplace(grid[row][col - 1]);
    }
    if (row + 1 != grid.size())
    {
        boom_cell_inplace(grid[row + 1][col]);
    }
    if (col + 1 != grid[0].size())
    {
        boom_cell_inplace(grid[row][col + 1]);
    }
}

void tick_boom_inplace(std::vector<std::vector<int>>& grid)
{
    for (int row = 0; row != grid.size(); ++row)
    {
        for (int col = 0; col != grid[0].size(); ++col)
        {
            if (grid[row][col] == 1)
            {
                boom_4_inplace(grid, row, col);
            }
        }
    }

    for (int row = 0; row != grid.size(); ++row)
    {
        for (int col = 0; col != grid[0].size(); ++col)
        {
            grid[row][col] = std::max(grid[row][col] - 1, 0);
        }
    }
}

void tick_plant_all(std::vector<std::vector<int>>& grid)
{
    std::for_each(grid.begin(), grid.end(), [](auto& row)
            {
                std::for_each(row.begin(), row.end(), [](auto& cell)
                        {
                            if (cell == 0)
                            {
                                cell = 3;
                            }
                            else
                            {
                                --cell;
                            }
                        });
            });
}

std::vector<std::vector<int>> transformToTimed(const std::vector<std::string>& grid)
{
    std::vector<std::vector<int>> result(grid.size(), std::vector<int>(grid[0].size(), 0));
    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[0].size(); ++col)
        {
            if (grid[row][col] == 79)
            {
                result[row][col] = 3;
            }
        }
    }

    return result;
}

std::vector<std::string> transformToUntimed(const std::vector<std::vector<int>>& timed_grid)
{
    std::vector<std::string> result(timed_grid.size(), std::string(timed_grid[0].size(), '.'));

    for (int row = 0; row < timed_grid.size(); ++row)
    {
        for (int col = 0; col < timed_grid[0].size(); ++col)
        {
            if (timed_grid[row][col])
            {
                result[row][col] = static_cast<char>(79);
            }
        }
    }

    return result;
}

vector<string> bomberMan(int n, vector<string> grid)
{
    auto timed_grid = transformToTimed(grid);

    for (int i = 1; i <= n; ++i)
    {
        if (i % 2)
        {
            tick_boom_inplace(timed_grid);
        }
        else
        {
            tick_plant_all(timed_grid);
        }
    }

    return transformToUntimed(timed_grid);
}

int main()
{
    //ofstream std::cout(getenv("OUTPUT_PATH"));

    string rcn_temp;
    getline(cin, rcn_temp);

    vector<string> rcn = split_string(rcn_temp);

    int r = stoi(rcn[0]);

    int c = stoi(rcn[1]);

    int n = stoi(rcn[2]);

    vector<string> grid(r);

    for (int i = 0; i < r; i++) {
        string grid_item;
        getline(cin, grid_item);

        grid[i] = grid_item;
    }

    vector<string> result = bomberMan(n, grid);

    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i];

        if (i != result.size() - 1) {
            std::cout << "\n";
        }
    }

    std::cout << "\n";

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
