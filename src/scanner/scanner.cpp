
#include "scanner/scanner.h"

#include <cctype>
#include <cmath>
#include <cstddef>
#include <string>
#include <utility>
#include "helper/helper.h"
#include "symbol/token.h"

namespace lox {
    
Scanner::Scanner(std::string src):
source_(std::move(src)),start_(source_.begin()),current_(source_.begin()){}


auto Scanner::ScanTokens()->std::vector<Token>&{
    while (!IsEnd()) {
        start_=current_;
        ScanToken();
    }
    tokens_.emplace_back(TokenType::EOF,"",line_);
    return tokens_;
}

void Scanner::ScanToken(){
    char c=Next();
    switch (c) {
        case '(': AddToken(TokenType::LEFT_PAREN); break;
        case ')': AddToken(TokenType::RIGHT_PAREN); break;
        case '{': AddToken(TokenType::LEFT_BRACE); break;
        case '}': AddToken(TokenType::RIGHT_BRACE); break;
        case ',': AddToken(TokenType::COMMA); break;
        case '.': AddToken(TokenType::DOT); break;
        case '-': AddToken(TokenType::MINUS); break;
        case '+': AddToken(TokenType::PLUS); break;
        case ';': AddToken(TokenType::SEMICOLON); break;
        case '*': AddToken(TokenType::STAR); break; 
        case '!':
            AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;

        case '/':
            if (Match('/')) {
                // A comment goes until the end of the line.
                while (Peek() != '\n' && !IsEnd()) { 
                    Next();
                }
            } else {
                AddToken(TokenType::SLASH);
            }
            break;

        
        // Ignore whitespace.
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line_++;
            break;

        default:
            if (std::isdigit(c)!=0) {
                ScanNumber();
            } else if (std::isalpha(c)!=0) {
                ScanIdentifier();
            }
            Error(line_, "Unexpected character.");
            break;
    }
}

void Scanner::ScanString(){
    while (Peek() != '"' && !IsEnd()) {
        if (Peek() == '\n') {
            line_++;
        }
        Next();
    }
    if (IsEnd()) {
        Error(line_, "Unterminated string.");
        return;
    }
    // The closing ".
    Next();
    // Trim the surrounding quotes.
    std::string value{start_ + 1, current_ - 1};
    tokens_.emplace_back(TokenType::STRING, value,line_);
}

void Scanner::ScanNumber(){
    while (std::isdigit(Peek()) != 0) {
        Next();
    }
    // Look for a fractional part.
    if (Peek() == '.' && std::isdigit(PeekNext())!=0) {
        // Consume the "."
        Next();

        while (std::isdigit(Peek())!=0){ 
            Next();
        }
    }
    tokens_.emplace_back(TokenType::NUMBER,std::string{start_, current_},line_);
    
}

void Scanner::ScanIdentifier(){
    
    while (std::isalnum(Peek())!=0) {
        Next();
    }
    std::string identifier{start_, current_};
    if(Token::key_words.find(identifier)!=Token::key_words.end()){
        tokens_.emplace_back(Token::key_words[identifier],std::string{start_, current_},line_);
    }
    else{
        tokens_.emplace_back(TokenType::IDENTIFIER,std::string{start_, current_},line_);
    }
}


void Scanner::AddToken(TokenType type){
    tokens_.emplace_back(type,std::string{start_,current_},line_);
}


auto Scanner::Match(char c)->bool{
    if(IsEnd()||*current_!=c){
        return false;
    }
    current_++;
    return true;
}
auto Scanner::Peek()->char{
    if(IsEnd()){
        return '\0';
    }

    return *current_;
}
auto Scanner::PeekNext()->char{
    if(IsEnd()||current_+1==source_.end()){
        return '\0';
    }

    return *(current_+1);
}

}  // namespace lox