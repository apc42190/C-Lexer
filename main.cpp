#include <string>
#include <fstream>
#include <iostream>
#include "include/tokenize.hpp"

int main(int argc, char* argv[]) { 
    if(argc > 1) {
        std::string filename = argv[1];
        tokenize(filename);
    }
    return 0;
}