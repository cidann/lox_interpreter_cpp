
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class VariableStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit VariableStatement(
        std::unique_ptr<Token> name_,
std::unique_ptr<AbstractExpression> initializer_
    ):name_(std::move(name_)),initializer_(std::move(initializer_)){}
    ~VariableStatement()override=default;
    
    std::unique_ptr<Token> name_;
std::unique_ptr<AbstractExpression> initializer_;
};

}  // namespace lox
    