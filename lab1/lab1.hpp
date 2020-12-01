#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

struct tree_node
{
    char c;
    tree_node *left_node = NULL, *right_node = NULL;
    tree_node(char c)
    {
        this->c = c;
    }
    ~tree_node()
    {
        if (left_node != NULL)
            delete left_node;
        if (right_node != NULL)
            delete right_node;
    }
};
struct tree
{
    unordered_map<char, tree_node *> _tree;
    tree_node *root_node = NULL;
    void add_root(char root)
    {
        root_node = new tree_node(root);
        _tree[root] = root_node;
    }
    void add_root(tree_node *root)
    {
        if (_tree.find(root->c) == _tree.end())
            _tree[root->c] = root;
        root_node = root;
    }
    void add_child(char parent, char child, bool left)
    {
        if (_tree.find(parent) != _tree.end())
        {
            tree_node *cn = new tree_node(child);
            _tree[child] = cn;
            if (left)
                _tree[parent]->left_node = cn;
            else
                _tree[parent]->right_node = cn;
        }
    }
    void add_child(tree_node *parent, tree_node *child, bool left)
    {
        if (_tree.find(parent->c) == _tree.end())
            _tree[parent->c] = parent;
        if (_tree.find(child->c) == _tree.end())
            _tree[child->c] = child;
        if (left)
            parent->left_node = child;
        else
            parent->right_node = child;
    }
    tree_node *get_parent(tree_node *n)
    {
        for (pair<char, tree_node *> n1 : _tree)
            if ((n1.second->left_node != NULL && n1.second->left_node->c == n->c) ||
                (n1.second->right_node != NULL && n1.second->right_node->c == n->c))
                return n1.second;
        return NULL;
    }
    ~tree()
    {
        if (root_node != NULL)
            delete root_node;
    }
    string traverse_pre_order()
    {
        string s = "";
        this->_traverse_pre_order(root_node, &s);
        return s;
    }
    void _traverse_pre_order(tree_node *node, string *s)
    {
        if (node == NULL)
            return;
        (*s) += node->c; // append wasn't working right on GCC
        this->_traverse_pre_order(node->left_node, s);
        this->_traverse_pre_order(node->right_node, s);
    }
    string traverse_post_order()
    {
        string s = "";
        this->_traverse_post_order(root_node, &s);
        return s;
    }
    void _traverse_post_order(tree_node *node, string *s)
    {
        if (node == NULL)
            return;
        this->_traverse_post_order(node->left_node, s);
        this->_traverse_post_order(node->right_node, s);
        (*s) += node->c;
    }
    tree_node *_constructTree(string pre_order, string post_order, int *pindex, int l, int h)
    {
        if (*pindex >= pre_order.size() || l > h)
            return NULL;

        tree_node *root = new tree_node(pre_order[*pindex]);
        (*pindex)++;

        if (l == h)
            return root;

        int i;
        for (i = l; i <= h; i++)
            if (pre_order[*pindex] == post_order[i])
                break;
        if (i <= h)
        {
            if ((i + 1) > (h - 1))
            {
                if (pre_order[*pindex - 1] > pre_order[*pindex])
                    add_child(root, _constructTree(pre_order, post_order, pindex, l, i), true);
                else
                    add_child(root, _constructTree(pre_order, post_order, pindex, l, i), false);
            }
            else
            {
                add_child(root, _constructTree(pre_order, post_order, pindex, l, i), true);
                add_child(root, _constructTree(pre_order, post_order, pindex, i + 1, h - 1), false);
            }
        }
        return root;
    }
    void constructTree(string pre_order, string post_order)
    {
        int pindex = 0;
        post_order = post_order.substr(0, post_order.size() - 1);
        add_root(_constructTree(pre_order, post_order, &pindex, 0, (int)post_order.size()));
    }
    void printTree()
    {
        _printTree("", root_node, true);
    }
    void _printTree(string p, tree_node *root, bool isLeft)
    {
        cout << p;
        cout << (!isLeft ? "├──" : "└──");
        if (root != NULL)
        {
            cout << root->c << endl;

            if (root->right_node || root->left_node)
            {
                _printTree(p + (!isLeft ? "│   " : "    "), root->right_node, false);
                _printTree(p + (!isLeft ? "│   " : "    "), root->left_node, true);
            }
        }
        else
            cout << endl;
    }
};