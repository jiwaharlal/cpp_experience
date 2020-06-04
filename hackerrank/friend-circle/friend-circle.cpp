#include <bits/stdc++.h>
#include <map>

using namespace std;

class DisjointSet
{
private:
    using IntPair = std::pair<int, int>;
    using IntPairList = std::list<IntPair>;

public:
    bool contains(int v)
    {
        return m_element_map.find(v) != m_element_map.end();
    }

    void addSet(int v)
    {
        auto it_emplaced = m_sets.emplace(v, IntPairList{std::make_pair(v, v)});
        m_element_map.emplace(v, it_emplaced.first->second.begin());
    }

    const IntPairList& unite(int u, int v)
    {
        auto it1 = m_element_map.find(u);
        auto it2 = m_element_map.find(v);

        auto list_it1 = it1->second;
        auto list_it2 = it2->second;

        if (list_it1->second == list_it2->second)
        {
            // elements are already in the same list
            return m_sets[u];
        }

        auto set_it1 = m_sets.find(list_it1->second);
        auto set_it2 = m_sets.find(list_it2->second);

        auto sets = std::minmax(
                set_it1,
                set_it2,
                [](const auto& lhs, const auto& rhs)
                { return lhs->second.size() < rhs->second.size(); });

        auto smaller_list_id = sets.first->first;
        auto& smaller_list = sets.first->second;
        auto larger_list_id = sets.second->first;
        auto& larger_list = sets.second->second;

        for (auto& p : smaller_list)
        {
            p.second = larger_list_id;
        }

        larger_list.splice(
                larger_list.begin(), smaller_list, smaller_list.begin(), smaller_list.end());
        m_sets.erase(smaller_list_id);

        return larger_list;
    }

    const std::map<int, IntPairList>& sets() const
    {
        return m_sets;
    }

private:

    std::map<int, IntPairList> m_sets;
    std::map<int, IntPairList::iterator> m_element_map;
};

// Complete the maxCircle function below.
vector<int> maxCircle(vector<vector<int>> queries) {
    std::vector<int> result;
    result.reserve(queries.size());

    DisjointSet ds;
    for (const auto& q : queries)
    {
        for (const auto v : q)
        {
            if (!ds.contains(v))
            {
                ds.addSet(v);
            }
        }

        int new_size = ds.unite(q[0], q[1]).size();
        result.push_back(std::max(result.back(), new_size));
    }

    return result;
}

int main()
{
    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> queries(q);
    for (int i = 0; i < q; i++) {
        queries[i].resize(2);

        for (int j = 0; j < 2; j++) {
            cin >> queries[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<int> ans = maxCircle(queries);

    for (int i = 0; i < ans.size(); i++) {
        std::cout << ans[i];

        if (i != ans.size() - 1) {
            std::cout << "\n";
        }
    }

    std::cout << "\n";

    return 0;
}
