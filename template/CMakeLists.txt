cmake_minimum_required(VERSION 3.4.1)

set(LIB_NAME "sample")
set(BUILD_TYPE SHARED) # SHARED or STATIC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/${ANDROID_ABI})

project("${LIB_NAME}_test")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/3rd)

aux_source_directory(${CMAKE_SOURCE_DIR}/src srcs)

add_library(${LIB_NAME} ${BUILD_TYPE} ${srcs})
target_link_libraries(${LIB_NAME}
                    ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/)

aux_source_directory(${CMAKE_SOURCE_DIR}/test tests)
add_executable(${LIB_NAME}_test ${srcs} ${tests})
