
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class LiteralExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit LiteralExpression(
        std::unique_ptr<Token> value_
    ):value_(std::move(value_)){}
    ~LiteralExpression()override=default;
    
    std::unique_ptr<Token> value_;
};

}  // namespace lox
    