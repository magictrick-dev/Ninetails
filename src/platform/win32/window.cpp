#include <windows.h>
#include <windowsx.h>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/win32/inputhandler.h>

typedef struct window_state
{

    ccptr title;
    i32 known_width;
    i32 known_height;
    i32 known_x;
    i32 known_y;
    b32 is_minimized;
    b32 is_maximized;
    b32 is_focused;
    b32 is_resizable;
    b32 is_visible;
    b32 is_borderless;
    b32 quit_received;

    b32 was_resized;
    b32 was_focused;
    b32 was_moved;

    HDC main_window_device_context;
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
#define UD_KEYBOARD_DOWN    (WM_USER + 0x00F0)
#define UD_KEYBOARD_UP      (WM_USER + 0x00F1)
#define UD_MOUSE_DOWN       (WM_USER + 0x00F2)
#define UD_MOUSE_UP         (WM_USER + 0x00F3)
#define UD_MOUSE_SCROLL     (WM_USER + 0x00F4)

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

    // The input handler requires this to be initialized before the window itself
    // since the window is what sends keyboard and mouse messages.
    initialize_input_states();

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

    NX_PEDANTIC_ASSERT(state->ghost_thread_initialized);
    if (state->ghost_thread_initialized == false) return false;

    // We can now create our window.
    WNDCLASSEXW display_window_class    = {0};
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
    RECT client_rect    = {0};
    client_rect.right   = width;
    client_rect.bottom  = height;

    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);

    // This will ensure that our client area is the size we specify.
    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    // Establish our creation context.
    window_creation_context create_params = {0};
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
    state->is_borderless        = false;
    state->is_visible           = show;
    state->quit_received        = false;

    state->main_window_device_context = GetDC(main_window_handle);

    if (show)
    {
        ShowWindow(main_window_handle, SW_SHOWNORMAL);
    }

    SetWindowTextA(main_window_handle, title);
    
    return true;

}

