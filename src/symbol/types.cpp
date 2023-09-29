#include "symbol/types.h"
#include <cstddef>
#include <stdexcept>
#include "symbol/token.h"
#include "interpreter/interpreter.h"

namespace lox {
    LoxCallable::LoxCallable(LoxCallableSignature fcn,const std::vector<Token>& params):params_(params),fcn_(std::move(fcn)){
        arity_=params.size();
    }
    auto LoxCallable::operator()(const Interpreter &interpreter,const std::vector<LoxTypes>& args)const->LoxTypes{
        
        return fcn_(interpreter,args);
    }
    auto LoxCallable::operator==(const LoxCallable &other)const->bool{
        return arity_==other.arity_;
    }
}  // namespace lox