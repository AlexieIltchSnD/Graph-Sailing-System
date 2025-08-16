#ifndef EDGE_H
#define EDGE_H

#include <memory>
#include <utility>
#include "SailDetails.h"

/**
 *  Generic Edge class
 *  This class represents an edge, each Node contains a vector of edges, (source to lots of destinations)
 *  Each edge uses a weak_ptr to destination Node, has its own weight,
 *  And a unique sail_id for identification later on.
 *
 *  The big 3:
 *  I only create and hold references to other nodes via smart pointers.
 *  Since there's no special copy or cleanup behavior needed, I rely on the compiler-generated versions
 *  thus, no need for the Big 3.
 ***/
template<typename T>    // Forward declaration.
class Node;

template<typename T>
class Edge {
    std::weak_ptr<Node<T>> destination;     // Weak pointer to destination.
    int weight;                             // The weight of the edge.
    int sail_id;                            // Unique sail_id.

public:
    explicit Edge(std::shared_ptr<Node<T>> _dest, const int _weight, const int _sail_id)    // Default ctor.
    : destination(std::move(_dest)), weight(_weight), sail_id(_sail_id) {}
    int get_weight() const { return weight; }                                               // Weight getter.
    void set_weight(const int w) { weight = w; }                                            // Weight setter. (updating)
    int get_sail_id() const { return sail_id; }                                             // Unique id getter.
    const std::weak_ptr<Node<T>>& get_destination() const { return destination; }           // Destination getter.
};

#endif //EDGE_H