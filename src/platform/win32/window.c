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
    b32 is_resizable;
    b32 is_visible;
    b32 quit_received;

    HWND main_window_handle;
    HWND ghost_window_handle;
    HANDLE ghost_thread_sync;
    HANDLE ghost_thread_handle;
    DWORD ghost_thread_identifier;
    DWORD main_thread_identifier;
    BOOL ghost_thread_initialized;
    BOOL ghost_thread_runtime;

} window_state;

typedef struct 
window_creation_context
{
    DWORD       ex_style;
    LPCWSTR     class_name;
    LPCWSTR     window_name;
    DWORD       style;
    int         x;
    int         y;
    int         width;
    int         height;
    HWND        parent_window;
    HMENU       menu;
    HINSTANCE   instance;
    LPVOID      user_parameter;
} window_creation_context;

#define UD_CREATE_WINDOW    (WM_USER + 0x0042)
#define UD_DESTROY_WINDOW   (WM_USER + 0x0043)

static inline window_state* get_window_state() { static window_state ws = {0}; return &ws; };

LRESULT CALLBACK wGhostWindowProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
LRESULT CALLBACK wMainWindowProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
DWORD WINAPI wGhostThreadMain(LPVOID user);

// --- Platform Window ---------------------------------------------------------
//
// The following functions below correspond to the front-facing API functions.
// There are additional helper functions that are needed to drive window behavior
// as well as the appropriate hooks needed to enable hardware acceleration.
//

b32
window_initialize(ccptr title, i32 width, i32 height, b32 show)
{

    window_state *state = get_window_state();
    NX_PEDANTIC_ASSERT(state != NULL);
    NX_PEDANTIC_ASSERT(state->ghost_window_handle == NULL);
    NX_PEDANTIC_ASSERT(state->main_window_handle == NULL);
    NX_PEDANTIC_ASSERT(state->ghost_thread_initialized == FALSE);

    // Essentially, we launch a separate thread that contains a ghost window
    // procedure that we send window create messages to. This will force the
    // window procedure to be run on that thread rather than main, thereby fixing
    // blocking window events like resize, maximize, unfocus, etc.

    state->main_thread_identifier = GetCurrentThreadId();
    state->ghost_thread_runtime = true;
    state->ghost_thread_sync = CreateEventA(NULL, FALSE, FALSE, NULL);
    state->ghost_thread_handle = CreateThread(0, 0, wGhostThreadMain, NULL, 0, 
            &state->ghost_thread_identifier);

    WaitForSingleObject(state->ghost_thread_sync, INFINITE);
    CloseHandle(state->ghost_thread_sync);

    NX_PEDANTIC_ASSERT(state->ghost_thread_initialized == true);
    if (state->ghost_thread_initialized == false) return false;

    // We can now create our window.
    WNDCLASSEXW display_window_class    = {};
    display_window_class.cbSize         = sizeof(display_window_class);
    display_window_class.lpfnWndProc    = &wMainWindowProc;
    display_window_class.hInstance      = GetModuleHandleW(NULL);
    display_window_class.hIcon          = LoadIconA(NULL, IDI_APPLICATION);
    display_window_class.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    display_window_class.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    display_window_class.lpszClassName  = L"displayWindowClass";
    display_window_class.style          = CS_OWNDC;
    RegisterClassExW(&display_window_class);

    // Calculate our window size from the requested client area; the value
    // we get back accomodate the title bar and the bordering stuff.
    RECT client_rect    = {};
    client_rect.right   = width;
    client_rect.bottom  = height;

    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);

    // This will ensure that our client area is the size we specify.
    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    // Establish our creation context.
    window_creation_context create_params = {};
    create_params.ex_style            = 0;
    create_params.class_name          = display_window_class.lpszClassName;
    create_params.window_name         = L"Ninetails Game Engine";
    create_params.style               = WS_OVERLAPPEDWINDOW;
    create_params.x                   = CW_USEDEFAULT;
    create_params.y                   = CW_USEDEFAULT;
    create_params.width               = window_width;
    create_params.height              = window_height;
    create_params.instance            = display_window_class.hInstance;
    create_params.user_parameter      = (LPVOID)NULL;

    HWND main_window_handle = (HWND)SendMessageW(state->ghost_window_handle,
            UD_CREATE_WINDOW, (WPARAM)&create_params, 0);

    state->title                = title;
    state->main_window_handle   = main_window_handle;
    state->known_width          = width;
    state->known_height         = height;
    state->is_minimized         = false;
    state->is_maximized         = false;
    state->is_focused           = true;
    state->is_resizable         = true;
    state->is_visible           = show;
    state->quit_received        = false;

    if (show == true)
    {
        ShowWindow(main_window_handle, SW_SHOWNORMAL);
    }

    SetWindowTextA(main_window_handle, title);
    
    return true;

}

