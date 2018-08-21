add_executable(tetra_test src/main.cpp)
set_target_properties(tetra_test PROPERTIES SUFFIX .out)
target_link_libraries(tetra_test Tetra::libTetra)

target_sources(tetra_test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/ColorTest.cpp
)

