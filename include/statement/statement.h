#pragma once

#include "statement/statement_visitor/statement_visitor.h"
#include "symbol/token.h"

namespace lox {
class AbstractStatement{
    public:
    virtual auto Accept(const StatementVisitor<LoxTypes> &visitor)->LoxTypes=0;
    virtual ~AbstractStatement()=default;
};
}
