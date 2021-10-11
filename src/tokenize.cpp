#include "../include/tokenize.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

//Checks if string is a keyword
bool isKeyword(const std::string& str) {
    std::vector<std::string> keywords{"if", "else", "while", "do", "break", "continue", "int", "double", "float", 
    "return", "char", "case", "long", "short", "size_t", "typedef", "switch", "unsigned", "void", "static", "struct", "sizeof", 
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

//Checks if string is an Operator
bool isOperator(const std::string& str) {
    const std::vector<std::string> operators{"<", ">", "*", "+", "-", "/", "=", "-=", "*=", "+=", "/=", "++", "--", "==", "<=", ">=", "||", "&&", "!", "!="};
    
    for(std::string oper : operators) {
        if(str == oper) {
            return true;
        }
    }

    return false;
}

//Returns appropriate token for each operator
std::string whichOperator(const std::string str) {
    std::unordered_map<std::string, std::string> op_tokens = {{"<", "LSTHAN"}, {">", "GRTHAN"}, {"*", "TIMES"}, {"+", "PLUS"}, 
        {"-", "MINUS"}, {"/", "DIVIDE"}, {"=", "ASSIGN"}, {"-=", "MINEQ"}, {"*=", "TIMESEQ"}, {"+=", "PLUSEQ"}, {"/=", "DIVEQ"}, 
        {"++", "INCRMT"}, {"--", "DECRMT"}, {"==", "CMPEQUAL"}, {"<=", "LSEQ"}, {">=", "GREQ"}, {"||", "OR"}, {"&&", "AND"}, 
        {"!", "NOT"}, {"!=", "NOTEQ"}};
        
        auto it = op_tokens.find(str);
        if (it != op_tokens.end()) {
            return it->second;
        }
        return "UNKNOWN";
}

//Checks if string is a separator
bool isSeparator(const std::string& str) {
    const std::vector<std::string> punctuations{"{", "}", ",", "(", ")", ";", "[", "]", ":", "'", "#", "\""};
    for(std::string punc : punctuations) {
        if(str == punc) {
            return true;
        }
    }

    return false;
}

//Return appropriate token for each separator
std::string whichSeparator(const std::string str) {
    std::unordered_map<std::string, std::string> sep_tokens = {{"{", "LCURLY"}, {"}", "RCURLY"}, {",", "COMMA"}, {"(", "LPAREN"},
    {")", "RPARAN"}, {";", "SEMICOL"}, {"[", "LSQRBRACE"}, {"]", "RSQRBRACE"}, {":", "COLON"}, {"'", "ASPOST"}, {"#", "POUND"},
    {"\"", "QUOTE"}};
    
    auto it = sep_tokens.find(str);
    if (it != sep_tokens.end()) {
        return it->second;
    }

    return "UNKNOWN";
}

void tokenize(std::string& filename) {
    
    //Creates output stream object and input stream object
    std::ofstream output("lexer_output.txt");
    std::ifstream fin(filename);

    //Opens output and input files
    if(!fin.is_open() && !output.is_open()) {
        std::cout << "File failed to open.\n";
        return;
    }

    //Stores input file as string called "input"
    std::string input((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));    
    
    //Declare variable current to keep track of current index in input string
    size_t current = 0;    
    size_t length = input.size();

    //Declare buffer for storing lexemes of more than one character
    std::string substring = "";

    //Begin parsing input string
    while(current < length) {

        //Dispose of whitespace and newline characters
        if(input[current] == ' ' || input[current] == '\n') {
            current++;
            continue;
        }

        //Identifies string literals when lexer encounters "
        if (input[current] == '"') {
            substring += input[current];
            current++;
            while (input[current] != '"') {
                substring += input[current];
                current++;
            }
            substring += input[current];
            current++;
            output << "STRING: " << substring << "\n";
            substring = "";
            continue;
        }

        //Identifies character literals when lexer encounters '
        if (input[current] == '\'') {
            substring += input[current];
            current++;
            while (input[current] != '\'') {
                substring += input[current];
                current++;
            }
            substring += input[current];
            current++;
            output << "CHAR: " << substring << "\n";
            substring = "";
            continue;
        }

        //Identifies identidiers and keywords
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
        
        //Identifies operators
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

        //Identifies integers and real numbers
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

        //Identifies separators
        if (isSeparator(std::string(1, input[current]))) {
            output << whichSeparator(std::string(1,input[current])) << ": " << input[current] << "\n";
            current++;
            substring = "";
            continue;
        }

        //If none of the above tokens are relevant, skip past this character
        current++;
    }
}