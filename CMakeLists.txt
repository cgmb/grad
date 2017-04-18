cmake_minimum_required(VERSION 2.8)
project(grad)
option(OPTIMIZE "Enable platform-specific optimizations")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include_directories(.)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS} SYSTEM)
include_directories(3rdparty/glad/include SYSTEM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-variable")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
endif()

if(${OPTIMIZE})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -ffast-math")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Werror")
endif()

set(SOURCES
 main.cxx
 opengl/opengl_common.cxx
 opengl/shader_program.cxx
 opengl/gl_model.cxx
 opengl/mesh.cxx
 misc/colormap.cxx
 3rdparty/glad/src/glad.c
)

add_executable(grad ${SOURCES})
target_link_libraries(grad
  ${GLFW_STATIC_LIBRARIES}
)
