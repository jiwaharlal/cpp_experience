#include <bits/stdc++.h>

using namespace std;

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

    std::string result;
    result.reserve(row_count * (col_count + 1) + 1);
    auto out_it = std::back_inserter(result);
    for (auto it = s.begin(); it != end_it; )
    {
        auto chunk_end_it = std::min(it + col_count, end_it);
        out_it = std::copy(it, chunk_end_it, out_it);
        it = chunk_end_it;
        *out_it = '\n';
        ++out_it;
    }

    *out_it = 0;

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
