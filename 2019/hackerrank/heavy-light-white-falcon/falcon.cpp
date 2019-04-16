#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

#include "TSegmentTreeIB.hpp"
#include "hld.hpp"

using namespace std;

vector<string> split_string(string);

// Complete the solve function below.
using AdjList = std::vector<std::vector<int>>;

std::vector<int> findPath(const AdjList& adj, int src, int dst)
{
    std::vector<int> result;
    std::vector<bool> visited(adj.size(), false);
    std::queue<int> q;
    q.push(src);
    std::vector<int> prev(adj.size(), 0);

    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        if (visited[cur])
        {
            continue;
        }
        visited[cur] = true;

        if (cur == dst) {
          for (; cur != src; cur = prev[cur]) {
            result.push_back(cur);
          }
          result.push_back(src);
          break;
        }

        for (auto next : adj[cur])
        {
            if (!visited[next])
            {
                q.push(next);
                prev[next] = cur;
            }
        }
    }

    return result;
}

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

vector<int> solve(vector<vector<int>> tree, vector<vector<int>> queries) {
    vector<int> result;

    auto adj = buildGraph(tree);

    std::vector<int> values(adj.size(), 0);
    for (const auto& q : queries)
    {
        if (q[0] == 1)
        {
            values[q[1]] = q[2];
        }
        else
        {
            auto path = findPath(adj, q[1], q[2]);
            int max_value = 0;
            for (int i : path)
            {
                max_value = std::max(max_value, values[i]);
            }
            result.push_back(max_value);
        }
    }

    return result;
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

std::vector<int> eulerWalk(const AdjList& adj)
{
    std::vector<int> visit_order;
    visit_order.reserve(adj.size());
    std::vector<int> last_direction(adj.size(), -1);

    for (int cur = 0; ; )
    {
        visit_order.push_back(cur);
        ++last_direction[cur];
        int next = adj[cur][last_direction[cur]];

        if (cur == next)
        {
            break;
        }
        cur = next;
    }

    return visit_order;
}

using MinSegmentTree = TSegmentTreeIB<int, std::less<int>>;

int minIdxInRange(const MinSegmentTree& st, int lo, int hi)
{
    return st.getTopIdxInRange(lo, hi);
}

int minIdxInRange(const std::vector<int>& v, int lo, int hi)
{
    return std::min_element(v.begin() + lo, v.begin() + hi) - v.begin();
}

std::vector<int> pathToParent(int child, int parent, const AdjList& adj)
{
    std::vector<int> result;
    for (int cur = child; cur != parent; cur = adj[cur].back())
    {
        result.push_back(cur);
    }
    result.push_back(parent);

    return result;
}

template <typename VisitedRanks>
std::vector<std::vector<int>> findTreePath(
        const AdjList& adj,
        const std::vector<int>& visit_order,
        const VisitedRanks& visited_ranks,
        const std::vector<int>& vertex_visits,
        int u,
        int v)
{
    std::vector<std::vector<int>> result;

    auto visits = std::minmax(vertex_visits[u], vertex_visits[v]);
    auto parent_visit_idx = minIdxInRange(visited_ranks, visits.first, visits.second + 1);
    int parent = visit_order[parent_visit_idx];

    for (int child : {u, v})
    {
        auto p = pathToParent(child, parent, adj);
        result.emplace_back(p.rbegin(), p.rend());
    }

    return result;
}

vector<int> solveST(vector<vector<int>> tree, vector<vector<int>> queries)
{
    vector<int> result;
    auto adj = buildGraph(tree);

    auto ranks = transformToTree(adj);

    auto visit_order = eulerWalk(adj);

    std::vector<int> last_visits(adj.size());
    std::vector<int> visited_ranks;
    visited_ranks.reserve(visit_order.size());
    for (int i = 0; i < visit_order.size(); ++i)
    {
        last_visits[visit_order[i]] = i;
        visited_ranks.push_back(ranks[visit_order[i]]);
    }

    std::vector<int> values(adj.size(), 0);
    MinSegmentTree visited_ranks_st(visited_ranks);
    for (const auto& q : queries)
    {
        if (q[0] == 1)
        {
            values[q[1]] = q[2];
        }
        else
        {
            auto path = findTreePath(adj, visit_order, visited_ranks_st, last_visits, q[1], q[2]);
            int max_value = 0;
            for (const auto& subpath : path)
            {
                for (int i : subpath)
                {
                    max_value = std::max(max_value, values[i]);
                }
            }
            result.push_back(max_value);
        }
    }

    return result;
}

AdjList getHldAdjacency(const HLD& hld, const AdjList& adj)
{
    AdjList result;
    result.reserve(hld.sequences.size());

    for (int seq_u = 0; seq_u < hld.sequences.size(); ++seq_u)
    {
        result.emplace_back();

        int size = 0;
        for (int u : hld.sequences[seq_u])
        {
            size += adj[u].size();
        }
        result.back().reserve(size);

        for (int u : hld.sequences[seq_u])
        {
            for (int v : adj[u])
            {
                int seq_v = hld.vertex_sequence[v].first;
                if (seq_v != seq_u)
                {
                    result.back().push_back(seq_v);
                }
            }
        }
    }

    return result;
}

using MaxSegmentTree = TSegmentTreeIB<int, std::greater<int>>;

vector<int> solveHLD(vector<vector<int>> tree, vector<vector<int>> queries)
{
    vector<int> result;

    auto adj = buildGraph(tree);
    auto ranks = transformToTree(adj);
    auto hld = hlDecompose(adj);

    auto hldAdj = getHldAdjacency(hld, adj);
    auto hdlRanks = transformToTree(hldAdj);

    auto visit_order = eulerWalk(hldAdj);

    std::vector<int> last_visits(hldAdj.size());
    std::vector<int> visited_ranks;
    visited_ranks.reserve(visit_order.size());
    for (int i = 0; i < visit_order.size(); ++i)
    {
        last_visits[visit_order[i]] = i;
        visited_ranks.push_back(ranks[visit_order[i]]);
    }

    MinSegmentTree visited_ranks_st(visited_ranks);

    std::vector<MaxSegmentTree> hld_sts;
    hld_sts.reserve(hldAdj.size());
    for (const auto& s : hld.sequences)
    {
        hld_sts.emplace_back(std::vector<int>(s.size(), 0));
    }

    // returns position in parent sequence from which child sequence originates
    auto parentVertexPosition = [&](int parent_seq_idx, int child_seq_idx)
    {
        int first_child_vertex = hld.sequences[child_seq_idx].front();
        int parent_vertex = adj[first_child_vertex].back();
        return hld.vertex_sequence[parent_vertex].second;
    };

    for (const auto& q : queries)
    {
        if (q[0] == 1)
        {
            auto seq_adr = hld.vertex_sequence[q[1]];
            hld_sts[seq_adr.first].update(seq_adr.second, q[2]);
        }
        else
        {
            std::vector<std::pair<int, int>> sequence_positions = {
                    hld.vertex_sequence[q[1]],
                    hld.vertex_sequence[q[2]]};

            auto path = findTreePath(
                    hldAdj,
                    visit_order,
                    visited_ranks_st,
                    last_visits,
                    sequence_positions[0].first,
                    sequence_positions[1].first);

            int max_value = 0;

            std::vector<int> root_connections(2);
            for (int i = 0; i < 2; ++i)
            {
                const auto& p = path[i];
                std::vector<int> his;
                his.reserve(p.size());

                for (int j = 0, last = p.size() - 1; j != last; ++j)
                {
                    his.push_back(parentVertexPosition(p[j], p[j + 1]));
                }
                his.push_back(sequence_positions[i].second);

                for (int j = 1; j != p.size(); ++j)
                {
                    auto seq_max = hld_sts[p[j]].getTopInRange(0, his[j] + 1);
                }

                root_connections[i] = his[0];
            }

            std::sort(root_connections.begin(), root_connections.end());
            int parent_max = hld_sts[path[0][0]].getTopInRange(
                    root_connections[0], root_connections[1] + 1);
            max_value = std::max(max_value, parent_max);

            result.push_back(max_value);
        }
    }

    return result;
}

int main()
{
    string nq_temp;
    getline(cin, nq_temp);

    vector<string> nq = split_string(nq_temp);

    int n = stoi(nq[0]);

    int q = stoi(nq[1]);

    vector<vector<int>> tree(n-1);
    for (int tree_row_itr = 0; tree_row_itr < n-1; tree_row_itr++) {
        tree[tree_row_itr].resize(2);

        for (int tree_column_itr = 0; tree_column_itr < 2; tree_column_itr++) {
            cin >> tree[tree_row_itr][tree_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<vector<int>> queries(q);
    for (int queries_row_itr = 0; queries_row_itr < q; queries_row_itr++) {
        queries[queries_row_itr].resize(3);

        for (int queries_column_itr = 0; queries_column_itr < 3; queries_column_itr++) {
            cin >> queries[queries_row_itr][queries_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<int> result = solveST(tree, queries);

    for (int result_itr = 0; result_itr < result.size(); result_itr++) {
        std::cout << result[result_itr];

        if (result_itr != result.size() - 1) {
            std::cout << "\n";
        }
    }
    //std::cout << "done";

    std::cout << "\n";

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
