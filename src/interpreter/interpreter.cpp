#include <unordered_map>
#include "helper/helper.h"
#include "statement/ClassStatement.h"
#include "statement/FunctionStatement.h"
#define FMT_HEADER_ONLY

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <vector>
#include "interpreter/environment.h"
#include "parser/parser.h"
#include "symbol/types.h"
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
#include "function/loxBuiltIn.h"

namespace lox {

auto Interpreter::Interpret(std::vector<AbstractStatementRef> &statments)const->LoxTypes{
    Box<LoxCallable> clock_builtin{LoxFunc{"clock",LoxClock,{},environment_,false}};
    environment_->Assign("clock", clock_builtin);
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
    return std::visit([](auto &v)->LoxTypes{return v;},exp.value_->literal_);
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
    return environment_->GetAt(exp.name_->start_scope_,exp.name_->lexeme_);
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
    
    return environment_->SetAt(exp.name_->start_scope_,exp.name_->lexeme_, exp.expression_->Accept(*this));
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
    if(std::holds_alternative<Box<LoxCallable>>(primary)){
        auto fcn=std::get<Box<LoxCallable>>(primary);
        /*Work around since I do not bind args in function but rather in interpreter before calling function
        and environment holding this should be before environment holding args.
        */
        auto class_ptr=dynamic_cast<LoxClass*>(fcn.Get());
        if(class_ptr!=nullptr){
            fcn->env_=std::make_shared<Environment>(std::move(fcn->env_));
            fcn->env_->Assign("this", std::make_shared<LoxInstance>(*class_ptr));
        }
        if(fcn->Arity()!=exp.args_->size()){
            throw LoxRuntimeError(*exp.paren_,"Invalid number of arguments");
        }
        auto primary_args=InterpretArgs(exp.args_);
        return InterpretCall(*fcn, primary_args);
    }
    throw LoxRuntimeError(*exp.paren_,"The value is not callable");
}

auto Interpreter::Visit(FunctionStatement &exp)const->LoxTypes{
    auto fcn_obj=Box<LoxCallable>{InterpretFunction(exp)};
    environment_->Assign(exp.name_->lexeme_, LoxTypes{fcn_obj});
    return fcn_obj;
}


auto Interpreter::Visit(ReturnStatement &exp)const->LoxTypes{
    if(!exp.initializer_){
        throw LoxReturn{LoxTypes{}};
    }
    throw LoxReturn{exp.initializer_->Accept(*this)};
}


auto Interpreter::Visit(ClassStatement &exp)const->LoxTypes{
    auto new_class=LoxClass{exp.name_->lexeme_,{},environment_,InterpretClassMethods(exp)};
    
    environment_->Assign(exp.name_->lexeme_, Box<LoxCallable>{std::move(new_class)});
    return {};
}

auto Interpreter::Visit(GetAttributeExpression &exp)const->LoxTypes{
    auto object=exp.object_->Accept(*this);
    if(std::holds_alternative<std::shared_ptr<LoxInstance>>(object)){
        auto instance=std::get<std::shared_ptr<LoxInstance>>(object);
        if(instance->fields_.find(exp.attribute_->lexeme_)!=instance->fields_.end()){
            return instance->fields_[exp.attribute_->lexeme_];
        }
        if(instance->loxclass_.methods_.find(exp.attribute_->lexeme_)!=instance->loxclass_.methods_.end()){
            auto &method=instance->loxclass_.methods_[exp.attribute_->lexeme_];
            method->env_=std::make_shared<Environment>(std::move(method->env_));
            method->env_->Assign("this", instance);
            return method;
        }
        throw LoxRuntimeError(*exp.attribute_,"Undefined property");
    }
    throw LoxRuntimeError(*exp.attribute_,"Only instance have property");
}

auto Interpreter::Visit(SetAttributeExpression &exp)const->LoxTypes{
    auto object=exp.object_->Accept(*this);
    if(std::holds_alternative<std::shared_ptr<LoxInstance>>(object)){
        auto instance=std::get<std::shared_ptr<LoxInstance>>(object);
        auto value=exp.value_->Accept(*this);
        instance->fields_[exp.attribute_->lexeme_]=value;
        return instance;
    }
    throw LoxRuntimeError(*exp.attribute_,"Only instance have property");
}


auto Interpreter::Visit(ThisExpression &exp)const->LoxTypes{
    return environment_->GetAt(exp.keyword_->start_scope_,exp.keyword_->lexeme_ );
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
    if(std::holds_alternative<std::monostate>(value)){
        return "nil";
    }
    if(std::holds_alternative<Box<LoxCallable>>(value)){
        return std::get<Box<LoxCallable>>(value)->ToString();
    }
    if(std::holds_alternative<std::shared_ptr<LoxInstance>>(value)){
        return std::get<std::shared_ptr<LoxInstance>>(value)->ToString();
    }
    throw std::runtime_error("Unkown type bug occured");
}

auto Interpreter::IsNumber(const LoxTypes &obj)const->bool{
    return std::holds_alternative<int64_t>(obj)||std::holds_alternative<double>(obj);
}


auto Interpreter::InterpretArgs(const std::unique_ptr<std::vector<AbstractExpressionRef>>& args)const->std::vector<LoxTypes>{
    std::vector<LoxTypes> primary_args(args->size());
    std::transform(
        args->begin(),
            args->end(),
        primary_args.begin(),[this](AbstractExpressionRef &arg){
            return arg->Accept(*this);
        }
    );
    return primary_args;
}

auto Interpreter::InterpretCall(const LoxCallable& fcn, const std::vector<LoxTypes>& args)const->LoxTypes{
    CurEnvGuard env_guard{*this,fcn.env_};

    auto& fcn_params=fcn.Param();
    for(std::size_t i=0;i<fcn.Arity();i++){
        environment_->Assign(fcn_params[i].lexeme_, args[i]);
    }
    LoxTypes return_val;
    try{
        fcn(*this,args);
    }
    catch(LoxReturn r){
        return_val=r.return_value_;
    }
    if(fcn.initializer_&&fcn.name_=="init"){
        return_val=environment_->Get("this");
    }
    return return_val;
}


auto Interpreter::InterpretFunction(const FunctionStatement &exp,bool is_method)const->Box<LoxFunc>{
    auto declared_fcn=[&exp](const Interpreter &interpreter,const std::vector<LoxTypes>& args)->LoxTypes{
        return exp.body_->Accept(interpreter);
    };
    bool is_initializer=is_method&&exp.name_->lexeme_=="init";
    auto fcn_obj=Box<LoxFunc>{LoxFunc{exp.name_->lexeme_,declared_fcn,*exp.params_,environment_,is_initializer}};
    return fcn_obj;
}

auto Interpreter::InterpretClassMethods(const ClassStatement &exp)const->std::unordered_map<std::string, Box<LoxCallable>>{
    std::unordered_map<std::string, Box<LoxCallable>> methods;
    for(auto &expr_ref:*exp.methods_){
        auto method_ref=DynamicUniquePointerCast<FunctionStatement>(std::move(expr_ref));
        if(method_ref){
            auto ptr_guard=PointerTransferGuard(expr_ref,method_ref);
            auto fcn_obj=InterpretFunction(*method_ref,true);
            methods[method_ref->name_->lexeme_]=fcn_obj;
        }
        else{
            throw LoxRuntimeError(*exp.name_,"Non function detected inside classes");
        }
    }
    return methods;
}

Interpreter::CurEnvGuard::CurEnvGuard(const Interpreter &interpreter,const std::shared_ptr<Environment> &scope_image):interpreter_(interpreter){
    old_=interpreter_.environment_;
    if(scope_image){
        interpreter_.environment_=scope_image;
    }
    interpreter_.environment_=std::make_shared<Environment>(std::move(interpreter_.environment_));
    
}
Interpreter::CurEnvGuard::~CurEnvGuard(){
    interpreter_.environment_=std::move(old_);
}


}  // namespace lox