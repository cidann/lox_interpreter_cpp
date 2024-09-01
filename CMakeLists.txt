cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clang-tidy.
#set(BUILD_SHARED_LIBS OFF) # We expect external libraries to be linked statically.
set(CMAKE_CXX_STANDARD 17) # Compile as C++17.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++17 support.

#toolchain for vcpkg
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-linux-clang")

#use clang
set(CMAKE_CXX_COMPILER clang)

project(Lox
        VERSION 1
        DESCRIPTION "The lox interpretor"
        LANGUAGES C CXX
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Setting build type to `Debug` as none was specified.")
        set(CMAKE_BUILD_TYPE "Debug")
endif()

# Compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-attributes")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fstandalone-debug -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(LOX_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${LOX_INCLUDE_DIR})



add_subdirectory(src)
add_subdirectory(tools)
#add_subdirectory(test)




