#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <fstream>

bool isKeyword(const std::string&);
bool isIdentifier(const std::string&);
bool isOperator(const std::string&);
std::string whichOperator(const std::string);
std::string whichSeparator(const std::string);
bool isSeparator(const std::string&);
void tokenize(std::string&);



#endif