add_subdirectory(input)
add_subdirectory(scanner)
add_subdirectory(core)
add_subdirectory(helper)
add_subdirectory(symbol)
add_subdirectory(expression)

add_library(lox STATIC)

set(
    LOX_LIBS
    lox_input
    lox_scanner
    lox_core
    lox_helper
    lox_symbol
    lox_expression
)


find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

set(
    LOX_THIRD_PARTY_LIB
    Threads::Threads
    fmt
)

target_link_libraries(
    lox
    ${LOX_LIBS}
)

target_include_directories(
    lox
    PUBLIC LOX_INCLUDE_DIR
)