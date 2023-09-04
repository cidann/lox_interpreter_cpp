#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#undef EOF

namespace lox {

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

class Literal{
    public:
    Literal():value_(std::monostate{}){}
    explicit Literal(
        std::variant<std::string,bool,int64_t,double,std::monostate> val
    ):value_(std::move(val)){}
    
    auto ToString()->std::string{
        if(std::holds_alternative<std::string>(value_)){
            return std::get<std::string>(value_);
        }
        if(std::holds_alternative<bool>(value_)){
            return std::get<bool>(value_)? "true":"false";
        }
        if(std::holds_alternative<int64_t>(value_)){
            return std::to_string(std::get<int64_t>(value_));
        }
        if(std::holds_alternative<std::string>(value_)){
            return std::to_string(std::get<double>(value_));
        }
        return "nil";
    }
    std::variant<std::string,bool,int64_t,double,std::monostate> value_;
};

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
    
    auto ToString()->std::string;


    TokenType type_;
    std::string lexeme_;
    Literal literal_;
    int line_; 
};

}  // namespace lox