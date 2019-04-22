#include <algorithm>
#include <iostream>
#include <vector>

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

AdjList

int jennysSubtrees(int n, int r, vector<vector<int>> edges)
{
    auto adj = buildGraph(edges);

    std::vector<> signatures;
    for (int v = 0; v < edges.size(); ++v)
    {
        auto subtree = getSubtree(adj, v, r);
        signatures.push_back(getSignature(subtee));
    }

    std::sort(signatures.begin(), signatures.end());

    return std::unique(signatures.begin(), signatures.end()) - signatures.begin();
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

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

    fout << result << "\n";

    fout.close();

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
