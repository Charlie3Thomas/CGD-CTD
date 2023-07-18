cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME "CGD-CTD")
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

include(CTest)

# Must be added before add_subdirectory commands
add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter) #-Werror

add_subdirectory(apps)
add_subdirectory(src)
add_subdirectory(tests)