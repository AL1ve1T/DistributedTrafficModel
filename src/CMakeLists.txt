# CMakeList.txt : CMake project for DistributedTrafficModel, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.2)

# Add source to this project's executable.
add_executable (app.exe "DistributedTrafficModel.cpp" "include/DistributedTrafficModel.h")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/debug)

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

set(BOOST_SOURCE_DIR "/usr/local/Cellar/boost/1.68.0")

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         ON)  # ignore debug libs and 
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs 
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF) 

set(Boost_DEBUG ON)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(app.exe ${Boost_LIBRARIES})

# TODO: Add tests and install targets if needed.
