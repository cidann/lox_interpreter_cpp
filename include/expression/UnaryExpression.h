
#pragma once

#include <memory>
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>

namespace lox {

class UnaryExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<std::any> &visitor)->std::any override{
        return visitor.Visit(*this);
    }
    explicit UnaryExpression(
        std::unique_ptr<Token> operator_,
std::unique_ptr<AbstractExpression> right_
    ):operator_(std::move(operator_)),right_(std::move(right_)){}
    ~UnaryExpression()override=default;
    
    std::unique_ptr<Token> operator_;
std::unique_ptr<AbstractExpression> right_;
};

}  // namespace lox
    