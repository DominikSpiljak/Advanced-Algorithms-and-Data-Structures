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

class AVLTree
{
public:
    struct tree *tree;
    unordered_map<char, int> heights;

    char find_parent(char current_parent, char key)
    {
        if (current_parent > key)
        {
            if (tree->_tree[current_parent]->left_node)
            {
                return find_parent(tree->_tree[current_parent]->left_node->c, key);
            }
            else
            {
                return current_parent;
            }
        }
        else
        {
            if (tree->_tree[current_parent]->right_node)
            {
                return find_parent(tree->_tree[current_parent]->right_node->c, key);
            }
            else
            {
                return current_parent;
            }
        }
    }

    bool child_left(tree_node *child, tree_node *parent)
    {
        return child->c < parent->c;
    }

    int calculateHeight(tree_node *node)
    {
        int left = 0, right = 0;

        if (node->right_node == NULL && node->left_node == NULL)
        {
            return 0;
        }

        if (node->left_node != NULL)
        {
            left = heights[node->left_node->c];
        }

        if (node->right_node != NULL)
        {
            right = heights[node->right_node->c];
        }

        return max(left, right) + 1;
    }

    void rotate(tree_node *node)
    {
        tree_node *parent, *grandparent;
        parent = tree->get_parent(node);
        grandparent = tree->get_parent(parent);

        if (child_left(node, parent))
        {
            parent->left_node = node->right_node;
            node->right_node = parent;
        }
        else
        {
            parent->right_node = node->left_node;
            node->left_node = parent;
        }

        heights[parent->c] = calculateHeight(parent);
        heights[node->c] = calculateHeight(node);

        if (tree->root_node == parent)
        {
            tree->root_node = node;
        }
        else
        {
            if (child_left(parent, grandparent))
            {
                grandparent->left_node = node;
            }
            else
            {
                grandparent->right_node = node;
            }
        }
    }

    int calculate_balance_score(tree_node *node)
    {
        if (!node->right_node and !node->left_node)
        {
            return 0;
        }
        return (node->right_node ? heights[node->right_node->c] : -1) - (node->left_node ? heights[node->left_node->c] : -1);
    }

    void balance_tree(tree_node *node, int balance_score)
    {
        if (balance_score < 0)
        {
            if (calculate_balance_score(node->left_node) <= 0)
            {
                rotate(node->left_node);
            }
            else
            {
                rotate(node->left_node->right_node);
                rotate(node->left_node);
            }
        }
        else
        {
            if (calculate_balance_score(node->right_node) >= 0)
            {
                rotate(node->right_node);
            }
            else
            {
                rotate(node->right_node->left_node);
                rotate(node->right_node);
            }
        }
    }

    AVLTree()
    {
        tree = new struct tree();
    }

    void InsertElement(char key)
    {
        if (tree->root_node)
        {
            char parent = find_parent(tree->root_node->c, key);
            tree->add_child(parent, key, parent > key);
            tree_node *node = tree->_tree[key];
            int balance_score;
            while (node != NULL)
            {
                heights[node->c] = calculateHeight(node);
                balance_score = calculate_balance_score(node);

                if (abs(balance_score) > 1)
                {
                    balance_tree(node, balance_score);
                }

                node = tree->get_parent(node);
            }
        }

        else
        {
            tree->add_root(key);
        }
    }

    pair<char *, char *> getChildrenNodesValues(char *key = NULL)
    {
        char *char1, *char2;
        char1 = char2 = NULL;

        if (key == NULL)
        {
            key = &tree->root_node->c;
        }

        if (tree->_tree.find(*key) != tree->_tree.end())
        {
            if (tree->_tree[*key]->left_node)
            {
                char1 = &tree->_tree[*key]->left_node->c;
            }

            if (tree->_tree[*key]->right_node)
            {
                char2 = &tree->_tree[*key]->right_node->c;
            }
        }

        return make_pair(char1, char2);
    }

    char *getRootNode()
    {
        if (tree->root_node)
        {
            return &tree->root_node->c;
        }
        else
        {
            return NULL;
        }
    }

    string PreOrderTraversal()
    {
        return tree->traverse_pre_order();
    }

    string PostOrderTraversal()
    {
        return tree->traverse_post_order();
    }
};

int main(void)
{
    AVLTree *avltree = new AVLTree();
    avltree->InsertElement('M');
    avltree->InsertElement('K');
    avltree->InsertElement('Q');
    avltree->InsertElement('P');
    avltree->InsertElement('A');
    avltree->InsertElement('L');
    avltree->InsertElement('B');
    avltree->InsertElement('C');
    avltree->InsertElement('D');
    avltree->InsertElement('E');
    avltree->InsertElement('N');
    avltree->InsertElement('Z');
    avltree->tree->printTree();
    for (auto i : avltree->heights)
        cout << i.first << "   " << i.second << endl;

    return 0;
}