void 
window_process_events()
{

    // Untoggle all single event flags at the start.
    window_state *state = get_window_state();
    state->was_resized = false;
    state->was_focused = false;
    state->was_moved = false;

    // Swap the input frames.
    swap_input_states();

    // Update the input states.
    input_state *previous_frame = get_previous_input_state();
    input_state *current_frame = get_current_input_state();
    for (u64 idx = 0; idx < 256; ++idx)
    {

        if (current_frame->keyboard[idx].is_released)
            current_frame->keyboard[idx].time = system_timestamp();
        if (current_frame->keyboard[idx].is_pressed)
            current_frame->keyboard[idx].time = system_timestamp();

        current_frame->keyboard[idx].is_released = false;
        current_frame->keyboard[idx].is_pressed = false;

    }

    for (u64 idx = 0; idx < 8; ++idx)
    {

        if (current_frame->mouse[idx].is_released)
            current_frame->mouse[idx].time = system_timestamp();
        if (current_frame->mouse[idx].is_pressed)
            current_frame->mouse[idx].time = system_timestamp();

        current_frame->mouse[idx].is_released = false;
        current_frame->mouse[idx].is_pressed = false;

    }

    current_frame->mouse_position.moved = false;
    current_frame->mouse_wheelie = false;
    current_frame->mouse_wheel.delta_x = 0.0;
    current_frame->mouse_wheel.delta_y = 0.0;

    // Process all events for the current thread.
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {

        WPARAM w_param = message.wParam;
        LPARAM l_param = message.lParam;

        switch (message.message)
        {

            case WM_KEYDOWN:
            {

                u32 mapping = convert_keycode((u32)message.wParam);

                if (!previous_frame->keyboard[mapping].is_down) 
                {
                    current_frame->keyboard[mapping].is_pressed = true;
                }

                current_frame->keyboard[mapping].is_down = true;

            } break;

            case WM_KEYUP:
            {

                u32 mapping = convert_keycode((u32)message.wParam);

                if (previous_frame->keyboard[mapping].is_down)
                {
                    current_frame->keyboard[mapping].is_released = true;
                }

                current_frame->keyboard[mapping].is_down = false;

                break;

            } break;

            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONDOWN:
            {
                
                if (message.wParam == MK_LBUTTON)
                {

                    u32 mapping = NxMouseLeft;

                    if (!previous_frame->mouse[mapping].is_down) 
                    {
                        current_frame->mouse[mapping].is_pressed = true;
                    }

                    current_frame->mouse[mapping].is_down = true;

                }

                else if (message.wParam == MK_RBUTTON)
                {

                    u32 mapping = NxMouseRight;

                    if (!previous_frame->mouse[mapping].is_down) 
                    {
                        current_frame->mouse[mapping].is_pressed = true;
                    }

                    current_frame->mouse[mapping].is_down = true;

                }

                else if (message.wParam == MK_MBUTTON)
                {

                    u32 mapping = NxMouseMiddle;

                    if (!previous_frame->mouse[mapping].is_down) 
                    {
                        current_frame->mouse[mapping].is_pressed = true;
                    }

                    current_frame->mouse[mapping].is_down = true;

                }

            } break;

            case WM_MBUTTONUP:
            {

                u32 mapping = NxMouseMiddle;

                if (previous_frame->mouse[mapping].is_down)
                {
                    current_frame->mouse[mapping].is_released = true;
                }

                current_frame->mouse[mapping].is_down = false;

            } break;

            case WM_RBUTTONUP:
            {

                u32 mapping = NxMouseRight;

                if (previous_frame->mouse[mapping].is_down)
                {
                    current_frame->mouse[mapping].is_released = true;
                }

                current_frame->mouse[mapping].is_down = false;

            } break;

            case WM_LBUTTONUP:
            {

                u32 mapping = NxMouseLeft;

                if (previous_frame->mouse[mapping].is_down)
                {
                    current_frame->mouse[mapping].is_released = true;
                }

                current_frame->mouse[mapping].is_down = false;

            } break;

            case WM_ACTIVATEAPP:
            {

                b32 is_active = (w_param == TRUE);
                state->is_focused = is_active;
                state->was_focused = true;

            } break;

            case WM_SIZE:
            {

                i32 width   = (0xFFFF & l_param);
                i32 height  = (l_param >> 16);

                if (w_param == SIZE_MINIMIZED)
                {
                    state->is_minimized = true;
                }
                else if (w_param == SIZE_MAXIMIZED)
                {
                    state->is_maximized = true;
                }
                else if (w_param == SIZE_RESTORED
                        && state->known_width == 0
                        && state->known_height == 0)
                {
                    state->is_minimized = false;
                    state->is_maximized = false;
                }

                state->known_width = width;
                state->known_height = height;
                state->was_resized = true;

            } break;

            case WM_MOUSEMOVE:
            {
                i32 x = GET_X_LPARAM(l_param); 
                i32 y = GET_Y_LPARAM(l_param); 
                current_frame->mouse_position.mouse_x = x;
                current_frame->mouse_position.mouse_y = y;
                current_frame->mouse_position.delta_x =
                    (x - previous_frame->mouse_position.mouse_x);
                current_frame->mouse_position.delta_y =
                    (y - previous_frame->mouse_position.mouse_y);
                current_frame->mouse_position.moved = true;
            } break;

            case WM_MOUSEWHEEL:
            {
                r32 scroll = (r32)GET_WHEEL_DELTA_WPARAM(w_param);
                current_frame->mouse_wheel.delta_y = scroll;
                current_frame->mouse_wheelie = true;
            } break;

        }

    }

}

void
window_swap_buffers()
{
    window_state *state = get_window_state();
    SwapBuffers(state->main_window_device_context);
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

vptr 
window_get_handle()
{

    window_state *state = get_window_state();
    vptr result = (vptr)state->main_window_handle;
    return result;

}

i32 
window_get_width()
{

    window_state *state = get_window_state();
    i32 result = state->known_width;
    return result;

}

i32 
window_get_height()
{

    window_state *state = get_window_state();
    i32 result = state->known_height;
    return result;

}

ccptr 
window_get_title()
{
    window_state *state = get_window_state();
    ccptr title = state->title;
    return title;
}

void 
window_get_size(i32 *width, i32 *height)
{

    NX_ENSURE_POINTER(width);
    NX_ENSURE_POINTER(height);
    window_state *state = get_window_state();

    *width = state->known_width;
    *height = state->known_height;
    return;

}

i32 
window_get_x_position()
{

    window_state *state = get_window_state();

    RECT position = {0};
    GetWindowRect(state->main_window_handle, &position);

    i32 result = position.left;
    return result;

}

i32 
window_get_y_position()
{

    window_state *state = get_window_state();

    RECT position = {0};
    GetWindowRect(state->main_window_handle, &position);

    i32 result = position.top;
    return result;

}

void 
window_get_position(i32 *x, i32 *y)
{

    NX_ENSURE_POINTER(x);
    NX_ENSURE_POINTER(y);
    window_state *state = get_window_state();

    RECT position = {0};
    GetWindowRect(state->main_window_handle, &position);

    *x = position.left;
    *y = position.top;
    return;

}

void 
window_set_title(ccptr title)
{

    window_state *state = get_window_state();
    SetWindowTextA(state->main_window_handle, title);

}

void
window_set_size(i32 width, i32 height)
{

    window_state *state = get_window_state();
    RECT client_rect    = {0};
    client_rect.right   = width;
    client_rect.bottom  = height;

    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);

    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    SetWindowPos(state->main_window_handle, NULL, 0, 0,
        window_width, window_height, SWP_NOMOVE);

}

