cmake_minimum_required(VERSION 3.5)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(stl_interfaces)

################################################## 
# C++ standard version selection
################################################## 
set(CXX_STD 14 CACHE STRING "Set to X to enable C++X builds.")
message("-- Using -std=c++${CXX_STD}")


##################################################
# Sanitizers
##################################################
set(USE_ASAN false CACHE BOOL "Set to true to enable -fsanitize=address when building tests.") 
set(USE_UBSAN false CACHE BOOL "Set to true to enable -fsanitize=undefined when building tests.")
if (USE_ASAN AND USE_UBSAN)
    message(FATAL_ERROR "USE_ASAN and USE_UBSAN must not be enabled at the same time")
elseif (USE_ASAN)
    set(compile_flags -fsanitize=address)
    set(link_flags -fsanitize=address)
    message("-- Using -fsanitize=address") 
elseif (USE_UBSAN)
    set(compile_flags -fsanitize=undefined)
    set(link_flags -fsanitize=undefined)
    message("-- Using -fsanitize=undefined")
endif()


##################################################
# Code coverage
##################################################
if (UNIX)
    set(BUILD_COVERAGE false CACHE BOOL "Set to true to enable code coverage when building tests.  Only Linux and Mac are supported.")
    if (BUILD_COVERAGE)
        message("-- Building for code coverage; disabling any sanitizers")
        if (APPLE)
            set(compile_flags -fprofile-arcs -ftest-coverage)
            set(CMAKE_BUILD_TYPE Debug)
            set(link_flags --coverage)
        else ()
            set(compile_flags --coverage)
            set(CMAKE_BUILD_TYPE Debug)
            set(link_flags --coverage)
        endif ()
    endif ()
endif ()


##################################################
# Dependencies
##################################################
set(boost_components)
include(dependencies)

##################################################
# stl_interfaces library
##################################################
add_library(stl_interfaces INTERFACE)

target_include_directories(stl_interfaces INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(stl_interfaces INTERFACE boost)
if (link_flags OR HAVE_CMCSTL2)
    target_link_libraries(stl_interfaces INTERFACE ${link_flags})
    target_compile_options(stl_interfaces INTERFACE ${compile_flags})
    if (HAVE_CMCSTL2)
        target_link_libraries(stl_interfaces INTERFACE stl2)
    endif()
endif ()
if (NOT MSVC)
    target_compile_options(stl_interfaces INTERFACE -Wall)
endif ()


##################################################
# Tests and examples
##################################################
## Built conditionally, because it relies on libFuzzer.
#set(BUILD_FUZZ_TESTS false CACHE BOOL "Set to true to build fuzz tests.")
#
#if (BUILD_FUZZ_TESTS AND NOT CMAKE_CXX_COMPILER_ID STREQUAL Clang)
#    message(FATAL_ERROR "BUILD_FUZZ_TESTS only works with Clang; it uses libFuzzer.")
#endif ()

add_subdirectory(test)
add_subdirectory(example)
#add_subdirectory(doc)
