#include <bits/stdc++.h>

#include "../common/graph.hpp"

using namespace std;

/*
 * Complete the componentsInGraph function below.
 */
vector<int> componentsInGraph(vector<vector<int>> gb) {
    /*
     * Write your code here.
     */
    auto adj = buildGraph(gb);
    return adj[0];
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> gb(n);
    for (int gb_row_itr = 0; gb_row_itr < n; gb_row_itr++) {
        gb[gb_row_itr].resize(2);

        for (int gb_column_itr = 0; gb_column_itr < 2; gb_column_itr++) {
            cin >> gb[gb_row_itr][gb_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<int> result = componentsInGraph(gb);

    for (int SPACE_itr = 0; SPACE_itr < result.size(); SPACE_itr++) {
        std::cout << result[SPACE_itr];

        if (SPACE_itr != result.size() - 1) {
            std::cout << " ";
        }
    }

    std::cout << "\n";

    return 0;
}
