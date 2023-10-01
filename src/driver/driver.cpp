#include "resolver/resolver.h"
#define FMT_HEADER_ONLY
#include "driver/driver.h"
#include <fmt/format.h>
#include "interpreter/interpreter.h"
#include "parser/parser.h"
#include "scanner/scanner.h"
#include "symbol/token.h"
#include <algorithm>
#include <any>
#include <memory>
#include <string>
#include <iostream>
#include <type_traits>
#include <vector>
#include "expression/expression_visitor/expression_visitor.h"

#include "expression/expression_headers.h"

namespace lox {
void Run(const std::string& s){
    Scanner scanner{s};
    std::vector<Token> tokens=scanner.ScanTokens();
    

    Parser parser{tokens};
    Interpreter interpreter;
    Resolver resolver;
    try{
        auto statements=parser.Parse();
        resolver.Resolve(statements);
        auto result=interpreter.Interpret(statements);
    }   
    catch(ParserException e){
        std::cout<<"Compile time error"<<std::endl;
        std::cout<<e.what()<<std::endl;
    }
    catch(LoxRuntimeError e){
        std::cout<<"Run time error"<<std::endl;
        std::cout<<e.what()<<std::endl;
    }
}
}  // namespace lox
