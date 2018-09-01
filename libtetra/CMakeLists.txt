cmake_minimum_required(VERSION 3.5)
project(libtetra VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure Custom Targets are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# --------------------------------------------------------------------------- #
# Declare Dependencies
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# --------------------------------------------------------------------------- #
# Define target and properties
add_library(libtetra src/Countdown.cpp)

# define this alias so the library can be used within the build tree (testing)
add_library(Tetra::libTetra ALIAS libtetra)

# set target properties
target_link_libraries(libtetra
    PUBLIC
        GLEW::GLEW
        OpenGL::GL
        ${SDL2_LIBRARIES}
        glm
)
target_include_directories(libtetra
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${SDL2_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(libtetra
    PRIVATE
        cxx_range_for
        cxx_auto_type
        cxx_constexpr
        cxx_decltype_auto
        cxx_final
        cxx_uniform_initialization
)
target_sources(libtetra PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/ColoredLine.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Turtle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Surface.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Color.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Ortho.cpp"

    "${CMAKE_CURRENT_LIST_DIR}/src/transforms/FixedOrtho.cpp"

    "${CMAKE_CURRENT_LIST_DIR}/src/gl/Shader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gl/Program.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gl/Texture.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gl/Framebuffer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gl/DebugLog.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gl/VAO.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/gl/Buffer.cpp"

    "${CMAKE_CURRENT_LIST_DIR}/src/sdl/SdlError.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/sdl/Window.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/sdl/SdlEventPump.cpp"
)

# --------------------------------------------------------------------------- #
# Installation Instructions
include(GNUInstallDirs)
install(TARGETS libtetra
    EXPORT Tetra-Export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Required so the exported target has the name 'libTetra' not libtetra
set_target_properties(libtetra PROPERTIES EXPORT_NAME libTetra)

# Install the include files
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Tetra)

# Export targets to a script
install(EXPORT Tetra-Export
    FILE
        TetraTargets.cmake
    NAMESPACE
        Tetra::
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

# Create ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/TetraConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/TetraConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/TetraConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# install the config
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/TetraConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/TetraConfigVersion.cmake
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

# --------------------------------------------------------------------------- #
# export from the build tree

export(
    EXPORT
        Tetra-Export
    FILE
        ${CMAKE_CURRENT_BINARY_DIR}/TetraTargets.cmake
    NAMESPACE
        Tetra::
)

# register the package in the user's package repository
export(PACKAGE Tetra)

# --------------------------------------------------------------------------- #
# include tests
add_subdirectory(tests)
