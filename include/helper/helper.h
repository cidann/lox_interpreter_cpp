
#pragma  once
#include <string>


namespace lox {

void Error(int line,const std::string &message);
void Report(int line,const std::string &where,const std::string &message);

}