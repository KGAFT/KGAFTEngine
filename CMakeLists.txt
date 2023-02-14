cmake_minimum_required(VERSION 3.24)
set(ProjectName "KGAFTEngine")


#insert path to your vulkan sdk here
set(VulkanSDKPath /home/kgaft/Documents/TarBalls/VulkanSDK/vulkansdk-linux-x86_64-1.3.239.0/1.3.239.0/x86_64/)

project(${ProjectName})

if(WIN32)
    include_directories(${VulkanSDKPath}/Include LibNativeWin/include)
    link_directories(${VulkanSDKPath}/Bin ${VulkanSDKPath}/Lib LibNativeWin/lib)
    file(GLOB_RECURSE BINARIES LibNativeWin/bin/*.dll)

    set(VulkanLibList shaderc_shared vulkan-1)
    set(NativesLibs libassimpd.dll)
endif()

if(UNIX)
    include_directories(${VulkanSDKPath}/include LibNativeLin/include)
    link_directories(${VulkanSDKPath}/bin ${VulkanSDKPath}/lib LibNativeLin/lib)
    file(GLOB_RECURSE BINARIES LibNativeLin/bin/*.so)

    set(VulkanLibList vulkan libshaderc_combined.a)
    set(NativesLibs libassimp.so)
endif()

file(COPY ${BINARIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY src/spir-v DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE CPP_FILES src/cxx/*.cpp)
file(GLOB_RECURSE HPP_FILES src/cxx/*.h)
add_executable(${ProjectName} ${HPP_FILES} ${CPP_FILES} src/cxx/Engine/PbrEngine.h src/cxx/Engine/Util/AssimpGLMHelper.h)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

target_link_libraries(${ProjectName} ${VulkanLibList} glfw glm ${NativesLibs} -static-libgcc -Wl,-Bstatic -lpthread -Wl,-Bdynamic)
