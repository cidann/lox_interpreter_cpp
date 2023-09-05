
#pragma once

#include <memory>
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>

namespace lox {

class GroupingExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<std::any> &visitor)->std::any override{
        return visitor.Visit(*this);
    }
    explicit GroupingExpression(
        std::unique_ptr<AbstractExpression> expression_
    ):expression_(std::move(expression_)){}
    ~GroupingExpression()override=default;
    
    std::unique_ptr<AbstractExpression> expression_;
};

}  // namespace lox
    