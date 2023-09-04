#pragma once

#include "symbol/token.h"
#include <cstddef>
#include <string>
#include <vector>

namespace lox {

class Scanner{
    public:
    explicit Scanner(std::string src); 
    Scanner(Scanner& another)=delete;
    auto operator=(Scanner& another)->Scanner& =delete;
    
    auto IsEnd()->bool{return current_==source_.end();}
    auto Next()->char{return *current_++;}
    
    auto ScanTokens()->std::vector<Token>&;
    void AddToken(TokenType type);
    auto Match(char c)->bool;
    auto Peek()->char;
    auto PeekNext()->char;

    private:
    void ScanToken();
    void ScanString();
    void ScanNumber();
    void ScanIdentifier();
    
    std::string source_;
    std::vector<Token> tokens_{};
    std::string::iterator start_;
    std::string::iterator current_;
    int line_=1;
};

}  // namespace lox