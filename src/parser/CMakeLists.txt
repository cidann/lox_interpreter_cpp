add_library(
    lox_parser
    OBJECT
    parser.cpp
)

target_link_libraries(lox_parser fmt::fmt)