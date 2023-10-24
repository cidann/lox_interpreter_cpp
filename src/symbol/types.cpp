#include "symbol/types.h"
#include <cstddef>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include "symbol/token.h"
#include "interpreter/interpreter.h"

namespace lox {
LoxCallable::LoxCallable(std::string name,const std::vector<Token>& params,const std::shared_ptr<Environment>& env,bool initializer):name_(std::move(name)),params_(params),env_(env),initializer_(initializer){
    arity_=params.size();
}

LoxFunc::LoxFunc(std::string name,LoxCallableSignature fcn,const std::vector<Token>& params,const std::shared_ptr<Environment>& env,bool initializer):LoxCallable(std::move(name),params,env,initializer),fcn_(std::move(fcn)){}

auto LoxFunc::operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes{
    return fcn_(interpreter,args);
}

auto LoxFunc::operator==(const LoxFunc &other)const->bool{
    return name_==other.name_&&arity_==other.arity_;
}
auto LoxFunc::ToString()const->std::string{
    std::ostringstream iss;
    iss<<name_<<"(";
    for(std::size_t i=0;i<params_.size();i++){
        iss<<params_[i].lexeme_;
        if(i!=params_.size()-1){
            iss<<",";
        }
    }
    iss<<")";
    return iss.str();
}
auto LoxFunc::Arity()const->std::size_t{
   return arity_;
}

auto LoxFunc::Param()const->const std::vector<Token>&{
   return params_;
}


auto LoxFunc::Clone()->LoxCallable*{
    return new LoxFunc(*this);
}

auto LoxFunc::ShallowClone()->LoxCallable*{
    return new LoxFunc(std::move(*this));
}

LoxClass::LoxClass(std::string name,const std::vector<Token>& params,const std::shared_ptr<Environment>& env,const std::unordered_map<std::string, Box<LoxCallable>>& methods):LoxCallable(std::move(name),params,env),methods_(methods){}

auto LoxClass::operator==(const LoxClass &other)const->bool{
    return name_==other.name_;
}

auto LoxClass::operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes{
    if(methods_.find("init")!=methods_.end()){
        (*methods_.at("init"))(interpreter,args);
    }
    throw LoxReturn{env_->Get("this")};
}

auto LoxClass::ToString()const->std::string{
    return name_;
}

auto LoxClass::Arity()const->std::size_t{
    if(methods_.find("init")==methods_.end()){
        return 0;
    }
    return methods_.at("init")->arity_;
}

auto LoxClass::Param()const->const std::vector<Token>&{
    if(methods_.find("init")==methods_.end()){
        return params_;
    }
    return methods_.at("init")->params_;
}

auto LoxClass::Clone()->LoxCallable*{
    return new LoxClass(*this);
}

auto LoxClass::ShallowClone()->LoxCallable*{
    return new LoxClass(std::move(*this));
}

LoxInstance::LoxInstance(LoxClass loxclass):loxclass_(std::move(loxclass)){}
LoxInstance::~LoxInstance()= default;

auto LoxInstance::operator==(const LoxInstance &other)const->bool{
    return loxclass_==other.loxclass_&&fields_==other.fields_;
}
auto LoxInstance::ToString()const->std::string{
    return loxclass_.ToString()+" instance";
}


auto LoxInstance::Clone()->LoxInstance*{
    return new LoxInstance(*this);
}
auto LoxInstance::ShallowClone()->LoxInstance*{
    return new LoxInstance(std::move(*this));
}

}  // namespace lox