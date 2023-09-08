#include "symbol/token.h"
#include <sstream>
#include <utility>
#include "interpreter/interpreter.h"

namespace lox{
Token::Token(TokenType type,std::string lexeme,int line):
            type_(type),lexeme_(std::move(lexeme)),line_(line){}

Token::Token(TokenType type,std::string lexeme,LoxTypes literal,int line):
            type_(type),lexeme_(std::move(lexeme)),literal_(std::move(literal)),line_(line){}




auto Token::ToString() -> std::string{
    std::ostringstream iss;
    
    iss<<Token::token_type_map[static_cast<int>(this->type_)]
    <<" "<<line_<<" "<<lexeme_;
    return iss.str();
}


/*
auto LoxTypes::Add(const LoxTypes &other,const Token &tok)->LoxTypes{
    if(std::holds_alternative<std::string>(value_)){
        return LoxTypes{
            ToString(*this)+ToString(other)
        };
    }
    if(std::holds_alternative<bool>(value_)){
        
    }
    if(std::holds_alternative<int64_t>(value_)){
        return LoxTypes{
            ToInt(*this)+ToInt(other)
        };
    }
    if(std::holds_alternative<double>(value_)){
        return LoxTypes{
            ToDouble(*this)+ToDouble(other)
        };
    }
    throw LoxRuntimeError(tok,"You can only add");
}

auto LoxTypes::ToString(const LoxTypes& value)->std::string{
    if(std::holds_alternative<std::string>(value.value_)){
        return std::get<std::string>(value.value_);
    }
    if(std::holds_alternative<bool>(value.value_)){
        return std::get<bool>(value.value_)? "true":"false";
    }
    if(std::holds_alternative<int64_t>(value.value_)){
        return std::to_string(std::get<int64_t>(value.value_));
    }
    if(std::holds_alternative<double>(value.value_)){
        return std::to_string(std::get<double>(value.value_));
    }
    return "nil";
}

auto LoxTypes::ToDouble(const LoxTypes& value)->double{
    if(std::holds_alternative<std::string>(value.value_)){
        return std::stod(std::get<std::string>(value.value_));
    }
    if(std::holds_alternative<bool>(value.value_)){
        return std::get<bool>(value.value_)? 1:0;
    }
    if(std::holds_alternative<int64_t>(value.value_)){
        return static_cast<double>(std::get<int64_t>(value.value_));
    }
    if(std::holds_alternative<double>(value.value_)){
        return std::get<double>(value.value_);
    }
    return 0;
}
auto LoxTypes::ToInt(const LoxTypes& value)->int64_t{
    if(std::holds_alternative<std::string>(value.value_)){
        return std::stoll(std::get<std::string>(value.value_));
    }
    if(std::holds_alternative<bool>(value.value_)){
        return std::get<bool>(value.value_)? 1:0;
    }
    if(std::holds_alternative<int64_t>(value.value_)){
        return std::get<int64_t>(value.value_);
    }
    if(std::holds_alternative<double>(value.value_)){
        return static_cast<int64_t>(std::get<double>(value.value_));
    }
    return 0;
}
*/
}  // namespace lox
