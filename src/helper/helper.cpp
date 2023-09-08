#include "helper/helper.h"
#include <iostream>
#include "symbol/token.h"

namespace lox {
void Error(const Token &token,const std::string& message){
    if(token.type_==TokenType::EOF){
        Report(token.line_," at end", message);
    }
    else{
        Report(token.line_," at "+token.lexeme_, message);
    }
}

void Error(int line,const std::string& message){
    Report(line, "", message);
}

void Report(int line,const std::string &where,const std::string& message){
    std::cout<<"[line "<<  line << "] Error"
     << where << ": " << message<<std::endl;
}

}  // namespace lox
