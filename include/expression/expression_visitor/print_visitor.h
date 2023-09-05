#pragma once

#include <any>
#include <string>
#include "expression/expression_visitor/expression_visitor.h"


namespace lox{
    class PrintVisitor:public ExpressionVisitor<std::any> {
        public:
    
        auto Visit(BinaryExpression &exp)const->std::any override;
        auto Visit(GroupingExpression &exp)const->std::any override;
        auto Visit(LiteralExpression &exp)const->std::any override;
        auto Visit(UnaryExpression &exp)const->std::any override;

        ~PrintVisitor() override =default;
    };
}  // namespace lox