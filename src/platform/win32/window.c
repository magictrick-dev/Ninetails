#include <windows.h>
#include <platform/window.h>

typedef struct window_state
{

    ccptr title;
    i32 known_width;
    i32 known_height;
    b32 is_minimized;
    b32 is_maximized;
    b32 is_focused;

    HWND viewable_window_handle;
    HWND ghost_window_handle;
    HANDLE ghost_thread_sync;
    HANDLE ghost_thread_handle;
    DWORD ghost_thread_identifier;
    DWORD main_thread_identifier;
    BOOL ghost_thread_initialized;
    BOOL ghost_thread_runtime;

} window_state;

static inline window_state* get_window_state() { static window_state ws = {0}; return &ws };

// --- Platform Window ---------------------------------------------------------
//
// The following functions below correspond to the front-facing API functions.
// There are additional helper functions that are needed to drive window behavior
// as well as the appropriate hooks needed to enable hardware acceleration.
//


// --- Win32 Backend API Functions ---------------------------------------------
//
// Due to the complex nature of window management on Win32, there are a set of
// backend functions and data management routines that need to be implemented to
// ensure that the window runs properly.
//


