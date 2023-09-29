
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class CallExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit CallExpression(
        std::unique_ptr<AbstractExpression> callee_,
std::unique_ptr<Token> paren_,
std::unique_ptr<std::vector<AbstractExpressionRef>> args_
    ):callee_(std::move(callee_)),paren_(std::move(paren_)),args_(std::move(args_)){}
    ~CallExpression()override=default;
    
    std::unique_ptr<AbstractExpression> callee_;
std::unique_ptr<Token> paren_;
std::unique_ptr<std::vector<AbstractExpressionRef>> args_;
};

}  // namespace lox
    