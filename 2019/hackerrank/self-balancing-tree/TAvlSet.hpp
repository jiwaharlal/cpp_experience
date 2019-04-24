#pragma once

#include <vector>

template <typename T>
struct TAvlNode
{
    using NodeType = TAvlNode<T>;

    T val;
    NodeType* left;
    NodeType* right;
    //NodeType* parent;

    int ht;
};

template <typename T>
struct TAvlIterator
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using node = TAvlNode<T>;

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

    //TAvlIterator& operator --()
    //{
        //while (!m_node_seq.back()->left)
        //{
            //m_node_seq.pop_back();
        //}

        //if (!m_node_seq.empty())
        //{
            //m_node_seq.push_back(m_node_seq.back()->left);
        //}
    //}

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

private:
    std::vector<node*> m_node_seq;
};

template <typename T>
class TAvlSet
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using node = TAvlNode<T>;
    using iterator = TAvlIterator<T>;

public:
    //TAvlSet() {}

    void insert(const T& value)
    {
        root = insert(root, value);
    }

    iterator begin()
    {
        std::vector<node*> sequence;
        for (auto n = root; n; sequence.push_back(n), n = n->left);
        return iterator(sequence);
    }

    iterator end()
    {
        return iterator({});
    }

private:
    node * insert(node * root, const_reference val)
    {
        if (root == nullptr)
        {
            return new node{val, nullptr, nullptr, 0};
        }

        if (val < root->val)
        {
            root->left = insert(root->left, val);
        }
        else
        {
            root->right = insert(root->right, val);
        }

        int right_ht = root->right ? root->right->ht : -1;
        int left_ht = root->left ? root->left->ht : -1;

        if (left_ht > right_ht + 1)
        {
            int left_left_ht = root->left->left ? root->left->left->ht : -1;
            int left_right_ht = root->left->right ? root->left->right->ht : -1;

            if (left_left_ht >= left_right_ht)
            {
                auto new_top = root->left;
                auto new_right = root;
                new_right->left = new_top->right;
                new_top->right = root;
                root = new_top;
            }
            else
            {
                auto new_top = root->left->right;
                auto new_right = root;
                auto new_left = root->left;
                new_right->left = new_top->right;
                new_left->right = new_top->left;
                new_top->left = new_left;
                new_top->right = new_right;

                root = new_top;
                updateHeight(root->left);
            }

            updateHeight(root->right);
        }
        else if (right_ht > left_ht + 1)
        {
            int right_left_ht = root->right->left ? root->right->left->ht : -1;
            int right_right_ht = root->right->right ? root->right->right->ht : -1;

            if (right_right_ht >= right_left_ht)
            {
                auto new_top = root->right;
                auto new_left = root;
                new_left->right = new_top->left;
                new_top->left = root;
                root = new_top;
            }
            else
            {
                auto new_top = root->right->left;
                auto new_left = root;
                auto new_right = root->right;
                new_left->right = new_top->left;
                new_right->left = new_top->right;
                new_top->left = new_left;
                new_top->right = new_right;

                root = new_top;
                updateHeight(root->right);
            }

            updateHeight(root->left);
        }
        updateHeight(root);

        return root;
    }

    void updateHeight(node* n)
    {
        int right_ht = n->right ? n->right->ht : -1;
        int left_ht = n->left ? n->left->ht : -1;

        n->ht = std::max(left_ht, right_ht) + 1;
    }

private:
    node* root = nullptr;
};
