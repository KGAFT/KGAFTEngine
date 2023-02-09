cmake_minimum_required(VERSION 3.24)
set(ProjectName "KGAFTEngine")


#insert path to your vulkan sdk here
set(VulkanSDKPath C:/VulkanSDK/1.3.239.0)

project(${ProjectName})

include_directories(${VulkanSDKPath}/Include LibNativeWin/include)
link_directories(${VulkanSDKPath}/Bin ${VulkanSDKPath}/Lib LibNativeWin/lib)

file(GLOB_RECURSE BINARIES LibNativeWin/bin/*.dll)

file(COPY ${BINARIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY src/spir-v DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE CPP_FILES src/cxx/*.cpp)
file(GLOB_RECURSE HPP_FILES src/cxx/*.h)
add_executable(${ProjectName} ${HPP_FILES} ${CPP_FILES} src/cxx/Engine/PbrEngine.h)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

target_link_libraries(${ProjectName} vulkan-1 glfw glm shaderc_shared libassimpd.dll -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