void 
window_set_borderless_fullscreen(b32 enable, i32 exit_width, i32 exit_height)
{

    window_state *state = get_window_state();

    if (enable)
    {

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        SetWindowLongPtr(state->main_window_handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        SetWindowPos(state->main_window_handle, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);

    }
    
    else
    {

        SetWindowLongPtr(state->main_window_handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
        SetWindowPos(state->main_window_handle, NULL, 0, 0, 600, 400, SWP_FRAMECHANGED);   
        window_set_size(exit_width, exit_height);

    }

    state->is_borderless = enable;

}


void
window_set_maximization(b32 enable)
{

    window_state *state = get_window_state();
    if (enable) ShowWindow(state->main_window_handle, SW_MAXIMIZE);
    else ShowWindow(state->main_window_handle, SW_RESTORE);


}

void
window_set_resizable(b32 enable)
{

    window_state *state = get_window_state();

    if (!enable)
    {

        DWORD style_change = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
        SetWindowLongPtrA(state->main_window_handle, GWL_STYLE, style_change);

    }
    else
    {

        DWORD style_change = WS_OVERLAPPEDWINDOW;
        SetWindowLongPtrA(state->main_window_handle, GWL_STYLE, style_change);

    }

    state->is_resizable = enable;

}

void 
window_set_visibility(b32 hide)
{

    window_state *state = get_window_state();
    if (hide)
        ShowWindow(state->main_window_handle, SW_SHOWNORMAL);
    else
        ShowWindow(state->main_window_handle, SW_HIDE);

    state->is_visible = hide;

}

b32 
window_is_open()
{

    window_state *state = get_window_state();
    b32 result = (state->main_window_handle != NULL);
    return result;


}

b32 
window_is_borderless_fullscreen()
{

    window_state *state = get_window_state();
    return state->is_borderless;

}

b32 
window_is_maximized()
{

    window_state *state = get_window_state();
    return state->is_maximized;

}

b32 
window_is_minimized()
{

    window_state *state = get_window_state();
    return state->is_minimized;

}

b32 
window_is_focused()
{

    window_state *state = get_window_state();
    return state->is_focused;

}

b32 
window_is_resizable()
{

    window_state *state = get_window_state();
    return state->is_resizable;

}

b32 
window_is_visible()
{

    window_state *state = get_window_state();
    return state->is_visible;

}

b32     
window_did_size_change()
{

    window_state *state = get_window_state();
    return state->was_resized;

}

b32     
window_did_focus_change()
{

    window_state *state = get_window_state();
    return state->was_focused;

}

b32     
window_did_position_change()
{

    window_state *state = get_window_state();
    return state->was_moved;

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

    WNDCLASSEXW ghost_window_class    = {0};
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

        MSG current_message = {0};
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

        case WM_MOVE:
        {

            i32 x = LOWORD(l_param);
            i32 y = HIWORD(l_param);
            state->known_x = x;          
            state->known_y = y;          
            state->was_moved = true;

        } break;

        case WM_MOUSEWHEEL:
        case WM_MOUSEMOVE:
        case WM_SIZE:
        case WM_ACTIVATEAPP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        {
            PostThreadMessageW(state->main_thread_identifier, message, w_param, l_param);
            break;
        };


/*

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

        default:
        {
            ret_result = DefWindowProcW(window, message, w_param, l_param);
        };

    };

    return ret_result;

}