void 
window_process_events()
{

    window_state *state = get_window_state();

    // Process all events for the current thread.
    MSG message;
    while (PeekMessage(&message, (HWND)-1, 0, 0, PM_REMOVE))
    {

    }

}

b32
window_should_close()
{

    window_state *state = get_window_state();
    b32 should_close = state->quit_received;
    return should_close;

}

void
window_close()
{
    
    window_state *state = get_window_state();
    SendMessageW(state->ghost_window_handle, UD_DESTROY_WINDOW, 0, 0);
    state->main_window_handle = NULL;

}

// --- Win32 Backend API Functions ---------------------------------------------
//
// Due to the complex nature of window management on Win32, there are a set of
// backend functions and data management routines that need to be implemented to
// ensure that the window runs properly.
//

LRESULT CALLBACK
wGhostWindowProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{

    LRESULT ret_result = 0;

    switch (message)
    {

        case UD_CREATE_WINDOW:
        {

            // Cast to the creation context and then run it.
            window_creation_context *create_params = (window_creation_context*)w_param;
            HWND window_handle = CreateWindowExW(
                    create_params->ex_style, 
                    create_params->class_name,
                    create_params->window_name,
                    create_params->style,
                    create_params->x,
                    create_params->y,
                    create_params->width,
                    create_params->height,
                    create_params->parent_window,
                    create_params->menu,
                    create_params->instance,
                    create_params->user_parameter
            );

            // The result can be packed as a LRESULT, which is kinda neat.
            ret_result = (LRESULT)window_handle;
            break;

        };

        case UD_DESTROY_WINDOW:
        {

            // A request to destroy the window is made.
            window_state *state = get_window_state();
            DestroyWindow(state->main_window_handle);
            break;

        };

        default:
        {

            // Generally, any other event that the ghost window receives can
            // be defaulted. We don't really care about this window's existence.
            ret_result = DefWindowProcW(window, message, w_param, l_param);
            break;

        };
    };

    return ret_result;

}

DWORD WINAPI 
wGhostThreadMain(LPVOID user)
{

    window_state *state = get_window_state();
    NX_PEDANTIC_ASSERT(state != NULL);

    WNDCLASSEXW ghost_window_class    = {};
    ghost_window_class.cbSize         = sizeof(ghost_window_class);
    ghost_window_class.lpfnWndProc    = &wGhostWindowProc;
    ghost_window_class.hInstance      = GetModuleHandleW(NULL);
    ghost_window_class.hIcon          = LoadIconA(NULL, IDI_APPLICATION);
    ghost_window_class.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    ghost_window_class.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    ghost_window_class.lpszClassName  = L"ghostWindowClass";
    RegisterClassExW(&ghost_window_class);

    HWND ghost_window_handle = CreateWindowExW(0, ghost_window_class.lpszClassName,
            L"Ghost Window", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            0, 0, ghost_window_class.hInstance, 0);
    state->ghost_window_handle = ghost_window_handle;

    if (state->ghost_window_handle != NULL) state->ghost_thread_initialized = true;
    SetEvent(state->ghost_thread_sync);

    while(state->ghost_thread_runtime)
    {

        MSG current_message = {};
        GetMessageW(&current_message, 0, 0, 0);
        TranslateMessage(&current_message);
        DispatchMessageW(&current_message);

    }

    return 0;

}

