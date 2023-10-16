#pragma once
#include <stdbool.h>


typedef enum rbtree_node_color { RED, BLACK } color_t;
template<typename key_t, typename data_t>

class node_t
{
public:
    key_t key;
    data_t data;
    color_t color;
    node_t
        * left = nullptr,
        * right = nullptr,
        * parent = nullptr;
    node_t() {};

    inline node_t* _get_sibling(const node_t* node) { return node == node->parent->left ? node->parent->right : node->parent->left; } // получить друг ребенка
    inline node_t* _get_uncle(const node_t* node) { return _get_sibling(node->parent); } // брат отца 
    inline color_t _get_color(const node_t* node) { return node ? node->color : BLACK; } // цвет вершинки , если указатель 0 , то вернет черн  


    node_t* _get_last_right_node(node_t* node)
    {
        while (node->right)
            node = node->right;

        return node;
    }

    node_t* _create_node(const key_t key, const data_t data, const color_t color, node_t* left, node_t* right)
    {
        node_t* res = (node_t*)calloc(1, sizeof(node_t));
        if (!res)
            return NULL;

        res->key = key;
        res->data = data;
        res->color = color;
        res->left = left;
        res->right = right;

        if (left)
            left->parent = res;

        if (right)
            right->parent = res;

        return res;
    }

    void _replace_node(node_t*& tree, node_t* old, node_t* newone)
    {
        if (!old->parent)
            tree = newone;
        else
        {
            if (old == old->parent->left)
                old->parent->left = newone;
            else
                old->parent->right = newone;
        }

        if (newone)
            newone->parent = old->parent;
    }

    node_t* _find_node(node_t* tree, const key_t key)
    {
        node_t* node = tree;
        while (node)
        {
            if (key == node->key)
                return node;
            else if (key < node->key)
                node = node->left;
            else
                node = node->right;
        }

        return node;
    }

    void _rotate_left(node_t*& tree, node_t* node)
    {
        node_t* right = node->right;
        _replace_node(tree, node, right);

        node->right = right->left;

        if (right->left)
            right->left->parent = node;

        right->left = node;
        node->parent = right;
    }

    void _rotate_right(node_t*& tree, node_t* node)
    {
        node_t* left = node->left;
        _replace_node(tree, node, left);

        node->left = left->right;

        if (left->right)
            left->right->parent = node;

        left->right = node;
        node->parent = left;
    }

    void _insert_fixup(node_t*& tree, node_t* node)
    {
        if (!node->parent)
        {
            node->color = BLACK;
            return;
        }
        else if (_get_color(node->parent) == BLACK)
            return;
        else if (_get_color(_get_uncle(node)) == RED)
        {
            node->parent->color = BLACK;
            _get_uncle(node)->color = BLACK;

            node->parent->parent->color = RED;
            _insert_fixup(tree, node->parent->parent);

            return;
        }
        else if (node == node->parent->right && node->parent == node->parent->parent->left)
        {
            _rotate_left(tree, node->parent);

            node = node->left;
        }
        else if (node == node->parent->left && node->parent == node->parent->parent->right)
        {
            _rotate_right(tree, node->parent);

            node = node->right;
        }

        node->parent->color = BLACK;
        node->parent->parent->color = RED;
        if (node == node->parent->left && node->parent == node->parent->parent->left)
            _rotate_right(tree, node->parent->parent);
        else
            _rotate_left(tree, node->parent->parent);
    }
    node_t* RBTreeInsert(node_t*& tree, const key_t key, const data_t data)
    {
        node_t* newone = _create_node(key, data, RED, NULL, NULL);
        if (!tree)
            tree = newone;
        else
        {
            node_t* node = tree;
            while (1)
            {
                if (key == node->key)
                {
                    free(newone);
                    return nullptr;
                }
                else if (key < node->key)
                {
                    if (!node->left)
                    {
                        node->left = newone;
                        break;
                    }
                    else
                        node = node->left;
                }
                else
                {
                    if (!node->right)
                    {
                        node->right = newone;
                        break;
                    }
                    else
                        node = node->right;
                }
            }

            newone->parent = node;
        }

        _insert_fixup(tree, newone);
        return newone;
    }

