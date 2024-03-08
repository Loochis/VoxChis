file(GLOB SUBSYS_FILES VKCSubsystem/*.cpp)
file(GLOB DYNAMIC_FILES VKCDynamicComponents/*.cpp)
file(GLOB ABSTRACT_FILES VKCAbstractions/*.cpp)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(VKChis ${DYNAMIC_FILES} ${SUBSYS_FILES})

target_link_libraries(VKChis PRIVATE glfw vulkan Threads::Threads OpenSSL::SSL ${CMAKE_DL_LIBS})