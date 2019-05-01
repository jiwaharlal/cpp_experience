#include <cstring>
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

using AdjList = std::vector<std::vector<int>>;

static const long c_magic_divider = std::pow(10, 9) + 7;

AdjList buildGraph(const std::vector<std::vector<int>>& tree)
{
    AdjList adj;

    int last_vertex = std::accumulate(
        tree.begin(),
        tree.end(),
        0,
        [](auto m, const auto& branch)
            { return std::max(m, std::max(branch[0], branch[1])); });
    int tree_size = last_vertex + 1;
    adj.resize(tree_size);
    for (const auto& edge : tree)
    {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }

    return adj;
}

// as a result, 0-th vertex is a root, for each vertex last adj.back() is a parent,
// and for 0-th vertex parent = 0
std::vector<int> transformToTree(AdjList& adj)
{
    std::vector<int> ranks(adj.size(), -1);
    adj[0].push_back(0);
    ranks[0] = 0;
    std::vector<int> stack;

    stack.push_back(0);

    while (!stack.empty())
    {
        int cur = stack.back();
        stack.pop_back();

        auto& children = adj[cur];
        for (int i = 0, last = children.size() - 1; i != last; ++i)
        {
            auto child = children[i];
            auto& child_adj = adj[child];

            if (adj[child].back() != cur)
            {
                std::swap(child_adj.back(), *std::find(child_adj.begin(), child_adj.end(), cur));
            }

            stack.push_back(child);
            ranks[child] = ranks[cur] + 1;
        }
    }

    return ranks;
}

std::vector<int> findAllSubtreeNodes(const AdjList& adj, const std::vector<int>& some_nodes)
{
    auto result = some_nodes;

    std::vector<bool> initial_nodes(adj.size(), false);
    std::vector<bool> visited(adj.size(), false);

    for (auto v : some_nodes)
    {
        visited[v] = 1;
        initial_nodes[v] = 1;
    }

    std::vector<int> new_nodes;
    std::vector<int> new_nodes_to_root;

    for (int n : some_nodes)
    {
        new_nodes.clear();
        for (int v = adj[n].back(); ; v = adj[v].back())
        {
            if (v == 0)
            {
                new_nodes_to_root.swap(new_nodes);
                break;
            }

            if (visited[v])
            {
                result.insert(result.end(), new_nodes.begin(), new_nodes.end());
                break;
            }

            visited[v] = 1;
            new_nodes.push_back(v);
        }
    }

    // cut nodes to root until either node from initial list, or node with >=2 children
    auto it = new_nodes_to_root.rbegin();
    for ( ; it != new_nodes_to_root.rend(); ++it)
    {
        if (initial_nodes[*it])
        {
            break;
        }

        const auto& adj_to = adj[*it];

        int visited_children_count = std::accumulate(
                adj_to.begin(), std::prev(adj_to.end()),
                0, [&](int s, int v){ return s + visited[v]; });

        if (visited_children_count > 1)
        {
            break;
        }
    }

    for ( ; it != new_nodes_to_root.rend(); ++it)
    {
        result.push_back(*it);
    }

    return result;
}

std::vector<long> calcSums(
        const std::vector<std::vector<int>>& edges,
        const std::vector<std::vector<int>>& queries)
{
    std::vector<long> result;
    result.reserve(queries.size());

    auto adj = buildGraph(edges);
    adj[0].push_back(1);
    adj[1].push_back(0);
    transformToTree(adj);

    std::vector<char> present(adj.size(), 0);
    std::vector<int> child_left(adj.size(), 0);
    std::vector<long> subtree_sums(adj.size(), 0l);

    for (const auto& query : queries)
    {
        //std::memset(present.data(), 0, present.size());
        //std::memset(child_left.data(), 0, child_left.size() * sizeof(int));
        //std::memset(subtree_sums.data(), 0, subtree_sums.size() + sizeof(long));

        auto subtree_nodes = findAllSubtreeNodes(adj, query);

        long total_sum = std::accumulate(
                query.begin(), query.end(), 0l, [](long a, long b){ return a + b; });
        total_sum %= c_magic_divider;

        for (auto v : subtree_nodes)
        {
            present[v] = 1;
            subtree_sums[v] = 0;
        }
        for (auto v : query)
        {
            subtree_sums[v] = v;
        }

        std::queue<int> q;
        for (auto v : subtree_nodes)
        {
            child_left[v] = std::accumulate(
                    adj[v].begin(), std::prev(adj[v].end()), 0,
                    [&](int s, int u){ return s + present[u]; });

            if (child_left[v] == 0)
            {
                q.push(v);
            }
        }

        long query_sum = 0l;
        while (!q.empty())
        {
            int v = q.front();
            q.pop();

            long subtree_sum = subtree_sums[v] % c_magic_divider;
            query_sum += (subtree_sum * (total_sum - subtree_sum)) % c_magic_divider;

            int parent = adj[v].back();

            if (present[parent])
            {
                subtree_sums[parent] += subtree_sum;
                --child_left[parent];
                if (child_left[parent] == 0)
                {
                    q.push(parent);
                }
            }
        }

        for (auto v : subtree_nodes)
        {
            present[v] = 0;
        }

        result.push_back(query_sum % c_magic_divider);
    }

    return result;
}

int main()
{
    int n, q;
    std::cin >> n >> q;

    std::vector<std::vector<int>> edges;
    for (int i = 0; i < n - 1; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        edges.push_back({u, v});
    }

    std::vector<std::vector<int>> queries;
    for (int i = 0; i < q; ++i)
    {
        int m;
        std::cin >> m;
        std::vector<int> vertices;
        vertices.resize(m);
        for (int j = 0; j != m; ++j)
        {
            std::cin >> vertices[j];
        }
        queries.push_back(std::move(vertices));
    }

    auto sums = calcSums(edges, queries);

    for (int i : sums)
    {
        std::cout << i << std::endl;
    }

    return 0;
}
