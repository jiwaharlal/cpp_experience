#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

static int c_edge_weight = 6;

class Graph
{
public:
    Graph(int n)
        : m_adj(n)
    {
    }

    void add_edge(int u, int v)
    {
        m_adj[u].push_back(v);
        m_adj[v].push_back(u);
    }

    vector<int> shortest_reach(int start)
    {
        std::vector<int> distances(m_adj.size(), -1);
        std::queue<int> q;
        q.push(start);
        distances[start] = 0;

        while (!q.empty())
        {
            auto u = q.front();
            q.pop();

            for (auto v : m_adj[u])
            {
                if (distances[v] > 0)
                {
                    continue;
                }
                distances[v] = distances[u] + c_edge_weight;
                q.push(v);
            }
        }

        return distances;
    }

private:
    std::vector<std::vector<int>> m_adj;
};

int main() {
    int queries;
    cin >> queries;

    for (int t = 0; t < queries; t++) {

		int n, m;
        cin >> n;
        // Create a graph of size n where each edge weight is 6:
        Graph graph(n);
        cin >> m;
        // read and set edges
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            u--, v--;
            // add each edge to the graph
            graph.add_edge(u, v);
        }
		int startId;
        cin >> startId;
        startId--;
        // Find shortest reach from node s
        vector<int> distances = graph.shortest_reach(startId);

        for (int i = 0; i < distances.size(); i++) {
            if (i != startId) {
                cout << distances[i] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}

