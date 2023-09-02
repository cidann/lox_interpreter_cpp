add_library(
    lox_input
    OBJECT
    input.cpp
)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lox_input>
    PARENT_SCOPE)