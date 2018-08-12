cmake_minimum_required (VERSION 3.0.0)
project (GRAPHS)

find_package(SDL2 REQUIRED)
find_library(UDEV_LIBS udev REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(graph src/main.cpp)
target_include_directories(graph PUBLIC ./inc)
target_include_directories(graph PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(graph PUBLIC
    ${SDL2_LIBRARIES}
    ${UDEV_LIBS}
)
target_compile_features(graph PUBLIC
    cxx_range_for
    cxx_auto_type
    cxx_constexpr
    cxx_decltype_auto
    cxx_final
    cxx_uniform_initialization
)

include(./glad/CMakeLists.txt)
include(./src/CMakeLists.txt)

