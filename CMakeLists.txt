cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

project(VoxChis)

add_subdirectory(VulkanChis)

include_directories(.)

file(GLOB SRC_FILES *.cpp)
add_executable(VoxChis ${SRC_FILES})

target_link_libraries(VoxChis PRIVATE VKChis)