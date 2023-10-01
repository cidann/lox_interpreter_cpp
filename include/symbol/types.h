#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>



namespace lox{
class Token;
class Environment;

class LoxCallable;
class Interpreter;
using LoxTypes=std::variant<std::monostate,std::string,bool,int64_t,double,LoxCallable>;
using LoxCallableSignature=std::function<const LoxTypes(const Interpreter&,const std::vector<LoxTypes>&)>;
class LoxCallable{
    public:
    LoxCallable(std::string name,LoxCallableSignature fcn,const std::vector<Token>& params,const std::shared_ptr<Environment>& env);
    auto operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes;
    auto operator==(const LoxCallable &other)const->bool;
    auto ToString()const->std::string;

    std::string name_;
    std::vector<Token> params_;
    std::size_t arity_;
    LoxCallableSignature fcn_;
    std::shared_ptr<Environment> env_;
};
/*
class ClockBuiltIn:public LoxCallable{
    public:
    auto operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes override{
        using std::chrono::high_resolution_clock;
        using std::chrono::milliseconds;
        
        std::cout<<high_resolution_clock::to_time_t(high_resolution_clock::now()) <<std::endl;
    }
}
*/

}  // namespace lox