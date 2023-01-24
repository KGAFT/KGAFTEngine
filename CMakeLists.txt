cmake_minimum_required(VERSION 3.22)

project(VulkanCXXLearn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CURRENT_BINARY_DIR output)

file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/*.h)
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/*.cpp)

find_package(Vulkan REQUIRED)

add_executable(VulkanCXXLearn ${HPP_FILES} ${CPP_FILES})

add_subdirectory(lib/glm)
add_subdirectory(lib/glfw)

target_include_directories(VulkanCXXLearn PUBLIC ${VULKAN_INCLUDE_DIR})
target_link_libraries(VulkanCXXLearn glm glfw ${Vulkan_LIBRARY})

