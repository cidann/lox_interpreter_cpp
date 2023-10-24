
#pragma once

#include <memory>
#include "statement/statement.h"
#include "expression/expression.h"
#include "symbol/types.h"
#include <any>
#include <vector>
#include "parser/parser.h"

namespace lox {

class SetAttributeExpression:public AbstractExpression{
    public:
    auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes override{
        return visitor.Visit(*this);
    }
    explicit SetAttributeExpression(
        std::unique_ptr<AbstractExpression> object_,
std::unique_ptr<Token> attribute_,
std::unique_ptr<AbstractExpression> value_
    ):object_(std::move(object_)),attribute_(std::move(attribute_)),value_(std::move(value_)){}
    ~SetAttributeExpression()override=default;
    
    std::unique_ptr<AbstractExpression> object_;
std::unique_ptr<Token> attribute_;
std::unique_ptr<AbstractExpression> value_;
};

}  // namespace lox
    