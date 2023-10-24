#pragma once

#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "symbol/token.h"
#include "symbol/box.h"



namespace lox{
class Environment;


class LoxClass;
class LoxCallable;
class LoxInstance;

class Interpreter;
using LoxTypes=std::variant<std::monostate,std::string,bool,int64_t,double,Box<LoxCallable>,std::shared_ptr<LoxInstance>>;
using LoxCallableSignature=std::function<const LoxTypes(const Interpreter&,const std::vector<LoxTypes>&)>;
class LoxCallable{
    public:
    LoxCallable(std::string name,const std::vector<Token>& params,const std::shared_ptr<Environment>& env,bool initializer=false);

    virtual auto operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes=0;
    virtual auto ToString()const->std::string=0;
    virtual auto Arity()const->std::size_t=0;
    virtual auto Param()const->const std::vector<Token>& =0;
    virtual auto Clone()->LoxCallable* =0;
    virtual auto ShallowClone()->LoxCallable* =0;
    virtual ~LoxCallable()=default;
    

    std::string name_;
    std::vector<Token> params_;
    std::size_t arity_;
    std::shared_ptr<Environment> env_;
    bool initializer_;
};

class LoxFunc:public LoxCallable{
    public:
    LoxFunc(std::string name,LoxCallableSignature fcn,const std::vector<Token>& params,const std::shared_ptr<Environment>& env,bool initializer);
    
    auto operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes override;
    auto ToString()const->std::string override;
    auto Arity()const->std::size_t override;
    auto Param()const->const std::vector<Token>& override;
    auto Clone()->LoxCallable* override;
    auto ShallowClone()->LoxCallable* override;
    auto operator==(const LoxFunc &other)const->bool;

    LoxCallableSignature fcn_;
};

class LoxClass:public LoxCallable{
    public:
    explicit LoxClass(std::string name,const std::vector<Token>& params,const std::shared_ptr<Environment>& env,const std::unordered_map<std::string, Box<LoxCallable>> &methods);
    auto operator==(const LoxClass &other)const->bool;
    auto operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes override;
    auto ToString()const->std::string override;
    auto Arity()const->std::size_t override;
    auto Param()const->const std::vector<Token>& override;
    auto Clone()->LoxCallable* override;
    auto ShallowClone()->LoxCallable* override;
    
    std::unordered_map<std::string, Box<LoxCallable>> methods_;
};


class LoxInstance{
    public:
    explicit LoxInstance(LoxClass loxclass);

    auto operator==(const LoxInstance &other)const->bool;
    auto ToString()const->std::string;
    auto Clone()->LoxInstance*;
    auto ShallowClone()->LoxInstance*;
    
    LoxInstance(const LoxInstance& other)=default;
    LoxInstance(LoxInstance&& other)=default;
    ~LoxInstance();

    LoxClass loxclass_;
    std::unordered_map<std::string, LoxTypes> fields_;
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