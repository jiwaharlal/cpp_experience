#include <bits/stdc++.h>

#include "../common/graph.hpp"

using namespace std;

/*
 * Complete the componentsInGraph function below.
 */
void trackComponent(
        int component_idx,
        const AdjList& adj,
        std::vector<int>& vertex_components,
        int initial_vertex)
{
    std::vector<int> stack;
    stack.push_back(initial_vertex);
    vertex_components[initial_vertex] = component_idx;

    while (!stack.empty())
    {
        int cur = stack.back();
        stack.pop_back();

        for (int v : adj[cur])
        {
            if (vertex_components[v] == component_idx)
            {
                continue;
            }

            vertex_components[v] = component_idx;
            stack.push_back(v);
        }
    }
}

std::vector<int> findComponents(const AdjList& adj)
{
    std::vector<int> vertex_components(adj.size(), -1);

    int cur_component = 0;

    for (int i = 0; i < vertex_components.size(); ++i)
    {
        if (vertex_components[i] == -1)
        {
            trackComponent(cur_component, adj, vertex_components, i);
            ++cur_component;
        }
    }

    return vertex_components;
}

vector<int> componentsInGraph(vector<vector<int>> gb) {
    /*
     * Write your code here.
     */
    for (auto& vertices : gb)
    {
        for (auto& v : vertices)
        {
            --v;
        }
    }

    auto adj = buildGraph(gb);
    auto vertex_components = findComponents(adj);
    auto components_count =
        *std::max_element(vertex_components.begin(), vertex_components.end()) + 1;

    std::vector<int> components_weight(components_count, 0);
    for (auto c : vertex_components)
    {
        ++components_weight[c];
    }

    int max_weight = *std::max_element(components_weight.begin(), components_weight.end());
    int min_weight = max_weight;

    for (int w : components_weight)
    {
        if (w > 1 && w < min_weight)
        {
            min_weight = w;
        }
    }

    return {min_weight, max_weight};
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
