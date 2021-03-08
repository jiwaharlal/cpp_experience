#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <variant>
#include <fstream>

struct TreeNode;
using NodePtr = std::shared_ptr<TreeNode>;

using NodeVariant = std::variant<NodePtr, int>;

struct TreeNode
{
    std::map<char, std::pair<std::string, NodeVariant>> children;
};

void addSuffix(const NodePtr& node, const std::string& suffix, int suffix_origin)
{
    auto first_letter = suffix.empty() ? '$' : suffix[0];
    auto same_branch_it = node->children.lower_bound(first_letter);

    if (same_branch_it->first != first_letter)
    {
        node->children.emplace_hint(same_branch_it, first_letter, std::make_pair(suffix + '$', NodeVariant{suffix_origin}));
        return;
    }

    auto& child_node = same_branch_it->second;
    auto& key_str = child_node.first;
    auto [substr_it, branch_key_it] = std::mismatch(suffix.begin(), suffix.end(), key_str.begin(), key_str.end());

    if (branch_key_it == key_str.end())
    {
        if (std::holds_alternative<NodePtr>(child_node.second))
        {
            addSuffix(std::get<NodePtr>(child_node.second), std::string{substr_it, suffix.end()}, suffix_origin);
        }
        else
        {
            std::cout << "Unexpected: not a NodePtr" << std::endl;
        }
        return;
    }

    // split node at branch_key_it and add
    auto intermediate_node = std::make_shared<TreeNode>();
    intermediate_node->children.emplace(*branch_key_it, child_node);
    child_node.first = std::string{branch_key_it, key_str.end()};
    same_branch_it->second = std::make_pair(
            std::string{key_str.begin(), branch_key_it},
            NodeVariant{intermediate_node});

    addSuffix(intermediate_node, std::string{substr_it, suffix.end()}, suffix_origin);
}

NodePtr buildSuffixTree(std::string str)
{
    //str.push_back('$');
    NodePtr root_node = std::make_shared<TreeNode>();

    for (auto it = str.begin(), last = std::prev(str.end()); it != last; ++it)
    {
        int substr_start = it - str.begin();
        std::string suffix(it, str.end());
        addSuffix(root_node, suffix, substr_start);
    }

    return root_node;
}

static const std::string tab("    ");

int printDotNode(std::ostream& out, const NodePtr& node, int index)
{
    auto child_idx = index + 1;
    for (const auto& child : node->children)
    {
        out << tab << 'n' << index << " -> n" << child_idx << " [label=\"" << child.second.first << "\"];\n";
        if (std::holds_alternative<NodePtr>(child.second.second))
        {
            child_idx = printDotNode(out, std::get<NodePtr>(child.second.second), child_idx);
        }
        else
        {
            ++child_idx;
        }
    }

    return child_idx;
}

void printDot(const NodePtr& root, const std::string& filename)
{
    std::ofstream fout(filename.c_str());
    fout << "digraph g {\n";
    fout << tab << "node [shape=circle];\n";
    printDotNode(fout, root, 0);
    fout << "}\n";
}

int main(int argc, char** argv)
{
    std::string str = "random string";
    auto tree = buildSuffixTree(str);

    printDot(tree, "tree.dot");

    return 0;
}
