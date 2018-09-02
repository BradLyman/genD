cmake_minimum_required(VERSION 3.5)
project(interpolation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(interpolation src/main)
set_target_properties(interpolation PROPERTIES SUFFIX .out)
target_compile_features(interpolation PRIVATE cxx_auto_type cxx_generic_lambdas)

find_package(Tetra REQUIRED)
target_link_libraries(interpolation Tetra::libTetra)
target_sources(interpolation
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/GlApp.cpp
)

