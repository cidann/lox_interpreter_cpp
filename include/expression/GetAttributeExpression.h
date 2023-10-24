
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class GetAttributeExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit GetAttributeExpression(
        std::unique_ptr<AbstractExpression> object_,
std::unique_ptr<Token> attribute_
    ):object_(std::move(object_)),attribute_(std::move(attribute_)){}
    ~GetAttributeExpression()override=default;
    
    std::unique_ptr<AbstractExpression> object_;
std::unique_ptr<Token> attribute_;
};

}  // namespace lox
    