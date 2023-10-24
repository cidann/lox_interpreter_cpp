#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "expression/CallExpression.h"
#include "expression/ThisExpression.h"
#include "expression/expression_visitor/expression_visitor.h"
#include "parser/parser.h"
#include "statement/ClassStatement.h"
#include "statement/statement_visitor/statement_visitor.h"
#include "symbol/types.h"
#include "expression/expression_headers.h"
#include "statement/statement_headers.h"

namespace lox {
enum class ScopeType{
    FUNCTION,
    METHOD,
    CLASS,
    NONE,
};

class Resolver:public ExpressionVisitor<LoxTypes>,public StatementVisitor<LoxTypes>{

    public:
    Resolver(){scope_stack_.emplace_back();}
    
    void Resolve(const std::vector<AbstractStatementRef> &stmts) const;
    void Resolve(const std::vector<AbstractExpressionRef> &stmts) const;
    void Resolve(AbstractStatement &stmts) const;
    void Resolve(AbstractExpression &expr) const;
    void ResolveLocal(Token& variable) const;

    private:
    auto Visit(BinaryExpression &exp)const->LoxTypes override;
    auto Visit(GroupingExpression &exp)const->LoxTypes override;
    auto Visit(LiteralExpression &exp)const->LoxTypes override;
    auto Visit(UnaryExpression &exp)const->LoxTypes override;
    auto Visit(PrintStatement &exp)const->LoxTypes override;
    auto Visit(ExpressionStatement &exp)const->LoxTypes override;
    auto Visit(VariableExpression &exp)const->LoxTypes override;
    auto Visit(VariableStatement &exp)const->LoxTypes override;
    auto Visit(AssignExpression &exp)const->LoxTypes override;
    auto Visit(BlockStatement &exp)const->LoxTypes override;
    auto Visit(IfStatement &exp)const->LoxTypes override;
    auto Visit(LogicalExpression &exp)const->LoxTypes override;
    auto Visit(WhileStatement &exp)const->LoxTypes override;
    auto Visit(CallExpression &exp)const->LoxTypes override;
    auto Visit(FunctionStatement &exp)const->LoxTypes override;
    auto Visit(ReturnStatement &exp)const->LoxTypes override;
    auto Visit(ClassStatement &exp)const->LoxTypes override;
    auto Visit(GetAttributeExpression &exp)const->LoxTypes override;
    auto Visit(SetAttributeExpression &exp)const->LoxTypes override;
    auto Visit(ThisExpression &exp)const->LoxTypes override;

    void EnterScope()const;
    void ExitScope()const;
    void DeclareVariable(const std::string &name)const;
    void DefineVariable(const std::string &name)const;

    mutable std::vector<std::unordered_map<std::string, bool>> scope_stack_;
    mutable ScopeType func_scope_type_;
    mutable ScopeType class_scope_type_;
};
}  // namespace lox