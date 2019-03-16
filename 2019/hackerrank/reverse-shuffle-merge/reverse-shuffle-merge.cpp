#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;

std::unordered_map<char, int> countChars(const std::string& s)
{
    std::unordered_map<char, int> char_counts;
    for (auto c : s)
    {
        ++char_counts[c];
    }

    return char_counts;
}

// Complete the reverseShuffleMerge function below.
string reverseShuffleMerge(string s)
{
    auto chars_in_shuffled = countChars(s);
    for (auto& char_count_pair : chars_in_shuffled)
    {
        char_count_pair.second /= 2;
    }
    auto chars_for_out = chars_in_shuffled;

    std::string result;
    result.reserve(s.size() / 2);
    std::reverse(s.begin(), s.end());
    int chars_left = s.size() / 2;

    for (auto start_it = s.cbegin(); start_it != s.cend();)
    {
        auto tmp_chars_in_shuffled = chars_in_shuffled;
        auto range_end = start_it;
        while (range_end != s.cend() && tmp_chars_in_shuffled[*range_end] != 0)
        {
            --tmp_chars_in_shuffled[*range_end];
            ++range_end;
        }

        range_end = std::min(std::next(range_end), s.cend());

        //std::cout << std::string(start_it, s.cend()) << std::endl;
        //std::cout << range_end - start_it << " : " << std::string(start_it, range_end) << std::endl;

        std::set<std::pair<char, std::string::const_iterator>> candidate_chars;
        for (auto it = start_it; it != range_end; ++it)
        {
            if (chars_for_out[*it] != 0)
            {
                candidate_chars.emplace(*it, it);
            }
        }
        if (candidate_chars.empty())
        {
            break;
        }

        //auto min_it = std::min_element(start_it, range_end);
        auto min_it = candidate_chars.begin()->second;
        for (auto it = start_it; it != min_it; ++it)
        {
            --chars_in_shuffled[*it];
        }

        result.push_back(*min_it);
        --chars_for_out[*min_it];
        start_it = std::next(min_it);
    }

    return result;
}

int main()
{
    string s;
    getline(cin, s);

    string result = reverseShuffleMerge(s);

    std::cout << result << "\n";

    return 0;
}
