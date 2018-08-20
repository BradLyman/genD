cmake_minimum_required (VERSION 3.0.0)
project (GRAPHS)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(graph src/main.cpp)
target_link_libraries(graph PUBLIC
    ${SDL2_LIBRARIES}
    OpenGL::GL
    GLEW::GLEW
)
target_include_directories(graph PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(graph PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_features(graph PUBLIC
    cxx_range_for
    cxx_auto_type
    cxx_constexpr
    cxx_decltype_auto
    cxx_final
    cxx_uniform_initialization
)

include(./src/CMakeLists.txt)

