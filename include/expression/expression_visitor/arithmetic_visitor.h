#pragma once

#include <any>
#include <string>
#include "expression/expression_visitor/expression_visitor.h"
#include "symbol/token.h"

/*
namespace lox{
    class ArithmeticVisitor:public ExpressionVisitor<LoxLiterals> {
        public:
    
        auto Visit(BinaryExpression &exp)const->LoxLiterals override;
        auto Visit(GroupingExpression &exp)const->LoxLiterals override;
        auto Visit(LiteralExpression &exp)const->LoxLiterals override;
        auto Visit(UnaryExpression &exp)const->LoxLiterals override;

        ~ArithmeticVisitor() override =default;
    };
}  // namespace lox
*/