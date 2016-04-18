cmake_minimum_required(VERSION 2.8)
project(test_iss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -W -Wall -Wextra -pthread -std=c++14 -Wl,--no-as-needed")
MESSAGE("-- CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

aux_source_directory(. SRC_LIST)
aux_source_directory(./send/impl SRC_LIST)
aux_source_directory(./receive/impl SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
