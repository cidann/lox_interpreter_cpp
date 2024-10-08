#Find before subdirectory to propagate targets
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(input)
add_subdirectory(scanner)
add_subdirectory(driver)
add_subdirectory(helper)
add_subdirectory(symbol)
add_subdirectory(parser)
add_subdirectory(interpreter)
add_subdirectory(resolver)
add_subdirectory(function)

add_library(lox STATIC)

set(
    LOX_LIBS
    lox_input
    lox_scanner
    lox_driver
    lox_helper
    lox_symbol
    lox_parser
    lox_interpreter
    lox_resolver
    lox_builtin_function
)


set(
    LOX_THIRD_PARTY_LIB
    Threads::Threads
    fmt::fmt
)

target_link_libraries(
    lox
    ${LOX_LIBS}
    ${LOX_THIRD_PARTY_LIB}
)

target_include_directories(
    lox
    PUBLIC LOX_INCLUDE_DIR
)