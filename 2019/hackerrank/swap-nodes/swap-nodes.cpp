#include <iostream>
#include <vector>
#include <memory>
#include <limits>
//#include <boost/range/adaptors.hpp>

using namespace std;

/*
 * Complete the swapNodes function below.
 */
struct Node
{
    explicit Node(int v) : value(v) {}

    int value = 0;
    int level = 0;
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
};

class Visitor
{
public:
    virtual void applyTo(Node& node)
    {
        if (node.left != nullptr)
        {
            applyTo(*node.left);
        }
        handle(node);
        if (node.right != nullptr)
        {
            applyTo(*node.right);
        }
    }

protected:
    virtual void handle(Node& node) = 0;
};

class Swapper : public Visitor
{
public:
    explicit Swapper(int level)
        : m_level(level)
    {}

    void applyTo(Node& node) override
    {
        if (node.left != nullptr)
        {
            applyTo(*node.left);
        }
        if (node.right != nullptr)
        {
            applyTo(*node.right);
        }
        handle(node);
    }
protected:
    void handle(Node& node) override
    {
        if (node.level % m_level == 0)
        {
            std::swap(node.left, node.right);
        }
    }
private:
    int m_level;
};

template <typename OutIterator>
class Collector : public Visitor
{
public:
    explicit Collector(OutIterator it)
        : m_it(it)
    {}
protected:
    void handle(Node& node) override
    {
        *m_it = node.value;
        ++m_it;
    }
private:
    OutIterator m_it;
};

template <typename Iterator> Collector<Iterator> makeCollector(Iterator it)
{
    return Collector<Iterator>(it);
}

std::shared_ptr<Node> makeTree(const std::vector<std::vector<int>>& indexes)
{
    std::vector<std::shared_ptr<Node>> nodes;
    nodes.reserve(indexes.size());
    for (int i = 0; i < indexes.size(); ++i)
    {
        nodes.emplace_back(std::make_shared<Node>(i + 1));
    }

    nodes[0]->level = 1;
    for (int i = 0; i < indexes.size(); ++i)
    {
        const auto& idxs = indexes[i];

        if (idxs[0] != -1)
        {
            nodes[i]->left = nodes[idxs[0] - 1];
            nodes[i]->left->level = nodes[i]->level + 1;
        }
        if (idxs[1] != -1)
        {
            nodes[i]->right = nodes[idxs[1] - 1];
            nodes[i]->right->level = nodes[i]->level + 1;
        }
    }

    return nodes[0];
}

vector<vector<int>> swapNodes(vector<vector<int>> indexes, vector<int> queries)
{
    /*
     * Write your code here.
     */
    auto root = makeTree(indexes);
    std::vector<std::vector<int>> result;

    for (const auto q : queries)
    {
        //std::cout << "applying query: " << q << std::endl;
        Swapper(q).applyTo(*root);
        std::vector<int> v;
        makeCollector(std::back_inserter(v)).applyTo(*root);

        //std::cout << boost::adaptors::format(v) << std::endl;

        result.push_back(v);
    }

    return result;
}

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<int>> indexes(n);
    for (int indexes_row_itr = 0; indexes_row_itr < n; indexes_row_itr++) {
        indexes[indexes_row_itr].resize(2);

        for (int indexes_column_itr = 0; indexes_column_itr < 2; indexes_column_itr++) {
            cin >> indexes[indexes_row_itr][indexes_column_itr];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int queries_count;
    cin >> queries_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<int> queries(queries_count);

    for (int queries_itr = 0; queries_itr < queries_count; queries_itr++) {
        int queries_item;
        cin >> queries_item;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        queries[queries_itr] = queries_item;
    }

    vector<vector<int>> result = swapNodes(indexes, queries);

    for (int result_row_itr = 0; result_row_itr < result.size(); result_row_itr++) {
        for (int result_column_itr = 0; result_column_itr < result[result_row_itr].size(); result_column_itr++) {
            std::cout << result[result_row_itr][result_column_itr];

            if (result_column_itr != result[result_row_itr].size() - 1) {
                std::cout << " ";
            }
        }

        if (result_row_itr != result.size() - 1) {
            std::cout << "\n";
        }
    }

    std::cout << "\n";

    return 0;
}
