#define FMT_HEADER_ONLY

#include "expression/expression_visitor/print_visitor.h"
#include <fmt/format.h>
#include "expression/BinaryExpression.h"
#include "expression/GroupingExpression.h"
#include "expression/LiteralExpression.h"
#include "expression/UnaryExpression.h"

namespace lox {


auto PrintVisitor::Visit(BinaryExpression &exp)const->std::any{
    return fmt::format(
        "{} {} {}",
        std::any_cast<std::string>(exp.left_->Accept(*this)),
        exp.operator_->lexeme_,
        std::any_cast<std::string>(exp.right_->Accept(*this))
    );
}


auto PrintVisitor::Visit(GroupingExpression &exp)const->std::any{
    return fmt::format(
        "({})",
        std::any_cast<std::string>(exp.expression_->Accept(*this))
    );
}


auto PrintVisitor::Visit(LiteralExpression &exp)const->std::any{
    return fmt::format(
        "{}",
        exp.value_->ToString()
    );
}


auto PrintVisitor::Visit(UnaryExpression &exp)const->std::any{
    return fmt::format(
        "{} {}",
        exp.operator_->lexeme_,
        std::any_cast<std::string>(exp.right_->Accept(*this))
    );
}

}  // namespace lox

