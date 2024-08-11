cl ./src/main.c ./src/engine/runtime.c ./src/core/arena.c ./src/core/memoryops.c `
user32.lib `
/I"./src" /Fe"./bin/ninetails.exe" /Fo"./bin/" /Fd"./bin/" /Zi /MT `
-DNX_DEBUG_BUILD -DNX_DEBUG_CONSOLE -DNX_DEBUG_USE_PEDANTIC_ASSERT `
/link /SUBSYSTEM:WINDOWS
