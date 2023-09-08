
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class PrintStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit PrintStatement(
        std::unique_ptr<AbstractExpression> expression_
    ):expression_(std::move(expression_)){}
    ~PrintStatement()override=default;
    
    std::unique_ptr<AbstractExpression> expression_;
};

}  // namespace lox
    