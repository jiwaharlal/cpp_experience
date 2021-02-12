#include <bits/stdc++.h>
#include <iterator>

using namespace std;

vector<string> split_string(string);

int getCrossSizeLimitByBorders(const std::vector<std::string>& grid, int row, int col)
{
    auto vertical_limit = std::min(row, static_cast<int>(grid.size()) - 1 - row);
    auto horizontal_limit = std::min(col, static_cast<int>(grid[0].size() - 1 - col));
    return std::min(vertical_limit, horizontal_limit) + 1;
}

std::ostream& operator <<(std::ostream& out, const std::pair<int, int>& p)
{
    return out << '(' << p.first << ", " << p.second << ')';
}

std::ostream& operator <<(std::ostream& out, const std::vector<std::pair<int, int>>& v)
{
    for (const auto& el : v)
    {
        out << el << ' ';
    }
    return  out;
}

std::vector<std::vector<std::pair<int, int>>> findPluses(const std::vector<std::string>& grid, int row, int col)
{
    std::vector<std::vector<std::pair<int, int>>> result;

    if (grid[row][col] == 'G')
    {
        result.push_back(std::vector<std::pair<int, int>>{std::make_pair(row, col)});
        auto size_limit = getCrossSizeLimitByBorders(grid, row, col);

        //std::cout << "Size limit: " << size_limit
            //<< " row=" << row << " col=" << col << std::endl;

        for (int i = 1; i < size_limit; ++i)
        {
            std::vector<std::pair<int, int>> new_cells{
                    std::make_pair(row, col - i),
                    std::make_pair(row, col + i),
                    std::make_pair(row - i, col),
                    std::make_pair(row + i, col)};

            //std::cout << "Cells: " << new_cells << std::endl;

            if (std::any_of(
                        new_cells.begin(),
                        new_cells.end(),
                        [&](const auto& cell) { return grid[cell.first][cell.second] != 'G'; }))
            {
                break;
            }

            const auto& prev_plus = result.back();
            new_cells.insert(new_cells.end(), prev_plus.begin(), prev_plus.end());
            std::sort(new_cells.begin(), new_cells.end());

            //std::cout << "New plus size: " << new_cells.size() << std::endl;
            result.push_back(std::move(new_cells));
        }
    }

    return result;
}

// Complete the twoPluses function below.
int twoPluses(vector<string> grid)
{
    std::vector<std::vector<std::pair<int, int>>> pluses;

    for (std::size_t row = 0u; row != grid.size(); ++row)
    {
        for (std::size_t col = 0u; col != grid[0].size(); ++col)
        {
            auto new_pluses = findPluses(grid, row, col);
            std::move(new_pluses.begin(), new_pluses.end(), std::back_inserter(pluses));
        }
    }

    //std::cout << "Pluses found: " << pluses.size() << std::endl;

    int max_product = 0;

    std::vector<std::pair<int, int>> intersection;
    for (std::size_t i = 0u; i != pluses.size() - 1; ++i)
    {
        for (std::size_t j = i + 1u; j != pluses.size(); ++j)
        {
            std::set_intersection(pluses[i].begin(), pluses[i].end(),
                                pluses[j].begin(), pluses[j].end(),
                                std::back_inserter(intersection));
            if (intersection.empty())
            {
                max_product = std::max(max_product, static_cast<int>(pluses[i].size() * pluses[j].size()));
            }
            else
            {
                intersection.clear();
            }
        }
    }

    return max_product;
}

int main()
{
    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    //int m = stoi(nm[1]);

    vector<string> grid(n);

    for (int i = 0; i < n; i++) {
        string grid_item;
        getline(cin, grid_item);

        grid[i] = grid_item;
    }

    int result = twoPluses(grid);

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
