#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#include <unordered_map>

using namespace std;

namespace std
{

template <typename T, typename U>
struct hash<pair<T, U>>
{
    size_t operator ()(const pair<T, U>& val) const
    {
        return hash<T>()(val.first) ^ (hash<U>()(val.second) << 32);
    }
};

} // namespace std

// Complete the abbreviation function below.
bool is_abbreviation(
        string a,
        std::size_t pos_a,
        string b,
        std::size_t pos_b,
        std::unordered_map<std::pair<size_t, size_t>, bool>& is_abbreviations)
{
    const auto elt_idx = std::make_pair(pos_a, pos_b);
    const auto it = is_abbreviations.find(elt_idx);
    if (it != is_abbreviations.end())
    {
        return it->second;
    }

    bool result = false;
    if (pos_b == b.size())
    {
        result = std::all_of(a.begin() + pos_a, a.end(), [](char c){ return std::islower(c); });
        is_abbreviations.emplace(elt_idx, result);
        return result;
    }

    if (pos_a == a.size())
    {
        result = false;
        is_abbreviations.emplace(elt_idx, false);
        return false;
    }

    if (a[pos_a] == b[pos_b] || std::toupper(a[pos_a]) == b[pos_b])
    {
        result |= is_abbreviation(a, pos_a + 1, b, pos_b + 1, is_abbreviations);
        if (result)
        {
            is_abbreviations.emplace(elt_idx, result);
            return result;
        }
    }
    if (std::islower(a[pos_a]))
    {
        result |= is_abbreviation(a, pos_a + 1, b, pos_b, is_abbreviations);
        if (result)
        {
            is_abbreviations.emplace(elt_idx, result);
            return result;
        }
    }

    is_abbreviations.emplace(elt_idx, result);

    return result;
}

bool is_abbreviation_dfs(
        string a,
        std::size_t pos_a,
        string b,
        std::size_t pos_b,
        std::unordered_map<std::pair<size_t, size_t>, bool>& is_abbreviations)
{
    std::vector<std::pair<std::size_t, std::size_t>> pos_stack;
    pos_stack.emplace_back(pos_a, pos_b);

    std::vector<bool> visited((a.size() + 1) * (b.size() + 1), false);

    while (!pos_stack.empty())
    {
        auto cur_pos = pos_stack.back();
        pos_stack.pop_back();

        visited[cur_pos.first * b.size() + cur_pos.second] = true;

        if (cur_pos.second == b.size())
        {
            if (std::all_of(a.begin() + cur_pos.first, a.end(), [](char c){ return std::islower(c); }))
            {
                return true;
            }

            continue;
        }

        if (cur_pos.first == a.size())
        {
            continue;
        }

        char ca = a[cur_pos.first];
        char cb = b[cur_pos.second];

        if (ca == cb || std::toupper(ca) == cb)
        {
            auto new_pos = std::make_pair(cur_pos.first + 1, cur_pos.second + 1);
            size_t index = new_pos.first * b.size() + new_pos.second;
            if (!visited[index])
            {
                pos_stack.push_back(new_pos);
            }
        }

        if (std::islower(ca))
        {
            auto new_pos = std::make_pair(cur_pos.first + 1, cur_pos.second);
            size_t index = new_pos.first * b.size() + new_pos.second;
            if (!visited[index])
            {
                pos_stack.push_back(new_pos);
            }
        }
    }

    return false;
}

string abbreviation(string a, string b) {
    std::unordered_map<std::pair<size_t, size_t>, bool> is_abbreviations;
    if (is_abbreviation_dfs(a, 0, b, 0, is_abbreviations))
    {
        return "YES";
    }

    return "NO";
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string a;
        getline(cin, a);

        string b;
        getline(cin, b);

        string result = abbreviation(a, b);

        std::cout << result << "\n";
    }

    return 0;
}
