cmake_minimum_required(VERSION 3.24)
set(ProjectName "KGAFTEngine")

set(CMAKE_CXX_STANDARD 17)

#insert path to your vulkan sdk here
set(VulkanSDKPath C:/VulkanSDK/1.3.243.0)

project(${ProjectName})

if(WIN32)
    include_directories(LibNativeWin/include)
    link_directories(LibNativeWin/lib)
    file(GLOB_RECURSE BINARIES LibNativeWin/bin/*.dll)

    set(NativesLibs libassimpd.dll)
    set(SystemNatives -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
endif()

if(UNIX)
    include_directories(LibNativeLin/include)
    link_directories(LibNativeLin/lib)
    file(GLOB_RECURSE BINARIES LibNativeLin/bin/*.so)

    set(NativesLibs libassimp.so)
    set(SystemNatives -static-libgcc -Wl,-Bstatic -lpthread -Wl,-Bdynamic)
endif()

file(COPY ${BINARIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY src/spir-v DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE CPP_FILES src/cxx/*.cpp)
file(GLOB_RECURSE HPP_FILES src/cxx/*.h)
add_executable(${ProjectName} ${HPP_FILES} ${CPP_FILES})

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
add_subdirectory(src/modules/VulkanTools)

target_link_libraries(${ProjectName} VulkanContext  glfw glm ${NativesLibs} ${SystemNatives})
