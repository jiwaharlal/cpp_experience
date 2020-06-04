#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

template <typename T>
T& other(std::pair<T, T>& p, const T& value)
{
    return value == p.first ? p.second : p.first;
}

template <typename T>
const T& other(const std::pair<T, T>& p, const T& value)
{
    return value == p.first ? p.second : p.first;
}

template <typename T, typename CollectionType>
const T& other(CollectionType& s, const T& value)
{
    return value == *s.begin() ? *std::next(s.begin()) : *s.begin();
}

using AdjWeightList = std::vector<std::vector<std::pair<int, int>>>;

AdjWeightList buildGraph(const std::vector<std::vector<int>>& roads)
{
    auto max_vertex = std::accumulate(
            roads.begin(),
            roads.end(),
            0,
            [](int max_index, const auto& road)
            { return std::max(max_index, std::max(road[0], road[1])); });

    AdjWeightList adj(max_vertex + 1);

    for (const auto& road : roads)
    {
        adj[road[0]].emplace_back(road[1], road[2]);
        adj[road[1]].emplace_back(road[0], road[2]);
    }

    return adj;
}

void bringToFront(std::vector<std::pair<int, int>>& pairs, int first_value)
{
    auto it = std::find_if(
            pairs.begin(),
            pairs.end(),
            [first_value](const std::pair<int, int>& p){ return p.first == first_value; });

    if (it == pairs.end())
    {
        throw std::runtime_error("first_value not found int pairs list");
    }

    std::swap(*pairs.begin(), *it);
}

void transformToTree(AdjWeightList& adj, int root)
{
    adj[root].emplace(adj[root].begin(), root, 0);
    std::vector<int> stack;
    stack.push_back(root);

    while (!stack.empty())
    {
        auto u = stack.back();
        stack.pop_back();

        for (auto it = std::next(adj[u].begin()); it != adj[u].end(); ++it)
        {
            int v = it->first;
            auto& adj_list = adj[v];
            bringToFront(adj_list, u);

            stack.push_back(v);
        }
    }
}

std::vector<int> findLeaves(const AdjWeightList& tree, int root)
{
    std::vector<int> result;
    for (int u = 0; u < tree.size(); ++u)
    {
        if (u == root)
        {
            continue;
        }

        if (tree[u].size() == 1u)
        {
            result.push_back(u);
        }
    }

    return result;
}

AdjWeightList findSubtree(const AdjWeightList& adj, int root, const std::vector<bool>& required)
{
    AdjWeightList result(adj.size());
    auto leaves = findLeaves(adj, root);

    std::vector<bool> accepted(adj.size(), false);
    accepted[root] = true;

    for (auto u : leaves)
    {
        bool is_accepted = required[u];
        while (true)
        {
            if (accepted[u])
            {
                break;
            }
            accepted[u] = is_accepted;

            u = adj[u].front().first;
            is_accepted |= required[u];
        }
    }

    for (int u = 0; u < adj.size(); ++u)
    {
        if (!accepted[u])
        {
            continue;
        }

        result[u].reserve(adj[u].size());

        for (auto v : adj[u])
        {
            if (accepted[v.first])
            {
                result[u].push_back(v);
            }
        }
    }

    return result;
}

using EdgeInfo = std::pair<int, std::pair<int, int>>;

EdgeInfo reduceNode(AdjWeightList& adj, int u)
{
    auto min_weight = std::min(adj[u][0].second, adj[u][1].second);
    auto new_edge = std::minmax(adj[u][0].first, adj[u][1].first);

    for (const auto& v_pair : adj[u])
    {
        int v = v_pair.first;
        auto it = find_if(
                adj[v].begin(),
                adj[v].end(),
                [u](const auto& p){ return u == p.first; });
        *it = std::make_pair(other(adj[u], v_pair).first, min_weight);
    }

    adj[u].clear();

    return std::make_pair(min_weight, new_edge);
}

AdjWeightList reduceGraph(const AdjWeightList& adj, const std::vector<int>& machines)
{
    AdjWeightList result(adj.size());
    result[machines[0]].emplace_back(machines[0], 0);

    std::vector<bool> is_machine(adj.size(), false);
    for (int machine : machines)
    {
        is_machine[machine] = true;
    }

    auto sub_adj = findSubtree(adj, machines[0], is_machine);

    for (int u = 0; u < sub_adj.size(); ++u)
    {
        if (!is_machine[u] && sub_adj[u].size() == 2)
        {
            reduceNode(sub_adj, u);
        }
    }

    return sub_adj;
}

using EdgeSet = std::set<EdgeInfo>;

EdgeSet transformToEdgeSet(const AdjWeightList& adj)
{
    EdgeSet result;

    for (int u = 0; u < adj.size(); ++u)
    {
        for (const auto& link : adj[u])
        {
            auto node_pair = std::minmax(u, link.first);
            result.emplace(link.second, node_pair);
        }
    }

    return result;
}

// Complete the minTime function below.
int minTime(vector<vector<int>> roads, vector<int> machines)
{
    if (machines.size() < 2)
    {
        return 0;
    }

    auto full_adj = buildGraph(roads);
    transformToTree(full_adj, machines[0]);
    auto adj = reduceGraph(full_adj, machines);

    std::vector<bool> is_machine(adj.size(), false);
    for (int machine : machines)
    {
        is_machine[machine] = true;
    }

    auto edges = transformToEdgeSet(adj);
    long sum = 0;

    while (!edges.empty())
    {
        const auto edge = *edges.begin();
        edges.erase(edges.begin());
        sum += edge.first;

        const auto& nodes = edge.second;

        for (int u : {nodes.first, nodes.second})
        {
            adj[u].erase(std::remove_if(
                        adj[u].begin(), adj[u].end(),
                        [&](const auto& p){ return p.first == other(nodes, u); }), adj[u].end());

            if (adj[u].size() > 2 || is_machine[u])
            {
                continue;
            }

            for (auto v_pair : adj[u])
            {
                auto edge_to_erase = std::make_pair(v_pair.second, std::minmax(v_pair.first, u));
                edges.erase(edge_to_erase);
            }

            auto new_edge = reduceNode(adj, u);
            edges.insert(new_edge);
        }
    }

    return sum;
}

#ifndef TESTING

int main()
{
    string nk_temp;
    getline(cin, nk_temp);

    vector<string> nk = split_string(nk_temp);

    int n = stoi(nk[0]);

    int k = stoi(nk[1]);

    vector<vector<int>> roads(n - 1);
    for (int i = 0; i < n - 1; i++) {
        roads[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> roads[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<int> machines(k);

    for (int i = 0; i < k; i++) {
        int machines_item;
        cin >> machines_item;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        machines[i] = machines_item;
    }

    int result = minTime(roads, machines);

    std::cout << result << "\n";

    return 0;
}

#endif // TESTING

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
