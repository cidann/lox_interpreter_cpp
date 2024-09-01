add_library(
    lox_interpreter
    OBJECT
    interpreter.cpp
    environment.cpp
)

target_link_libraries(lox_interpreter fmt::fmt)