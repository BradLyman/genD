target_include_directories(graph PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/inc"
)
target_sources(graph PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/src/glad.c"
)
target_link_libraries(graph PUBLIC ${CMAKE_DL_LIBS})
