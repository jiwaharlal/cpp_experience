#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

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

vector<int> solve(vector<vector<int>> tree, vector<vector<int>> queries) {
    vector<int> result;
    int last_vertex = std::accumulate(
        tree.begin(),
        tree.end(),
        0,
        [](auto m, const auto& branch)
            { return std::max(m, std::max(branch[0], branch[1])); });
    int tree_size = last_vertex + 1;
    AdjList adj(tree_size);
    for (const auto& edge : tree)
    {
        adj[edge[0]].push_back(edge[1]);
        adj[edge[1]].push_back(edge[0]);
    }

    std::vector<int> values(tree_size, 0);
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

    vector<int> result = solve(tree, queries);

    //for (int result_itr = 0; result_itr < result.size(); result_itr++) {
        //std::cout << result[result_itr];

        //if (result_itr != result.size() - 1) {
            //std::cout << "\n";
        //}
    //}
    std::cout << "done";

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
