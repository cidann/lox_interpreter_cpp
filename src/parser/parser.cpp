#include <algorithm>
#include <vector>
#include "expression/AssignExpression.h"
#include "expression/CallExpression.h"
#include "expression/LiteralExpression.h"
#include "statement/ExpressionStatement.h"
#include "statement/statement.h"
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
    auto expr=LogicalOr();
    if(Match(TokenType::EQUAL)){
        Advance();
        auto identifier=DynamicUniquePointerCast<VariableExpression>(std::move(expr));
        if(identifier){
            return std::make_unique<AssignExpression>(
                std::move(identifier->name_),
                Assign()
            );
        }
        throw ParserException("Expected an Identifier to be assigned to");
    }
    return expr;
}
auto Parser::LogicalOr()->AbstractExpressionRef{
    auto logical=LogicalAnd();
    while(Match(TokenType::OR)){
        logical=std::make_unique<LogicalExpression>(
            std::move(logical),
            std::make_unique<Token>(Advance()),
            LogicalAnd()
        );
    }
    return logical;
}
auto Parser::LogicalAnd()->AbstractExpressionRef{
    auto logical=Equality();
    while(Match(TokenType::AND)){
        logical=std::make_unique<LogicalExpression>(
            std::move(logical),
            std::make_unique<Token>(Advance()),
            Equality()
        );
    }
    return logical;
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
    return Call();
    
}

auto Parser::Call()->AbstractExpressionRef{ //→ primary("(" argument? ")")* ;
    auto prime=Primary();
    while(Match(TokenType::LEFT_PAREN)){
        auto left_paren=Advance();
        std::vector<AbstractExpressionRef> args;
        if(!Match(TokenType::RIGHT_PAREN)){
            args=Argument();
        }
        Consume(TokenType::RIGHT_PAREN, "expect closing parenthese to function call");
        prime=std::make_unique<CallExpression>(
            std::move(prime),
            std::make_unique<Token>(left_paren),
            std::make_unique<std::vector<AbstractExpressionRef>>(std::move(args))
        );
    }
    return prime;
}

auto Parser::Argument()->std::vector<AbstractExpressionRef>{//→ expression ("," expression)*;
    std::vector<AbstractExpressionRef> args;
    args.push_back(Expression());
    while (Match(TokenType::COMMA)) {
        auto token=Advance();
        if(args.size()>255){
            Error(token,"Function can not have more than 255 arguments");
        }
        args.push_back(Expression());
    }
    return args;
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
    if(Match(TokenType::FUN)){
        return FunctionDeclaration();
    }
    if(Match(TokenType::VAR)){
        return VariableDeclaration();
    }
    return Statement();
}

auto Parser::FunctionDeclaration()->AbstractStatementRef{
    Consume(TokenType::FUN, "Expect fun for function declaration");
    return FunctionDefintion();
}
auto Parser::FunctionDefintion()->AbstractStatementRef{
    auto name=Consume(TokenType::IDENTIFIER, "expected identifier after fun");
    Consume(TokenType::LEFT_PAREN, "Expect ( after function declartion");
    std::unique_ptr<std::vector<Token>> params=nullptr;
    if(!Match(TokenType::RIGHT_PAREN)){
        params=std::make_unique<std::vector<Token>>(Parameter());
    }
    Consume(TokenType::RIGHT_PAREN, "expected ending ) after params");
    auto body=Block();
    
    return std::make_unique<FunctionStatement>(
        std::make_unique<Token>(name),
        std::move(params),
        std::move(body)
    );
}
auto Parser::Parameter()->std::vector<Token>{
    std::vector<Token> params{Consume(TokenType::IDENTIFIER, "Expected identifiers for parameters")};
    while(Match(TokenType::COMMA)){
        auto token=Advance();
        if(params.size()>255){
            Error(token,"Max number of parameter is 255");
        }
        params.push_back(Consume(TokenType::IDENTIFIER, "Expected identifiers for parameters"));
    }

    return params;
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
    if(Match(TokenType::FOR)){
        return ForStmt();
    }
    if(Match(TokenType::WHILE)){
        return WhileStmt();
    }
    if(Match(TokenType::IF)){
        return IfStmt();
    }
    if(Match(TokenType::PRINT)){
        return PrintStmt();
    }
    if(Match(TokenType::LEFT_BRACE)){
        return Block();
    }
    return ExpressionStmt();
}

auto Parser::ForStmt()->AbstractStatementRef{
    Consume(TokenType::FOR, "For statement starts with for");
    Consume(TokenType::LEFT_PAREN, "expect ( after for ");

    AbstractStatementRef initialize=nullptr;
    if(Match(TokenType::VAR)){
        initialize=VariableDeclaration();
    }
    else if(!Match(TokenType::SEMICOLON)){
        initialize=ExpressionStmt();
    }
    else{
        Consume(TokenType::SEMICOLON, "expected semicolon after empty initializer");
    }

    AbstractExpressionRef condition=nullptr;
    if(!Match(TokenType::SEMICOLON)){
        condition=Expression();
    }
    else{
        condition=std::make_unique<LiteralExpression>(
            std::make_unique<Token>(TokenType::TRUE,"for loop default true",LoxTypes{true},Peek().line_)
        );
    }
    Consume(TokenType::SEMICOLON, "Expect semicolon after condtion expression in for loop");
    
    AbstractStatementRef tail=nullptr;
    if(!Match(TokenType::RIGHT_PAREN)){
        tail=std::make_unique<ExpressionStatement>(Expression());
    }
    Consume(TokenType::RIGHT_PAREN, "Expect ) to close for loop");

    AbstractStatementRef body=Statement();
    if(tail){
        auto new_body_stmt=std::make_unique<std::vector<AbstractStatementRef>>(2);
        (*new_body_stmt)[0]=std::move(body);
        (*new_body_stmt)[1]=std::move(tail);
        body=std::make_unique<BlockStatement>(std::move(new_body_stmt));
    }
    
    AbstractStatementRef loop=std::make_unique<WhileStatement>(std::move(condition),std::move(body));
    if(initialize){
        auto new_loop_stmt=std::make_unique<std::vector<AbstractStatementRef>>(2);
        (*new_loop_stmt)[0]=std::move(initialize);
        (*new_loop_stmt)[1]=std::move(loop);
        loop=std::make_unique<BlockStatement>(std::move(new_loop_stmt));
    }


    return loop;
    
}   

auto Parser::WhileStmt()->AbstractStatementRef{
    Consume(TokenType::WHILE, "while statement starts with while");
    Consume(TokenType::LEFT_PAREN, "expect ( after while ");
    auto condition=Expression();
    Consume(TokenType::RIGHT_PAREN, "expect ) after condition in while");
    return std::make_unique<WhileStatement>(std::move(condition),Statement());
}

auto Parser::IfStmt()->AbstractStatementRef{
    Consume(TokenType::IF, "If statement start with if token");
    Consume(TokenType::LEFT_PAREN, "If statement expect (");
    auto expression_ref=Expression();
    Consume(TokenType::RIGHT_PAREN, "If statement expect ) after expression");
    auto then_ref=Statement();
    AbstractStatementRef else_ref=nullptr;
    if(Match(TokenType::ELSE)){
        Advance();
        else_ref=Statement();
    }
    return std::make_unique<IfStatement>(std::move(expression_ref),std::move(then_ref),std::move(else_ref));
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