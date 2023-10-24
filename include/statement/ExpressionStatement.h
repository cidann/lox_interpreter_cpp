
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class ExpressionStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit ExpressionStatement(
        std::unique_ptr<AbstractExpression> expression_
    ):expression_(std::move(expression_)){}
    ~ExpressionStatement()override=default;
    
    std::unique_ptr<AbstractExpression> expression_;
};

}  // namespace lox
    