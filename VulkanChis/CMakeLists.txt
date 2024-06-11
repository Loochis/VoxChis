file(GLOB VULKCOMP_FILES VKCVulkanComponents/*.cpp)
file(GLOB UTIL_FILES VKCUtils/*.cpp)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(VKChis VKCManager.cpp ${VULKCOMP_FILES} ${UTIL_FILES})

target_link_libraries(VKChis PRIVATE glfw vulkan Threads::Threads OpenSSL::SSL ${CMAKE_DL_LIBS})