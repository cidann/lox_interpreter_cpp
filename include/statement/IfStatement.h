
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class IfStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit IfStatement(
        std::unique_ptr<AbstractExpression> condtion_,
std::unique_ptr<AbstractStatement> then_,
std::unique_ptr<AbstractStatement> else_
    ):condtion_(std::move(condtion_)),then_(std::move(then_)),else_(std::move(else_)){}
    ~IfStatement()override=default;
    
    std::unique_ptr<AbstractExpression> condtion_;
std::unique_ptr<AbstractStatement> then_;
std::unique_ptr<AbstractStatement> else_;
};

}  // namespace lox
    