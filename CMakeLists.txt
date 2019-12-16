cmake_minimum_required(VERSION 2.8)

project(brtirus0805a)
# 设置输出文件路劲
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
SET(LIBRARY_OUTPUT_PATH    ${CMAKE_SOURCE_DIR}/build/lib)
aux_source_directory(./src  DIR_SRCS)
aux_source_directory(./test TEST_SRCS)
include_directories(
    ./
    ./src
    ./test
    )
link_directories(${LIBRARY_OUTPUT_PATH})

add_library(${PROJECT_NAME}_dev STATIC ${DIR_SRCS})

add_executable(${PROJECT_NAME} ${TEST_SRCS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_dev)
