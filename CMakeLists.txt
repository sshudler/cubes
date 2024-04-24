cmake_minimum_required(VERSION 3.15)

project(Cubes)

set(CMAKE_CXX_STANDARD 14)

# Load GLFW3.3
find_package(glfw3 3.3 REQUIRED)

set(SOURCES
    glad/src/glad.c     # Should be compiled first
    misc/stb_image.h
    scenegraph/mesh.cxx
    scenegraph/mesh.hxx
    scenegraph/model.cxx
    scenegraph/model.hxx
    scenegraph/node.cxx
    scenegraph/node.hxx
    camera.hxx
    environment.cxx
    environment.hxx
    main.cxx
    shaderprog.cxx
    shaderprog.hxx
    texture.cxx
    texture.hxx
)

add_executable(cubes ${SOURCES})

target_link_libraries(cubes glfw)
target_include_directories(cubes PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/glad/include
)
