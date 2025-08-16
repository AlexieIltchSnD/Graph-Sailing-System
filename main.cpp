#include "Terminal.h"
// Main function, Creates and starts the mini Terminal.
int main(const int argv, char* argc[]) {
    Terminal miniTerminal{};
    miniTerminal.read_files(argv,argc);
    return 0;
}