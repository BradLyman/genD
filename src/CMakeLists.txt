target_sources(graph PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/GlApp.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/ColoredLine.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/Turtle.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/gl/Shader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/gl/Program.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/gl/VAO.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/gl/Buffer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/sdl/SdlError.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tetra/sdl/Window.cpp"
)
