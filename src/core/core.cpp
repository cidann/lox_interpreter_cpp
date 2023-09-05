#include "core/core.h"
#include <fmt/format.h>
#include "expression/expression_visitor/arithmetic_visitor.h"
#include "expression/expression_visitor/print_visitor.h"
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
    auto tree=parser.Expression();
    std::cout<<std::any_cast<std::string>(tree->Accept(PrintVisitor{}))<<std::endl;
    std::cout<<std::any_cast<double>(tree->Accept(ArithmeticVisitor{}))<<std::endl;

}
}  // namespace lox
