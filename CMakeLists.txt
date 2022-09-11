cmake_minimum_required(VERSION 3.10)
project(learnGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(deps)
include(warnings)
get_dependencies()

add_executable(hello_world src/main.cpp)
target_link_libraries(
  hello_world
  PRIVATE fmt::fmt
          glad
          glfw
          glm::glm
          stb_image
          warnings)
