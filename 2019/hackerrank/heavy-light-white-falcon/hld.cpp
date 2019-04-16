#include "hld.hpp"

#include <algorithm>

void countChildrenPriv(const AdjList& tree, int idx, std::vector<int>& counts)
{
    const auto& children = tree[idx];
    counts[idx] = 1;
    for (int i = 0, last = children.size() - 1; i != last; ++i)
    {
        int c = children[i];
        countChildrenPriv(tree, c, counts);
        counts[idx] += counts[c];
    }
}

std::vector<int> countChildren(const AdjList& tree)
{
    std::vector<int> result(tree.size(), 0);
    countChildrenPriv(tree, 0, result);

    return result;
}

HLD hlDecompose(const AdjList& tree)
{
    HLD hld;
    hld.vertex_sequence.resize(tree.size());

    std::vector<int> child_count = countChildren(tree);

    std::vector<int> stack(1, 0);

    while (!stack.empty())
    {
        int cur = stack.back();
        stack.pop_back();
        hld.sequences.emplace_back(1, cur);

        while (true)
        {
            const auto& children = tree[cur];
            auto child_last = std::prev(children.end());
            const auto heavy_child_it = std::find_if(
                    children.begin(),
                    child_last,
                    [&](int c){ return child_count[c] * 2 >= child_count[cur]; });

            for (auto it = children.begin(); it != std::prev(children.end()); ++it)
            {
                if (it != heavy_child_it)
                {
                    stack.push_back(*it);
                }
            }

            if (heavy_child_it == child_last)
            {
                break;
            }

            cur = *heavy_child_it;
            hld.vertex_sequence[cur] =
                std::make_pair(hld.sequences.size() - 1, hld.sequences.back().size());
            hld.sequences.back().push_back(cur);
        }
    }

    return hld;
}
