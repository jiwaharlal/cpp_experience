#include <bits/stdc++.h>

using namespace std;

// Complete the kaprekarNumbers function below.
void kaprekarNumbers(int p, int q) {
    int divisor = 10;
    std::vector<int> result;
    for (std::int64_t i = p; i <= q; ++i)
    {
        for (; divisor <= i; divisor *= 10);
        std::int64_t s = i * i;
        std::int64_t i2 = s / divisor + s % divisor;

        if (i2 == i)
        {
            result.push_back(i);
        }
    }
    if (result.empty())
    {
        std::cout << "INVALID RANGE";
    }
    else 
    {
        std::copy(result.begin(), result.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }
}

int main()
{
    int p;
    cin >> p;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    kaprekarNumbers(p, q);

    return 0;
}
