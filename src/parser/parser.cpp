#include <vector>
#include "expression/AssignExpression.h"
#include "statement/statement_visitor/statement_visitor.h"
#define FMT_HEADER_ONLY
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
#include "statement/statement_headers.h"

namespace lox {

Parser::Parser(std::vector<Token> tokens):
    tokens_(std::move(tokens)),cur_(tokens_.begin()){}


auto Parser::Check(TokenType type)->bool{
    return Peek().type_==type;
}

auto Parser::Parse()->std::vector<AbstractStatementRef>{
    std::vector<AbstractStatementRef> statements;
    while(!IsEnd()){
        statements.push_back(Declaration());
    }
    return statements;

}
auto Parser::Expression()->AbstractExpressionRef{
    return Assign();
}

auto Parser::Assign()->AbstractExpressionRef{
    auto expr=Equality();
    if(Match(TokenType::EQUAL)){
        auto equal=Advance();
        auto var_expr=DynamicUniquePointerCast<VariableExpression>(std::move(expr));
        if(var_expr){
            return std::make_unique<AssignExpression>(
                std::move(var_expr->name_),
                Assign()
            );
        }
        throw ParserException("Only l-value(variables) can be assigned to");
    }
    return expr;
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
    if(Match(TokenType::IDENTIFIER)){
        return std::make_unique<VariableExpression>(
            std::make_unique<Token>(Advance())
        );
    }
    
    Consume(TokenType::LEFT_PAREN,"Unexpected token");
    auto expr=Expression();
    Consume(TokenType::RIGHT_PAREN,"Expected closing parenthese");
    return expr;
    

}

auto Parser::Declaration()->AbstractStatementRef{
    if(Match(TokenType::VAR)){
        return VariableDeclaration();
    }
    return Statement();
}

auto Parser::VariableDeclaration()->AbstractStatementRef{
    Consume(TokenType::VAR, "Expected var declaration");
    Token identifier=Consume(TokenType::IDENTIFIER, "Expected identifier after var");
    if(Match(TokenType::EQUAL)){
        Advance();
        auto expr=Expression();
        Consume(TokenType::SEMICOLON,"Expected ; after var declaration");
        return std::make_unique<VariableStatement>(
            std::make_unique<Token>(std::move(identifier)),
            std::move(expr)
        );
    }
    int line=Consume(TokenType::SEMICOLON,"Expected ; after var declaration").line_;
    return std::make_unique<VariableStatement>(
        std::make_unique<Token>(std::move(identifier)),
        std::make_unique<LiteralExpression>(
            std::make_unique<Token>(TokenType::NIL,"",line)
        )
    );
}

auto Parser::Statement()->AbstractStatementRef{
    if(Match(TokenType::PRINT)){
        return PrintStmt();
    }
    if(Match(TokenType::LEFT_BRACE)){
        return Block();
    }
    return ExpressionStmt();
}
auto Parser::PrintStmt()->AbstractStatementRef{
    Consume(TokenType::PRINT, "expected print statement");
    auto print_stmt= std::make_unique<PrintStatement>(Expression());
    Consume(TokenType::SEMICOLON, "expected semicolon to end satement");
    return print_stmt;
}
auto Parser::ExpressionStmt()->AbstractStatementRef{
    auto expr_stmt= std::make_unique<ExpressionStatement>(Expression());
    Consume(TokenType::SEMICOLON, "expected semicolon to end satement");
    return expr_stmt;
}

auto Parser::Block()->AbstractStatementRef{
    Consume(TokenType::LEFT_BRACE, "Interpreter error, should be { here");
    std::vector<AbstractStatementRef> stmts;
    while (!Match(TokenType::RIGHT_BRACE)&&!IsEnd()) {
        stmts.push_back(Declaration());
    }
    Consume(TokenType::RIGHT_BRACE, "Expected } here!");
    BlockStatement a{{}};
    return std::make_unique<BlockStatement>(
        std::make_unique< std::vector<AbstractStatementRef>>(std::move(stmts))
    );
}


auto Parser::Advance()->Token{
    if(IsEnd()){
        return *cur_;
    }
    return *(cur_++);
}

auto Parser::Consume(TokenType type,const std::string &msg)->Token{
    if(!Check(type)){
        throw Error(Peek(), msg);
    }
    return Advance();
}

auto Parser::IsEnd()->bool{
    return cur_==tokens_.end()||(*cur_).type_==TokenType::EOF;
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

}  // namespace lox