#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <vector>
#include "interpreter/environment.h"
#include "parser/parser.h"
#include "symbol/types.h"
#define FMT_HEADER_ONLY
#include "interpreter/interpreter.h"
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <utility>
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
            return InterpretMinus(left, right,*exp.operator_);
        case TokenType::SLASH:
            return InterpretDivide(left, right,*exp.operator_);
        case TokenType::STAR:
            return InterpretMultiply(left, right,*exp.operator_);
        case TokenType::PLUS:
            return InterpretAdd(left, right,*exp.operator_);
            throw LoxRuntimeError(*exp.operator_,"Expected string+string or num+num");
        case TokenType::GREATER:
            return InterpretGreater(left, right,*exp.operator_);
        case TokenType::LESS:
            return InterpretLess(left, right,*exp.operator_);
        case TokenType::GREATER_EQUAL:
            return InterpretGreater(left, right,*exp.operator_)||InterpretEqual(left, right,*exp.operator_);
        case TokenType::LESS_EQUAL:
            return InterpretLess(left, right,*exp.operator_)||InterpretEqual(left, right,*exp.operator_);

        
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
            return !InterpretTruthy(right);
        case TokenType::MINUS:
            return -InterpretType<double>(right,*exp.operator_);
        default:
            throw LoxRuntimeError(*exp.operator_,"Expected unary token");
    }
}

auto Interpreter:: Visit(PrintStatement &exp)const->LoxTypes{
    LoxTypes val=exp.expression_->Accept(*this);
    std::cout<<InterpretString(val)<<std::endl;
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
    
    return environment_->Set(exp.name_->lexeme_, exp.expression_->Accept(*this));
}


auto Interpreter::Visit(BlockStatement &exp)const->LoxTypes{
    auto env_guard=CurEnvGuard{*this};
    for(auto &stmt_ptr:*exp.statments_){
        stmt_ptr->Accept(*this);
    }
    return LoxTypes{};
}


auto Interpreter::Visit(IfStatement &exp)const->LoxTypes{
    if(InterpretTruthy(exp.condtion_->Accept(*this))){
        exp.then_->Accept(*this);
    }
    else if(exp.else_){
        exp.else_->Accept(*this);
    }
    return LoxTypes{};
}


auto Interpreter::Visit(LogicalExpression &exp)const->LoxTypes{
    if(exp.operator_->type_==TokenType::OR){
        if(InterpretTruthy(exp.left_->Accept(*this))){
            return LoxTypes{true};
        }
        return exp.right_->Accept(*this);
    }
    
    if(!InterpretTruthy(exp.left_->Accept(*this))){
        return LoxTypes{false};
    }
    return exp.right_->Accept(*this);
}

auto Interpreter::Visit(WhileStatement &exp)const->LoxTypes{
    while(InterpretTruthy(exp.condtion_->Accept(*this))){
        exp.body_->Accept(*this);
    }
    return LoxTypes{};
}


auto Interpreter::Visit(CallExpression &exp)const->LoxTypes {
    auto primary=exp.callee_->Accept(*this);
    if(std::holds_alternative<LoxCallable>(primary)){
        auto fcn=std::get<LoxCallable>(primary);
        if(fcn.arity_!=exp.args_->size()){
            throw LoxRuntimeError(*exp.paren_,"Invalid number of arguments");
        }
        std::vector<LoxTypes> primary_args(exp.args_->size());
        std::transform(
            exp.args_->begin(),
            exp.args_->end(),
            primary_args.begin(),[this](AbstractExpressionRef &arg){
                return arg->Accept(*this);
            }
        );
        CurEnvGuard env_guard{*this};
        for(std::size_t i=0;i<fcn.arity_;i++){
            environment_->Assign(fcn.params_[i].lexeme_, primary_args[i]);
        }

        return fcn(*this,primary_args);
    }
    throw LoxRuntimeError(*exp.paren_,"The value is not callable");
}


auto Interpreter::Visit(FunctionStatement &exp)const->LoxTypes{
    auto declared_fcn=[&exp](const Interpreter &interpreter,const std::vector<LoxTypes>& args)->LoxTypes{
        return exp.body_->Accept(interpreter);
    };
    auto fcn_obj=LoxCallable{declared_fcn,*exp.params_};
    environment_->Assign(exp.name_->lexeme_, fcn_obj);
    return LoxTypes{};
}

