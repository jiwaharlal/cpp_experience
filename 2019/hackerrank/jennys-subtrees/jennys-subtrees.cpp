#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<string> split_string(string);

/*
 * Complete the jennysSubtrees function below.
 */
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

AdjList getSubtree(const AdjList& adj, int center, int radius)
{
    AdjList result(adj.size());

    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(center, 0));
    std::vector<bool> visited(adj.size(), false);

    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();
        visited[cur.first] = true;

        if (cur.second >= radius)
        {
            continue;
        }

        for (auto next : adj[cur.first])
        {
            if (visited[next])
            {
                continue;
            }

            result[cur.first].push_back(next);
            result[next].push_back(cur.first);

            q.push(std::make_pair(next, cur.second + 1));
        }
    }

    return result;
}

std::vector<int> getCenters(const AdjList& adj)
{
    std::vector<int> result;

    std::vector<int> neighbors_left;
    neighbors_left.reserve(adj.size());

    for (int i = 0; i < static_cast<int>(adj.size()); ++i)
    {
        neighbors_left.push_back(adj[i].size());
    }

    std::vector<std::pair<int, int>> centers;
    centers.emplace_back(0, -1);

    std::queue<std::pair<int, int>> q;
    for (int v = 1; v != static_cast<int>(adj.size()); ++v)
    {
        if (adj[v].size() == 1)
        {
            q.push(std::make_pair(v, 0));
        }
    }

    while (!q.empty())
    {
        auto cur = q.front();
        q.pop();

        if (cur.second > centers[0].second)
        {
            centers.clear();
        }
        centers.push_back(cur);

        for (auto next : adj[cur.first])
        {
            --neighbors_left[next];
            if (neighbors_left[next] == 1)
            {
                q.push(std::make_pair(next, cur.second + 1));
            }
        }
    }

    std::transform(
            centers.begin(),
            centers.end(),
            std::back_inserter(result),
            [](const auto& p){ return p.first; });

    return result;
}

using Signature = std::vector<std::pair<int, int>>;

std::queue<Signature> empty_signatures;

Signature getSignature()
{
    Signature s;

    if (!empty_signatures.empty())
    {
        s.swap(empty_signatures.front());
        empty_signatures.pop();
    }

    return s;
}

void releaseSignature(Signature&& s)
{
    s.clear();
    empty_signatures.push(std::move(s));
}

//bool cmpSignatures(const Signature& lhs, const Signature& rhs)
//{
    //return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
//}

Signature getSignaturePriv(const AdjList& adj, int v, int level, int exclude_vertex)
{
    Signature s = getSignature();

    std::vector<Signature> sub_sigs;
    sub_sigs.reserve(adj[v].size());

    for (int c : adj[v])
    {
        if (c == exclude_vertex)
        {
            continue;
        }
        sub_sigs.emplace_back(getSignaturePriv(adj, c, level + 1, v));
    }

    std::sort(sub_sigs.begin(), sub_sigs.end());

    s.reserve(std::accumulate(
                  sub_sigs.begin(),
                  sub_sigs.end(),
                  1,
                  [](int s, const auto& v){ return s + v.size(); }));
    s.emplace_back(level, adj[v].size());

    for (int i = 0; i < static_cast<int>(sub_sigs.size()); ++i)
    {
        const auto& ss = sub_sigs[i];
        std::copy(ss.begin(), ss.end(), std::back_inserter(s));
        releaseSignature(std::move(sub_sigs[i]));
    }

    return s;
}

Signature getSignature(const AdjList& adj)
{
    Signature result;
    auto centers = getCenters(adj);

    if (centers.size() == 1)
    {
        result = getSignaturePriv(adj, centers[0], 0, 0);
    }
    else
    {
        std::vector<Signature> sub_sigs = {
            getSignaturePriv(adj, centers[0], 0, centers[1]),
            getSignaturePriv(adj, centers[1], 0, centers[0])};

        std::sort(sub_sigs.begin(), sub_sigs.end());
        for (const auto& ss : sub_sigs)
        {
            std::copy(ss.begin(), ss.end(), std::back_inserter(result));
        }
    }

    return result;
}

int jennysSubtrees(int n, int r, vector<vector<int>> edges)
{
    (void) n;
    auto adj = buildGraph(edges);

    std::vector<Signature> signatures;
    signatures.reserve(adj.size());
    for (int v = 1; v < static_cast<int>(adj.size()); ++v)
    {
        auto subtree = getSubtree(adj, v, r);
        signatures.push_back(getSignature(subtree));
    }

    std::sort(signatures.begin(), signatures.end());
    auto it = std::unique(signatures.begin(), signatures.end());
    auto unique_count = it - signatures.begin();

    return unique_count;
}

int main()
{
    //ofstream fout(getenv("OUTPUT_PATH"));

    string nr_temp;
    getline(cin, nr_temp);

    vector<string> nr = split_string(nr_temp);

    int n = stoi(nr[0]);

    int r = stoi(nr[1]);

    vector<vector<int>> edges(n-1);
    for (int edges_row_itr = 0; edges_row_itr < n-1; edges_row_itr++) {
        edges[edges_row_itr].resize(2);

        for (int edges_column_itr = 0; edges_column_itr < 2; edges_column_itr++) {
            cin >> edges[edges_row_itr][edges_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = jennysSubtrees(n, r, edges);

    std::cout << result << "\n";

    //fout.close();

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
