cmake_minimum_required(VERSION 3.7.1)

set(Recipe_Name "KGAFTEngine")

project(${Recipe_Name})

include_directories(C:\\VulkanSDK\\1.3.231.1\\Include)
link_directories(C:\\VulkanSDK\\1.3.231.1\\Bin32 C:\\VulkanSDK\\1.3.231.1\\Bin C:\\VulkanSDK\\1.3.231.1\\Lib)

file(COPY src/spir-v DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE CPP_FILES src/cxx/*.cpp)
file(GLOB_RECURSE HPP_FILES src/cxx/*.h)
add_executable(${Recipe_Name} ${HPP_FILES} ${CPP_FILES} src/cxx/VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

target_link_libraries(${Recipe_Name} vulkan-1 glfw glm shaderc_shared -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)

