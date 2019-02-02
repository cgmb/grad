cmake_minimum_required(VERSION 2.8.12)
project(grad)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(AutoCCache)
include(IncludeShaders)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(UNIX)
  set(GLAD_LIBRARIES "dl")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=reorder")

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_ASSERTIONS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")
  endif()
endif()

if(WIN32)
  set(DEFINES
    _CRT_SECURE_NO_WARNINGS
    _USE_MATH_DEFINES
    NOMINMAX
  )
endif()

add_shaders(SHADERS
  shaders/gradient.frag
  shaders/gradient.vert
)

add_executable(grad
  main.cxx
  opengl/opengl_common.cxx
  opengl/shader_program.cxx
  opengl/gl_model.cxx
  opengl/mesh.cxx
  misc/colormap.cxx
  3rdparty/glad/src/glad.c
  ${SHADERS}
)
target_link_libraries(grad
  glfw
  ${GLAD_LIBRARIES}
)
target_include_directories(grad SYSTEM PRIVATE
  3rdparty/glad/include
)
target_include_directories(grad PRIVATE
  .
)
target_compile_definitions(grad PRIVATE
  ${DEFINES}
)
