
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class BinaryExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit BinaryExpression(
        std::unique_ptr<AbstractExpression> left_,
std::unique_ptr<Token> operator_,
std::unique_ptr<AbstractExpression> right_
    ):left_(std::move(left_)),operator_(std::move(operator_)),right_(std::move(right_)){}
    ~BinaryExpression()override=default;
    
    std::unique_ptr<AbstractExpression> left_;
std::unique_ptr<Token> operator_;
std::unique_ptr<AbstractExpression> right_;
};

}  // namespace lox
    