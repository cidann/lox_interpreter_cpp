
#pragma once

#include <memory>
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>

namespace lox {

class LiteralExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<std::any> &visitor)->std::any override{
        return visitor.Visit(*this);
    }
    explicit LiteralExpression(
        std::unique_ptr<Token> value_
    ):value_(std::move(value_)){}
    ~LiteralExpression()override=default;
    
    std::unique_ptr<Token> value_;
};

}  // namespace lox
    