#pragma once

#include <vector>
#include <iterator>

template <typename T>
class TAvlSet;

template <typename T>
struct TAvlNode
{
    using NodeType = TAvlNode<T>;

    T val;
    NodeType* left;
    NodeType* right;

    int ht;
    int child_count = 0;
};

template <typename T>
struct TAvlIterator
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using node = TAvlNode<T>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

public:
    TAvlIterator(const std::vector<node*>& node_seq)
        : m_node_seq(node_seq)
    {}

    TAvlIterator& operator ++()
    {
        if (m_node_seq.back()->right == nullptr)
        {
            auto prev = m_node_seq.back();
            m_node_seq.pop_back();

            while (!m_node_seq.empty() && prev == m_node_seq.back()->right)
            {
                prev = m_node_seq.back();
                m_node_seq.pop_back();
            }

            if (m_node_seq.empty())
            {
                // end reached
                m_node_seq.push_back(prev); // save root node
                m_node_seq.push_back(nullptr);
            }

            return *this;
        }
        else
        {
            m_node_seq.push_back(m_node_seq.back()->right);
            while (m_node_seq.back()->left)
            {
                m_node_seq.push_back(m_node_seq.back()->left);
            }
            return *this;
        }
    }

    TAvlIterator& operator --()
    {
        // special case is the end()
        if (m_node_seq.size() == 2 && m_node_seq.back() == nullptr)
        {
            m_node_seq.pop_back();
            while (true)
            {
                auto next = m_node_seq.back()->right;
                if (next == nullptr)
                {
                    return *this;
                }
                m_node_seq.push_back(next);
            }
        }

        if (m_node_seq.back()->left == nullptr)
        {
            auto prev = m_node_seq.back();
            m_node_seq.pop_back();

            while (!m_node_seq.empty() && prev == m_node_seq.back()->left)
            {
                prev = m_node_seq.back();
                m_node_seq.pop_back();
            }

            return *this;
        }
        else
        {
            m_node_seq.push_back(m_node_seq.back()->left);
            while (m_node_seq.back()->right)
            {
                m_node_seq.push_back(m_node_seq.back()->right);
            }
            return *this;
        }
    }

    bool operator !=(const TAvlIterator<T>& other) const
    {
        return !operator ==(other);
    }

    bool operator ==(const TAvlIterator<T>& other) const
    {
        bool both_empty = m_node_seq.empty() && other.m_node_seq.empty();
        bool both_non_empty = !m_node_seq.empty() && !other.m_node_seq.empty();

        return both_empty || (both_non_empty && m_node_seq.back() == other.m_node_seq.back());
    }

    reference operator *()
    {
        return m_node_seq.back()->val;
    }

    const node& getNode() const
    {
        return m_node_seq.top();
    }

private:
    std::vector<node*> m_node_seq;

    friend class TAvlSet<T>;
};

template <typename T>
std::size_t treeNodeCount(const TAvlNode<T>* root)
{
    return root ? root->child_count + 1 : 0;
}

template <typename T>
class TAvlSet
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using node = TAvlNode<T>;
    using iterator = TAvlIterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

