cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME "boingo")
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

# Must be added before add_subdirectory commands
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(apps)
add_subdirectory(src)