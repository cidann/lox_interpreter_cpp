#include "symbol/token.h"
#include <sstream>
#include <utility>

namespace lox{
Token::Token(TokenType type,std::string lexeme,int line):
            type_(type),lexeme_(std::move(lexeme)),line_(line){}

Token::Token(TokenType type,std::string lexeme,Literal literal,int line):
            type_(type),lexeme_(std::move(lexeme)),literal_(std::move(literal)),line_(line){}




auto Token::ToString() -> std::string{
    std::ostringstream iss;
    
    iss<<Token::token_type_map[static_cast<int>(this->type_)]
    <<" "<<line_<<" "<<lexeme_;
    return iss.str();
}
}  // namespace lox
