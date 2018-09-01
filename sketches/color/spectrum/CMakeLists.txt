cmake_minimum_required(VERSION 3.5)
project(spectrum VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(spectrum src/main)
set_target_properties(spectrum PROPERTIES SUFFIX .out)
target_compile_features(spectrum PRIVATE cxx_auto_type cxx_generic_lambdas)

find_package(Tetra REQUIRED)
target_link_libraries(spectrum Tetra::libTetra)
target_sources(spectrum
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/GlApp.cpp
)

