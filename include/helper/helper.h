
#pragma  once
#include <memory>
#include <string>
#include "symbol/token.h"


namespace lox {

void Error(const Token &token,const std::string &message);
void Error(int line,const std::string& message);
void Report(int line,const std::string &where,const std::string &message);

template<typename T,typename U>
auto DynamicUniquePointerCast(std::unique_ptr<U>&& ptr)->std::unique_ptr<T>{
    T* converted=dynamic_cast<T*>(ptr.get());
    if(converted!=nullptr){
        ptr.release();
        return std::unique_ptr<T>{converted};
    }
    return nullptr;
}

template <typename T,typename U>
class PointerTransferGuard{
    public:
    PointerTransferGuard(T& origin,U& now):origin_(origin),now_(now){}
    ~PointerTransferGuard(){
        origin_=std::move(now_);
    }

    private:
    T& origin_;
    U& now_;
};

}  // namespace lox