auto Interpreter::InterpretEqual(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->bool{
    return obj1==obj2;
}
auto Interpreter::InterpretLess(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->bool{
    if(std::holds_alternative<int64_t>(obj1)){
        return std::get<int64_t>(obj1)<InterpretType<int64_t>(obj2, t);
    }
    if(std::holds_alternative<double>(obj1)){
        return std::get<double>(obj1)<InterpretType<double>(obj2, t);
    }
    if(std::holds_alternative<std::string>(obj1)){
        return std::get<std::string>(obj1)<InterpretType<std::string>(obj2, t);
    }

    throw LoxRuntimeError{t,"Can't have comparsion(less,greater) with bool or nil value"};
}

auto Interpreter::InterpretGreater(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->bool{
    if(std::holds_alternative<int64_t>(obj1)){
        return std::get<int64_t>(obj1)>InterpretType<int64_t>(obj2, t);
    }
    if(std::holds_alternative<double>(obj1)){
        return std::get<double>(obj1)>InterpretType<double>(obj2, t);
    }
    if(std::holds_alternative<std::string>(obj1)){
        return std::get<std::string>(obj1)>InterpretType<std::string>(obj2, t);
    }

    throw LoxRuntimeError{t,"Can't have comparsion(less,greater) with bool or nil value"};
}

auto Interpreter::InterpretAdd(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes{
    if(std::holds_alternative<int64_t>(obj1)){
        return {std::get<int64_t>(obj1)+InterpretInt(obj2,t)};
    }
    if(std::holds_alternative<double>(obj1)){
        return {std::get<double>(obj1)+InterpretDouble(obj2,t)};
    }
    if(std::holds_alternative<std::string>(obj1)){
        return std::get<std::string>(obj1)+InterpretString(obj2);
    }
    
    throw LoxRuntimeError{t,"Can't add bool or nil"};
}

auto Interpreter::InterpretMinus(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes{
    if(IsNumber(obj1)&&IsNumber(obj2)){
        if(std::holds_alternative<int64_t>(obj1)){
            return {std::get<int64_t>(obj1)-InterpretInt(obj2,t)};
        }
        if(std::holds_alternative<double>(obj1)){
            return {std::get<double>(obj1)-InterpretDouble(obj2,t)};
        }
    }
    
    throw LoxRuntimeError{t,"Can't subtract string,bool or nil"};
}


auto Interpreter::InterpretDivide(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes{
    if(IsNumber(obj1)&&IsNumber(obj2)){
        if(std::holds_alternative<int64_t>(obj1)){
            return {std::get<int64_t>(obj1)/InterpretInt(obj2,t)};
        }
        if(std::holds_alternative<double>(obj1)){
            return {std::get<double>(obj1)/InterpretDouble(obj2,t)};
        }
    }
    
    throw LoxRuntimeError{t,"Can't divide string,bool or nil"};
}

auto Interpreter::InterpretMultiply(const LoxTypes &obj1,const LoxTypes &obj2,const Token &t)const->LoxTypes{
    if((IsNumber(obj1)||std::holds_alternative<std::string>(obj1))&&IsNumber(obj2)){
        if(std::holds_alternative<int64_t>(obj1)){
            return {std::get<int64_t>(obj1)*InterpretInt(obj2,t)};
        }
        if(std::holds_alternative<double>(obj1)){
            return {std::get<double>(obj1)*InterpretDouble(obj2,t)};
        }
        if(std::holds_alternative<std::string>(obj1)){
            auto str=std::get<std::string>(obj1);
            auto mult=InterpretInt(obj2,t);
            std::string mult_string;
            mult_string.reserve(str.size()*mult);
            for(int i=0;i<mult;i++){
                mult_string+=str;
            }
            return {mult_string};
        }
    }
    
    throw LoxRuntimeError{t,"Can't multiply bool or nil or string as multiplicand"};
}

auto Interpreter::InterpretTruthy(const LoxTypes &obj)const->bool{
    if(std::holds_alternative<std::monostate>(obj)){
        return false;
    }
    if(std::holds_alternative<bool>(obj)){
        return std::get<bool>(obj);
    }
    return true;
}


auto Interpreter::InterpretInt(const LoxTypes &obj,const Token &t)const->int64_t{
    if(std::holds_alternative<int64_t>(obj)){
        return std::get<int64_t>(obj);
    }
    if(std::holds_alternative<double>(obj)){
        return std::get<double>(obj);
    }
    if(std::holds_alternative<std::string>(obj)){
        return std::stoll(std::get<std::string>(obj));
    }
    throw LoxRuntimeError{t,"Invalid value can't evaluate bool or nil to int"};
}
auto Interpreter::InterpretDouble(const LoxTypes &obj,const Token &t)const->double{
    if(std::holds_alternative<double>(obj)){
        return std::get<double>(obj);
    }
    if(std::holds_alternative<int64_t>(obj)){
        return std::get<int64_t>(obj);
    }
    if(std::holds_alternative<std::string>(obj)){
        return std::stod(std::get<std::string>(obj));
    }
    throw LoxRuntimeError{t,"Invalid value can't evaluate bool or nil to double"};
}


template<typename T>
auto Interpreter::InterpretType(const LoxTypes &obj,const Token& token)const->T{
    if(std::holds_alternative<T>(obj)){
        return std::get<T>(obj);
    }
    throw LoxRuntimeError(token,fmt::format("expected type {}\n",typeid(T).name()));
}

auto Interpreter::InterpretString(const LoxTypes& value)const->std::string{
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

auto Interpreter::IsNumber(const LoxTypes &obj)const->bool{
    return std::holds_alternative<int64_t>(obj)||std::holds_alternative<double>(obj);
}

Interpreter::CurEnvGuard::CurEnvGuard(const Interpreter &interpreter):interpreter_(interpreter){
    interpreter_.environment_=std::make_unique<Environment>(std::move(interpreter_.environment_));
}
Interpreter::CurEnvGuard::~CurEnvGuard(){
    interpreter_.environment_=std::move(interpreter_.environment_->Prev());
}


}  // namespace lox