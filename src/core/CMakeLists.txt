add_library(
    lox_core
    OBJECT
    core.cpp
)

target_link_libraries(lox_core lox_parser)