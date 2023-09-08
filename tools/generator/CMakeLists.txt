
set(
    GENERATED_FILES
    BinaryExpression.h
    GroupingExpression.h
    LiteralExpression.h
    UnaryExpression.h
)

#foreach(file ${GENERATED_FILES})
#    set(
#        GENERATED_PATH
#        ${GENERATED_PATH} ${LOX_INCLUDE_DIR}/expression/${file}
#    )
#endforeach()



find_package(Python COMPONENTS Interpreter)

add_custom_target(
    generate_ast_expression ALL
    COMMAND ${Python_EXECUTABLE} 
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_ast.py ${PROJECT_SOURCE_DIR}/include expression
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/expression
    COMMENT "Generate AST expression classes"
)

add_custom_target(
    generate_ast_statement ALL
    COMMAND ${Python_EXECUTABLE} 
    ${CMAKE_CURRENT_SOURCE_DIR}/generate_ast.py ${PROJECT_SOURCE_DIR}/include statement
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/statement
    COMMENT "Generate AST statement classes"
)
add_custom_target(
    generate_ast
    DEPENDS generate_ast_expression generate_ast_statement
)

add_dependencies(lox generate_ast)