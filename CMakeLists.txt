cmake_minimum_required(VERSION 3.31)
project(render-train)

# -- sources --

file(GLOB_RECURSE sources src/*.c lib/*.c)
file(GLOB_RECURSE shaders src/shaders/*)

# -- libraries --

# define glfw library
add_library(glfw SHARED IMPORTED)
set_target_properties(glfw PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libglfw.3.dylib"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
)

# -- target --
# add executable from source files
add_executable(game ${sources})

# set include paths
target_include_directories(game
  PRIVATE
    src
)

# link libraries
target_link_libraries(game
  PRIVATE
    glfw
    "-framework OpenGL"
)

# copy shaders relative to build
file(COPY ${shaders} DESTINATION shaders)