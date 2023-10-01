#include "function/loxBuiltIn.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include "interpreter/interpreter.h"
#include "symbol/types.h"
#include "symbol/token.h"

namespace lox {
auto LoxClock(const Interpreter &interpreter,const std::vector<LoxTypes>& args)->LoxTypes{
    using std::chrono::high_resolution_clock;

    std::int64_t t=high_resolution_clock::to_time_t(high_resolution_clock::now());
    
    throw LoxReturn{t};
}
}