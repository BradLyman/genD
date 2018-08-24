cmake_minimum_required(VERSION 3.5)
project(hello_color VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(hello_color src/main)
set_target_properties(hello_color PROPERTIES SUFFIX .out)
target_compile_features(hello_color PRIVATE cxx_auto_type)

find_package(Tetra REQUIRED)
target_link_libraries(hello_color Tetra::libTetra)
target_sources(hello_color
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/GlApp.cpp
)

