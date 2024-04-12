cmake_minimum_required(VERSION 3.21.0)
project(Demo)
 
# include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
# conan_basic_setup()
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp)
message(STATUS "==================================")
message(STATUS ${SRCS})
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${CONAN_LIBS})

# find_package(protobuf REQUIRED)
# find_package(lz4 REQUIRED)
# target_link_libraries(YOUR_TARGET protobuf::protobuf)
# find_package(zstd REQUIRED)
 
add_executable(${PROJECT_NAME} 
main.cpp
data.pb.cc
${SRCS}
)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} )

