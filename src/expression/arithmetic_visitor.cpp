#include "expression/expression_visitor/arithmetic_visitor.h"
#include "symbol/token.h"
#define FMT_HEADER_ONLY

#include "expression/expression_visitor/print_visitor.h"
#include <fmt/format.h>
#include "expression/BinaryExpression.h"
#include "expression/GroupingExpression.h"
#include "expression/LiteralExpression.h"
#include "expression/UnaryExpression.h"

namespace lox {


auto ArithmeticVisitor::Visit(BinaryExpression &exp)const->std::any{
    auto left=std::any_cast<double>(exp.left_->Accept(*this));
    auto right=std::any_cast<double>(exp.right_->Accept(*this));
    if(exp.operator_->type_==TokenType::PLUS){
        return left+right;
    }
    if(exp.operator_->type_==TokenType::MINUS){
        return left-right;
    }
    if(exp.operator_->type_==TokenType::SLASH){
        return left/right;
    }
    return left*right;
    
}


auto ArithmeticVisitor::Visit(GroupingExpression &exp)const->std::any{
    return exp.expression_->Accept(*this);
}


auto ArithmeticVisitor::Visit(LiteralExpression &exp)const->std::any{
    return exp.value_->literal_.ToDouble();
}


auto ArithmeticVisitor::Visit(UnaryExpression &exp)const->std::any{
    auto right=std::any_cast<double>(exp.right_->Accept(*this));
    if(exp.operator_->type_==TokenType::BANG){
        if(right!=0){
            return 0;
        }
        return 1;
    }
    return -right;
    
}

}  // namespace lox

