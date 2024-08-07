cmake_minimum_required(VERSION 3.5)

project(ServerMessengerProj LANGUAGES CXX)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.85.0)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(ServerMessengerProj main.cpp)
    target_link_libraries(ServerMessengerProj ${Boost_LIBRARIES})
endif()

if(WIN32)
    target_link_libraries(ServerMessengerProj wsock32 ws2_32)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
