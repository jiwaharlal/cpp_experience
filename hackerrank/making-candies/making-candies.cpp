#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

vector<string> split_string(string);

std::pair<long, long> calcNewProds(long m, long w, long add_prod)
{
    std::pair<long, long> result = std::minmax(m, w);

    long total = m + w + add_prod;
    long half = total / 2 + total % 2;

    result.second = std::max(result.second, half);
    result.first = total - result.second;

    return result;
}

template <typename T>
T upperDiv(T a, T b)
{
    T result = a / b;
    if (a % b != 0)
    {
        ++result;
    }

    return result;
}

long minimumPasses(long m, long w, long p, long n)
{
    if ((n / m) / w == 0)
    {
        return 1;
    }

    long candy = 0;
    long invest = 0;
    long spend = std::numeric_limits<long>::max();

    while (candy < n)
    {
        long passes = (p - candy) / (m * w);
        if (passes <= 0)
        {
            std::tie(m, w) = calcNewProds(m, w, candy / p);
            candy %= p;
            passes = 1;
        }
        candy += passes * m * w;
        invest += passes;
        spend = std::min(spend, invest + upperDiv(n - candy, m * w));
    }

    return std::min(invest, spend);
}

int main()
{
    string mwpn_temp;
    getline(cin, mwpn_temp);

    vector<string> mwpn = split_string(mwpn_temp);

    long m = stol(mwpn[0]);

    long w = stol(mwpn[1]);

    long p = stol(mwpn[2]);

    long n = stol(mwpn[3]);

    long result = minimumPasses(m, w, p, n);

    std::cout << result << "\n";

    return 0;
}

vector<string> split_string(string input_string)
{
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
