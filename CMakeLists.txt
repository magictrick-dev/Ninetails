CMAKE_MINIMUM_REQUIRED(VERSION 3.21)

PROJECT(ninetails)

ADD_EXECUTABLE(ninetails WIN32
    "src/main.c"
    "src/engine/runtime.h"
    "src/engine/runtime.c"
    "src/engine/primitives.h"
    "src/core/definitions.h"
    "src/core/arena.h"
    "src/core/arena.c"
    "src/core/memoryops.h"
    "src/core/memoryops.c"
    "src/core/linear.h"
    "src/core/linear.c"
    "src/platform/filesystem.h"
    "src/platform/input.h"
    "src/platform/opengl.h"
    "src/platform/system.h"
    "src/platform/window.h"
    "vnd/glad/glad.c"
    "vnd/glad/glad.h"
    "vnd/glad/glad_wgl.c"
    "vnd/glad/glad_wgl.h"
    "vnd/glad/khrplatform.h"
)

TARGET_INCLUDE_DIRECTORIES(ninetails PUBLIC "src/" "vnd/")

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../bin)
SET(CMAKE_CXX_STANDARD 20)

IF (WIN32)
    SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
        VS_STARTUP_PROJECT ninetails) 
    ADD_DEFINITIONS(/MP /MT /Zi /Debug
        -DNX_DEBUG_CONSOLE
        -DNX_DEBUG_USE_PEDANTIC_ASSERT)
ENDIF (WIN32)

FIND_PACKAGE(OpenGL REQUIRED)

TARGET_LINK_LIBRARIES(ninetails
    ${OPENGL_LIBRARY}
    winmm.lib
    Shlwapi.lib
    Xinput.lib
    Xaudio2.lib
)

TARGET_COMPILE_DEFINITIONS(ninetails PUBLIC NX_DEBUG_BUILD NX_DEBUG_CONSOLE)
