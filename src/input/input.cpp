
#include "input/input.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "core/core.h"


void runPrompt(){
    std::string line;
    while(true){
        std::cout<<"> ";
        if(!std::getline(std::cin,line)){
            std::cout<<std::endl;
            return;
        }
        run(line);
    }
}

void runFile(std::string file_name){
    std::filesystem::path path{file_name};
    
    if(!std::filesystem::is_regular_file(path)){
        std::cout<<"No file found\n";
        std::abort();
    }
    std::ifstream fin{path};
    std::stringstream sstream;
    sstream<<fin.rdbuf();

    run(sstream.str());
    

}