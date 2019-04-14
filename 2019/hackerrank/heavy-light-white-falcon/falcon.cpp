#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

vector<string> split_string(string);

// Complete the solve function below.
using AdjList = std::vector<std::vector<int>>;

//int maxIdx(int idx_1, int idx_2, const std::vector<int>& v)
//{
    //if (v[idx_1] < v[idx_2])
    //{
        //return idx_2;
    //}
    //return idx_1;
//}

//int levelBase(int level)
//{
    //return (1 << level) - 1;
//}

//class MaxHeap
//{
//public:
    //explicit MaxHeap(const std::vector<int>& arr)
        //: m_original_size(arr.size())
    //{
        //int levels = 1;
        //for (int size = 1; size < arr.size(); size <<= 1, ++levels);

        //m_max_heap.resize(levelBase(levels), 0);

        //m_data.resize(1 << (levels - 1), std::numeric_limits<int>::min());
        //std::copy(arr.begin(), arr.end(), m_data.begin());

        //for (int i = 0, j = levelBase(levels - 1); i < m_data.size(); ++i, ++j)
        //{
            //m_max_heap[j] = i;
        //}

        //for (int i = levelBase(levels - 1); i >= 0; --i)
        //{
            //int child_idx_1 = i * 2 + 1;
            //int child_idx_2 = child_idx_1 + 1;
            //m_max_heap[i] = maxIdx(m_max_heap[child_idx_1], m_max_heap[child_idx_2], m_data);
        //}

        //m_levels = levels;
    //}

    //void update(int idx, int value)
    //{
        //if (idx < 0 || idx >= m_original_size)
        //{
            //throw std::out_of_range("Index out or original size");
        //}

        //int idx_base = levelBase(m_levels - 1);
        //int child_idx = idx_base + idx;

        //for (int l = m_levels - 2; l >= 0; --l)
        //{
            //int parent_idx = (child_idx - 1) / 2;
            //int child_idx_2 = parent_idx * 2 + 1 + child_idx % 2;

            //int new_max_idx = maxIdx(m_max_heap[child_idx], m_max_heap[child_idx_2], m_data);
            //if (new_max_idx == m_max_heap[parent_idx])
            //{
                //break;
            //}

            //m_max_heap[parent_idx] = new_max_idx;
            //child_idx = parent_idx;
        //}
    //}

    //int getMaxInRange(int lo, int hi)
    //{
        //int max_idx = getMaxIdxInRange(0, 0, lo, hi);
        //return max_idx;
    //}

    //int getMaxIdxInRange(int level, int idx, int lo, int hi)
    //{
        //int full_idx = levelBase(level) + idx;
        //int idx_max = m_max_heap[full_idx];

        //if (isInRange(idx_max, std::make_pair(lo, hi)))
        //{
            //return idx_max;
        //}

        //int left_child_idx = idx * 2;
        //int right_child_idx = left_child_idx + 1;

        //int level_diff = m_levels - level;
        //int child_lo = idx << level_diff;
        //int child_mid = child_lo + (1 << (level_diff / 2));
        //int child_hi = (idx + 1) << level_diff;

        //if (isInRange(std::make_pair(lo, hi), std::make_pair(child_lo, child_mid)))
        //{
            //return getMaxIdxInRange(level + 1, left_child_idx, lo, hi);
        //}

        //if (isInRange(std::make_pair(lo, hi), std::make_pair(child_mid, child_hi)))
        //{
            //return getMaxIdxInRange(level + 1, right_child_idx, lo, hi);
        //}

        //int left_idx = getMaxIdxInRange(level + 1, left_child_idx, lo, child_mid);
        //int right_idx = getMaxIdxInRange(level + 1, right_child_idx, child_mid, hi);

        //return maxIdx(left_idx, right_idx, m_data);
    //}

//private:
    //int m_levels;
    //int m_original_size;
    //std::vector<int> m_data;
    //std::vector<int> m_max_heap;
//};

//std::vector<int> makeMaxHeap(const std::vector<int>& arr)
//{
    //int size_2 = 1;
    //for (; size_2 < arr.size(); size_2 <<= 1);
    //std::vector<int> result;
    //result.reserve(size_2 * 2);
    //for (int i = 0; i < size_2; ++i)
    //{
        //result.push_back(i);
    //}

    //std::vector<int> v(size_2, std::numeric_limits<int>::min());
    //std::copy(arr.begin(), arr.end(), v.begin());

    //int base = 0;
    //for (int level = 1; ; ++level)
    //{
        //int limit = size_2 << level;
        //for (int i = 0; i < limit; ++i)
        //{
            //int idx_1 = result[base + i * 2];
            //int idx_2 = result[base + i * 2 + 1];

            //auto p1 = std::make_pair(v[idx_1], idx_1);
            //auto p2 = std::make_pair(v[idx_2], idx_2);
            //auto idx = std::max(p1, p2).second;

            //result.push_back(idx);
        //}

        //if (limit == 1)
        //{
            //break;
        //}
        //base += limit;
    //}

    //return result;
//}

//struct HeapNode
//{
    //int lo_idx;
    //int hi_idx;
    //int max_idx;
    //HeapNode* left;
    //HeapNode* right;
//};

//class MaxHeap
//{
//public:
    //explicit MaxHeap(const std::vector<int> data)
        //: m_data(data)
        //, m_max_heap(makeMaxHeap(data))
    //{
        //int levels = 1;
        //for (int i = 1; i < data.size(); i <<= 1, ++levels);
        //m_levels = levels;
    //}

    //const std::vector<int>& data() const { return m_data; }

    //void update(int idx, int value)
    //{
        //m_data[idx] = value;

        //int low_offset = 0;
        //int hi_offset = 1 << m_levels;

        //for (int level = 1; level < m_levels; ++level)
        //{

        //}
    //}

    //int getMaxInRange(int lo, int hi)
    //{
        //return getMaxInRangePriv(m_levels - 1, 0, lo, hi);
    //}

//private:
    //int getMaxInRangePriv(int level, int idx, int lo, int hi)
    //{
        //int base = 0;
        //for (int l = 0; l < level; ++l)
        //{
            //base <<= 1;
            //base += 1;
        //}
        //for (int l = level; l < m_levels; ++l)
        //{
            //base <<= 1;
        //}

        //int max_idx = m_max_heap[idx];
        //if (max_idx <= hi && max_idx >= lo)
        //{
            //return max_idx;
        //}

        //int child_begin = idx << level;
        //int child_mid = ((idx << 1) + 1) << (level - 1);
        //int child_end = (idx + 1) << level;

        //if (hi < child_mid)
        //{
            //return getMaxInRangePriv(level - 1, idx << 1, lo, hi);
        //}

        //if (lo >= child_mid)
        //{
            //return getMaxInRangePriv(level - 1, (idx << 1) + 1, lo, hi);
        //}

        //int idx1 = getMaxInRangePriv(level - 1, idx << 1, lo, hi);
        //int idx2 = getMaxInRangePriv(level - 1, (idx << 1) + 1, lo, hi);

        //auto p1 = std::make_pair(m_data[idx1], idx1);
        //auto p2 = std::make_pair(m_data[idx2], idx2);

        //return std::max(p1, p2).second;
    //}

//private: // fields
    //std::vector<int> m_data;
    //std::vector<int> m_max_heap;
    //int m_levels;
//};


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
