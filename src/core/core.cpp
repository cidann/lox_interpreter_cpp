#include "core/core.h"
#include <fmt/format.h>
#include "expression/expression_visitor/print_visitor.h"
#include "scanner/scanner.h"
#include "symbol/token.h"
#include <algorithm>
#include <memory>
#include <string>
#include <iostream>
#include <type_traits>
#include "expression/expression_visitor/expression_visitor.h"

#include "expression/BinaryExpression.h"
#include "expression/GroupingExpression.h"
#include "expression/LiteralExpression.h"
#include "expression/UnaryExpression.h"

namespace lox {
void Run(const std::string& s){
    Scanner scanner{s};
    auto tokens=scanner.ScanTokens();
    std::cout<<tokens.size()<<"\n";
    std::for_each(tokens.begin(),tokens.end(),[](Token &v){
        std::cout<<v.ToString()<<std::endl;
    });
    std::cout<<"Temp place holder echo:\n";
    std::cout<<s<<std::endl;
    
    
    BinaryExpression be;
    auto left=std::make_unique<LiteralExpression>();
    left->value_=std::make_unique<Literal>(100);
    auto right=std::make_unique<LiteralExpression>();
    right->value_=std::make_unique<Literal>(200);
    auto op=std::make_unique<Token>(TokenType::MINUS,"-",0);
    be.left_=std::move(left);
    be.right_=std::move(right);
    be.operator_=std::move(op);
    PrintVisitor pv;
    std::cout<< std::any_cast<std::string>(be.Accept(pv));
    
}
}  // namespace lox
