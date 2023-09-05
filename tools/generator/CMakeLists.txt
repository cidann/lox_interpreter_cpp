
set(
    GENERATED_FILES
    BinaryExpression.h
    GroupingExpression.h
    LiteralExpression.h
    UnaryExpression.h
)

foreach(file ${GENERATED_FILES})
    set(
        GENERATED_PATH
        ${GENERATED_PATH} ${LOX_INCLUDE_DIR}/expression/${file}
    )
endforeach()



find_package(Python COMPONENTS Interpreter)

add_custom_target(
    generate_ast ALL
    COMMAND ${Python_EXECUTABLE} 
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_ast.py ${PROJECT_SOURCE_DIR}/include/expression
    BYPRODUCTS GENERATED_PATH
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grammer
    COMMENT "Generate AST classes"
)

add_dependencies(lox generate_ast)