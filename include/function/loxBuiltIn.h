#pragma once
#include "symbol/types.h"
#include "symbol/token.h"

namespace lox {
auto LoxClock(const Interpreter &interpreter,const std::vector<LoxTypes>& args)->LoxTypes;
}