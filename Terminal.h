#ifndef TERMINAL_H
#define TERMINAL_H

#include <memory>
#include <string>
#include <regex>
#include "Graph.h"

// Terminal.h simulates a simple terminal; written commands are executed, on any error, a unique exception is thrown.
// Includes Graph<T> class, which is the database of graphs that is used throughout the project.
#define DEFAULT_OUTPUT_FILE "output.dat"
template<typename T>
class Graph;

/**
 *  Represents a Terminal, Supports all the commands in the exercise.
 *  The Terminal reads all the files provided at the initialization stage or later,
 *  And checks for errors, then inserts all the data into the Graphs.
 *  Holds the graphs and the outputfile name.
 *
 *  The big 3:
 *  Not implemented because this class uses smart pointers and standard library types only.
 *  Resource management is handled automatically by std::unique_ptr and std::string.
 *  No deep copying or custom cleanup logic is required,
 *  so the compiler-generated defaults are enough.
 * */
class Terminal {
    std::unique_ptr<Graph<std::string>> graphs;     // < Both graphs. (container, timing)
    std::string output_file;                        // < outputfile.

public:
    explicit Terminal();                            // Default ctor.
    void start_terminal();                          // Starts the mini terminal.
    void load(const char* file_name) const;         // Loads a file into the graphs.
    void write_output_file();                       // Write the graphs into the outputfile.
    void read_files(int argc, char *argv[]);        // Initialization stage.
    int read_lines(std::ifstream &file) const;      // Read all lines from 1 file.
};

#endif //TERMINAL_H