
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class ReturnStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit ReturnStatement(
        std::unique_ptr<Token> keyword_,
std::unique_ptr<AbstractExpression> initializer_
    ):keyword_(std::move(keyword_)),initializer_(std::move(initializer_)){}
    ~ReturnStatement()override=default;
    
    std::unique_ptr<Token> keyword_;
std::unique_ptr<AbstractExpression> initializer_;
};

}  // namespace lox
    