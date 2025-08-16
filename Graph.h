#ifndef GRAPH_H
#define GRAPH_H

#include <memory>
#include <fstream>
#include <unordered_map>
#include "SailDetails.h"
#include "Node.h"
#include <iomanip>
#include <iostream>
#include "Utils.h"
#define SPACE_AMOUNT 16 // Used for printing spaces inside outputfile.

/**
 *  Graph class
 *  This class represents a generic graph, where each node (port) can be connected
 *  to others in two ways: a container graph and a timing graph.
 *  The graph stores:
 *   - dual_graph: maps each port name to a Node that holds edges to other ports,
 *   - sailing_details: maps each unique sail_id to its historical SailDetails entries.
 *  The class provides methods to build the graph from file input, check for specific sail entries,
 *  compute reachability, balance container flows, and output the graph to a file.
 *
 *  The Big 3:
 *  I do not implement the Big 3 (copy constructor, assignment operator, destructor)
 *  because this class uses only standard containers and smart pointers,
 *  all of which are automatically managed.
 */
template<typename T>
class Graph {
    std::unordered_map<T, std::shared_ptr<Node<T>>> dual_graph;         // Source key - > value is a container graph and a timing graph.
    std::unordered_map<int, std::vector<SailDetails>> sailing_details;  // Unique sail_id key -> vector of SailDetails.
public:

    // Adds all the file contents into the 2 maps.
    void add_file(const std::string &source, const std::vector<SailDetails> &database,
                 const std::vector<std::string> &destinations) {
        add_port(source);

        std::string timing_edge = source;
        auto& edges = dual_graph.at(source)->get_container_edges();

        for (size_t i = 1, j = 0; i < database.size() ; ++i, ++j) {
            add_port(destinations[j]);
            edges.emplace_back(dual_graph.at(destinations[j]),database[i].get_containers(),SailDetails::uniqueID);  // Connect source to *all* destinations (container graph)

            if (!check_existence(timing_edge,destinations[j],database[i])) {    // Check the existence of edge update if needed
                dual_graph[timing_edge]->get_timing_edges().emplace_back(dual_graph.at(destinations[j]),database[i].get_timings(),SailDetails::uniqueID);   // Connect source to destination (timing graph)
            }
            timing_edge = destinations[j];  // Used for timing graph connections
        }

        sailing_details[SailDetails::uniqueID] = database;  // Move the database to my SailDetails database
        SailDetails::next_unique_id();                      // Next sail id
    }

