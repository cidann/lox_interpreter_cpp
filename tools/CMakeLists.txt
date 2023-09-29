add_subdirectory(generator)
add_executable(lox_compiler lox.cpp)

target_link_libraries(lox_compiler lox)

set_target_properties(lox_compiler PROPERTIES OUTPUT_NAME lox_compiler)


#generate ast then build compiler
add_executable(lox_compiler_all lox.cpp)
target_link_libraries(lox_compiler_all lox)
set_target_properties(lox_compiler_all PROPERTIES OUTPUT_NAME lox_compiler)
add_dependencies(lox_compiler_all generate_ast)