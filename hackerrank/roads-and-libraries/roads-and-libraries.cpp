#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "../common/graph.hpp"
#include "../common/connected-components.hpp"

using namespace std;

vector<string> split_string(string);

// Complete the roadsAndLibraries function below.
long roadsAndLibraries(int n, int c_lib, int c_road, vector<vector<int>> cities)
{
    if (c_road >= c_lib)
    {
        return static_cast<long>(c_lib) * static_cast<long>(n);
    }

    for (auto& city : cities)
    {
        for (auto& connection : city)
        {
            --connection;
        }
    }

    auto adj = buildGraph(cities);
    adj.resize(n);
    auto components = findComponents(adj);

    long components_count = *std::max_element(components.begin(), components.end()) + 1;

    std::vector<int> weights(components_count, 0);
    for (auto c : components)
    {
        ++weights[c];
    }

    long total_roads = std::accumulate(weights.begin(), weights.end(), 0l) - components_count;

    return components_count * static_cast<long>(c_lib) + total_roads * static_cast<long>(c_road);
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string nmC_libC_road_temp;
        getline(cin, nmC_libC_road_temp);

        vector<string> nmC_libC_road = split_string(nmC_libC_road_temp);

        int n = stoi(nmC_libC_road[0]);

        int m = stoi(nmC_libC_road[1]);

        int c_lib = stoi(nmC_libC_road[2]);

        int c_road = stoi(nmC_libC_road[3]);

        vector<vector<int>> cities(m);
        for (int i = 0; i < m; i++) {
            cities[i].resize(2);

            for (int j = 0; j < 2; j++) {
                cin >> cities[i][j];
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        long result = roadsAndLibraries(n, c_lib, c_road, cities);

        std::cout << result << "\n";
    }

    return 0;
}

vector<string> split_string(string input_string) {
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