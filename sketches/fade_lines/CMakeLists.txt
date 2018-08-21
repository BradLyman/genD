cmake_minimum_required(VERSION 3.5)
project(fade_lines VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(fade_lines src/main)
set_target_properties(fade_lines PROPERTIES SUFFIX .out)
target_compile_features(fade_lines PRIVATE cxx_auto_type)

find_package(Tetra REQUIRED)
target_link_libraries(fade_lines Tetra::libTetra)
target_sources(fade_lines
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/GlApp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/PingPong.cpp
)

