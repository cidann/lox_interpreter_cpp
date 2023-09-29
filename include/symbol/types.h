#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>



namespace lox{
class Token;

class LoxCallable;
class Interpreter;
using LoxTypes=std::variant<std::string,bool,int64_t,double,LoxCallable,std::monostate>;
using LoxCallableSignature=std::function<const LoxTypes(const Interpreter&,const std::vector<LoxTypes>&)>;
class LoxCallable{
    public:
    LoxCallable(LoxCallableSignature fcn,const std::vector<Token>& params);
    auto operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes;
    auto operator==(const LoxCallable &other)const->bool;
    
    std::vector<Token> params_;
    std::size_t arity_;
    LoxCallableSignature fcn_;
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