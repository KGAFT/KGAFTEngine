cmake_minimum_required(VERSION 3.24)
set(Recipe_Name "KGAFTEngine")


#insert path to your vulkan sdk here
set(VulkanSDKPath C:/VulkanSDK/1.3.231.1/)

project(${Recipe_Name})

include_directories(${VulkanSDKPath}/Include)
link_directories(${VulkanSDKPath}/Bin ${VulkanSDKPath}/Lib)

file(COPY src/spir-v DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE CPP_FILES src/cxx/*.cpp)
file(GLOB_RECURSE HPP_FILES src/cxx/*.h)
add_executable(${Recipe_Name} ${HPP_FILES} ${CPP_FILES})

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

target_link_libraries(${Recipe_Name} vulkan-1 glfw glm shaderc_shared -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
