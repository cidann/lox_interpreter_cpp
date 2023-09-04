#include "helper/helper.h"
#include <iostream>

namespace lox {
void Error(int line,const std::string& message){
    Report(line, "", message);
}
void Report(int line,const std::string &where,const std::string& message){
    std::cout<<"[line "<<  line << "] Error"
     << where << ": " << message<<std::endl;
}

}
