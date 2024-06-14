cmake_minimum_required(VERSION 3.10)
project(BM_Compiler)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE HEADER_FILES "inc/*.h")
foreach(FILE ${HEADER_FILES})
    get_filename_component(DIR_PATH ${FILE} PATH)
    include_directories(${DIR_PATH})
endforeach()

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main main.cpp ${SOURCES})