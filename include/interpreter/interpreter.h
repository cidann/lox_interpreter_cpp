#pragma once


#include <any>
#include <cstdint>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "expression/expression_visitor/expression_visitor.h"
#include "interpreter/environment.h"
#include "statement/FunctionStatement.h"
#include "statement/ReturnStatement.h"
#include "statement/VariableStatement.h"
#include "statement/statement_visitor/statement_visitor.h"
#include "parser/parser.h"
#include "symbol/token.h"
#include "symbol/types.h"

namespace lox {
    
class LoxRuntimeError:public std::runtime_error{
    public:
    LoxRuntimeError(Token  token,const std::string& msg)
        :std::runtime_error(msg),token_(std::move(token)){}
    private:
    Token token_;
};

class LoxReturn:public std::runtime_error{
    public:
    explicit LoxReturn(LoxTypes return_value):std::runtime_error(""),return_value_(std::move(return_value)){}
    LoxTypes return_value_;
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
    auto Visit(IfStatement &exp)const->LoxTypes override;
    auto Visit(LogicalExpression &exp)const->LoxTypes override;
    auto Visit(WhileStatement &exp)const->LoxTypes override;
    auto Visit(CallExpression &exp)const->LoxTypes override;
    auto Visit(FunctionStatement &exp)const->LoxTypes override;
    auto Visit(ReturnStatement &exp)const->LoxTypes override;

    auto InterpretEqual(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->bool;
    auto InterpretLess(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->bool;
    auto InterpretGreater(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->bool;
    auto InterpretAdd(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes;
    auto InterpretMinus(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes;
    auto InterpretDivide(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes;
    auto InterpretMultiply(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes;
    auto InterpretTruthy(const LoxTypes &obj)const->bool;
    auto InterpretInt(const LoxTypes &obj,const Token &t)const->int64_t;
    auto InterpretDouble(const LoxTypes &obj,const Token &t)const->double;
    auto InterpretString(const LoxTypes& value)const->std::string;

    template<typename T>
    auto InterpretType(const LoxTypes &obj,const Token& token)const->T;

    auto IsNumber(const LoxTypes &obj)const->bool;

    ~Interpreter() override =default;

    private:
    class CurEnvGuard{
        public:
            explicit CurEnvGuard(const Interpreter &interpreter,const std::shared_ptr<Environment> &scope_image=nullptr);
            ~CurEnvGuard();
        private:
        const Interpreter& interpreter_;
        std::shared_ptr<Environment> old_;
    };
    mutable std::shared_ptr<Environment> environment_=std::make_shared<Environment>();
};



}  // namespace lox