public:
    //TAvlSet() {}
    ~TAvlSet()
    {
        deleteWithChildren(m_root);
    }

    void insert(const T& value)
    {
        m_root = insert(m_root, value);
    }

    template <typename IterType>
    void insert(IterType begin, IterType end)
    {
        for (; begin != end; ++begin)
        {
            insert(*begin);
        }
    }

    std::size_t size() const
    {
        return treeNodeCount(m_root);
    }

    iterator begin() const
    {
        // special case for empty set
        std::vector<node*> sequence;
        if (m_root == nullptr)
        {
            sequence.push_back(nullptr);
        }
        for (auto n = m_root; n; sequence.push_back(n), n = n->left);
        return iterator(sequence);
    }

    iterator end() const
    {
        return iterator({m_root, nullptr});
    }

    reverse_iterator rbegin() const
    {
        return reverse_iterator{end()};
    }

    reverse_iterator rend() const
    {
        return reverse_iterator{begin()};
    }

    template <typename U>
    iterator lower_bound(const U& val) const
    {
        std::vector<node*> node_stack;
        if (m_root == nullptr)
        {
            node_stack.push_back(m_root);
            return iterator(node_stack);
        }

        node* cur_node = m_root;
        while (cur_node != nullptr)
        {
            node_stack.push_back(cur_node);
            if (val < cur_node->val)
            {
                cur_node = cur_node->left;
            }
            else if (cur_node->val < val)
            {
                cur_node = cur_node->right;
            }
            else
            {
                break;
            }
        }
        return iterator(node_stack);
    }

    template <typename U>
    iterator upper_bound(const U& val) const
    {
        std::vector<node*> node_stack;
        if (m_root == nullptr)
        {
            node_stack.push_back(m_root);
            return iterator(node_stack);
        }

        node* cur_node = m_root;
        while (cur_node != nullptr)
        {
            node_stack.push_back(cur_node);
            if (val < cur_node->val)
            {
                cur_node = cur_node->left;
            }
            else
            {
                cur_node = cur_node->right;
            }
        }

        while (!node_stack.empty() && !(val < node_stack.back()->val))
        {
            node_stack.pop_back();
        }
        if (node_stack.empty())
        {
            node_stack.push_back(nullptr);
        }

        return iterator(node_stack);
    }

    int count_before(const iterator& it) const
    {
        auto cur_node_it = it.m_node_seq.begin();
        auto next_node_it = std::next(cur_node_it);

        // special case of end()
        //if (it.m_node_seq.size() == 2 && it.m_node_seq.back() == nullptr)
        if (it.m_node_seq.back() == nullptr)
        {
            return treeNodeCount(m_root);
        }

        int total = 0;

        while (true)
        {
            const node* cur_node = *cur_node_it;
            if (next_node_it == it.m_node_seq.end())
            {
                return total + treeNodeCount(cur_node->left);
            }
            const node* next_node = *next_node_it;

            if (next_node == cur_node->right)
            {
                total += treeNodeCount(cur_node->left) + 1;
            }

            ++cur_node_it;
            ++next_node_it;
        }
    }

    void swap(TAvlSet<T>& other)
    {
        std::swap(m_root, other.m_root);
    }

private:
    void deleteWithChildren(node* n)
    {
        if (n == nullptr)
        {
            return;
        }

        deleteWithChildren(n->left);
        deleteWithChildren(n->right);
        delete n;
    }

    node * insert(node * root, const_reference val)
    {
        if (root == nullptr)
        {
            return new node{val, nullptr, nullptr, 0, 0};
        }

        if (val < root->val)
        {
            root->left = insert(root->left, val);
        }
        else
        {
            root->right = insert(root->right, val);
        }

        ++root->child_count;
        updateHeight(root);

        auto new_root = rebalanceIfNeeded(root);

        return new_root;
    }

    node* rotateLeft(node* top)
    {
        auto new_top = top->right;
        auto new_left = top;
        new_left->right = new_top->left;
        new_top->left = new_left;
        updateHeight(new_left);
        updateHeight(new_top);

        updateChildCount(new_left);
        updateChildCount(new_top);

        return new_top;
    }

    node* rotateRight(node* top)
    {
        auto new_top = top->left;
        auto new_right = top;
        new_right->left = new_top->right;
        new_top->right = new_right;
        updateHeight(new_right);
        updateHeight(new_top);

        updateChildCount(new_right);
        updateChildCount(new_top);

        return new_top;
    }

    node* rebalanceIfNeeded(node* root)
    {
        int right_ht = root->right ? root->right->ht : -1;
        int left_ht = root->left ? root->left->ht : -1;

        if (left_ht > right_ht + 1)
        {
            // right rotation group
            int left_left_ht = root->left->left ? root->left->left->ht : -1;
            int left_right_ht = root->left->right ? root->left->right->ht : -1;

            if (left_left_ht >= left_right_ht)
            {
                // right rotation
                root = rotateRight(root);
            }
            else
            {
                // left-right rotation
                root->left = rotateLeft(root->left);
                root = rotateRight(root);
            }
        }
        else if (right_ht > left_ht + 1)
        {
            //left rotation group
            int right_left_ht = root->right->left ? root->right->left->ht : -1;
            int right_right_ht = root->right->right ? root->right->right->ht : -1;

            if (right_right_ht >= right_left_ht)
            {
                // left rotation
                root = rotateLeft(root);
            }
            else
            {
                // right-left rotation
                root->right = rotateRight(root->right);
                root = rotateLeft(root);
            }
        }

        return root;
    }

    void updateHeight(node* n)
    {
        const int right_ht = n->right ? n->right->ht : -1;
        const int left_ht = n->left ? n->left->ht : -1;

        n->ht = std::max(left_ht, right_ht) + 1;
    }

    void updateChildCount(node* n)
    {
        const int left_count = n->left ? n->left->child_count + 1 : 0;
        const int right_count = n->right ? n->right->child_count + 1 : 0;

        n->child_count = left_count + right_count;
    }

private:
    node* m_root = nullptr;
};
