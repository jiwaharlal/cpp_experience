#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the findShortest function below.

using AdjList = std::vector<std::vector<int>>;

AdjList buildGraph(int num_nodes, const std::vector<int>& from, const std::vector<int>& to)
{
    AdjList adj(num_nodes);
    for (std::size_t i = 0; i < from.size(); ++i)
    {
        int u = from[i] - 1;
        int v = to[i] - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    return adj;
}

/*
 * For the unweighted graph, <name>:
 *
 * 1. The number of nodes is <name>_nodes.
 * 2. The number of edges is <name>_edges.
 * 3. An edge exists between <name>_from[i] to <name>_to[i].
 *
 */
int findShortest(int graph_nodes, vector<int> graph_from, vector<int> graph_to, vector<long> ids, int val)
{
    auto adj = buildGraph(graph_nodes, graph_from, graph_to);

    std::vector<std::pair<int, int>> distances;
    distances.reserve(graph_nodes);
    std::list<int> q;
    for (int i = 0; i < ids.size(); ++i)
    {
        if (ids[i] == val)
        {
            distances.emplace_back(i, 0);
            q.push_back(i);
        }
        else
        {
            distances.emplace_back(-1, std::numeric_limits<int>::max());
        }
    }

    while (!q.empty())
    {
        auto cur = q.front();
        q.pop_front();

        for (auto v : adj[cur])
        {
            if (distances[v].first == distances[cur].first)
            {
                continue;
            }

            if (distances[v].second != std::numeric_limits<int>::max())
            {
                return distances[v].second + distances[cur].second + 1;
            }

            distances[v] = std::make_pair(distances[cur].first, distances[cur].second + 1);
            q.push_back(v);
        }
    }

    return -1;
}

int main()
{
    int graph_nodes;
    int graph_edges;

    cin >> graph_nodes >> graph_edges;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<int> graph_from(graph_edges);
    vector<int> graph_to(graph_edges);

    for (int i = 0; i < graph_edges; i++) {
        cin >> graph_from[i] >> graph_to[i];
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string ids_temp_temp;
    getline(cin, ids_temp_temp);

    vector<string> ids_temp = split_string(ids_temp_temp);

    vector<long> ids(graph_nodes);

    for (int i = 0; i < graph_nodes; i++) {
        long ids_item = stol(ids_temp[i]);

        ids[i] = ids_item;
    }

    int val;
    cin >> val;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int ans = findShortest(graph_nodes, graph_from, graph_to, ids, val);

    std::cout << ans << "\n";

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
