#include <string>
#include <fstream>
#include <iostream>
#include "../include/tokenize.hpp"

int main(int argc, char* argv[]) { 
    if(argc > 1) {
        std::string filename = argv[1];     //Gets filename of code-to-be-analyzed from terminal    
        tokenize(filename);                 //Starts lexer
    }
    return 0;
}