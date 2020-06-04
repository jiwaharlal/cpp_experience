#include <bits/stdc++.h>

using namespace std;

// Complete the organizingContainers function below.
string organizingContainers(vector<vector<int>> container) {
    std::vector<long> capacities;
    capacities.reserve(container.size());
    for (int i = 0; i < container.size(); ++i)
    {
        const auto& c = container[i];
        capacities.push_back(std::accumulate(c.begin(), c.end(), 0l));
    }
    std::vector<long> balls(container.size(), 0l);
    for (int cont_idx = 0; cont_idx != container.size(); ++cont_idx)
    {
        const auto& c = container[cont_idx];
        for (int color_idx = 0; color_idx != c.size(); ++color_idx)
        {
            balls[color_idx] += c[color_idx];
        }
    }

    std::sort(balls.begin(), balls.end());
    std::sort(capacities.begin(), capacities.end());

    const auto mismatch_it = std::mismatch(
        capacities.begin(), capacities.end(), balls.begin());

    if (mismatch_it.first == capacities.end())
    {
        return "Possible";
    }

    return "Impossible";
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<vector<int>> container(n);
        for (int i = 0; i < n; i++) {
            container[i].resize(n);

            for (int j = 0; j < n; j++) {
                cin >> container[i][j];
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string result = organizingContainers(container);

        std::cout << result << "\n";
    }

    return 0;
}
