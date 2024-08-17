cl ./src/main.c ./src/engine/runtime.c ./src/core/arena.c ./src/core/memoryops.c `
./vnd/glad/glad.c ./vnd/glad/glad_wgl.c ./src/core/linear.c `
user32.lib gdi32.lib opengl32.lib /std:c++17 `
/I"./src" /I"./vnd" /Fe"./bin/ninetails.exe" /Fo"./bin/" /Fd"./bin/" /Zi /MT `
-DNX_DEBUG_BUILD -DNX_DEBUG_CONSOLE -DNX_DEBUG_USE_PEDANTIC_ASSERT `
/link /SUBSYSTEM:WINDOWS