    // Check if the edge already exists. (avg time update)
    bool check_existence(const std::string& source, const std::string& destination, const SailDetails& details) {
        auto& edges = dual_graph.at(source)->get_timing_edges();
        for (size_t i = 0; i < edges.size(); ++i) {                         // Iterate over source edges
            if (const auto& edge = edges[i].get_destination().lock()){
                if (edge->get_source() == destination) {                    // Source goes to dest found!
                    auto& sails = sailing_details[edges[i].get_sail_id()];
                    for (size_t j = 0; j < sails.size(); ++j) {             // Iterate over all sails to find sail and update
                        if (sails[j].get_destination() == destination) {
                            sails[j].set_avg_timings(details.get_timings());
                            edges[i].set_weight(sails[j].get_timings());
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    // Adds a port into the map.
    void add_port(const T& src) {
        if (dual_graph.find(src) != dual_graph.end()) return;
        dual_graph[src] = std::make_shared<Node<T>>(src);
    }

    // Prints all neighbors from source, via a single step.
    void get_immediate_neighbors(const T& source_port) const {
        if (dual_graph.find(source_port) == dual_graph.end()) {
            std::cout << source_port <<" does not exist in the database." << std::endl;;
            return;
        }
        const auto& edges = dual_graph.at(source_port)->get_timing_edges();

        if (edges.empty()) {
            std::cout << source_port <<": no outbound ports" << std::endl;;
            return;
        }
        for (size_t i = 0; i < edges.size(); ++i) {
            if (const auto& edge = edges[i].get_destination().lock()) {
                std::cout << edge->get_source() << "," << edges[i].get_weight() << std::endl;;
            }
        }
    }

    // Prints all nodes that are connected to target port.
    void reachable_nodes_to_source(const T& target_port) const {
        if (dual_graph.find(target_port) == dual_graph.end()) {
            std::cout << target_port <<" does not exist in the database." << std::endl;;
            return;
        }
        bool found = false;
        for (const auto& key_val : dual_graph) {  // Iterate over the whole graph
            if (key_val.first != target_port) {
                auto& timing_edges = key_val.second->get_timing_edges();
                for (size_t i = 0 ; i < timing_edges.size(); ++i) {                 // Iterate over the edges of the port that's not a target.
                    if (const auto& destination = timing_edges[i].get_destination().lock()) {
                        if (destination->get_source() == target_port) {             // port connected to target!
                            std::cout << key_val.first << "," << timing_edges[i].get_weight() << std::endl;;
                            found = true;
                        }
                    }
                }
            }
        }
        if (!found) {
            std::cout << target_port << ": no inbound ports" << std::endl;;
        }
    }

    // Returns the container amount in target port provided a date.
    int balance(const T& target_port, const std::string& date) {
        if (dual_graph.find(target_port) == dual_graph.end()) return 0;
        int balance = 0;

        const SailDetails input_date(date);
        const auto& edges = dual_graph.at(target_port)->get_container_edges();

        for (size_t i = 0; i < edges.size(); ++i) {             // Subtract all containers from target port outwards by date
            const int uniqueID = edges[i].get_sail_id();
            if(sailing_details[uniqueID][0] <= input_date)
                balance -= edges[i].get_weight();
        }

        for (auto& key_val : dual_graph) {  // Iterate over the whole graph to find Nodes connected to target
            if (key_val.first != target_port) {
                auto& container_edges = key_val.second->get_container_edges();

                for (size_t i = 0 ; i < container_edges.size(); ++i) {      // Iterate over the edges of every Node
                    if (const auto& destination = container_edges[i].get_destination().lock()) {
                        if (destination->get_source() == target_port) {     // Found a connection (any Node to Target)

                            const int uniqueID = container_edges[i].get_sail_id();
                            const auto& sail_vector = sailing_details[uniqueID];

                            std::tm starting_point = datetime(sail_vector[0].get_departure());
                            std::time_t time = std::mktime(&starting_point);

                            std::string departure;
                            for (size_t j = 1; j < sail_vector.size(); ++j) {       // Check for any date errors (arrival - departure)
                                time += sail_vector[j].get_timings() * MINUTES;
                                const std::string formatted_time = format_time(time);

                                const SailDetails starting_point_date(formatted_time); // Found the sail, and date is valid, add balance and break
                                if (starting_point_date <= input_date && sailing_details[uniqueID][j].get_destination() == target_port) {
                                    balance += container_edges[i].get_weight();
                                    break;
                                }

                                const SailDetails check_delay(sail_vector[j].get_departure());  // Check for any date delays
                                if (starting_point_date <= check_delay) {
                                    starting_point = datetime(check_delay.get_departure());
                                    time = std::mktime(&starting_point);
                                }
                            }
                        }
                    }
                }
            }
        }
        return balance;
    }

    // Prints dual_graph into the outputfile.
    void print(std::ofstream& file, bool flag) const {
        if (!file.is_open()) return;
        std::string category = flag ? "Containers" : "Time";
        for ( auto& key_val : dual_graph) {
            auto& edges = flag ? key_val.second->get_container_edges() : key_val.second->get_timing_edges();;
            file << key_val.first << ":\n";
            for (size_t i = 0 ; i < edges.size(); ++i)
                if (const auto& container_edge = edges[i].get_destination().lock())
                    file << "\t\t-" << std::left << std::setw(SPACE_AMOUNT) << container_edge->get_source()  << category <<  "(" << edges[i].get_weight() << ")\n";
        }
        file << "\n";
    }
};

#endif //GRAPH_H