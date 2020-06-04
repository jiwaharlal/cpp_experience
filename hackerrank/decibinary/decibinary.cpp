#include <bits/stdc++.h>
#include <algorithm>
#include <iterator>

#include "decibinary-generator.h"

using namespace std;

// Complete the decibinaryNumbers function below.
long decibinaryNumbers(long x)
{
    if (x == 1)
    {
        return 0l;
    }

    x -= 2;

    for (std::int64_t v = 1; v <= std::numeric_limits<std::int64_t>::max(); ++v)
    {
#if __cplusplus >= 201703L
        auto [lines, count] = generateDecibinaries(v, true);
#else
        std::vector<std::string> lines;
        std::int64_t count;
        std::tie(lines, count) = generateDecibinaries(v, true);
#endif

        if (count > x)
        {
            std::tie(lines, count) = generateDecibinaries(v, false);
            return std::atol(lines[x].c_str());
        }

        x -= count;
    }
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        long x;
        cin >> x;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        long result = decibinaryNumbers(x);

        std::cout << result << "\n";
    }

    return 0;
}
