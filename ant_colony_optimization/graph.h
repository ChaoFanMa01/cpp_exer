/*
 * Classes used to define an undirected graph.
 */

#ifndef ACO_GRAPH_H
#define ACO_GRAPH_H

#include <vector>
#include <utility>       // move()
#include <cstdint>       // intxx_t, uintxx_t

#include "header.h"

namespace aco {
    enum class VertexType: uint8_t;
    enum class VertexStatus: uint8_t;
    template <class O> class Vertex;
    template <class O> class Edge;

    /* class Edge
     * Definition of the edge in undirected graph.
     */
    template <class O>
    class Edge {
    public:
        typedef O           data_t;
        typedef Vertex<O>   vertex_t;
        typedef double      weight_t;

        Edge(): _end(nullptr), _weight(weight_t()) {}
        Edge(const vertex_t* pv, weight_t w = weight_t()): _end(pv), _weight(w) {}
        Edge(const Edge& e): _end(e._end), _weight(e._weight) {}
        Edge(Edge&& e)
        : _end(std::move(e._end)), _weight(std::move(e._weight)) {
            e._end = nullptr;
        }
        ~Edge() { _end = nullptr; }

        Edge& operator=(const Edge&);
        Edge& operator=(Edge&&);

        /*
         * edges are compared according to their weights.
         */
        bool operator==(const Edge&);
        bool operator!=(const Edge& e) { return !(*this == e); }
        bool operator>(const Edge& e) const { return _weight > e._weight; }
        bool operator>=(const Edge& e) const { return _weight >= e._weight; }
        bool operator<(const Edge& e) const { return _weight < e._weight; }
        bool operator<=(const Edge& e) const { return _weight <= e._weight; }

        /* @fn isSame
         * Check whether this edge is same as the given one.
         */
        bool isSame(const Edge& e) const;

        vertex_t* end() const { return _end; }
        weight_t  weight() const  { return _weight; }

        void setEnd(const vertex_t* pv) { _end = pv; }
        void setWeight(weight_t w) { _weight = w; }

    private:
        vertex_t*      _end;
        weight_t       _weight;
    };

    template <class O>
    Edge<O>&
    Edge<O>::operator=(const Edge& e) {
        _end = e.end; _weight = e._weight;
        return *this;
    }

    template <class O>
    Edge<O>&
    Edge<O>::operator=(Edge&& e) {
        _end = std::move(e._end);
        _weight = std::move(e._weight);
        e._end = nullptr;
        return *this;
    }

    template <class O>
    bool
    Edge<O>::operator==(const Edge& e) {
        return _weight == e._weight;
    }

    template <class O>
    bool
    Edge<O>::isSame(const Edge& e) const {
        return _weight == e._weight && _end == e._weight;
    }

    /* @enum VertexType
     * Enumerate type to identify the type of a vertex, i.e.,
     * 0: a source vertex,
     * 1: a destination vertex,
     * 2: an mediate vertex.
     */
    enum class VertexType: uint8_t {
        SOURCE,
        DESTINATION,
        MEDIATE
    };

    /* @enum VertexStatus
     * Enumerate type to identify the status of a vertex, i.e.,
     * 0: unselected,
     * 1: selected.
     */
    enum class VertexStatus: uint8_t {
        UNSELECTED,
        SELECTED
    };

    template <class O>
    class Vertex {
    public:
        typedef O         data_t;
        typedef int32_t   weight_t;

        Vertex(data_t d = data_t(), size_type id = -1, weight_t w = weight_t(),
               std::vector<Edge<data_t>>& n = std::vector<Edge<data_t>>(),
               size_type p = -1, VertexType t = VertexType::MEDIATE,
               VertexStatus s = Vertex::UNSELECTED)
        : _data(d), _id(id), _weight(w), _neighbors(n),
          _parent(p), _type(t), _status(s) {}
        Vertex(const Vertex&);
        Vertex(Vertex&&);
        ~Vertex() = default;

        Vertex& operator=(const Vertex&);
        Vertex& operator=(Vertex&&);

        /*
         * vertices are compared according to their weights.
         */
        bool operator==(const Vertex& v) const { return _weight == v._weight; }
        bool operator!=(const Vertex& v) const { return !(*this == v); }
        bool operator>(const Vertex& v) const { return _weight > v._weight; }
        bool operator>=(const Vertex& v) const { return _weight >= v._weight; }
        bool operator<(const Vertex& v) const { return _weight < v._weight; }
        bool operator<=(const Vertex& v) const { return _weight <= v._weight; }

        bool isSame(const Vertex& v) const { return _id == v._id; }

        data_t                     data() const { return _data; }
        size_type                  id() const { return _id; }
        weight_t                   weight() const { return _weight; }
        std::vector<Edge<data_t>>  neighbors() const { return _neighbors; }
        std::vector<Edge<data_t>>& neighbors() { return _neighbors; }
        size_type                  parent() const { return _parent; }
        VertexType                 type() const { return _type; }
        VertexStatus               status() const { return _status; }

        void setData(data_t d) { _data = d; }
        void setId(size_type id) { _id = id; }
        void setWeight(weight_t w) { _weight = w; }
        void setNeighbors(std::vector<Edge<data_t>>& n) { _neighbors = n; }
        void setNeighbors(std::vector<Edge<data_t>>&& n) { _neighbors = std::move(n); }
        void setParent(size_type p) { _parent = p; }
        void setType(VertexType t) { _type = t; }
        void setStatus(VertexStatus s) { _status = s; }

        void pushNeighbor(Vertex& v, weight_t w) { _neighbors.push_back(Edge<data_t>(&v, w)); }
        void popNeighbor() { _neighbors.pop_back(); }
        void clearNeighbor() { _neighbors.clear(); }

        size_type neighborSize() const { return _neighbors.size(); }

    private:
        data_t                       _data;    // data storing on this vertex.
        size_type                    _id;      // ID of this vertex.
        weight_t                     _weight;  
        std::vector<Edge<data_t>>    _neighbors;
        size_type                    _parent;
        VertexType                   _type;
        VertexStatus                 _status;
    };

    template <class O>
    Vertex<O>::Vertex(const Vertex& v)
    : _data(v._data), _id(v._id), _weight(v._weight), _neighbors(v._neighbors),
      _parent(v._parent), _type(v._type), _status(v._status) {}

    template <class O>
    Vertex<O>::Vertex(Vertex&& v)
    : _data(std::move(v._data)), _id(std::move(v._id)), _weight(std::move(v._weight)),
      _neighbors(std::move(v._neighbors)), _parent(std::move(v._parent)),
      _type(std::move(v._type)), _status(std::move(v._status)) {}

    template <class O>
    Vertex<O>&
    Vertex<O>::operator=(const Vertex& v) {
        _data = v._data; _id = v._id; _weight = v._weight;
        _neighbors = v._neighbors; _parent = v._parent;
        _type = v._type; _status = v._status;

        return *this;
    }

    template <class O>
    Vertex<O>&
    Vertex<O>::operator=(Vertex&& v) {
        _data = std::move(v._data); _id = std::move(v._id);
        _weight = std::move(v._weight);
        _neighbors = std::move(v._neighbors);
        _parent = std::move(v._parent);
        _type = std::move(v._type); _status = std::move(v._status);

        _data = data_t();

        return *this;
    }
}

#endif
