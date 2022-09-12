cmake_minimum_required(VERSION 3.10)
project(learnGL LANGUAGES C CXX)

if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(FATAL_ERROR "Project cannot be used as a subproject.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

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
