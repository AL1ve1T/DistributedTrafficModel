# CMakeList.txt : CMake project for DistributedTrafficModel, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.7)

# Add source to this project's executable.
add_executable (app.exe "DistributedTrafficModel.cpp" "include/DistributedTrafficModel.h")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/debug)

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)

# Windows-specific build settings
if (UNIX)
    set(BOOST_ROOT "/cygdrive/c/boost_1_68_0")
    set(BOOST_INDLUDEDIR "/cygdrive/c/boost_1_68_0/boost")
    set(BOOST_LIBRARYDIR "/cygdrive/c/boost_1_68_0/stage/lib")
elseif (MSVC)
    set(BOOST_SOURCE_DIR "/usr/local/Cellar/boost/1.68.0")
endif()

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs 
set(Boost_USE_DEBUG_LIBS         ON)  # ignore debug libs and 
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs 
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF) 
set(Boost_USE_STATIC_LIBS       OFF)

set(Boost_DEBUG ON)

find_package(Boost 1.66.0)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(app.exe ${Boost_LIBRARIES})
endif()

# TODO: Add tests and install targets if needed.
