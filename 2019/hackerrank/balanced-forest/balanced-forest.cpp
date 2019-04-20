#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <map>

using namespace std;

vector<string> split_string(string);

// Complete the balancedForest function below.
using AdjList = std::vector<std::vector<int>>;

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

std::vector<int> calcWeights(const AdjList& tree, const std::vector<int>& vertex_weights)
{
    std::vector<int> stack;
    stack.reserve(tree.size());

    std::vector<int> weights(tree.size(), 0);

    std::vector<int> assign_stack;
    assign_stack.reserve(tree.size());
    std::queue<int> wait_queue;
    wait_queue.push(0);

    while (!wait_queue.empty())
    {
        auto cur = wait_queue.front();
        wait_queue.pop();
        assign_stack.push_back(cur);

        for (int i = 0, limit = tree[cur].size() - 1; i != limit; ++i)
        {
            auto c = tree[cur][i];
            wait_queue.push(c);
        }
    }

    while (!assign_stack.empty())
    {
        auto cur = assign_stack.back();
        assign_stack.pop_back();

        weights[cur] = vertex_weights[cur];
        for (int i = 0, limit = tree[cur].size() - 1; i != limit; ++i)
        {
            auto c = tree[cur][i];
            weights[cur] += weights[c];
        }
    }

    return weights;
}

//template <typename PairType>
//struct LessFirst
//{
    //bool operator ()(const PairType& lhs, const PairType& rhs) const
    //{
        //return lhs.first < rhs.first;
    //}

    //bool operator ()(const typename PairType::first_type& lhs, const PairType& rhs) const
    //{
        //return lhs < rhs.first;
    //}

    //bool operator ()(const PairType& lhs, const typename PairType::first_type& rhs) const
    //{
        //return lhs.first < rhs;
    //}
//};

bool isParentOf(int p, int c, const AdjList& tree)
{
    for ( ; c != 0; c = tree[c].back())
    {
        if (c == p)
        {
            return true;
        }
    }
    return false;
}

int balancedForest(vector<int> c, vector<vector<int>> edges)
{
    c.insert(c.begin(), 0);
    edges.push_back({0, 1});
    auto adj = buildGraph(edges);
    auto ranks = transformToTree(adj);
    auto weights = calcWeights(adj, c);

    std::multimap<int, int> vertex_by_weight;
    for (int i = 0; i < weights.size(); ++i)
    {
        vertex_by_weight.emplace(weights[i], i);
    }

    const auto third_it = vertex_by_weight.upper_bound(weights[0] / 3);
    const auto half_it = vertex_by_weight.upper_bound(weights[0] / 2);

    int first_min = [&]()
    {
        for (auto it = third_it; it != half_it; ++it)
        {
            auto v = it->second;
            if (it->first == std::next(it)->first)
            {
                return it->first * 3 - weights[0];
            }

            for (int pi = adj[it->second].back(); pi != 0; pi = adj[pi].back())
            {
                int updated_weight = weights[pi] - it->first;
                if (weights[0] - updated_weight == it->first * 2)
                {
                    return it->first - updated_weight;
                }
            }

            auto lightest = weights[0] - (it->first * 2);

            auto lightest_it = vertex_by_weight.find(lightest);
            if (lightest_it != vertex_by_weight.end() && !isParentOf(v, lightest_it->second, adj))
            {
                return weights[v] - lightest;
            }

            auto heaviest = lightest + it->first;
            auto heaviest_it = vertex_by_weight.find(heaviest);
            if (heaviest_it != vertex_by_weight.end() && isParentOf(v, heaviest_it->second, adj))
            {
                return weights[v] - lightest;
            }
        }
        return std::numeric_limits<int>::max();
    }();

    int second_min = [&]()
    {
        for (auto it = std::prev(third_it); it != std::prev(vertex_by_weight.begin()); --it)
        {
            for (int pi = adj[it->second].back(); pi != 0; pi = adj[pi].back())
            {
                int updated_weight = weights[pi] - it->first;
                if (weights[0] - it->first == updated_weight * 2)
                {
                    return updated_weight - it->first;
                }
            }
        }
        return std::numeric_limits<int>::max();
    }();

    auto total_min = std::min(first_min, second_min);
    if (total_min > weights[0])
    {
        return -1;
    }
    return total_min;
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

        string c_temp_temp;
        getline(cin, c_temp_temp);

        vector<string> c_temp = split_string(c_temp_temp);

        vector<int> c(n);

        for (int i = 0; i < n; i++) {
            int c_item = stoi(c_temp[i]);

            c[i] = c_item;
        }

        vector<vector<int>> edges(n - 1);
        for (int i = 0; i < n - 1; i++) {
            edges[i].resize(2);

            for (int j = 0; j < 2; j++) {
                cin >> edges[i][j];
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int result = balancedForest(c, edges);

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
