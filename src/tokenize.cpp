#include "../include/tokenize.hpp"
#include <vector>
#include <string>
#include <iostream>


bool isKeyword(const std::string& str) {
    std::vector<std::string> keywords{"if", "else", "while", "do", "break", "continue", "int", "double", "float", 
    "return", "char", "case", "long", "short", "typedef", "switch", "unsigned", "void", "static", "struct", "sizeof", 
    "long", "volatile", "typedef", "enum", "const", "union", "extern", "bool", "asm", "auto", "catch", "class", "default", 
    "delete", "explicit", "export", "for", "friend", "goto", "inline", "mutable", "namespace", "new", "operator", "private", 
    "protected", "public", "register", "return", "signed", "template", "this", "throw", "true", "try", "typeid", "typename", 
    "using", "virtual", "volatile", "wchar_t"};

    for(std::string keyword : keywords) {
        if(str == keyword) {
            return true;
        }
    }

    return false;
}

bool isOperator(const std::string& str) {
    const std::vector<std::string> operators{"<", ">", "*", "+", "-", "/", "=", "-=", "*=", "+=", "/=", "++", "--", "==", "<=", ">=", "||", "&&", "!", "!="};
    
    for(std::string oper : operators) {
        if(str == oper) {
            return true;
        }
    }

    return false;
}

std::string whichOperator(const std::string str) {
        if (str == "<") {
            return "LSTHAN";
        } else if (str == ">") {
            return "GRTHAN";
        } else if (str == "*") {
            return "MUL";
        } else if (str == "+") {
            return "PLUS";
        } else if (str == "-") {
            return "MINUS";
        } else if (str == "/") {
            return "DIVIDE";
        } else if (str == "=") {
            return "ASSIGN";
        } else if (str == "-=") {
            return "MINEQ";
        } else if (str == "*=") {
            return "MULEQ";
        } else if (str == "+=") {
            return "PLUSEQ";
        } else if (str == "/=") {
            return "DIVEQ";
        } else if (str == "++") {
            return "INCRMT";
        } else if (str == "--") {
            return "DECRMT";
        } else if (str == "==") {
            return "CMPEQUAL";
        } else if (str == "<=") {
            return "LSEQ";
        } else if (str == ">=") {
            return "GREQ";
        } else if (str == "||") {
            return "OR";
        } else if (str == "&&") {
            return "AND";
        } else if (str == "!") {
            return "NOT";
        } else if (str == "!=") {
            return "NOTEQ";
        } else {
            return "UNKNOWN";
        }
}

bool isSeperator(const std::string& str) {
    const std::vector<std::string> punctuations{"{", "}", ",", "(", ")", ";", "[", "]", ":", "'", "#", "\""};
    for(std::string punc : punctuations) {
        if(str == punc) {
            return true;
        }
    }

    return false;
}

std::string whichSeperator(const std::string str) {
    if (str == "{") {
        return "LCURLY";
    } else if (str == "}") {
        return "RCURLY";
    } else if (str == ",") {
        return "COMMA";
    } else if (str == "(") {
        return "LPAREN";
    } else if (str == ")") {
        return "RPAREN";
    } else if (str == ";") {
        return "SEMICOL";
    } else if (str == "[") {
        return "LSQRBRACE";
    } else if (str == "]") {
        return "RSQRBRACE";
    } else if (str == ":") {
        return "COLON";
    } else if (str == "'") {
        return "APOST";
    } else if (str == "#") {
        return "POUND";
    } else if (str == "\"") {
        return "QUOTES";
    } else {
        return "UNKNOWN";
    }
}

void tokenize(std::string& filename) {
    std::ofstream output("lexer_output.txt");
    std::ifstream fin(filename);

    if(!fin.is_open() && !output.is_open()) {
        std::cout << "File failed to open.\n";
        return;
    }
    std::string input((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));    
    
    size_t current = 0;    
    size_t length = input.size();
    std::string substring = "";

    
    while(current < length) {
        std::cout << "loop\n";
        
        if(input[current] == ' ' || input[current] == '\n') {
            current++;
            continue;
        }

        if (isalpha(input[current]) || input[current] == '_') {
            while (isalnum(input[current]) || input[current] == '_'){
                substring += input[current];
                current++;
            }
            if (isKeyword(substring)) {
                output << "KEYWORD: " << substring << "\n";
                substring = "";
                continue;
            } else {
                output << "ID: " << substring << "\n";
                substring = "";
                continue;
            }
        }
        
        if (isOperator(std::string(1, input[current]))) {
            substring += input[current];

            if (input[current] == '/'){
                if (input[current + 1] == '/') {
                    while(input[current] != '\n') {
                        current++;
                    }
                    substring = "";
                    continue;
                } else if (input[current + 1] == '*') {
                    while(substring != "*/") {
                        current++;
                        if (input[current] == '*' && input[current + 1] == '/') {
                            current += 2;
                            substring = "";
                            break;
                        }
                    }
                    continue;
                }
            } else if (isOperator(substring + input[current + 1])) {
                substring += input[current + 1];
                output << whichOperator(substring) << ": " << substring << "\n";
                current += 2;
                substring = "";
                continue;
            } else {
                output << whichOperator(substring) << ": " << substring << "\n";
                current++;
                substring = "";
                continue;
            }
        }

        if (isdigit(input[current])) {
            bool real_or_not = false;
            while (isdigit(input[current]) || input[current] == '.') {
                substring += input[current];
                current++;
                if (input[current] == '.') {
                    real_or_not = true;
                }
            }
            if(real_or_not == false) {
                output << "INTEGER: " << substring << "\n";
            } else {
                output << "REAL: " << substring << "\n";
            }
            substring = "";
            continue;
        }

        if (isSeperator(std::string(1, input[current]))) {
            output << whichSeperator(std::string(1,input[current])) << ": " << input[current] << "\n";
            current++;
            substring = "";
            continue;
        }

        current++;
    }
}