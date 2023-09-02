add_executable(lox_compiler lox.cpp)

target_link_libraries(lox_compiler lox)

set_target_properties(lox_compiler PROPERTIES OUTPUT_NAME lox_compiler)
