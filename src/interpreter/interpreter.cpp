#include <memory>
#include <ostream>
#include "interpreter/environment.h"
#define FMT_HEADER_ONLY
#include "interpreter/interpreter.h"
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <variant>
#include "expression/expression_headers.h"
#include "expression/expression.h"
#include "symbol/token.h"
#include "statement/statement_headers.h"

namespace lox {

auto Interpreter::Interpret(std::vector<AbstractStatementRef> &statments)const->LoxTypes{
    for(auto &stmt:statments){
        stmt->Accept(*this);
    }
    return LoxTypes{};
}

auto Interpreter:: Visit(BinaryExpression &exp) const->LoxTypes{
    auto left=exp.left_->Accept(*this);
    auto right=exp.right_->Accept(*this);
    switch (exp.operator_->type_) {
        case TokenType::MINUS:
            return InterpretType<double>(left,*exp.operator_)-InterpretType<double>(right,*exp.operator_);
        case TokenType::SLASH:
            return InterpretType<double>(left,*exp.operator_)/InterpretType<double>(right,*exp.operator_);
        case TokenType::STAR:
            return InterpretType<double>(left,*exp.operator_)*InterpretType<double>(right,*exp.operator_);
        case TokenType::PLUS:
            if(std::holds_alternative<std::string>(left)&&std::holds_alternative<std::string>(right)){
                return std::get<std::string>(left)+std::get<std::string>(right);
            }
            if(std::holds_alternative<double>(left)&&std::holds_alternative<double>(right)){
                return std::get<double>(left)+std::get<double>(right);
            }
            if(std::holds_alternative<int64_t>(left)&&std::holds_alternative<int64_t>(right)){
                return std::get<int64_t>(left)+std::get<int64_t>(right);
            }
            throw LoxRuntimeError(*exp.operator_,"Expected string+string or num+num");
        case TokenType::GREATER:
            return InterpretType<double>(left,*exp.operator_)>InterpretType<double>(right,*exp.operator_);
        case TokenType::LESS:
            return InterpretType<double>(left,*exp.operator_)<InterpretType<double>(right,*exp.operator_);
        case TokenType::GREATER_EQUAL:
            return InterpretType<double>(left,*exp.operator_)>=InterpretType<double>(right,*exp.operator_);
        case TokenType::LESS_EQUAL:
            return InterpretType<double>(left,*exp.operator_)<=InterpretType<double>(right,*exp.operator_);

        
        default:
            throw LoxRuntimeError(*exp.operator_,"Expected unary token");
    }
}
auto Interpreter:: Visit(GroupingExpression &exp)const->LoxTypes{
    return exp.expression_->Accept(*this);
}
auto Interpreter:: Visit(LiteralExpression &exp)const->LoxTypes{
    return exp.value_->literal_;
}
auto Interpreter:: Visit(UnaryExpression &exp)const->LoxTypes{
    auto right=exp.right_->Accept(*this);
    switch (exp.operator_->type_) {
        case TokenType::BANG:
            return !EvaluateTruthy(right);
        case TokenType::MINUS:
            return -InterpretType<double>(right,*exp.operator_);
        default:
            throw LoxRuntimeError(*exp.operator_,"Expected unary token");
    }
}

auto Interpreter:: Visit(PrintStatement &exp)const->LoxTypes{
    LoxTypes val=exp.expression_->Accept(*this);
    std::cout<<ToString(val)<<std::endl;
    return LoxTypes{};
}
auto Interpreter:: Visit(ExpressionStatement &exp)const->LoxTypes{
    exp.expression_->Accept(*this);
    return LoxTypes{};
}


auto Interpreter::Visit(VariableExpression &exp)const->LoxTypes{
    if(!environment_->Contains(exp.name_->lexeme_)){
        throw LoxRuntimeError(*exp.name_,"Use of undefined variable");
    }
    return environment_->Get(exp.name_->lexeme_);
}
auto Interpreter::Visit(VariableStatement &exp)const->LoxTypes{
    LoxTypes val=exp.initializer_->Accept(*this);
    environment_->Assign(exp.name_->lexeme_, val);
    return LoxTypes{};
}

auto Interpreter::Visit(AssignExpression &exp)const->LoxTypes{
    if(!environment_->Contains(exp.name_->lexeme_)){
        throw LoxRuntimeError(*exp.name_,"Use of undefined variable");
    }
    
    auto var=environment_->Get(exp.name_->lexeme_);
    var=exp.expression_->Accept(*this);
    return var;
}


auto Interpreter::Visit(BlockStatement &exp)const->LoxTypes{
    auto env_guard=CurEnvGuard{*this};
    for(auto &stmt_ptr:*exp.statments_){
        stmt_ptr->Accept(*this);
    }
    return LoxTypes{};
}


auto Interpreter::EvaluateTruthy(LoxTypes &obj)const->bool{
    if(std::holds_alternative<std::monostate>(obj)){
        return false;
    }
    if(std::holds_alternative<bool>(obj)){
        return std::get<bool>(obj);
    }
    return true;
}

auto Interpreter::EvaluateEqual(LoxTypes &obj1,LoxTypes &obj2)const->bool{
    return obj1==obj2;
}

template<typename T>
auto Interpreter::InterpretType(LoxTypes &obj,const Token& token)const->T{
    if(std::holds_alternative<T>(obj)){
        return std::get<T>(obj);
    }
    throw LoxRuntimeError(token,fmt::format("expected type {}\n",typeid(T).name()));
}

auto Interpreter::ToString(const LoxTypes& value)const->std::string{
    if(std::holds_alternative<std::string>(value)){
        return std::get<std::string>(value);
    }
    if(std::holds_alternative<bool>(value)){
        return std::get<bool>(value)? "true":"false";
    }
    if(std::holds_alternative<int64_t>(value)){
        return std::to_string(std::get<int64_t>(value));
    }
    if(std::holds_alternative<double>(value)){
        return std::to_string(std::get<double>(value));
    }
    return "nil";
}

Interpreter::CurEnvGuard::CurEnvGuard(const Interpreter &interpreter):interpreter_(interpreter){
    interpreter_.environment_=std::make_unique<Environment>(std::move(interpreter_.environment_));
}
Interpreter::CurEnvGuard::~CurEnvGuard(){
    interpreter_.environment_=std::move(interpreter_.environment_->Prev());
}


}  // namespace lox