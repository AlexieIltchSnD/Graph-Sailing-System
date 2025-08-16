#ifndef NODE_H
#define NODE_H

#include <vector>
#include "Edge.h"

/**
 *  Generic Node class
 *  This class represents a Node, each Node contains two vectors of edges, and a source.
 *  The First vector represents all edges in the container graph,
 *  the Second vector represents all edges in the timing graph.
 *
 *  The big 3:
 *  Not implemented because this class only contains standard containers and value types.
 *  The vectors and source are automatically managed, and there is no manual memory handling.
 *  As a result, the compiler-generated versions are enough.
 ***/
template<typename T>
class Node {
    T source;                               // Name of each Node.
    std::vector<Edge<T>> container_edges;   // All edges from the current node to others in the container graph.
    std::vector<Edge<T>> timing_edges;      //  ''   ''   ''   ''    ''    ''  ''   ''   ''  '' timing graph.

public:
    explicit Node(const T& src) : source(std::move(src)) {}                             // Default ctor.
    const T& get_source() const { return source; }                                      // Source getter.

    std::vector<Edge<T>>& get_container_edges() { return container_edges; }             // Read/Write.
    const std::vector<Edge<T>>& get_container_edges() const { return container_edges; } // Container graph getter.

    std::vector<Edge<T>>& get_timing_edges() { return timing_edges; }                   // Read/Write.
    const std::vector<Edge<T>>& get_timing_edges() const { return timing_edges; }       // Timing graph getter.
};

#endif //NODE_H