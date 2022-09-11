cmake_minimum_required(VERSION 3.0)
project(learnGL LANGUAGES C)

set(MAIN_SOURCES "src/main.c" "src/utils.c")
set(SHADER_CLASS "src/shader/shader.c")

find_package(GLFW3 CONFIG REQUIRED)

function(set_options_and_libs target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(
      ${target}
      PRIVATE "-Wall"
              "-Wextra"
              "-Wshadow"
              "-Wcast-align"
              "-Wunused"
              "-Wpedantic"
              "-Wconversion"
              "-Wsign-conversion"
              "-Wnull-dereference"
              "-Wdouble-promotion"
              "-Wformat=2"
              "-Wimplicit-fallthrough"
              "-Wunreachable-code"
              "-Wdeclaration-after-statement")
  endif()
  set_target_properties(${target} PROPERTIES C_STANDARD 90)

  target_include_directories(${target} PRIVATE "external/glad/include"
                                               "${CMAKE_CURRENT_BINARY_DIR}")
  target_sources(${target} PRIVATE "external/glad/src/glad.c")

  target_link_libraries(${target} PRIVATE glfw)
endfunction()

add_executable(HelloTriangle ${MAIN_SOURCES} "src/ch1-triangle/triangle.c")
set_options_and_libs(HelloTriangle)

add_executable(Shaders ${MAIN_SOURCES} ${SHADER_CLASS}
                       "src/ch2-shaders/shading.c")
get_filename_component(VERTEX_SHADER_FILE "src/ch2-shaders/example.vs" ABSOLUTE)
get_filename_component(FRAGMENT_SHADER_FILE "src/ch2-shaders/example.fs"
                       ABSOLUTE)
configure_file(src/ch2-shaders/shading_files.cmake shading_files.h)
set_options_and_libs(Shaders)

add_executable(Texture ${MAIN_SOURCES} ${SHADER_CLASS}
                       "src/ch3-texture/texture.c")
get_filename_component(VERTEX_SHADER_FILE "src/ch3-texture/shades.vs" ABSOLUTE)
get_filename_component(FRAGMENT_SHADER_FILE "src/ch3-texture/shades.fs"
                       ABSOLUTE)
get_filename_component(SMUG_TEXT "src/ch3-texture/smug.jpg" ABSOLUTE)
get_filename_component(SHOCK_TEXT "src/ch3-texture/shock.jpg" ABSOLUTE)
configure_file(src/ch3-texture/texture_files.cmake texture_files.h)
set_options_and_libs(Texture)
