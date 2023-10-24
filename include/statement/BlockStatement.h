
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class BlockStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit BlockStatement(
        std::unique_ptr<std::vector<AbstractStatementRef>> statments_
    ):statments_(std::move(statments_)){}
    ~BlockStatement()override=default;
    
    std::unique_ptr<std::vector<AbstractStatementRef>> statments_;
};

}  // namespace lox
    