


#include <memory>

namespace lox{
template <typename T>
class Box{
    public:
    Box()= default;
    explicit Box(const T &other):data_(static_cast<T*>(other.Clone())){}
    explicit Box(T &&other):data_(std::unique_ptr<T>(static_cast<T*>(other.ShallowClone()))){};
    Box(const Box &other){
        data_=std::unique_ptr<T>(static_cast<T*>((*other).Clone()));
    }
    Box(Box &&other) noexcept =default;

    template<typename U>
    explicit Box(const Box<U> &other){
        data_=std::unique_ptr<T>(static_cast<T*>((*other).Clone()));
    }
    template<typename U>
    explicit Box(Box<U> &&other) noexcept{
        data_=std::unique_ptr<T>(static_cast<T*>((*other).ShallowClone()));
    }
    template<typename U>
    auto Test(U& other) noexcept -> U{
        return other;
    }

    auto operator=(const Box &other)->Box&{
        data_=std::unique_ptr<T>(static_cast<T*>((*other).Clone()));
        return *this;
    }
    auto operator=(Box &&other) noexcept ->Box& =default;

    template<typename U>
    auto operator=(const Box<U> &other)->Box&{
        data_=std::unique_ptr<T>(static_cast<T*>((*other).Clone()));
        return *this;
    }
    template<typename U>
    auto operator=(Box<U> &&other) noexcept ->Box&{
        data_=std::unique_ptr<T>(static_cast<T*>((*other).Clone()));
        return *this;
    }

    auto operator==(const Box &other)->bool{
        return data_==other.data_;
    }
    friend auto operator==(const Box &lhs,const Box &rhs)->bool{
        return lhs.data_==rhs.data_;
    }

    auto operator*()->T&{
        return *data_;
    }
    auto operator*()const->T&{
        return *data_;
    }
    auto operator->()->T*{
        return data_.get();
    } 
    auto operator->()const->T*{
        return data_.get();
    }
    auto Get()->T*{
        return data_.get();
    }

    ~Box()=default;

    private:
    std::unique_ptr<T> data_;
};


}  // namespace lox