cmake_minimum_required(VERSION 3.21)
project(OpenCraft)
include(FetchContent)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/lukasino1214/glad.git)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_Declare(FastNoise2 GIT_REPOSITORY https://github.com/lukasino1214/FastNoise2.git)
FetchContent_Declare(Tracy GIT_REPOSITORY https://github.com/wolfpld/tracy.git)
FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git)

FetchContent_GetProperties(stb)
if(NOT stb)
    FetchContent_Populate(stb)
    #include the headerfiles and libs
endif()

FetchContent_MakeAvailable(Tracy)
FetchContent_MakeAvailable(glfw glad glm FastNoise2 stb)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set_option(TRACY_ENABLE "Enable profiling" ON)
add_compile_definitions(TRACY_ENABLE)
add_executable(OpenCraft
        src/main.cpp
        src/Client/Rendering/Buffers.cpp
        src/Client/Rendering/Camera.cpp 
        src/Client/Rendering/Shader.cpp
        src/Client/Rendering/VertexArray.cpp
        src/Client/Voxel/Block.h
        src/Client/Voxel/Chunk.cpp
        src/Client/Voxel/ChunkManager.cpp
        src/Tracy/TracyClient.cpp
        src/Tracy/Tracy.hpp
        )

target_link_libraries(OpenCraft glfw glad glm FastNoise2)
target_include_directories(OpenCraft PRIVATE ${stb_SOURCE_DIR})