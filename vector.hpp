#ifndef M_VECTOR
#define M_VECTOR

#include <iostream>
#include <initializer_list>
#include <utility>    // move()

namespace my {
    template <typename T>
    class vector {
    public:
        typedef size_t     size_type;
        typedef T          value_type;
        typedef T*         iterator;
        typedef const T*   const_iterator;

        // constructors and destructor.
        vector(size_type size = 0);
        vector(size_type size, const value_type &val);
        template <class Itr>
        vector(Itr begin, Itr end);
        vector(const vector &vec);
        vector(std::initializer_list<T> il);
        vector(vector &&vec);

        ~vector() {s = 0; cap = 0; delete [] b;}

        // assginments.
        vector& operator=(const vector &vec);
        vector& operator=(vector &&vec);
        vector& operator=(std::initializer_list<T> il);

        // iterator.
        iterator begin() { return b; }
        const_iterator begin() const { return b; }
        iterator end() { return b + s; }
        const_iterator end() const { return b + s; }

        const_iterator cbegin() const { return b; }
        const_iterator bend() const { return b + s; }

        // size.
        size_type size(void) const { return s; }
        void resize(size_type n);
        void resize(size_type n, const value_type &val);
        size_type capacity(void) const { return cap; }
        bool empty(void) const { return s == 0; }
        void reserve(size_type n);

        // access.
        T& operator[](size_type n) { return b[n]; }
        const T& operator[](size_type n) const { return b[n]; }
        T& front(void) { return b[0]; }
        const T& front(void) const { return b[0]; }
        T& back(void) {return b[s - 1]; }
        const T& back(void) const { return b[s - 1]; }

        // modifiers.
        void push_back(const value_type &val);
        void push_back(value_type &&val);
        void pop_back(void) { s--; }
        iterator insert(const_iterator pos, const value_type &val);
        iterator insert(const_iterator pos, size_type n, const value_type &val);
        template <class Iter> iterator insert(const_iterator pos, Iter pb, Iter pe);
        iterator insert(const_iterator pos, value_type &&val);
        iterator insert(const_iterator pos, std::initializer_list<value_type> il);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator pb, const_iterator pe);

        void clear() { s= 0; }

