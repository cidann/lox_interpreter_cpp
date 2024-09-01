add_library(
    lox_scanner
    OBJECT
    scanner.cpp
)

target_link_libraries(lox_scanner fmt::fmt)

# not needed since it is already included in top level CMake
# include directory
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:lox_scanner>
    PARENT_SCOPE)