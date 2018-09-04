#ifndef MY_BINARY_SEARCH_TREE_H
#define MY_BINARY_SEARCH_TREE_H

#include <iostream>
#include <cstddef>  // size_t
#include <utility>  // move()
#include <initializer_list>

namespace my {
    template <typename T>
    class binary_search_tree {
    public:
        typedef T          value_type;
        typedef size_t     size_type;
    private:
        struct node {
            value_type     key;
            node*          left;
            node*          right;

            // constructors.
            node(const value_type &k = value_type(), 
                 node* l = nullptr, node* r = nullptr)
            : key(k), left(l), right(r) {}
            node(value_type&& k, node* l = nullptr, 
                 node* r = nullptr)
            : key(std::move(k)), left(l), right(r) {}
            node(const node &nd) = delete;
            node(node &&nd) = delete;
            ~node() {left = right = nullptr;}

            // operations.
            node& operator=(const node& nd) = delete;
            node& operator=(node&& nd) = delete;
        };

    public:
        // constructors.
        binary_search_tree(): root(nullptr), _size(0) {}
        binary_search_tree(const binary_search_tree& bst);
        binary_search_tree(binary_search_tree&& bst);
        binary_search_tree(std::initializer_list<value_type> il);

        // operators.
        binary_search_tree &operator=(const binary_search_tree& bst);
        binary_search_tree &operator=(binary_search_tree&& bst);

        // operations.
        void insert(const value_type& k);
        void insert(value_type&& k);
        void print(void) const {print_subtree(root, 0);}

        size_type size(void) const {return _size;}

    private:
        node* recursive_copy_subtree(const node* rt);
        node* destroy_subtree(node* rt);
        void print_subtree(const node* rt, size_type depth) const;
        void print_n_space(size_type n) const;
    private:
        node*         root;
        size_type     _size;
    };

    template <typename T>
    binary_search_tree<T>::binary_search_tree(const binary_search_tree& bst) {
        if (root == bst.root)
            return;
        root = destroy_subtree(root);
        root = recursive_copy_subtree(bst.root);
        _size = bst._size;
    }

    template <typename T>
    binary_search_tree<T>::binary_search_tree(binary_search_tree&& bst)
    : root(bst.root), _size(bst.size()) {
        bst.root = nullptr;
        bst._size = 0;
    }

    template <typename T>
    binary_search_tree<T>::binary_search_tree(std::initializer_list<T> il)
    : root(nullptr), _size(0) {
        for (auto &i : il)
            insert(i);
    }

    template <typename T>
    typename binary_search_tree<T>::node *
    binary_search_tree<T>::destroy_subtree(node* rt) {
        if (rt == nullptr)
            return nullptr;
        --_size;
        rt->left = destroy_subtree(rt->left);
        rt->right = destroy_subtree(rt->right);
        delete rt;
        return nullptr;
    }

    template <typename T>
    typename binary_search_tree<T>::node *
    binary_search_tree<T>::recursive_copy_subtree(const node* rt) {
        if (rt == nullptr)
            return nullptr;
        ++_size;
        return new node(rt->key, recursive_copy_subtree(rt->left), 
                                 recursive_copy_subtree(rt->right));
    }

    template <typename T>
    binary_search_tree<T> &
    binary_search_tree<T>::operator=(const binary_search_tree& bst) {
        if (root == bst.root)
            return *this;
        root = destroy_subtree(root);
        root = recursive_copy_subtree(bst.root);
        _size = bst._size;
        return *this;
    }

    template <typename T>
    binary_search_tree<T> &
    binary_search_tree<T>::operator=(binary_search_tree&& bst) {
        root = bst.root;
        _size = bst._size;

        bst.root = nullptr;
        bst._size = 0;
    }

    template <typename T>
    void
    binary_search_tree<T>::insert(const value_type& k) {
        node *nd = root;
        if (root == nullptr) {
            root = new node(k);
            ++_size;
            return;
        }
        while (true) {
            if (k > nd->key)
                if (nd->right == nullptr) {
                    nd->right = new node(k);
                    ++_size;
                    return;
                } else {
                    nd = nd->right;
                }
            else if (k < nd->key)
                if (nd->left == nullptr) {
                    nd->left = new node(k);
                    ++_size;
                    return;
                } else {
                    nd = nd->left;
                }
            else
                return;
        }
    }

    template <typename T>
    void
    binary_search_tree<T>::insert(value_type&& k) {
        node *nd = root;
        if (root == nullptr) {
            root = new node(std::move(k));
            ++_size;
            return;
        }
        while (true) {
            if (k > nd->key)
                if (nd->right == nullptr) {
                    nd->right = new node(std::move(k));
                    ++_size;
                    return;
                } else {
                    nd = nd->right;
                }
            else if (k < nd->key)
                if (nd->left == nullptr) {
                    nd->left = new node(std::move(k));
                    ++_size;
                    return;
                } else {
                    nd = nd->left;
                }
            else
                return;
        }
    }

    template <typename T>
    void
    binary_search_tree<T>::print_subtree(const node* rt, size_type depth) const {
        if (rt == nullptr)
            return;
        print_subtree(rt->left, depth + 1);
        print_n_space(depth);
        std::cout << rt->key << std::endl;
        print_subtree(rt->right, depth + 1);
    }

    template <typename T>
    void
    binary_search_tree<T>::print_n_space(size_type n) const {
        for (size_type i = 0; i < n; ++i)
            std::cout << " ";
    }
}

#endif
