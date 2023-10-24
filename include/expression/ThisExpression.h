
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class ThisExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit ThisExpression(
        std::unique_ptr<Token> keyword_
    ):keyword_(std::move(keyword_)){}
    ~ThisExpression()override=default;
    
    std::unique_ptr<Token> keyword_;
};

}  // namespace lox
    