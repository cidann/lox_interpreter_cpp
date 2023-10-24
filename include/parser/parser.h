#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "expression/expression.h"
#include "helper/helper.h"
#include "statement/statement.h"
#include "symbol/token.h"


namespace lox {
    
using AbstractExpressionRef=std::unique_ptr<AbstractExpression>;
using AbstractStatementRef=std::unique_ptr<AbstractStatement>;
using TokenRef = std::unique_ptr<Token>;

class ParserException: public std::runtime_error{
    public:
    ParserException():std::runtime_error(""){}
    explicit ParserException(const std::string& msg):std::runtime_error(msg){}
};

class Parser{
    public:
        explicit Parser(std::vector<Token> tokens);
        auto Advance()->Token;
        auto Consume(TokenType type,const std::string &msg)->Token;
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

        auto Parse()->std::vector<AbstractStatementRef>;
        //Context free language rules from low to high priority
        auto Expression()->AbstractExpressionRef; //→ assigment ;
        auto Assign()->AbstractExpressionRef;//→ IDENTIFIER "=" assignment | logical_or ;
        auto LogicalOr()->AbstractExpressionRef; //logical_or-> logical_and ("or" logical_and)*;
        auto LogicalAnd()->AbstractExpressionRef; //logical_and-> Equality ("and" Equality)*;
        auto Equality()->AbstractExpressionRef; //→ comparison ( ( "!=" | "==" ) comparison )* ;
        auto Comparison()->AbstractExpressionRef; //→ term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
        auto Term()->AbstractExpressionRef; //→ factor ( ( "-" | "+" ) factor )* ;
        auto Factor()->AbstractExpressionRef; //→ unary ( ( "/" | "*" ) unary )* ;
        auto Unary()->AbstractExpressionRef; //→ ( "!" | "-" ) unary | call ;
        auto Call()->AbstractExpressionRef; //→ primary("(" argument? ")"| "." IDENTIFIER )* ;
        auto Argument()->std::vector<AbstractExpressionRef>; //→ expression ("," expression)*;
        auto Primary()->AbstractExpressionRef; //→ NUMBER | STRING | "true" | "false" | "nil" |IDENTIFIER| "(" expression ")" ;
        
        auto Declaration()->AbstractStatementRef; // declaration → classDecl | funDecl | varDecl | statement ;
        auto ClassDeclaration()->AbstractStatementRef; // "class" IDENTIFIER "{" functionDef* "}" ;
        auto FunctionDeclaration()->AbstractStatementRef;// "fun" functionDef
        auto FunctionDefintion()->AbstractStatementRef; //IDENTIFIER "(" parameters? ")" block;
        auto Parameter()->std::vector<Token>; //→ IDENTIFIER ( "," IDENTIFIER )* ;
        auto VariableDeclaration()->AbstractStatementRef; //"var" IDENTIFIER ( "=" expression )? ";" ;
        auto Statement()->AbstractStatementRef; // statement → returnStmt|forStmt|whileStmt|ifStmt |exprStmt | printStmt |block;
        auto ReturnStmt()->AbstractStatementRef;// "return" expression? ";" ;
        auto ForStmt()->AbstractStatementRef; // "for" "(" varDecl|exprStmt|";" expression? ";" expression? ")" statement;
        auto WhileStmt()->AbstractStatementRef;// whileStmt-> "while" "("expression ")" statement ;
        auto IfStmt()->AbstractStatementRef; // ifStmt → "if" "("expression")" statement ("else" statement)?;
        auto PrintStmt()->AbstractStatementRef;// printStmt → "print" expression ";" ;
        auto ExpressionStmt()->AbstractStatementRef; // exprStmt → expression ";" ;
        auto Block()->AbstractStatementRef; //"{" declaration* "}" ;

        auto FinishCall(AbstractExpressionRef&& caller)->AbstractExpressionRef;

        static auto Error(const Token &token, const std::string &message)->ParserException{
            lox::Error(token,message);
            return ParserException{};
        }
        
    private:
    std::vector<Token> tokens_;
    std::vector<Token>::iterator cur_;
};



}  // namespace lox
