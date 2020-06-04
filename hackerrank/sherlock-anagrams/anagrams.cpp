#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>

using namespace std;

using CharCounter = std::map<char, int>;

void decCount(CharCounter& counter, char c)
{
    auto it = counter.find(c);
    if (it == counter.end())
    {
        --counter[c];
    }
    else
    {
        --(it->second);
        if (it->second == 0)
        {
            counter.erase(it);
        }
    }
}

// Complete the sherlockAndAnagrams function below.
int countAnagrams(string s)
{
    int result = 0;

    CharCounter initial;

    for (int len = 1; len < s.size(); ++len)
    {
        ++initial[s[len - 1]];
        auto first = initial;

        for (int s1 = 0, s1_limit = s.size() - len; s1 < s1_limit; ++s1)
        {
            auto second = first;

            for (int s2 = s1 + 1, s2_limit = s.size() - len + 1; s2 < s2_limit; ++s2)
            {
                decCount(second, s[s2 - 1]);
                ++second[s[s2 + len - 1]];

                if (std::equal(first.begin(), first.end(), second.begin()))
                {
                    ++result;
                }
            }

            ++first[s[s1 + len]];
            decCount(first, s[s1]);
        }
    }

    return result;
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string s;
        getline(cin, s);

        int result = countAnagrams(s);

        std::cout << result << "\n";
    }

    return 0;
}
