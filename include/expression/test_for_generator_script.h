#pragma once

#include <memory>
#include "expression/expression.h"
#include "symbol/token.h"

namespace lox {

class BinaryExpressionTest:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<std::any> &visitor)->std::any override{
        return visitor.Visit(*this);
    }
    BinaryExpression(
        std::unique_ptr<AbstractExpression> left,
        std::unique_ptr<Token> op,
        std::unique_ptr<AbstractExpression> right
    ):left_(std::move(left)),operator_(std::move(op)),right_(std::move(right)){}
    ~BinaryExpression()override=default;
    private:
    std::unique_ptr<AbstractExpression> left_;
    std::unique_ptr<Token> operator_;
    std::unique_ptr<AbstractExpression> right_;
};

}  // namespace lox