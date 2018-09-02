#ifndef M_VECTOR
#define M_VECTOR

#include <iostream>
#include <initializer_list>

namespace my {
    template <typename T>
    class vector {
    public:
        typedef size_t     size_type;
        typedef T          value_type;
        typedef T*         iterator;
        typedef const T*   const_iterator;

        vector(size_type size = 0);
        vector(size_type size, const value_type &val);
        template <class Itr>
        vector(Itr begin, Itr end);
        vector(const vector &vec);
        vector(std::initializer_list<T> il);

        size_type size(void) const { return s; }
        void resize(size_type n);
        void resize(size_type n, const value_type &val);
        size_type capacity(void) const { return cap; }
        bool empty(void) const { return s == 0; }
        void reserve(size_type n);

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
    void
    vector<T>::resize(size_type n) {
        if (cap <= n)
            reserve(2 * n);
        s = n;
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
}

#endif
