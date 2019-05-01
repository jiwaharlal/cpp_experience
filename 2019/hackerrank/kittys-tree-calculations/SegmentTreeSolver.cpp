#include "SegmentTreeSolver.hpp"

#include <algorithm>
#include <cmath>
#include <list>
#include <queue>
#include <vector>

namespace
{

using AdjList = std::vector<std::vector<int>>;

static const long c_magic_divider = std::pow(10, 9) + 7;

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

} // unnamed namespace

SegmentTreeSolver::SegmentTreeSolver(const std::vector<std::vector<int>>& edges)
    : m_visited_ranks_st({})
{
    m_tree = buildGraph(edges);
    m_tree[0].push_back(1);
    m_tree[1].push_back(0);
    m_ranks = transformToTree(m_tree);
    m_visit_order = eulerWalk(m_tree);

    m_last_visits.resize(m_tree.size());
    std::vector<int> visited_ranks;
    visited_ranks.reserve(m_visit_order.size());
    for (int i = 0; i < m_visit_order.size(); ++i)
    {
        m_last_visits[m_visit_order[i]] = i;
        visited_ranks.push_back(m_ranks[m_visit_order[i]]);
    }

    m_visited_ranks_st = MinSegmentTree(visited_ranks);
}

int SegmentTreeSolver::getCommonParent(const std::vector<int>& vertices) const
{
    int min_visit = m_last_visits[vertices.front()];
    int max_visit = min_visit;

    for (auto v : vertices)
    {
        min_visit = std::min(min_visit, m_last_visits[v]);
        max_visit = std::max(max_visit, m_last_visits[v]);
    }

    return m_visit_order[m_visited_ranks_st.getTopIdxInRange(min_visit, max_visit + 1)];
}

/*
long SegmentTreeSolver::getSum(const std::vector<int>& query) const
{
    int root = getCommonParent(query);

    std::vector<long> subtree_sums(m_tree.size(), 0);
    std::for_each(query.begin(), query.end(), [&](int v){ subtree_sums[v] = v; });

    using RankNodePair = std::pair<int, int>;
    std::vector<bool> discovered(m_tree.size(), false);

    std::vector<RankNodePair> q_src;
    q_src.reserve(query.size());

    for (int v : query)
    {
        q_src.emplace_back(m_ranks[v], v);
        discovered[v] = true;
    }

    std::priority_queue<RankNodePair> q(std::less<RankNodePair>(), std::move(q_src));

    long query_sum = 0l;
    long total_sum = std::accumulate(query.begin(), query.end(), 0l, std::plus<long>());

    auto cur = q.top();
    q.pop();

    while (true)
    {
        int v = cur.second;
        if (v == root)
        {
            break;
        }

        long subtree_sum = subtree_sums[v] % c_magic_divider;
        query_sum += (subtree_sum * (total_sum - subtree_sum)) % c_magic_divider;

        int parent = m_tree[v].back();
        subtree_sums[parent] += subtree_sum;

        if (!discovered[parent])
        {
            discovered[parent] = true;
            int parent_rank = cur.first - 1;

            if (q.top().first <= parent_rank)
            {
                cur = std::make_pair(parent_rank, parent);
                continue;
            }
            else
            {
                q.emplace(parent_rank, parent);
            }
        }

        if (q.empty())
        {
            break;
        }

        cur = q.top();
        q.pop();
    }

    return query_sum % c_magic_divider;
}*/

long SegmentTreeSolver::getSum(const std::vector<int>& query) const
{
    int root = getCommonParent(query);

    std::vector<long> subtree_sums(m_tree.size(), 0);
    std::for_each(query.begin(), query.end(), [&](int v){ subtree_sums[v] = v; });

    using RankNodePair = std::pair<int, int>;
    std::vector<bool> discovered(m_tree.size(), false);

    std::vector<RankNodePair> q_src;
    q_src.reserve(query.size());
    //std::list<RankNodePair> q;

    for (int v : query)
    {
        q_src.emplace_back(m_ranks[v], v);
        //q.emplace_back(m_ranks[v], v);
        discovered[v] = true;
    }

    std::sort(q_src.begin(), q_src.end());
    std::list<RankNodePair> q(q_src.rbegin(), q_src.rend());
    //q.sort(std::greater<RankNodePair>());

    long query_sum = 0l;
    long total_sum = std::accumulate(query.begin(), query.end(), 0l, std::plus<long>());

    auto cur_it = q.begin();
    auto lower_rank_it = std::next(cur_it);

    while (true)
    {
        for ( ; lower_rank_it != q.end() && lower_rank_it->first == cur_it->first; ++lower_rank_it);

        int v = cur_it->second;
        if (v == root)
        {
            break;
        }

        long subtree_sum = subtree_sums[v] % c_magic_divider;
        query_sum += (subtree_sum * (total_sum - subtree_sum)) % c_magic_divider;

        int parent = m_tree[v].back();
        subtree_sums[parent] += subtree_sum;

        auto next_it = std::next(cur_it);
        if (!discovered[parent])
        {
            discovered[parent] = true;
            --(cur_it->first);
            cur_it->second = parent;

            if (next_it->first <= cur_it->first)
            {
                continue;
            }

            q.splice(lower_rank_it, q, cur_it);
        }
        cur_it = next_it;
    }

    return query_sum % c_magic_divider;
}
