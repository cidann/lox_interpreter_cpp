#pragma once

#include <memory>
#include <vector>
#include "expression/expression.h"
#include "symbol/token.h"


namespace lox {
    
using AbstractExpressionRef=std::unique_ptr<AbstractExpression>;
using TokenRef = std::unique_ptr<Token>;

class Parser{
    public:
        explicit Parser(std::vector<Token> tokens);
        auto Advance()->Token;
        auto IsEnd()->bool;
        auto Peek()->Token;
        auto Previous()->Token;

        template<typename T,typename ...Ts>
        auto Match(T type,Ts...types)->bool{
            if(Check(type)){
                return true;
            }
            return Match(types...);
        }

        auto Match()->bool{return false;}
        auto Check(TokenType type)->bool;

        //Context free language rules from low to high priority
        auto Expression()->AbstractExpressionRef; //→ equality ;
        auto Equality()->AbstractExpressionRef; //→ comparison ( ( "!=" | "==" ) comparison )* ;
        auto Comparison()->AbstractExpressionRef; //→ term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        auto Term()->AbstractExpressionRef; //→ factor ( ( "-" | "+" ) factor )* ;
        auto Factor()->AbstractExpressionRef; //→ unary ( ( "/" | "*" ) unary )* ;
        auto Unary()->AbstractExpressionRef; //→ ( "!" | "-" ) unary | primary ;
        auto Primary()->AbstractExpressionRef; //→ NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;

    private:
    std::vector<Token> tokens_;
    std::vector<Token>::iterator cur_;
};




}  // namespace lox