LRESULT CALLBACK
wMainWindowProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{

    window_state *state = get_window_state();

    LRESULT ret_result = 0;   
    //ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param);

    switch (message)
    {

        case WM_CLOSE:
        {

            state->quit_received++;
            printf("-- Close event received from user, times invoked: %d.\n", state->quit_received);

            // If the x-button was ignored more than 3 times, we messed something up.
            NX_PEDANTIC_ASSERT(state->quit_received <= 4);

        } break;



/*
        case WM_CREATE:
        {

            CREATESTRUCTA *creation_structure = (CREATESTRUCTA*)l_param;
            window_context *context = (window_context*)creation_structure->lpCreateParams;
            SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)context);
            break;

        };

        case WM_ACTIVATEAPP:
        {
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            b32 is_active = (w_param == TRUE);
            context->state.focused = is_active;

            // Releases all active inputs when the window goes inactive.
            if (!is_active)
                input_release_all();

            break;
        };

        case WM_CLOSE:
        {
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            window_handle handle = context;
            window_close(&handle);
            break;
        };  

        case WM_SIZE:
        {
        
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            s32 width   = (0xFFFF & l_param);
            s32 height  = (l_param >> 16);

            if (w_param == SIZE_MINIMIZED)
            {
                context->state.minimized = true;
            }
            else if (w_param == SIZE_MAXIMIZED)
            {
                context->state.maximized = true;
            }
            else if (w_param == SIZE_RESTORED
                    && context->state.size.width == 0
                    && context->state.size.height == 0)
            {
                context->state.minimized = false;
                context->state.maximized = false;
            }

            context->state.size = { width, height };

            break;
        };

        case WM_KEYDOWN:
        {
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);

            u32 key_code = w_param;
            PostThreadMessage(context->ghost.main_thread_identifier, UD_KEYBOARD_DOWN,
                (WPARAM)context, (LPARAM)key_code);
            break;
        };

        case WM_KEYUP:
        {
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);

            u32 key_code = w_param;
            PostThreadMessage(context->ghost.main_thread_identifier, UD_KEYBOARD_UP, 
                    (WPARAM)context, (LPARAM)key_code);
            break;
        };

        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        {
            
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            if (w_param == MK_LBUTTON)
                PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_DOWN, 
                        (WPARAM)context, (LPARAM)KitMouseLeft);
            else if (w_param == MK_RBUTTON)
                PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_DOWN, 
                        (WPARAM)context, (LPARAM)KitMouseRight);
            else if (w_param == MK_RBUTTON)
                PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_DOWN, 
                        (WPARAM)context, (LPARAM)KitMouseMiddle);
            break;
        }

        case WM_MBUTTONUP:
        {

            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_UP, 
                    (WPARAM)context, (LPARAM)KitMouseMiddle);
            break;

        };

        case WM_RBUTTONUP:
        {

            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_UP, 
                    (WPARAM)context, (LPARAM)KitMouseRight);
            break;

        };

        case WM_LBUTTONUP:
        {
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);
            PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_UP, 
                    (WPARAM)context, (LPARAM)KitMouseLeft);
            break;
        };

        case WM_MOUSEWHEEL:
        {
            window_context *context = (window_context*)GetWindowLongPtrA(window, GWLP_USERDATA);

            int scroll = GET_WHEEL_DELTA_WPARAM(w_param);
            PostThreadMessage(context->ghost.main_thread_identifier, UD_MOUSE_SCROLL, 
                    (WPARAM)context, (LPARAM)scroll);
        };
*/

        // All messages we don't actually care about, default.
        default:
        {
            ret_result = DefWindowProcW(window, message, w_param, l_param);
        };

    };

    return ret_result;

}
