
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class VariableExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit VariableExpression(
        std::unique_ptr<Token> name_
    ):name_(std::move(name_)){}
    ~VariableExpression()override=default;
    
    std::unique_ptr<Token> name_;
};

}  // namespace lox
    