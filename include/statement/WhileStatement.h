
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class WhileStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit WhileStatement(
        std::unique_ptr<AbstractExpression> condtion_,
std::unique_ptr<AbstractStatement> body_
    ):condtion_(std::move(condtion_)),body_(std::move(body_)){}
    ~WhileStatement()override=default;
    
    std::unique_ptr<AbstractExpression> condtion_;
std::unique_ptr<AbstractStatement> body_;
};

}  // namespace lox
    