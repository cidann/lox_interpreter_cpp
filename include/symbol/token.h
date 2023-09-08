#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>


#undef EOF

namespace lox {
class LoxLiterals;
using LoxTypes=std::variant<std::string,bool,int64_t,double,std::monostate>;
enum class TokenType{
    // Single-character tokens.
    LEFT_PAREN=0, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,EOF,

    count
};

/*
class LoxTypes{
    public:
    LoxTypes():value_(std::monostate{}){}
    explicit LoxTypes(
        LoxLiterals val
    ):value_(std::move(val)){}
    
    auto Add(const LoxTypes &other,const Token &tok)->LoxTypes;

    static auto ToString(const LoxTypes& value)->std::string;

    static auto ToDouble(const LoxTypes& value)->double;
    static auto ToInt(const LoxTypes& value)->int64_t;

    LoxLiterals value_;
};
*/

class Token{
    public:
    inline static std::array<const std::string, static_cast<int>(TokenType::count)> token_type_map{
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
        "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",
        "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL",
        "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
        "IDENTIFIER", "STRING", "NUMBER",
        "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
        "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE", "EOF"
    };
    inline static std::unordered_map<std::string,TokenType> key_words{
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
    };

    Token(TokenType type,std::string lexeme,int line);
    Token(TokenType type,std::string lexeme,LoxTypes literal,int line);
    Token(Token &&token)=default;
    Token(const Token &token)=default;
    

    auto ToString()->std::string;


    TokenType type_;
    std::string lexeme_;
    LoxTypes literal_;
    int line_; 
};


}  // namespace lox