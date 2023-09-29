#pragma once
#include "symbol/types.h"

namespace lox {
auto LoxClock(const Interpreter &interpreter,const std::vector<LoxTypes>& args)->LoxTypes;
}