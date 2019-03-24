# CMakeList.txt : CMake project for DistributedTrafficModel, 
# include source and define
# project specific logic here.
#
#   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
#                               <aliveit.elnur@gmail.com>
# 

cmake_minimum_required (VERSION 3.7)

# Add source to this project's executable.
add_executable (app "main.cpp")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/debug)

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs 
set(Boost_USE_DEBUG_LIBS         ON)  # ignore debug libs and 
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs 
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF) 
set(Boost_USE_STATIC_LIBS       OFF)

set(Boost_DEBUG ON)

find_package(Boost 1.62.0 REQUIRED system date_time serialization thread)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(app ${Boost_LIBRARIES})
endif()

# Copy resource directory into binary tree
file(COPY "../resource" DESTINATION "../build/debug/resource")

# TODO: Add tests and install targets if needed.
