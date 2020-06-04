#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

std::vector<std::vector<int>> indexString(const std::string& s)
{
    int letters_count = 'Z' - 'A' + 1;
    std::vector<std::vector<int>> result;
    result.resize(s.size() + 1, std::vector<int>(letters_count, -1));

    for (int i = s.size() - 2; i >= 0; --i)
    {
        result[i] = result[i + 1];
        result[i][s[i + 1] - 'A'] = i + 1;
    }

    result[s.size()] = result[0];
    result[s.size()][s[0] - 'A'] = 0;

    return result;
}

// Complete the commonChild function below.
int commonChild(string s1, string s2)
{
    auto indexes1 = indexString(s1);
    auto indexes2 = indexString(s2);

    std::map<std::pair<int, int>, int> lengths;
    lengths[std::make_pair(s1.size(), s2.size())] = 0;
    int max_len = 0;

    while (!lengths.empty())
    {
        std::pair<int, int> positions;
        int length;
        std::tie(positions, length) = *lengths.begin();
        lengths.erase(lengths.begin());

        const auto& i1 = indexes1[positions.first];
        const auto& i2 = indexes2[positions.second];

        int new_len = length + 1;
        for (int i = 0; i < i1.size(); ++i)
        {
            if (i1[i] < 0 || i2[i] < 0)
            {
                continue;
            }
            const auto next_pos = std::make_pair(i1[i], i2[i]);
            auto& pos_len = lengths[next_pos];
            pos_len = std::max(pos_len, new_len);
            max_len = std::max(pos_len, max_len);
        }
    }

    return max_len;
}

int main()
{
    string s1;
    getline(cin, s1);

    string s2;
    getline(cin, s2);

    int result = commonChild(s1, s2);

    std::cout << result << "\n";

    return 0;
}
