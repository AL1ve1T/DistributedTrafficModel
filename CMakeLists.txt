# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("DistributedTrafficModel")

# Include sub-projects.
add_subdirectory ("src")

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

set(BOOST_SOURCE_DIR "/usr/local/Cellar/boost/1.66.0")

include_directories(${BOOST_SOURCE_DIR}/include)
link_directories(${BOOST_SOURCE_DIR}/lib)
