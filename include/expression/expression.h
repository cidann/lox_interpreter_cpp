#pragma once    

#include <any>
#include <memory>
#include "expression/expression_visitor/expression_visitor.h"
#include "symbol/token.h"

namespace lox {
    
class AbstractExpression{
    public:
    virtual auto Accept(const ExpressionVisitor<LoxTypes> &visitor)->LoxTypes=0;
    virtual ~AbstractExpression()=default;
};


}  // namespace lox