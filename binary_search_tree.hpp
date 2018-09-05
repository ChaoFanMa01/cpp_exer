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
        ~binary_search_tree() {destroy_subtree(root);}

        // operators.
        binary_search_tree &operator=(const binary_search_tree& bst);
        binary_search_tree &operator=(binary_search_tree&& bst);

        // operations.
        void insert(const value_type& k);
        void insert(value_type&& k);
        void remove(const value_type& k);
        void print(void) const {print_subtree(root, 0);}

        size_type size(void) const {return _size;}

    private:
        node* recursive_copy_subtree(const node* rt);
        node* destroy_subtree(node* rt);
        void print_subtree(const node* rt, size_type depth) const;
        void print_n_space(size_type n) const;
        node* find_parent(const node* nd) const;
        node* find_node(const value_type& k) const;
        node* find_min_node(const node* rt) const;
        node* find_max_node(const node* rt) const;
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

    template <typename T>
    typename binary_search_tree<T>::node *
    binary_search_tree<T>::find_parent(const node* nd) const {
        node* cnt = root;
// TODO if (nd == nullptr) throw except;
        if (cnt == nd)
            return nullptr;
        while (true) {
            if (nd->key > cnt->key) {
                if (nd->key == cnt->right->key)
                    return cnt;
                cnt = cnt->right;
            } else {
                if (nd->key == cnt->left->key)
                    return cnt;
                cnt = cnt->left;
            }
        }
    }

    template <typename T>
    typename binary_search_tree<T>::node *
    binary_search_tree<T>::find_node(const value_type& k) const {
        node* nd = root;

        if (root == nullptr)
            return nullptr;

        while (true)
            if (nd->key == k) {
                return nd;
            } else if (nd->key < k) {
                if (nd->right != nullptr)
                    nd = nd->right;
                else
                    return nullptr;
            } else {
                if (nd->left != nullptr)
                    nd = nd->left;
                else
                    return nullptr;
            }
        return nullptr;
    }

    template <typename T>
    typename binary_search_tree<T>::node*
    binary_search_tree<T>::find_min_node(const node* rt) const {
        node *nd = rt;

        while (nd->left != nullptr)
            nd = nd->left;

        return nd;
    }

    template <typename T>
    typename binary_search_tree<T>::node*
    binary_search_tree<T>::find_max_node(const node* rt) const {
        node* nd = rt;

        while (nd->right != nullptr)
            nd = nd->right;

        return nd;
    }

    template <typename T>
    void
    binary_search_tree<T>::remove(const value_type& val) {
        node* nd, *pa, *min;

        if (!(nd = find_node(val)))
            return;
        pa = find_parent(nd);
        if (nd->left == nullptr) {
            // root is the node to be deleted.
            if (pa == nullptr) {
                root = root->right;
            } else {
                if (nd == pa->right)
                    pa->right = nd->right;
                else
                    pa->left = nd->right;
            }
        } else if (nd->right == nullptr) {
            if (pa == nullptr) {
                root = root->left;
            } else {
                if (nd == pa->right)
                    pa->right = nd->left;
                else
                    pa->left = nd->left;
            }
        } else {
            if ((min = find_min_node(nd->right)) != nd->right) {
                node* ppa = find_parent(min);
                ppa->left = min->right;
                min->right = nd->right;
            }
            min->left = nd->left;
            if (pa == nullptr)
                root = min;
            else
                if (nd == pa->right)
                    pa->right = min;
                else
                    pa->left =min;
        }
        delete nd;
        --_size;
    }
}

#endif
