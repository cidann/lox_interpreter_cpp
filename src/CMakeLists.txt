add_subdirectory(input)
add_subdirectory(scanner)
add_subdirectory(core)
add_subdirectory(helper)

add_library(lox STATIC ${ALL_OBJECT_FILES})

set(
    LOX_LIBS
    lox_input
    lox_scanner
    lox_core
)


find_package(Threads REQUIRED)

set(
    LOX_THIRD_PARTY_LIB
    Threads::Threads
)

target_link_libraries(
    lox
    ${LOX_LIBS}
)

target_include_directories(
    lox
    PUBLIC LOX_INCLUDE_DIR
)