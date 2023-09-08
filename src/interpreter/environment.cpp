#include "interpreter/environment.h"
#include <string>
#include "interpreter/interpreter.h"

namespace lox {

Environment::Environment(std::unique_ptr<Environment>&& prev):prev_(std::move(prev)){}
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


auto Environment::Assign(const std::string& name,const LoxTypes& val)->LoxTypes{
    names_[name]=val;
    return val;
}



auto Environment::Prev()->std::unique_ptr<Environment>&{
    return prev_;
}



}  // namespace lox