
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>

namespace lox {

class VarStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit VarStatement(
        std::unique_ptr<Token> name_,
std::unique_ptr<AbstractExpression> initializer_
    ):name_(std::move(name_)),initializer_(std::move(initializer_)){}
    ~VarStatement()override=default;
    
    std::unique_ptr<Token> name_;
std::unique_ptr<AbstractExpression> initializer_;
};

}  // namespace lox
    