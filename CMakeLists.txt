cmake_minimum_required(VERSION 3.10)
project (main)

set(PROJECT_NAME main)

set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Options
option(BUILD_WEB "Build for Web (Emscripten)" OFF)
option(BUILD_ANDROID "Build for Android (NDK)" OFF)

# Compiler flags for different build types

if (BUILD_WEB)
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_C_COMPILER /Users/amrose/raylib_playground/emsdk-main/upstream/emscripten/emcc)
    set(CMAKE_CXX_COMPILER /Users/amrose/raylib_playground/emsdk-main/upstream/emscripten/em++)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(PLATFORM "Web" CACHE STRING "" FORCE)
    add_definitions(-DGRAPHICS_API_OPENGL_ES2)
    add_definitions(-DPLATFORM_WEB)

    # Web-specific compiler and linker flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s ASSERTIONS=1 -s TOTAL_MEMORY=16777216 -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s ASSERTIONS=1 -s TOTAL_MEMORY=16777216 -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s ASSERTIONS=1 -s TOTAL_MEMORY=16777216 -s GL_ENABLE_GET_PROC_ADDRESS=1 --shell-file ${CMAKE_SOURCE_DIR}/raylib-master/src/minshell.html --preload-file ${CMAKE_SOURCE_DIR}/resources@/resources")

    # Set output directory
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/web)

    # Disable finding threads for Emscripten
    set(THREADS_PREFER_PTHREAD_FLAG OFF)
    set(CMAKE_THREAD_LIBS_INIT "")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 0)
    set(THREADS_FOUND 1)
elseif (BUILD_ANDROID)
    
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    string(REGEX MATCH "android-([0-9]+)" _ ${ANDROID_PLATFORM} FORCE)
    set(ANDROID_API_VERSION ${CMAKE_MATCH_1} CACHE STRING "" FORCE)
    set(ANDROID_ARCH ${CMAKE_ANDROID_ARCH} CACHE STRING "" FORCE)
    set(PLATFORM "Android" CACHE STRING "" FORCE)
    add_definitions(-DGRAPHICS_API_OPENGL_ES2)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -DPLATFORM_ANDROID -DGRAPHICS_API_OPENGL_ES2 -m64 -g -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -DPLATFORM_ANDROID -DGRAPHICS_API_OPENGL_ES2 -m64 -g -Wno-deprecated-declarations")
    set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -DPLATFORM_ANDROID -DGRAPHICS_API_OPENGL_ES2 -m64 -O2 -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -DPLATFORM_ANDROID -DGRAPHICS_API_OPENGL_ES2 -m64 -O2 -Wno-deprecated-declarations")


else()
    message(STATUS "going to desktop build")
    # Desktop-specific configuration
    set(CMAKE_CXX_COMPILER /usr/bin/c++)
    set(CMAKE_C_COMPILER /usr/bin/cc)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_C_FLAGS_DEBUG "-DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -m64 -g -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -m64 -g -Wno-deprecated-declarations")
    set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -m64 -O2 -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -m64 -O2 -Wno-deprecated-declarations")
endif()

add_subdirectory(raylib-master)
add_subdirectory(box2d)


# Include directories
include_directories(
    game
    game/src
    game/include
    game/src/include
    raylib-master/src
    raylib-master/src/external
    raylib-master/src/external/glfw/include
)

# Source files
file(GLOB_RECURSE SOURCES game/src/*.cpp)

if (ANDROID)
    add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})
else()
    add_executable(game ${SOURCES})
endif()

# Link libraries
target_link_libraries(game
    raylib
    box2d
)

if (APPLE)
    target_link_libraries(game "-framework IOKit")
    target_link_libraries(game "-framework Cocoa")
    target_link_libraries(game "-framework OpenGL")
    target_link_libraries(game "-framework CoreFoundation")
    target_link_libraries(game "-framework CoreAudio")
    target_link_libraries(game "-framework CoreVideo")
    target_link_libraries(game "-framework AudioToolbox")
endif()

if (ANDROID)
    target_link_libraries(game log android raylib box2d)
endif()

# Set output directories
set_target_properties(game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
)

if (BUILD_WEB)
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
elseif(BUILD_ANDROID)
else()
    add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
endif()
