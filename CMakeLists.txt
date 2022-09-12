cmake_minimum_required(VERSION 3.10)
project(learnGL LANGUAGES C CXX)

if(NOT
   CMAKE_SOURCE_DIR
   STREQUAL
   PROJECT_SOURCE_DIR)
  message(FATAL_ERROR "Project cannot be used as a subproject.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(deps)
include(warnings)
get_dependencies()
set_warnings()

add_library(Shader src/Shader/Shader.cpp)
target_include_directories(Shader PUBLIC src/Shader)
target_link_libraries(
  Shader
  PUBLIC glad glm::glm
  PRIVATE fmt::fmt)

add_library(Utils src/Utils/Utils.cpp)
target_include_directories(Utils PUBLIC src/Utils)

add_executable(HelloTriangle src/ch1-hello-triangle/HelloTriangle.cpp)
target_link_libraries(
  HelloTriangle
  PRIVATE fmt::fmt
          glad
          glfw
          Shader
          Utils)

get_filename_component(VERTEX_SHADER_FILE "src/ch1-hello-triangle/shader.vs" ABSOLUTE)
get_filename_component(HORIZONTAL_FRAGMENT_SHADER_FILE "src/ch1-hello-triangle/horizontal-shader.fs" ABSOLUTE)
get_filename_component(VERTICAL_FRAGMENT_SHADER_FILE "src/ch1-hello-triangle/vertical-shader.fs" ABSOLUTE)
configure_file(src/ch1-hello-triangle/HelloTriangleFiles.hpp.in HelloTriangleFiles.hpp)
