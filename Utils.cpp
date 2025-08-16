#include "Utils.h"
#include <iomanip>
#include <iostream>
#include <regex>
#include <ctime>

std::vector<std::string> split_line(const std::string& line){
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

bool is_valid_port(const std::string& port) {
    return !port.empty() && port.length() <= MAX_STRING_LENGTH;
}

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool isValidDateTime(const std::string& input) {
    std::tm tm = {};
    std::istringstream stream(input);
    stream >> std::get_time(&tm, TIME_PATTERN);

    if (stream.fail()) {
        return false;
    }

    tm.tm_year = YEAR - LOWER_YEAR;
    const std::tm original = tm;
    std::mktime(&tm);

    return original.tm_mday == tm.tm_mday &&
           original.tm_mon  == tm.tm_mon &&
           original.tm_hour == tm.tm_hour &&
           original.tm_min  == tm.tm_min;
}

bool matchesDateTime(const std::string& input1, const std::string& input2){
    const std::regex pattern(REGEX_PATTERN);
    if (!std::regex_match(input1, pattern) || !std::regex_match(input2, pattern))
        return false;
    return isValidDateTime(input1) && isValidDateTime(input2);
}

bool check_input(const std::string& dest_port, const std::string& arrival_time, const std::string& container_quantity,
    const std::string& departure_time) {
    if (is_valid_port(dest_port) && is_number(container_quantity) && matchesDateTime(arrival_time,departure_time))
        return true;
    return false;
}

int calculate_time_minutes(const std::string &input1, const std::string &input2) {
    std::tm tm1 = {}, tm2 = {};

    std::istringstream stream1(input1);
    std::istringstream stream2(input2);

    stream1 >> std::get_time(&tm1, TIME_PATTERN);
    stream2 >> std::get_time(&tm2, TIME_PATTERN);

    tm1.tm_year = tm2.tm_year = YEAR - LOWER_YEAR;

    time_t time1 = std::mktime(&tm1);
    time_t time2 = std::mktime(&tm2);

    const int diffMinutes = static_cast<int>(std::difftime(time2,time1) / MINUTES);
    if (diffMinutes < 0)
        return -1;
    return diffMinutes;
}

std::tm datetime(const std::string &_datetime) {
    std::tm tm = {};
    std::istringstream stream(_datetime);
    stream >> std::get_time(&tm, TIME_PATTERN);
    tm.tm_year = YEAR - LOWER_YEAR;
    return tm;
}

std::string format_time(const std::time_t& _time) {
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&_time), TIME_PATTERN);
    return oss.str();
}

void printError() {
    std::cerr << "USAGE:\n"
              << "'load' <file> *or*\n"
              << "<node>, 'inbound' *or*\n"
              << "<node>, 'outbound' *or*\n"
              << "<node>, 'balance', dd/mm HH:mm *or*\n"
              << "'print' *or*\n"
              << "'exit' *to terminate*\n";
}