
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class GroupingExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit GroupingExpression(
        std::unique_ptr<AbstractExpression> expression_
    ):expression_(std::move(expression_)){}
    ~GroupingExpression()override=default;
    
    std::unique_ptr<AbstractExpression> expression_;
};

}  // namespace lox
    