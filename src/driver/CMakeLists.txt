add_library(
    lox_driver
    OBJECT
    driver.cpp
)

target_link_libraries(lox_driver fmt::fmt)
