#include <bits/stdc++.h>

using namespace std;

std::vector<std::string> transpose(const std::vector<std::string>& lines)
{
    std::vector<std::string> result(lines[0].size());

    for (int symbol_idx = 0; symbol_idx < lines[0].size(); ++symbol_idx)
    {
        result[symbol_idx].reserve(lines.size());
        for (const auto& line : lines)
        {
            if (line.size() == symbol_idx)
                break;
            result[symbol_idx].push_back(line[symbol_idx]);
        }
    }

    return result;
}

// Complete the encryption function below.
string encryption(string s)
{
    auto end_it = std::remove(s.begin(), s.end(), ' ');
    double len = static_cast<double>(end_it - s.begin());
    int col_count = static_cast<int>(std::floor(std::sqrt(len)));
    int row_count = col_count;

    if (row_count * col_count < static_cast<int>(len))
    {
        ++col_count;
    }

    if (row_count * col_count < static_cast<int>(len))
    {
        ++row_count;
    }

    std::vector<std::string> lines;
    lines.reserve(row_count);
    for (auto it = s.begin(); it != end_it; )
    {
        auto chunk_end_it = std::min(it + col_count, end_it);
        lines.emplace_back(it, chunk_end_it);
        it = chunk_end_it;
    }

    std::string result;

    for (int symbol_idx = 0; symbol_idx < lines[0].size(); ++symbol_idx)
    {
        for (const auto& line : lines)
        {
            if (line.size() == symbol_idx)
                break;
            result.push_back(line[symbol_idx]);
        }
        result.push_back(' ');
    }
    result.pop_back();
    return result;
}

int main()
{
    string s;
    getline(cin, s);

    string result = encryption(s);

    std::cout << result << "\n";

    return 0;
}
