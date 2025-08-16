#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include <exception>
#include <string>
/**
 * Welcome to FileExceptions.h! Contains all custom exception classes used in the file system project.
 * **/

// Base exception class for file system errors.
class FileException : public std::exception{
    std::string message;
public:
    explicit FileException(std::string  message) : message(std::move(message)) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Custom exception that is used for invalid input from files loaded after the initial loading.
class InvalidInputException final : public FileException {
public:
    explicit InvalidInputException(const std::string& file_name, const int line_number = 0) :
    FileException("Invalid input in file <" + file_name + "> at line " + std::to_string(line_number) + ".\n") {}
};

// Custom exception that is used for invalid input from the initial loading,
// after this is thrown program exits with 1.
class InvalidInputExceptionExit final : public FileException {
public:
    explicit InvalidInputExceptionExit(const std::string& file_name, const int line_number = 0) :
    FileException("Invalid input in file <" + file_name + "> at line " + std::to_string(line_number) + ".\n") {}
};

// Custom exception that is used when running the program with invalid arguments.
class InvalidFileArgumentsException final : public FileException {
public:
    explicit InvalidFileArgumentsException() : FileException("USAGE: -i <infile1> [<infile2> ...]\n") {}
};

// Custom exception that is used for invalid files that cannot be opened or read from.
class FileNotFoundException final : public FileException {
public:
    explicit FileNotFoundException(const std::string& file_name) : FileException("ERROR opening/reading the specified file. <" + file_name + ">\n") {}
};

#endif //FILEEXCEPTION_H