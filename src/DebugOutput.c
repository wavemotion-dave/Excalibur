// ========================================================================================
// Debug Output Window for Excalibur RPN Calculator
// 
// This module provides a dprintf() function that outputs debug strings to a dedicated
// window with a scrollbar. The debug window is only created if ENABLE_DEBUG_WINDOW is
// defined at compile-time.
//
// Usage:
//   dprintf("Format string %d, %s\n", value, string);
// 
// To enable: Add -DENABLE_DEBUG_WINDOW to compiler flags or define in this file
// ========================================================================================

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "Excal.h"

// ========================================================================================
// COMPILE-TIME CONFIGURATION
// ========================================================================================
// Define this to enable the debug output window. Comment it out to disable.
// This can also be passed via compiler flags: -DENABLE_DEBUG_WINDOW
//#define ENABLE_DEBUG_WINDOW

// Maximum size of debug output buffer (in characters)
#define DEBUG_OUTPUT_BUFFER_SIZE    (512 * 1024)    // 512KB

#ifdef ENABLE_DEBUG_WINDOW

// ========================================================================================
// GLOBALS
// ========================================================================================
static HWND g_hDebugWindow = NULL;
static HWND g_hDebugEditBox = NULL;
static char g_DebugBuffer[DEBUG_OUTPUT_BUFFER_SIZE] = {0};
static uint32_t g_DebugBufferPos = 0;
static CRITICAL_SECTION g_DebugCS;

// Forward declarations
static LRESULT CALLBACK DebugOutputWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void AppendToDebugOutput(const char *str, uint32_t len);

// ========================================================================================
// DebugOutputWndProc - Window procedure for debug output window
// ========================================================================================
static LRESULT CALLBACK DebugOutputWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
        {
            // Create the edit control that will hold the debug output
            g_hDebugEditBox = CreateWindow(
                "EDIT",
                "",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | 
                ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                0, 0, 0, 0,
                hwnd,
                (HMENU)IDC_DEBUG_EDIT,
                GetModuleHandle(NULL),
                NULL
            );

            if (g_hDebugEditBox)
            {
                // Set font to fixed-width for better readability
                HFONT hFont = CreateFont(
                    12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, "System"
                );
                if (hFont)
                {
                    SendMessage(g_hDebugEditBox, WM_SETFONT, (WPARAM)hFont, FALSE);
                }
            }
            return 0;
        }

        case WM_SIZE:
        {
            // Resize the edit control to fill the window
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            
            if (g_hDebugEditBox)
            {
                MoveWindow(g_hDebugEditBox, 0, 0, width, height, TRUE);
            }
            return 0;
        }

        case WM_CLOSE:
        {
            // Hide the window instead of destroying it
            ShowWindow(hwnd, SW_HIDE);
            return 0;
        }

        case WM_DESTROY:
        {
            g_hDebugWindow = NULL;
            if (g_hDebugEditBox)
            {
                DestroyWindow(g_hDebugEditBox);
                g_hDebugEditBox = NULL;
            }
            return 0;
        }

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

// ========================================================================================
// CreateDebugOutputWindow - Creates or shows the debug output window
// ========================================================================================
void CreateDebugOutputWindow(HWND hParent, HINSTANCE hInstance)
{
    if (!g_hDebugWindow)
    {
        // Register the window class
        static BOOL classRegistered = FALSE;
        if (!classRegistered)
        {
            WNDCLASS wndclass = {0};
            wndclass.lpfnWndProc = DebugOutputWndProc;
            wndclass.hInstance = hInstance;
            wndclass.lpszClassName = "ExcalDebugOutput";
            wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
            wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wndclass.style = CS_HREDRAW | CS_VREDRAW;

            if (!RegisterClass(&wndclass))
            {
                return;  // Failed to register class
            }
            classRegistered = TRUE;
        }

        // Create the window
        g_hDebugWindow = CreateWindowEx(
            0,
            "ExcalDebugOutput",
            "Excalibur Debug Output",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            600, 400,
            hParent,
            NULL,
            hInstance,
            NULL
        );

        if (g_hDebugWindow)
        {
            ShowWindow(g_hDebugWindow, SW_SHOW);
            UpdateWindow(g_hDebugWindow);
        }
    }
    else
    {
        // Window already exists, just show it
        ShowWindow(g_hDebugWindow, SW_SHOW);
        SetForegroundWindow(g_hDebugWindow);
    }
}

// ========================================================================================
// AppendToDebugOutput - Appends text to the debug output edit box
// ========================================================================================
static void AppendToDebugOutput(const char *str, uint32_t len)
{
	int textLen;
    if (!g_hDebugEditBox)
        return;

    // Append to the edit control
    textLen = GetWindowTextLength(g_hDebugEditBox);
    SendMessage(g_hDebugEditBox, EM_SETSEL, textLen, textLen);
    SendMessage(g_hDebugEditBox, EM_REPLACESEL, FALSE, (LPARAM)str);

    // Scroll to the end
    SendMessage(g_hDebugEditBox, EM_SCROLL, SB_BOTTOM, 0);
}

// ========================================================================================
// dprintf - Printf-like debug output function
//
// Parameters:
//   format - printf-style format string
//   ...    - variadic arguments
//
// Returns: Number of characters printed
// ========================================================================================
int dprintf(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    int result;

    if (!format)
        return 0;

    // Ensure debug window exists
    if (!g_hDebugWindow)
    {
        return 0;  // Debug window not initialized
    }

    // Format the string
    va_start(args, format);
    result = vsprintf(buffer, format, args);
    va_end(args);

    if (result > 0)
    {
        // Thread-safe append to output
        EnterCriticalSection(&g_DebugCS);
        AppendToDebugOutput(buffer, result);
        LeaveCriticalSection(&g_DebugCS);
    }

    return result;
}

// ========================================================================================
// InitDebugOutput - Initialize the debug output system
// ========================================================================================
void InitDebugOutput(void)
{
    InitializeCriticalSection(&g_DebugCS);
}

// ========================================================================================
// CleanupDebugOutput - Cleanup the debug output system
// ========================================================================================
void CleanupDebugOutput(void)
{
    if (g_hDebugWindow)
    {
        DestroyWindow(g_hDebugWindow);
        g_hDebugWindow = NULL;
    }
    DeleteCriticalSection(&g_DebugCS);
}

// ========================================================================================
// ClearDebugOutput - Clear all text from the debug output window
// ========================================================================================
void ClearDebugOutput(void)
{
    if (g_hDebugEditBox)
    {
        SetWindowText(g_hDebugEditBox, "");
    }
}

#else

// Stub versions when debug window is disabled
void CreateDebugOutputWindow(HWND hParent, HINSTANCE hInstance) { }
void InitDebugOutput(void) { }
void CleanupDebugOutput(void) { }
void ClearDebugOutput(void) { }
int dprintf(const char *format, ...) { return 0; }

#endif  // ENABLE_DEBUG_WINDOW
