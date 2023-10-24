
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class ClassStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit ClassStatement(
        std::unique_ptr<Token> name_,
std::unique_ptr<std::vector<AbstractStatementRef>> methods_
    ):name_(std::move(name_)),methods_(std::move(methods_)){}
    ~ClassStatement()override=default;
    
    std::unique_ptr<Token> name_;
std::unique_ptr<std::vector<AbstractStatementRef>> methods_;
};

}  // namespace lox
    