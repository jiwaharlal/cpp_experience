#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

// Complete the queensAttack function below.
template <typename T, typename U>
std::pair<T, U> operator +(const std::pair<T, U>& lhs, const std::pair<T, U>& rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

struct Ray
{
    std::vector<std::pair<int, int>> points;
    bool is_reversed = false;
};

bool isInBoard(int n, const std::pair<int, int>& p)
{
    return p.first > 0 && p.second > 0 && p.first <= n && p.second <= n;
}

Ray createRay(int n, const std::pair<int, int>& initial, const std::pair<int, int>& dir)
{
    Ray ray;
    ray.is_reversed = dir < std::make_pair(0, 0);
    for (auto cur = initial + dir; isInBoard(n, cur); cur = cur + dir)
    {
        ray.points.push_back(cur);
    }
    if (ray.is_reversed)
    {
        std::reverse(ray.points.begin(), ray.points.end());
    }

    return ray;
}

std::vector<std::pair<int, int>> getPossibleDirections()
{
    std::vector<std::pair<int, int>> directions;
    for (int row = -1; row <= 1; ++row)
    {
       for (int col = -1; col <= 1; ++col)
       {
          if (row == 0 && col == 0)
              continue;
          directions.emplace_back(row, col);
       }
    }

    return directions;
}

int posDiff(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
{
    auto first_diff = std::abs(lhs.first - rhs.first);
    auto second_diff = std::abs(lhs.second - rhs.second);
    return std::max(first_diff, second_diff);
}

int queensAttack(int n, int k, int r_q, int c_q, vector<vector<int>> obstacles)
{
    auto directions = getPossibleDirections();
    std::vector<Ray> rays;
    auto queen_pos = std::make_pair(r_q, c_q);

    for (const auto& dir : directions)
    {
        auto ray = createRay(n, queen_pos, dir);
        if (ray.points.size())
        {
            rays.push_back(createRay(n, queen_pos, dir));
        }
    }

    std::vector<std::pair<int, int>> obst;
    obst.reserve(obstacles.size());

    std::transform(
            obstacles.begin(),
            obstacles.end(),
            std::back_inserter(obst),
            [](const std::vector<int>& o){ return std::make_pair(o[0], o[1]); });
    std::sort(obst.begin(), obst.end());

    int count = 0;
    for (const auto& ray : rays)
    {
        std::vector<std::pair<int, int>> intersections;
        std::set_intersection(
                obst.begin(), obst.end(),
                ray.points.begin(), ray.points.end(),
                std::back_inserter(intersections));

        if (intersections.empty())
        {
            count += ray.points.size();
            continue;
        }

        if (!ray.is_reversed)
        {
            count += posDiff(queen_pos, intersections.front()) - 1;
        }
        else
        {
            count += posDiff(queen_pos, intersections.back()) - 1;
        }
    }

    return count;
}

int main()
{
    string nk_temp;
    getline(cin, nk_temp);

    vector<string> nk = split_string(nk_temp);

    int n = stoi(nk[0]);

    int k = stoi(nk[1]);

    string r_qC_q_temp;
    getline(cin, r_qC_q_temp);

    vector<string> r_qC_q = split_string(r_qC_q_temp);

    int r_q = stoi(r_qC_q[0]);

    int c_q = stoi(r_qC_q[1]);

    vector<vector<int>> obstacles(k);
    for (int i = 0; i < k; i++) {
        obstacles[i].resize(2);

        for (int j = 0; j < 2; j++) {
            cin >> obstacles[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = queensAttack(n, k, r_q, c_q, obstacles);

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
