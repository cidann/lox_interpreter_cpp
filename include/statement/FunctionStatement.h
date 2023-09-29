
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class FunctionStatement:public AbstractStatement{
    public:
    auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit FunctionStatement(
        std::unique_ptr<Token> name_,
std::unique_ptr<std::vector<Token>> params_,
std::unique_ptr<AbstractStatement> body_
    ):name_(std::move(name_)),params_(std::move(params_)),body_(std::move(body_)){}
    ~FunctionStatement()override=default;
    
    std::unique_ptr<Token> name_;
std::unique_ptr<std::vector<Token>> params_;
std::unique_ptr<AbstractStatement> body_;
};

}  // namespace lox
    