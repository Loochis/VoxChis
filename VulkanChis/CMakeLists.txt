file(GLOB SRC_FILES *.cpp)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(VKChis ${SRC_FILES})

target_link_libraries(VKChis PRIVATE glfw vulkan Threads::Threads ${CMAKE_DL_LIBS})