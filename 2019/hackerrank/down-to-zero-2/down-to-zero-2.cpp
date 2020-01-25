//#include <bits/stdc++.h>
#include <numeric>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

class DownToZeroCalc
{
public:
    explicit DownToZeroCalc(int limit);
    int getMin(int n);

private:
    std::vector<int> m_min_steps;
};

DownToZeroCalc::DownToZeroCalc(int limit)
    : m_min_steps(limit, std::numeric_limits<int>::max() - 2)
{
    std::vector<std::vector<int>> predecessors(limit, std::vector<int>());

    int mul1_limit = limit / 2;
    for (int mul1 = 1; mul1 <= mul1_limit; ++mul1)
    {
        for (int mul2 = 2; mul2 <= mul1; ++mul2)
        {
            int cell = mul1 * mul2;
            if (cell >= limit)
            {
                break;
            }
            predecessors[cell].push_back(mul1);
        }
    }

    for (int i = 1; i < limit; ++i)
    {
        predecessors[i].push_back(i - 1);
    }

    m_min_steps[0] = 0;

    for (int i = 1; i < limit; ++i)
    {
        const auto& pred = predecessors[i];
        m_min_steps[i] = std::accumulate(
                pred.begin(),
                pred.end(),
                limit,
                [&](int prev_min, int idx){ return std::min(prev_min, m_min_steps[idx]); }) + 1;
    }
}

int DownToZeroCalc::getMin(int n)
{
    return m_min_steps[n];
}

/*
 * Complete the downToZero function below.
 */
//int downToZero(int n) {
    /*
     * Write your code here.
     */

//}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DownToZeroCalc solver(1000001);

    for (int q_itr = 0; q_itr < q; q_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //int result = downToZero(n);
        int result = solver.getMin(n);

        std::cout << result << "\n";
    }

    return 0;
}