    void _delete_fixup(node_t* tree, node_t* node)
    {
        if (!node->parent)
            return;
        else if (_get_color(_get_sibling(node)) == RED)
        {
            node->parent->color = RED;
            _get_sibling(node)->color = BLACK;

            if (node == node->parent->left)
                _rotate_left(tree, node->parent);
            else
                _rotate_right(tree, node->parent);
        }

        if (_get_color(node->parent) == BLACK && _get_color(_get_sibling(node)) == BLACK && _get_color(_get_sibling(node)->left) == BLACK && _get_color(_get_sibling(node)->right) == BLACK)
        {
            _get_sibling(node)->color = RED;
            _delete_fixup(tree, node->parent);

            return;
        }
        else if (_get_color(node->parent) == RED && _get_color(_get_sibling(node)) == BLACK && _get_color(_get_sibling(node)->left) == BLACK && _get_color(_get_sibling(node)->right) == BLACK)
        {
            _get_sibling(node)->color = RED;
            node->parent->color = BLACK;

            return;
        }
        else if (node == node->parent->left && _get_color(_get_sibling(node)) == BLACK && _get_color(_get_sibling(node)->left) == RED && _get_color(_get_sibling(node)->right) == BLACK)
        {
            _get_sibling(node)->color = RED;
            _get_sibling(node)->left->color = BLACK;

            _rotate_right(tree, _get_sibling(node));
        }
        else if (node == node->parent->right && _get_color(_get_sibling(node)) == BLACK && _get_color(_get_sibling(node)->left) == BLACK && _get_color(_get_sibling(node)->right) == RED)
        {
            _get_sibling(node)->color = RED;
            _get_sibling(node)->right->color = BLACK;

            _rotate_left(tree, _get_sibling(node));
        }

        _get_sibling(node)->color = _get_color(node->parent);
        node->parent->color = BLACK;
        if (node == node->parent->left)
        {
            _get_sibling(node)->right->color = BLACK;
            _rotate_left(tree, node->parent);
        }
        else
        {
            _get_sibling(node)->left->color = BLACK;
            _rotate_right(tree, node->parent);
        }
    }

    void _dealloc_tree(node_t* node)
    {
        if (!node)
            return;

        _dealloc_tree(node->left);
        free(node->left);
        node->left = nullptr;

        _dealloc_tree(node->right);
        free(node->right);
        node->right = nullptr;

        node->parent = nullptr;
    }


    node_t* RBTreeAlloc(void)
    {
        return (node_t*)calloc(1, sizeof(node_t));
    }

    void RBTreeDealloc(node_t** tree)
    {
        //assert(tree && *tree);

        node_t* root = (*tree);

        _dealloc_tree(root);
    }

    int RBTreeInsert1(node_t*& tree, const key_t key, const data_t data)
    {
        assert(tree);

        node_t* newone = _create_node(key, data, RED, NULL, NULL);
        if (!tree)
            tree = newone;
        else
        {
            node_t* node = tree;
            while (1)
            {
                //int comp_result = strcmp(key, node->key);
                if (key == node->key)
                {
                    free(newone);

                    data_t ptr = node->data;
                    while (ptr != NULL && ptr->next)
                        ptr = ptr->next;

                    //ptr->next = data;

    //                free(key); // TODO: check logic
                    return 1;
                }
                else if (key < node->key)
                {
                    if (!node->left)
                    {
                        node->left = newone;
                        break;
                    }
                    else
                        node = node->left;
                }
                else
                {
                    if (!node->right)
                    {
                        node->right = newone;
                        break;
                    }
                    else
                        node = node->right;
                }
            }

            newone->parent = node;
        }

        _insert_fixup(tree, newone);
    }


    bool RBTreeRemove(node_t*& tree, const key_t key)
    {
        //assert(tree);

        node_t* node = _find_node(tree, key);
        if (!node)
            return false;

        if (node->data)
        {
            

            return true;
        }

        if (node->left && node->right)
        {
            node_t* pred = _get_last_right_node(node->left);

            

            node = pred;
        }

        node_t* child = node->right ? node->right : node->left;
        if (_get_color(node) == BLACK)
        {
            node->color = _get_color(child);
            _delete_fixup(tree, node);
        }

        _replace_node(tree, node, child);
        if (!node->parent && child)
            child->color = BLACK;

        free(node);

        return true;
    }


    node_t* RBTreeFind(node_t* tree, key_t key)
    {
        node_t* node = _find_node(tree, key);

        return node ? node : nullptr;
    }

    node_t* RBTreeFindMin(node_t* tree) {
        node_t* ptr = tree;
        while (ptr && ptr->left) {
            ptr = ptr->left;
        }
        return ptr;
    }

    node_t* RBTreeFindMax(node_t* tree) {
        node_t* ptr = tree;
        while (ptr && ptr->right) {
            ptr = ptr->right;
        }
        return ptr;
    }

    node_t* RBTreeFindPrevious(node_t* treenode)
    {
        node_t* node = treenode;
        while (node && node->parent) {
            node = node->parent;
        }
        node_t* match = nullptr;

        while (node)
        {
            //int comp_result = strcmp(key, node->key);
            if (treenode->key <= node->key)
                node = node->left;
            else
            {
                if (!match || match->key < node->key)
                    match = node;

                node = node->right;
            }
        }

        return match ? (match->key == treenode->key ? nullptr : match) : nullptr;
    }

    node_t* RBTreeFindNext(node_t* treenode)
    {
        node_t* node = treenode;
        while (node && node->parent) {
            node = node->parent;
        }
        node_t* match = nullptr;

        while (node)
        {
            //int comp_result = strcmp(key, node->key);
            if (treenode->key >= node->key)
                node = node->right;
            else
            {
                if (!match || match->key > node->key)
                    match = node;

                node = node->left;
            }
        }

        return match ? (match->key == treenode->key ? nullptr : match) : nullptr;
    }

};

