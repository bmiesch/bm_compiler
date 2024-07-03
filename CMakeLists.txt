cmake_minimum_required(VERSION 3.10)
project(BM_Compiler)

set(CMAKE_CXX_STANDARD 14)

# Include header files
file(GLOB_RECURSE HEADER_FILES "inc/*.h")
foreach(FILE ${HEADER_FILES})
    get_filename_component(DIR_PATH ${FILE} PATH)
    include_directories(${DIR_PATH})
endforeach()

# Include source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Find Capstone library and include directories
find_path(CAPSTONE_INCLUDE_DIR capstone/capstone.h)
find_library(CAPSTONE_LIBRARY capstone)

# Check if Capstone was found
if (NOT CAPSTONE_INCLUDE_DIR OR NOT CAPSTONE_LIBRARY)
    message(FATAL_ERROR "Capstone library not found. Please install Capstone.")
endif()

# Include Capstone headers
include_directories(${CAPSTONE_INCLUDE_DIR})

# Add executable
add_executable(main main.cpp ${SOURCES})

# Link Capstone library
target_link_libraries(main ${CAPSTONE_LIBRARY})