#ifndef UTILLS_H
#define UTILLS_H

#include <ctime>
#include <string>
#include <vector>
#include "Node.h"

/**
 *  Welcome to the Utils helper functions file!
 *  Here are some helper functions I am using throughout the project.
 *  Mostly are date comparisons, and basic input checks.
 ***/

// All the constants below are used to check if the date provided is valid and port name validation.
#define YEAR 2023
#define LOWER_YEAR 1900
#define MINUTES 60
#define MAX_STRING_LENGTH 16
#define TIME_PATTERN "%d/%m %H:%M"
#define REGEX_PATTERN R"(^\d{2}/\d{2} \d{2}:\d{2}$)"

// Splits a string into 2 or 4 words via ','.
std::vector<std::string> split_line(const std::string& line);

// Check if the name of port is valid, returns true if valid, false otherwise.
bool is_valid_port(const std::string& port);

// Check if a string is a valid number, return true if valid, false otherwise.
bool is_number(const std::string& s);

// Check if the date is valid. (regex)
// return true if date string is valid, false otherwise.
bool isValidDateTime(const std::string& input);

// Receives 2 strings and returns true if both are regex valid dates, false otherwise.
bool matchesDateTime(const std::string& input1, const std::string& input2);

// Checks the input of a whole line from a file, containing port string, 2 dates, and a number string.
// If the input is valid, returns true, false otherwise.
bool check_input(const std::string& dest_port, const std::string& arrival_time, const std::string& container_quantity,
    const std::string& departure_time);

// Receives 2 dates via string, and returns the amount of minute difference between input1 to input2.
// If the difference is negative, 0 is returned.
int calculate_time_minutes(const std::string &input1, const std::string &input2);

// Receives a string representation of time; and std::tm returned of said time,
// Via the assumption year given in exercise -> 2023
std::tm datetime(const std::string& _datetime);

// Receives time_t& and returns a string representation of said time.
// Returned string example 11/05 10:48.
std::string format_time(const std::time_t& _time);

// Print into cerr the error message upon a bad terminal input.
void printError();

#endif //UTILLS_H