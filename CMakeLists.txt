cmake_minimum_required (VERSION 2.8)

# include(cmake/internal.cmake)
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_CXX_COMPILER "g++")

# PROJECT_NAME
project (sig-mesh-demo)

#add_compile_options(-std=c++11)
add_compile_options(-g)
add_compile_options(-Wno-pointer-to-int-cast)
add_compile_options(-Wno-int-to-pointer-cast)

# compile aux_source_directory
aux_source_directory(. DIR_SRCS)
aux_source_directory(./cjson DIR_SRCS)
aux_source_directory(./core DIR_SRCS)
aux_source_directory(./efs DIR_SRCS)
aux_source_directory(./platform DIR_SRCS)

#include include_directories
include_directories(.)
include_directories(./cjson)
include_directories(./core)
include_directories(./efs)
include_directories(./platform)

#target
add_executable(${PROJECT_NAME} ${DIR_SRCS})
# link libarary
target_link_libraries (${PROJECT_NAME}  pthread)
