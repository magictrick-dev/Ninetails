cl ./src/main.c user32.lib `
/I"./src" /Fe"./bin/ninetails.exe" /Fo"./bin/" /Fd"./bin/" /Zi -DNX_DEBUG_CONSOLE `
/link /SUBSYSTEM:WINDOWS
