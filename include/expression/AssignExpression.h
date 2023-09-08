
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class AssignExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit AssignExpression(
        std::unique_ptr<Token> name_,
std::unique_ptr<AbstractExpression> expression_
    ):name_(std::move(name_)),expression_(std::move(expression_)){}
    ~AssignExpression()override=default;
    
    std::unique_ptr<Token> name_;
std::unique_ptr<AbstractExpression> expression_;
};

}  // namespace lox
    