#include <cstdlib>
#include <thread>
#include <vector>
#include <iostream>
#include "input/input.h"

auto main(int argc, char **argv)->int{
    if (argc > 2) {
        std::cout<<"Usage: jlox [script]";
        exit(64);
    } 
    else if (argc== 2) {
        lox::RunFile(argv[1]);
    } else {
        lox::RunPrompt();
    }
}