#include "interpreter/environment.h"
#include <string>
#include "interpreter/interpreter.h"

namespace lox {

Environment::Environment(std::shared_ptr<Environment>&& prev):prev_(std::move(prev)){}
auto Environment::Contains(const std::string& name)->bool{
    if(names_.find(name)!=names_.end()){
        return true;
    }
    if(prev_){
        return prev_->Contains(name);
    }
    return false;
}
auto Environment::Find(const std::string& name)->decltype(names_.find(name)){
    auto itr=names_.find(name);
    if(itr!=names_.end()){
        return itr;
    }
    if(prev_){
        return prev_->Find(name);
    }
    return names_.end();
}

auto Environment::End()->decltype(names_.end()){
    if(prev_){
        return prev_->End();
    }
    return names_.end();
}

auto Environment::Get(const std::string& name)->LoxTypes&{
    auto itr=Find(name);
    if(itr!=End()){
        return itr->second;
    }
    throw EnvironmentException("Trying to Get non existent variable");
}

auto Environment::Set(const std::string& name,const LoxTypes& val)->LoxTypes{
    auto itr=Find(name);
    if(itr!=End()){
        itr->second=val;
        return itr->second;
    }
    
    throw EnvironmentException("Trying to Set non existent variable");
}

auto Environment::Assign(const std::string& name,const LoxTypes& val)->LoxTypes{
    names_[name]=val;
    return val;
}

auto Environment::GetAt(int start,const std::string& name)->LoxTypes&{
    auto start_env=this;
    for(int i=0;i<start;i++){
        start_env=start_env->prev_.get();
    }
    auto itr=start_env->Find(name);
    if(itr!=End()){
        return itr->second;
    }
    throw EnvironmentException("Trying to Get non existent variable");
}

auto Environment::SetAt(int start,const std::string& name,const LoxTypes& val)->LoxTypes{
    auto start_env=this;
    for(int i=0;i<start;i++){
        start_env=start_env->prev_.get();
    }
    auto itr=start_env->Find(name);
    if(itr!=End()){
        itr->second=val;
        return itr->second;
    }
    
    throw EnvironmentException("Trying to Set non existent variable");
}

auto Environment::Prev()->std::shared_ptr<Environment>&{
    return prev_;
}



}  // namespace lox