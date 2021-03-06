
cmake_minimum_required(VERSION 3.6)

project(mono1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")

# Chipmunk2D
set(BUILD_SHARED OFF)
set(BUILD_STATIC ON)
set(BUILD_DEMOS OFF CACHE BOOL "Build chipmunk demo application" FORCE)
set(LIB_INSTALL_DIR ${LIBRARY_OUTPUT_PATH})
add_subdirectory("third_party/Chipmunk2D")


include_directories("MonoEngine")
include_directories(SYSTEM "third_party")
include_directories(SYSTEM ${chipmunk_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
endif(OPENGL_FOUND)

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
    include_directories(SYSTEM ${OPENAL_INCLUDE_DIR})
endif(OPENAL_FOUND)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
endif(SDL2_FOUND)

# imgui
file(GLOB_RECURSE imgui_source_files "third_party/imgui/*.cpp")
add_library(imgui STATIC ${imgui_source_files})
set_target_properties(imgui PROPERTIES COMPILE_FLAGS -Wno-implicit-fallthrough)

# gtest
file(GLOB_RECURSE gtest_source_files "third_party/gtest-1.7.0/src/*.cc")
add_library(gtest STATIC ${gtest_source_files})
target_include_directories(gtest PRIVATE "third_party/gtest-1.7.0/include" "third_party/gtest-1.7.0")
set_target_properties(gtest PROPERTIES COMPILE_FLAGS -Wno-missing-field-initializers)

# Mono
file(GLOB_RECURSE engine_source_files "MonoEngine/*.cpp")
add_library(mono STATIC ${engine_source_files})
add_dependencies(mono imgui chipmunk_static)
target_link_libraries(mono imgui chipmunk_static ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENAL_LIBRARY})

# Unit test
file(GLOB_RECURSE unittest_source_files "UnitTests/*.cpp")
add_executable(unittest ${unittest_source_files})
add_dependencies(unittest mono gtest)
target_include_directories(unittest PRIVATE "third_party/gtest-1.7.0/include")
target_link_libraries(unittest mono gtest)
