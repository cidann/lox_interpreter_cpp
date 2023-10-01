#include "symbol/types.h"
#include <cstddef>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include "symbol/token.h"
#include "interpreter/interpreter.h"

namespace lox {
    LoxCallable::LoxCallable(std::string name,LoxCallableSignature fcn,const std::vector<Token>& params,const std::shared_ptr<Environment>& env):name_(std::move(name)),params_(params),fcn_(std::move(fcn)),env_(env){
        arity_=params.size();
    }
    auto LoxCallable::operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes{
        
        return fcn_(interpreter,args);
    }
    auto LoxCallable::operator==(const LoxCallable &other)const->bool{
        return arity_==other.arity_;
    }
    auto LoxCallable::ToString()const->std::string{
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
}  // namespace lox