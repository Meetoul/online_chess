cmake_minimum_required(VERSION 2.8.12)

project(chess)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set (CMAKE_BUILD_TYPE Debug)
#set (CMAKE_BUILD_TYPE Release)
# Boost
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)


set (CMAKE_CXX_STANDARD 14)

find_package(Boost 1.58  REQUIRED COMPONENTS
    system
    coroutine
    thread
    context
    serialization)
if(Boost_FOUND )
  include_directories(${Boost_INCLUDE_DIRS})
endif()

FILE(GLOB_RECURSE HEADER "*engine/*.h")
#FILE(GLOB_RECURSE SRC "engine/*.cpp")
SET (SRC engine/chessboard.cpp
    engine/global.cpp
    engine/aiplayer.cpp    
    engine/perfomancemeasurement.cpp
    engine/config.cpp
    engine/asyncplayer.cpp
    engine/asyncaiplayer.cpp
    engine/asyncgame.cpp

    engine/asynconlineplayer.cpp
    
    server/server.cpp)

include_directories("engine")

add_executable(chess_client client/chess_client.cpp ${SRC} ${HEADER})
target_link_libraries(chess_client ${Boost_LIBRARIES})

add_executable(chess_server server/chess_server.cpp ${SRC} ${HEADER})
target_link_libraries(chess_server ${Boost_LIBRARIES})
