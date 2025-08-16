#include "Terminal.h"
#include "FileException.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include "SailDetails.h"
#include "CommandGenerator.cpp"

#define FILLER_STRING "0"
#define ARRIVAL_BEFORE_DEPARTURE (-1) // bad date inside a file. arrival of a boat before the previous boat arrived.

Terminal::Terminal() {
    // graphs = std::make_unique<Graph<std::string>>(); <-- csweb compiler didn't like this line, so line 14 was born
    graphs = std::unique_ptr<Graph<std::string>>(new Graph<std::string>());
    output_file = DEFAULT_OUTPUT_FILE;
}

// Starts the mini terminal after the initialization.
// Command maps are ['command': lambda function], for more information, go to CommandGenerator.cpp
void Terminal::start_terminal() {
    std::string inputString;
    auto commands = buildCommandsMap(*this, *this->graphs);

    while (true) {
        std::getline(std::cin, inputString);

        std::stringstream stream(inputString);
        std::string token1, token2, token3, extra;

        if (inputString.find(',') != std::string::npos) {   // Read the line, separate by ',' should be 2 or 3 words.
            std::getline(stream, token1, ',');
            std::getline(stream, token2, ',');
            std::getline(stream, token3);
        } else {
            stream >> token1 >> token2 >> token3 >> extra;   // Command without ',' should be 1 word.
        }
        if (!extra.empty()) {   // Extra command! -> bad input.
            printError();
            continue;
        }
        if (token1 == "exit") break;

        auto command_at_token1 = commands.find(token1);
        auto command_at_token2 = commands.find(token2);
        try {                                                  // Find command and execute, else exception is thrown,
            if (command_at_token1 != commands.end()) {         // Or bad input.
                command_at_token1->second(token2,token3);
            }
            else if (command_at_token2 != commands.end()) {
                command_at_token2->second(token1,token3);
            }else {
                printError();
            }
        }catch (std::exception& e) {
            std::cerr << e.what();
        }
    }
}

// Load 1 file command, receives a file name, reads its contents, upon any error a custom exception
// is thrown, uses read_lines function.
void Terminal::load(const char* file_name) const{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw FileNotFoundException(file_name);
    }
    const int line_number = read_lines(file);
    if (line_number != 0)
        throw InvalidInputException(file_name , line_number);
    std::cout << "Update was successful." << std::endl;;
}

// Write into the output file command, prints both graphs inside the file.
void Terminal::write_output_file(){
    if(output_file.empty())
        output_file = "output.dat";
    std::ofstream outputFile(output_file);
    this->graphs->print(outputFile,true);
    this->graphs->print(outputFile,false);
}

// Initialization stage, find the outputfile if provided, and load all other files provided via read_lines
// Function, upon any error here, exit the program with 1.
void Terminal::read_files(const int argc, char* argv[]){
    std::vector<std::string> argFiles;
    try {
        // Basic validation: must have at least "-i" and one input file
        if (argc < 3 || std::strcmp(argv[1], "-i") != 0) {
            throw InvalidFileArgumentsException();
        }
        int k = 2;

        // Collect input files until "-o" or end of args
        for (; k < argc; ++k) {
            if (std::strcmp(argv[k], "-o") == 0)
                break;
            argFiles.emplace_back(argv[k]);
        }

        if (argFiles.empty()) {
            throw InvalidFileArgumentsException();  // Must contain at least 1 input file
        }

        // If -o is present and followed by a valid filename, set output_file
        if (k < argc && std::strcmp(argv[k], "-o") == 0)
            if (k + 1 < argc && argv[k + 1][0] != '-') {
                output_file = argv[k + 1];
            }

        std::ifstream firstFile(argFiles[0]);
        if (!firstFile.is_open()) {
            throw InvalidInputExceptionExit(argFiles[0]);   // Could not open file
        }
        const int line_number = read_lines(firstFile);
        if (line_number != 0)
            throw InvalidInputExceptionExit(argFiles[0] , line_number); // The First file must be valid
    }catch (std::exception& e) {
        std::cerr << e.what();
        exit(1);
    }

    if (!argFiles.empty()) {                        // Load all other files, can fail
        argFiles.erase(argFiles.begin());
        for (auto& string : argFiles) {
            try {
                load(string.c_str());
            }catch (std::exception& e) {
                std::cerr << e.what();
            }
        }
    }
    start_terminal();
}

// Read_lines helper function, receives a file, and reads its contents via
// The provided guidelines, check the dates provided, and the non-negative container count,
// And the amounts of data in a single line, upon any error, return the line number that the error occurred in.
// Uses helper functions from Utils.h.
int Terminal::read_lines(std::ifstream& file) const {
    std::string line ,source_port;
    bool first_rotation = true;
    int line_number = 1;

    std::vector<SailDetails> details;
    std::vector<std::string> dest_ports;

    while(std::getline(file, line)) {
        auto tokens = split_line(line);

        if (first_rotation) {                       // source port at the first line of each file. (2 words)
            if (tokens.size() != 2) return line_number;

            source_port = tokens[0];
            const std::string& departure_time = tokens[1];

            if (!check_input(source_port, departure_time, FILLER_STRING, departure_time)) {
                return line_number;
            }

            details.emplace_back(0,0,departure_time,source_port);
            first_rotation = false;
        }
        else {                                      // All lines after the source port. (4 words)
            if (tokens.size() != 4) return line_number;

            const std::string& dest_port          = tokens[0];
            const std::string& arrival_time       = tokens[1];
            const std::string& container_quantity = tokens[2];
            const std::string& departure_time     = tokens[3];

            if (!check_input(dest_port, arrival_time, container_quantity, departure_time)) {
                return line_number;
            }
            int timing = calculate_time_minutes(details.back().get_departure(), arrival_time);
            if (timing == ARRIVAL_BEFORE_DEPARTURE) {
                return line_number;
            }
            int containers = std::stoi(container_quantity);

            dest_ports.push_back(dest_port);
            details.emplace_back(containers, timing, departure_time,dest_port);
        }
        ++line_number;
    }
    graphs->add_file(source_port, details, dest_ports);         // Insert all the file into the database.
    file.close();
    return 0;
}