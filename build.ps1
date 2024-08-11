cl ./src/main.c ./src/engine/runtime.c `
user32.lib `
/I"./src" /Fe"./bin/ninetails.exe" /Fo"./bin/" /Fd"./bin/" /Zi `
-DNX_DEBUG_BUILD -DNX_DEBUG_CONSOLE -DNX_DEBUG_USE_PEDANTIC_ASSERT `
/link /SUBSYSTEM:WINDOWS
