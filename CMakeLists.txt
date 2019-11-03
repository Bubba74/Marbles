
cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)

project (NormalDistribution)

include_directories("include")
add_subdirectory("src")
include_directories("glm")

file( COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}" )

add_executable(NormalDistribution main.cpp)
target_link_libraries(NormalDistribution Source)
target_link_libraries(NormalDistribution glfw dl)