    private:
        T                         *b;
        size_type                 s;
        size_type                 cap;
        static const size_type    init_cap = 10;
    };

    /*
     * At the first time, this function is writted as
     * follows:
     * template <typename T>
     * vector<T>::vector(size_type size = 0)
     * Then, the compiler complains that 
     * "vector.hpp:23:5: error: redeclaration of 
     * ‘my::vector<T>::vector(my::vector<T>::size_type)’ 
     * may not have default arguments [-fpermissive]"
     * This is because the initializer of the default 
     * argument should not be rewritten here, i.e.,
     * simply written as size_type size not size_type = 0.
     */
    template <typename T>
    vector<T>::vector(size_type size)
    : b(nullptr), s(size), cap(init_cap + size) {
        cap = init_cap + size;
        b = new T[cap];
    }

    template <typename T>
    vector<T>::vector(size_type size, const value_type &val)
    : b(nullptr), s(size), cap(init_cap + size) {
        b = new T[cap];

        for (size_type i = 0; i < size; ++i)
            b[i] = val;
    }

   template <typename T>
   template <class Itr>
   vector<T>::vector(Itr begin, Itr end)
   : b(nullptr), s(0), cap(init_cap) {
       b = new T[cap];
       for ( ; begin != end; ++s, ++begin) {
           if (cap < s)
               reserve(2 * cap);
           b[s] = *begin;
       }
   }

   template <typename T>
   vector<T>::vector(const vector &vec)
   : b(nullptr), s(vec.size()),
     cap(vec.capacity()) {
       b = new T[cap];
       for (size_type i = 0; i < s; ++i)
           b[i];
   }

   template <typename T>
   vector<T>::vector(std::initializer_list<T> il)
   : b(nullptr), s(0), cap(il.size() * 2) {
       b = new T[cap];
       for (auto &e : il)
           b[s++] = e;
   }

   template <typename T>
   vector<T>::vector(vector &&vec)
   : b(vec.b), s(vec.s), cap(vec.cap) {
       vec.b = nullptr;
       vec.s = 0;
       vec.cap = 0;
   }

   template <typename T>
   vector<T> &
   vector<T>::operator=(const vector &vec) {
       if (cap < vec.s) {
           cap = 2 * vec.s;
           reserve(cap);
       }
       
       s = 0;
       for ( ; s < vec.s; ++s)
           b[s] = vec.b[s];

       return *this;
   }

   template <typename T>
   vector<T> &
   vector<T>::operator=(vector &&vec) {
       delete [] b;
       b = vec.b; s = vec.s; cap = vec.cap;

       vec.b = nullptr; vec.s = 0; vec.cap = 0;

       return *this;
   }

   template <typename T>
   vector<T> &
   vector<T>::operator=(std::initializer_list<T> il) {
       if (cap < il.size()) {
           cap = 2 * il.size();
           reserve(cap);
       }

       s = 0;
       for (auto &i : il)
           b[s++] = i;

       return *this;
   }

    template <typename T>
    void
    vector<T>::resize(size_type n) {
        if (cap <= n)
            reserve(2 * n);
        s = n;
    }

    template <typename T>
    void
    vector<T>::push_back(const value_type &val) {
        if (cap < s + 1) {
            cap = 2 * cap;
            reserve(cap);
        }
        b[s++] = val;
    }

    template <typename T>
    void
    vector<T>::push_back(value_type &&val) {
        if (cap < s + 1) {
            cap = 2 * cap;
            reserve(cap);
        }
        b[s++] = std::move(val);
    }

    template <typename T>
    void
    vector<T>::resize(size_type n, const value_type &val) {
        if (cap <= n)
            reserve(2 * n);
        for (size_type i = s; i < n; ++i)
            b[i] = val;
        s = n;
    }

    template <typename T>
    void
    vector<T>::reserve(size_type n) {
        if (cap < n) {
            cap = n;
            T *temp = new T[cap];
            for (size_type i = 0; i < s; ++i)
                temp[i] = b[i];
            delete [] b;
            b = temp;
        }
    }

    template <typename T>
    typename vector<T>::iterator
    vector<T>::insert(const_iterator pos, const value_type &val) {
        if (cap < s + 1) {
            cap = 2 * cap;
            reserve(cap);
        }
        for (iterator iter = b + s; iter != pos; --iter)
            *iter = *(iter - 1);
        *pos = val; s++;

        return pos;
    }

    template <typename T>
    typename vector<T>::iterator
    vector<T>::insert(const_iterator pos, value_type &&val) {
        if (cap < s + 1) {
            cap = 2 * cap;
            reserve(cap);
        }
        for (iterator iter = b + s; iter != pos; --iter)
            *iter = *(iter - 1);
        *pos = std::move(val); s++;

        return pos;
    }

    template <typename T>
    typename vector<T>::iterator
    vector<T>::insert(const_iterator pos, size_type n, const value_type &val) {
        while (n--)
            insert(val);
        return pos;
    }

    template <typename T>
    template <class Iter>
    typename vector<T>::iterator
    vector<T>::insert(const_iterator pos, Iter pb, Iter pe) {
        for (Iter iter = pb; iter != pe; ++iter)
            insert(pos, *iter);
        return pos;
    }

    template <typename T>
    typename vector<T>::iterator
    vector<T>::insert(const_iterator pos, std::initializer_list<T> il) {
        for (auto &i : il)
            insert(i);
        return pos;
    }

    template <typename T>
    typename vector<T>::iterator
    vector<T>::erase(const_iterator pos) {
        for (iterator iter = pos; iter - b != s; ++iter)
            *iter = *(iter + 1);
        return pos;
    }

    template <typename T>
    typename vector<T>::iterator
    vector<T>::erase(const_iterator pb, const_iterator pe) {
        for (size_type i = pe - pb; i >= 0; --i)
            erase(pb);
        return pb;
    }
}

#endif
