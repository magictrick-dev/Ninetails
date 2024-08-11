#include <core/definitions.h>
#include <engine/runtime.h>

#if defined(_WIN32)
#   include <windows.h>
#   include <stdio.h>

int WINAPI 
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

#if defined(NX_DEBUG_CONSOLE)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    printf("Hello, world.\n");
    MessageBoxA(NULL, "Hello", "World", MB_OK);

    return 0;
}

#else
#   error "Platform has not been defined."
#endif

