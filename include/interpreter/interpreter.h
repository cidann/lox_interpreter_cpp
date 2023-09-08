#pragma once


#include <any>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "expression/expression_visitor/expression_visitor.h"
#include "interpreter/environment.h"
#include "statement/VariableStatement.h"
#include "statement/statement_visitor/statement_visitor.h"
#include "expression/expression_visitor/print_visitor.h"
#include "parser/parser.h"
#include "symbol/token.h"

namespace lox {
    
class LoxRuntimeError:public std::runtime_error{
    public:
    LoxRuntimeError(Token  token,const std::string& msg)
        :std::runtime_error(msg),token_(std::move(token)){}
    private:
    Token token_;
};

class CurEnvGuard;
class Interpreter:public ExpressionVisitor<LoxTypes>,public StatementVisitor<LoxTypes>{
    public:
    Interpreter()=default;
    auto Interpret(std::vector<AbstractStatementRef> &statments)const->LoxTypes;

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


    auto EvaluateTruthy(LoxTypes &obj)const->bool;
    auto EvaluateEqual(LoxTypes &obj1,LoxTypes &obj2)const->bool;

    auto ToString(const LoxTypes& value)const->std::string;

    template<typename T>
    auto InterpretType(LoxTypes &obj,const Token& token)const->T;

    ~Interpreter() override =default;

    private:
    class CurEnvGuard{
        public:
            explicit CurEnvGuard(const Interpreter &interpreter);
            ~CurEnvGuard();
        private:
        const Interpreter& interpreter_;
    };
    mutable std::unique_ptr<Environment> environment_=std::make_unique<Environment>();
};



}  // namespace lox

