#include "parser/parser.h"
#include <fmt/format.h>
#include <iostream>
#include <memory>
#include <utility>
#include "expression/expression.h"
#include "expression/expression_visitor/expression_visitor.h"
#include "helper/helper.h"
#include "symbol/token.h"
#include "expression/expression_headers.h"

namespace lox {

Parser::Parser(std::vector<Token> tokens):
    tokens_(std::move(tokens)),cur_(tokens_.begin()){}

auto Parser::Advance()->Token{
    if(IsEnd()){
        return *cur_;
    }
    return *(cur_++);
}
auto Parser::IsEnd()->bool{
    return cur_==tokens_.end();
}
auto Parser::Peek()->Token{
    return *cur_;
}
auto Parser::Previous()->Token{
    if(cur_==tokens_.begin()){
        return *cur_;
    }
    return *(cur_-1);
}



auto Parser::Check(TokenType type)->bool{
    return Peek().type_==type;
}


auto Parser::Expression()->AbstractExpressionRef{
    return Equality();
}
auto Parser::Equality()->AbstractExpressionRef{
    AbstractExpressionRef expr=Comparison();
    while (Match(TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL)) {
        expr=std::make_unique<BinaryExpression>(
            std::move(expr),
            std::make_unique<Token>(Advance()),
            Comparison()
        );
    }
    return expr;
}
auto Parser::Comparison()->AbstractExpressionRef{
    AbstractExpressionRef expr=Term();
    while (
        Match(
            TokenType::LESS,
            TokenType::LESS_EQUAL,
            TokenType::GREATER,
            TokenType::GREATER_EQUAL
        )
    ) {
        expr=std::make_unique<BinaryExpression>(
            std::move(expr),
            std::make_unique<Token>(Advance()),
            Term()
        );
    }
    return expr;
}
auto Parser::Term()->AbstractExpressionRef{
    AbstractExpressionRef expr=Factor();
    while (Match(TokenType::MINUS,TokenType::PLUS)) {
        expr=std::make_unique<BinaryExpression>(
            std::move(expr),
            std::make_unique<Token>(Advance()),
            Factor()
        );
    }
    return expr;
}
auto Parser::Factor()->AbstractExpressionRef{
    AbstractExpressionRef expr=Unary();
    while (Match(TokenType::SLASH,TokenType::STAR)) {
        expr=std::make_unique<BinaryExpression>(
            std::move(expr),
            std::make_unique<Token>(Advance()),
            Unary()
        );
    }
    return expr;
}
auto Parser::Unary()->AbstractExpressionRef{
    if(Match(TokenType::BANG,TokenType::MINUS)){
        return std::make_unique<UnaryExpression>(
            std::make_unique<Token>(Advance()),
            Unary()
        );
    }
    return Primary();
    
}
auto Parser::Primary()->AbstractExpressionRef{
    if(
        Match(
            TokenType::NUMBER,
            TokenType::STRING,
            TokenType::TRUE,
            TokenType::FALSE,
            TokenType::NIL
        )        
    ){
        return std::make_unique<LiteralExpression>(
            std::make_unique<Token>(Advance())
        );
    }
    
    if(Match(TokenType::LEFT_PAREN)){
        Advance();
        auto expr=Expression();
        if(Match(TokenType::RIGHT_PAREN)){
            Advance();
            return expr;
        }
    }
    
    auto tok=Advance();
    Error(tok.line_, fmt::format("Error while parsing lexeme{}\n",tok.lexeme_));
    exit(64);
}

}  // namespace lox