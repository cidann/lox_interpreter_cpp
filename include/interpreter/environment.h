#pragma once
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "symbol/types.h"

namespace lox {
class EnvironmentException:public std::runtime_error{
    public:
    explicit EnvironmentException(const std::string& msg):std::runtime_error(msg){};
    auto what() const noexcept -> const char * override{
        return std::runtime_error::what();
    }
};
class Environment{
    public:
    Environment()=default;
    explicit Environment(std::shared_ptr<Environment>&& prev);
    auto Contains(const std::string& name)->bool;
    auto Get(const std::string& name)->LoxTypes&;
    //This is for setting the variable that was already created starting from current scope and going up
    auto Set(const std::string& name,const LoxTypes& val)->LoxTypes;
    //This is intended for var declaration only and so only assign to current scope
    auto Assign(const std::string& name,const LoxTypes& val)->LoxTypes;
    auto Prev()-> std::shared_ptr<Environment> &;

    
    auto GetAt(int start,const std::string& name)->LoxTypes&;
    auto SetAt(int start,const std::string& name,const LoxTypes& val)->LoxTypes;
    
    
    private:
    auto Find(const std::string& name)->std::unordered_map<std::string, LoxTypes>::iterator;
    auto End()->std::unordered_map<std::string, LoxTypes>::iterator;
    std::unordered_map<std::string, LoxTypes> names_;
    std::shared_ptr<Environment> prev_;

};



}  // namespace lox
