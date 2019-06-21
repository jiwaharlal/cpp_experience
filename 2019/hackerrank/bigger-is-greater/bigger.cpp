#include <bits/stdc++.h>

using namespace std;

// Complete the biggerIsGreater function below.
std::string biggerIsGreater(string w) {
    char max_char = w.back();

    auto exchange_pos = [&]()
    {
        for (int pos = w.size() - 2; pos >= 0; --pos)
        {
            if (max_char > w[pos])
            {
                return pos;
            }
            max_char = std::max(max_char, w[pos]);
        }
        return -1;
    }();

    if (exchange_pos == -1)
    {
        return "no answer";
    }

    int donor_pos = exchange_pos + 1;

    for (std::size_t pos = donor_pos + 1; pos < w.size(); ++pos)
    {
        if (w[pos] < w[donor_pos] && w[pos] > w[exchange_pos])
        {
            donor_pos = pos;
        }
    }

    std::swap(w[donor_pos], w[exchange_pos]);
    std::sort(w.begin() + exchange_pos + 1, w.end());

    return w;
}

int main()
{
    int T;
    cin >> T;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int T_itr = 0; T_itr < T; T_itr++) {
        string w;
        getline(cin, w);

        string result = biggerIsGreater(w);

        std::cout << result << "\n";
    }

    return 0;
}
