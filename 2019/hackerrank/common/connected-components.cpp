#include "connected-components.hpp"

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
