find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

if(NOT TARGET Tetra::libTetra)
    get_filename_component(Tetra_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    include("${Tetra_CMAKE_DIR}/TetraTargets.cmake")
endif()

set(Tetra_LIBRARIES Tetra::libTetra)
