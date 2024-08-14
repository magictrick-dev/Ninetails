// --- Windows Entry Point -----------------------------------------------------
//
// The Windows entry point is the black sheep of entry-points since we are forced
// to use a special entry point function and must manually reallocate the debug
// console. Other than that, it's pretty normal.
//

#if defined(_WIN32)
#include <windows.h>
#include <stdio.h>

#include <core/definitions.h>
#include <core/memoryops.h>

#include <platform/system.h>
#include <platform/filesystem.h>
#include <platform/window.h>
#include <platform/opengl.h>

#include <engine/runtime.h>

int WINAPI 
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

#   if defined(NX_DEBUG_CONSOLE)
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);
#   endif

    // --- Startup Preamble ----------------------------------------------------
    //
    // When the debug console is open, display the preamble header for the user.
    // For production builds, you will need to remove -DNX_DEBUG_CONSOLE flag from
    // the build script to ensure the user does not see this. This is separate from
    // the -DNX_DEBUG_BUILD flag, since non-debug builds may need access to cout.
    //
    
    printf("-- Ninetails Game Engine Version 1.7.0A\n");
    printf("-- Developed by Chris DeJong, magictrick-dev on GitHub\n");
    printf("-- Debugging Output Console (-DNX_DEBUG_CONSOLE)\n");
    printf("\n");

    // --- System Startup ------------------------------------------------------
    //
    // Before runtime begins, the game engine needs some critical resources that
    // must be loaded before runtime can even start. Think of it like an init step
    // for the init step. We collect memory, initialize any backend things like
    // lookup tables, and other critical stuff.
    //

    u64 application_memory_size = NX_GIGABYTES(4);
    u64 application_page_granularity = system_memory_page_size();
    printf("-- Runtime Memory Parameters\n");
    printf("--      %-32s : %llu bytes\n", "Application Memory Size", application_memory_size);
    printf("--      %-32s : %llu bytes\n", "Page Granularity Size", application_page_granularity);

    vptr application_memory_ptr = system_virtual_alloc(NULL, application_memory_size);
    printf("--      %-32s : OK!\n", "Application Memory Buffer");

    buffer heap_buffer = { application_memory_ptr, application_memory_size };

#   if defined(NX_DEBUG_BUILD)

        // Additional diagnostic information.
        printf("-- Runtime Diagnostics\n");
        u64 os_frequency = system_timestamp_frequency();
        printf("--      %-32s : %llu\n", "OS Frequency Interval", os_frequency);
        printf("--      %-32s : ...\r", "Estimating CPU Frequency");
        u64 cpu_frequency = system_cpustamp_frequency();
        printf("--      %-32s : OK!   \n", "Estimating CPU Frequency");
        printf("--      %-32s : %llu\n", "CPU Frequency Interval", cpu_frequency);

#   endif

    // --- Runtime Environment -------------------------------------------------
    //
    // Here is where we forward our execution to "user land". Since we don't want
    // to bulk out the entry function, we pass it over to our runtime functions.
    // We let these functions soft-fail gracefully.
    //

    if (!runtime_init(heap_buffer))
    {

        printf("-- Application initialization failed.\n");

#       if defined(NX_DEBUG_CONSOLE)
            printf("-- Press any key to continue.\n");
            getch();
#       endif

        return 1;
    }

    if (runtime_main(heap_buffer))
    {

        printf("-- Application main returned a non-zero return code.\n");

#       if defined(NX_DEBUG_CONSOLE)
            printf("-- Press any key to continue.\n");
            getch();
#       endif

        return 1;
    }

#   if defined(NX_DEBUG_CONSOLE)
        printf("-- Press any key to continue.\n");
        getch();
#   endif

    return 0;
}

#include <platform/win32/system.c>
#include <platform/win32/filesystem.c>
#include <platform/win32/window.c>
#include <platform/win32/opengl.c>

#else
#   error "Platform has not been defined."
#endif
