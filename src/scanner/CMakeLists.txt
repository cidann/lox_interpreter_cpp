add_library(
    lox_scanner
    OBJECT
    scanner.cpp
)

# not needed since it is already included in top level CMake
# include directory
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lox_scanner>
    PARENT_SCOPE)