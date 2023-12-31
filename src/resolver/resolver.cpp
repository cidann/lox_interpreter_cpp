#include "resolver/resolver.h"
#include <cstddef>
#include "helper/helper.h"
#include "parser/parser.h"
#include "symbol/types.h"

namespace lox {

auto Resolver::Visit(BinaryExpression &exp)const->LoxTypes{
    Resolve(*exp.left_);
    Resolve(*exp.right_);
    return {};
}
auto Resolver::Visit(GroupingExpression &exp)const->LoxTypes{
    Resolve(*exp.expression_);
    return {};
}
auto Resolver::Visit(LiteralExpression &exp)const->LoxTypes{
    return {};
}
auto Resolver::Visit(UnaryExpression &exp)const->LoxTypes{
    Resolve(*exp.right_);
    return {};
}
auto Resolver::Visit(PrintStatement &exp)const->LoxTypes{
    Resolve(*exp.expression_);
    return {};
}
auto Resolver::Visit(ExpressionStatement &exp)const->LoxTypes{
    Resolve(*exp.expression_);
    return {};
}
//******** Note this only give a warning
auto Resolver::Visit(VariableExpression &exp)const->LoxTypes{
    auto &scope=scope_stack_.back();
    if(scope.count(exp.name_->lexeme_)!=0&&!scope[exp.name_->lexeme_]){
        Error(*exp.name_,"Read variable in its own initializer");
    }
    
    ResolveLocal(*exp.name_);   

    return {};
}
auto Resolver::Visit(VariableStatement &exp)const->LoxTypes{
    DeclareVariable(exp.name_->lexeme_);
    Resolve(*exp.initializer_);
    DefineVariable(exp.name_->lexeme_);
    
    return {};
}
auto Resolver::Visit(AssignExpression &exp)const->LoxTypes{
    Resolve(*exp.expression_);
    return {};
}
auto Resolver::Visit(BlockStatement &exp)const->LoxTypes{
    scope_stack_.emplace_back();
    Resolve(*exp.statments_);
    scope_stack_.pop_back();
    return {};
}
auto Resolver::Visit(IfStatement &exp)const->LoxTypes{
    Resolve(*exp.condtion_);
    Resolve(*exp.then_);
    if(exp.else_) { 
        Resolve(*exp.else_);
    }
    return {};
}
auto Resolver::Visit(LogicalExpression &exp)const->LoxTypes{
    Resolve(*exp.left_);
    Resolve(*exp.right_);
    return {};
}
auto Resolver::Visit(WhileStatement &exp)const->LoxTypes{
    Resolve(*exp.condtion_);
    Resolve(*exp.body_);
    return {};
}
auto Resolver::Visit(CallExpression &exp)const->LoxTypes{
    Resolve(*exp.callee_);
    Resolve(*exp.args_);
    return {};
}
auto Resolver::Visit(FunctionStatement &exp)const->LoxTypes{
    DeclareVariable(exp.name_->lexeme_);
    DefineVariable(exp.name_->lexeme_);
    auto current_fun_scope_type=func_scope_type_;
    func_scope_type_=class_scope_type_==ScopeType::CLASS? ScopeType::METHOD:ScopeType::FUNCTION;

    EnterScope();
    for(auto& param:*exp.params_){
        DeclareVariable(param.lexeme_);
        DefineVariable(param.lexeme_);
    }
    Resolve(*exp.body_);

    func_scope_type_=current_fun_scope_type;
    ExitScope();
    return {};
}
auto Resolver::Visit(ReturnStatement &exp)const->LoxTypes{
    if(func_scope_type_!=ScopeType::FUNCTION&&func_scope_type_!=ScopeType::METHOD){
        Error(*exp.keyword_,"return statement can only be inside function or methods");
    }
    Resolve(*exp.initializer_);
    return {};
}


auto Resolver::Visit(ClassStatement &exp)const->LoxTypes{
    DeclareVariable(exp.name_->lexeme_);
    DefineVariable(exp.name_->lexeme_);
    auto current_class_scope_type=class_scope_type_;
    class_scope_type_=ScopeType::CLASS;

    EnterScope();
    DeclareVariable("this");
    DefineVariable("this");

    for(auto &method_ref:*exp.methods_){
        method_ref->Accept(*this);
    }

    class_scope_type_=current_class_scope_type;
    ExitScope();
    return {};
}

auto Resolver::Visit(GetAttributeExpression &exp)const->LoxTypes{
    Resolve(*exp.object_);
    return {};
}
auto Resolver::Visit(SetAttributeExpression &exp)const->LoxTypes{
    Resolve(*exp.object_);
    Resolve(*exp.value_);
    return {};
}

auto Resolver::Visit(ThisExpression &exp)const->LoxTypes{
    if(func_scope_type_!=ScopeType::METHOD){
        Error(*exp.keyword_,"this statement can only be inside methods");
    }
    ResolveLocal(*exp.keyword_);
    return {};
}

void Resolver::Resolve(const std::vector<AbstractStatementRef> &stmts)const{
    for(auto &stmt_ref:stmts){
        Resolve(*stmt_ref);
    }
}
void Resolver::Resolve(const std::vector<AbstractExpressionRef> &stmts)const{
    for(auto &stmt_ref:stmts){
        Resolve(*stmt_ref);
    }
}
void Resolver::Resolve(AbstractStatement &stmts)const{
    stmts.Accept(*this);
}
void Resolver::Resolve(AbstractExpression &expr)const{
    expr.Accept(*this);
}

void Resolver::ResolveLocal(Token& variable) const{
    for(std::size_t i=0;i<scope_stack_.size();i++){
        if(scope_stack_[i].count(variable.lexeme_)!=0){
            variable.start_scope_=scope_stack_.size()-i-1;
        }
    }
}

void Resolver::EnterScope()const{
    scope_stack_.emplace_back();
}
void Resolver::ExitScope()const{
    scope_stack_.pop_back();
}
void Resolver::DeclareVariable(const std::string &name)const{
    scope_stack_.back()[name]=false;
}
void Resolver::DefineVariable(const std::string &name)const{
    scope_stack_.back()[name]=true;
}


}  // namespace lox