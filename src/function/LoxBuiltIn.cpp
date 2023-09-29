#include "function/LoxBuiltIn.h"
#include <chrono>
#include <iostream>
#include "symbol/types.h"

namespace lox {
auto LoxClock(const Interpreter &interpreter,const std::vector<LoxTypes>& args)->LoxTypes{
    using std::chrono::high_resolution_clock;

    std::cout<<high_resolution_clock::to_time_t(high_resolution_clock::now())<<std::endl;
    
    return {};
}
}