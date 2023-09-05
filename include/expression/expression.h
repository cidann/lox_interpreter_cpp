#pragma once    

#include <any>
#include <memory>
#include "expression/expression_visitor/expression_visitor.h"

namespace lox {
    
class AbstractExpression{
    public:
    virtual auto Accept(const ExpressionVisitor<std::any> &visitor)->std::any=0;
    virtual ~AbstractExpression()=default;
};


}  // namespace lox