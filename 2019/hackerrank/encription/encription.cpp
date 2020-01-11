#include <bits/stdc++.h>

using namespace std;

// Complete the encryption function below.
string encryption(string s) {
    auto end_it = std::remove_if(s.begin(), s.end(), [](char c){ return std::isspace(c); });
    s.erase(end_it, s.end());
    int char_count = s.size();
    int col_count = std::ceil(std::sqrt(char_count));

    std::string result;
    result.reserve(char_count + col_count);
    for (int col = 0; col != col_count; ++col)
    {
        for (int idx = col; idx < char_count; idx += col_count)
        {
            result.push_back(s[idx]);
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
