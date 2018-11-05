cmake_minimum_required(VERSION 3.4.1)

set(LIB_NAME "audio_al")
set(BUILD_TYPE STATIC) # SHARED or STATIC
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})

add_definitions(-std=c++11)
add_definitions(-frtti)
add_definitions(-fexceptions)

project("${LIB_NAME}_test")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/3rd)

aux_source_directory(${CMAKE_SOURCE_DIR}/src srcs)

add_library(${LIB_NAME} ${BUILD_TYPE} ${srcs})
target_link_libraries(${LIB_NAME}
                    ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libopenal.a)

aux_source_directory(${CMAKE_SOURCE_DIR}/test tests)
add_executable(${LIB_NAME}_test ${tests})
