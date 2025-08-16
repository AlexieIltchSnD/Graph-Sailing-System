#include "Terminal.h"
#include <functional>
#include <iostream>
#include <map>
#include "Utils.h"

/**
 * Welcome to the commandMap generator!
 * Here is where I activate all the command functions!
 * We are transferred to here from the Terminal, when a command is inserted before execution.
 * **/

using CommandFunction = std::function<void(const std::string& source, const std::string& date)>;
inline std::map<std::string, CommandFunction> buildCommandsMap(Terminal &terminal, Graph<std::string>& graphs) {
    std::map<std::string,CommandFunction> commandsMap;

    /**
     * Load command, if one of the parameters is wrong, print the error message, otherwise load the file into
     * the graph.
     ***/
    commandsMap["load"] = [&terminal](const std::string& filename, const std::string& date) {
        if (date.empty() && !filename.empty()) {
            terminal.load(filename.c_str());
        }else {
            printError();
        }
    };

    /**
    * Inbound command, if one of the parameters is wrong, print the error message, otherwise print the reachable
    * nodes to source.
    ***/
    commandsMap["inbound"] = [&graphs](const std::string& source, const std::string& date) {
        if (date.empty() && !source.empty()) {
            graphs.reachable_nodes_to_source(source);
        }else {
            printError();
        }
    };

     /**
    * Outbound command, if one of the parameters is wrong, print the error message, otherwise print the reachable
    * neighbor nodes from source.
    ***/
    commandsMap["outbound"] = [&graphs](const std::string& source, const std::string& date) {
        if (date.empty() && !source.empty()) {
            graphs.get_immediate_neighbors(source);
        }else {
            printError();
        }
    };

     /**
    * Balance command, if one of the parameters is wrong, print the error message, otherwise print the
    * container balance at source via the provided date.
    ***/
    commandsMap["balance"] = [&graphs](const std::string& source, const std::string& date) {
        bool flag = false;
        if (!source.empty() && !date.empty()) {
            if (check_input(source,date,"0",date)) {
                std::cout << graphs.balance(source,date) << "\n";
                flag = true;
            }
        }
        if (!flag) printError();
    };
    /**
    * Print command, if one of the parameters is wrong, print the error message, otherwise print the
    * the container graph and timing graph into a provided output file, if output file isnt provided,
    * prints into the default file -> output.dat.
    ***/
    commandsMap["print"] = [&terminal](const std::string& source, const std::string& date) {
        if (source.empty() && date.empty()) {
            terminal.write_output_file();
        }else {
            printError();
        }
    };
    return commandsMap;
}