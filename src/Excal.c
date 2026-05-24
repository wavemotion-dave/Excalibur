// ========================================================================================
// Excalibur RPN Calculator is Copyright(c) 1994-2026 by Dave Bernazzani(wavemotion-dave)
//
// This is legacy code that was created to run under Visual C++ 4.5 and 5.0 circa 1995 and
// was largely developed with Windows 95 through Windows 98SE(some very early portions
// of code were started during the Windows 3.1 era but were quickly ported for 32-bit).
//
// I don't think there is any proprietary code here... and as such I release all of this
// source code into the wild using the permissive MIT license as follows:
//
// Copyright(c) 1994-2026 Dave Bernazzani
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
// ========================================================================================
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <mmsystem.h>
#include <float.h>
#include <signal.h>
#include <shlobj.h>
#include "Excal.h"

#define WINDOW_TITLE "Excalibur 32-bit"

#define VERSION_STR "v3.XX-04"

#define ABOUT_MSG "Excalibur for Windows 32-bit\n"                    \
                  "Version 3.XX-04  -  May 25, 2026\n\n"              \
                  "Copyright 1994-2026 David Bernazzani\n\n"          \
                  "Please read the disclaimer and understand the\n"   \
                  "accuracy and precision issues before using.\n\n"   \
                  "Excalibur is freeware - but see the github page\n" \
                  "if you want to donate to support the effort.\n\n"  \
                  "https://github.com/wavemotion-dave/Excalibur"      \
                  "\n\nThis version is BETA - Expect and report Bugs!"

#define CONFIG_VERSION_MAIN 0xF00C  // If this changes, we wipe EVERYTHING
#define CONFIG_VERSION_SUB  0xF004  // If this changes, we reset x,y window position and reset constant tables (currency, physics constants, etc)

#define END_OF_PROGRAM_STR "<End Of Program>"
#define PROGRAM_ASSIGNED_KEY_STR "Program Assigned Key"

int16_t playBackSave[MAX_MACROS][MAX_REC_PLAYBACK + 1];
int16_t playBackIdxSave[MAX_MACROS];
uint8_t recModeON = 0;
uint8_t macroPlayback = FALSE;

uint8_t modifiers = 0x00;

int16_t playBack[MAX_REC_PLAYBACK + 1];
int16_t playBackIdx = 0;
int16_t currentMacroPlaybackIdx = 0;
uint8_t showTrace = FALSE;
uint8_t toolTipCounter = 0;
uint8_t bExactFont = TRUE;
int16_t MacroStack[MAX_MACRO_STACK];
int16_t MacroStackIdx = 0;
uint32_t macroFlags = 0x00000000;
uint8_t rpnStoreRecall = 0x00;

uint32_t wordSize = 32;
uint64_t wordSizeMask = (uint64_t) 0xFFFFFFFFL;
uint8_t padZeros = COMPSCI_NOPADZEROS;
uint8_t wordMode = COMPSCI_SIGNED;
uint8_t hexSpacing = HEX_SPACE_NONE;
uint8_t numberDisplayMode = INTERNATIONAL;
uint8_t lastProgMode = PROG_FLOAT;
uint16_t traceDelayValueMs = 1000;

int16_t totalMappedButtonFuncs = 0;
uint32_t lastTickCount = 0;
uint32_t ticksUsed = 0;

uint8_t ClearStackOnExit = 0;
uint8_t eRPN = 0;
uint8_t progModecarry = 0;
uint8_t rightAlignStack = 0;
uint8_t showXMinimized = 0;
uint8_t traceMacroPlayback = FALSE;
int32_t lastChosenMacro = 0;
uint8_t showTime24HourFormat = FALSE;
uint8_t alwaysOnTop = 0;
uint8_t reservedOpt1 = 0;
uint8_t reservedOpt2 = 0;
uint8_t reservedOpt3 = 0;

char macroName[MAX_MACROS][MAX_MACRO_FUNC_TEXT];
char macro_short_names[MAX_MACROS][7];
char clipboardBuffer[MAX_IMPORT_CLIPBOARD_SIZE + 1];
char statusBar[32];
char helpTitle[64];
char functionBar[64];
char helpMsg[256];
char tmpStr[256];
BYTE keyState[256];

// ----------------------------------------------
// Buffers and status for editing the X register
// ----------------------------------------------
uint8_t Xedit = X_NEW; // One of X_NEW, X_EDIT, X_NULL, etc.
char Xstr[64];         // Global buffer for X editing

// ----------------
// Global registers
// ----------------
double STACK[MAX_STACK];     // The main RPN stack (X, Y, Z, T, A, B, C D)
PROG_LONG STACKL[MAX_STACK]; // The main RPN stack in long integer form for Comp-Sci mode (X, Y, Z, T, A, B, C, D)

double LASTX; // LAST X register
double LASTY; // LAST Y register

double lastFloat = 0.0;

PROG_LONG LASTXL; // LAST X when in Comp-Sci mode
PROG_LONG LASTYL; // LAST Y when in Comp-Sci mode

// Some statistics registers for how Excalibur is being utilized
uint64_t stackPushes = 0; // Total number of Stack Pushes
uint64_t stackPops = 0;   // Total number of Stack Pops
uint32_t inFocusTime = 0; // Number of minutes Excalibur window in 'focus'

// A number of status registers for various modes and functions
uint8_t  AngleMode = 0;         // 0=Degrees, 1=Radians, 2=Gradients
uint8_t  commaMode = 1;         // 0=International, 1=US (commas vs periods for decimal and thousand separators)
uint8_t  eexMode = 1;           // 0=EEX, 1=E, CHS vs +/-
uint8_t  numLockMode = 1;       // Turn on NumLock when program starts?
uint8_t  toolTipsOn = 1;        // Enable tooltips?
uint8_t  extendedStack = 0;     // Standard Stack is 4 deep. Extended is 8 deep.
uint8_t  footPrint = 0;         // Classic layout by default
uint8_t  popFillZero = 0;       // T register fills with zero?
int32_t  lastChosenConst = 0;   // Last chosen constant
int32_t  lastConstBank = 0;     // Last chosen constant bank
int32_t  decimal_places = 13;   // Default decimal places
uint8_t  sci_format = 'g';      // Default scientific display format
uint32_t indirectRegister = 0;  // For programming - (i) register
uint8_t  progMode = PROG_FLOAT; // Normal floating-point mode (vs HEX, DEC, OCT, BIN)
uint16_t lastUniqueIndex = 0;   // Index of the last function that was called (useful in Financial Register handling)

// Various storage arrays for RPN use
double STO[MAX_STO];             // Storage registers R0-R99
double SUM[SUM_MAX];             // Statistics registers for the Statistics bank
double cashFlow[MAX_CF];         // Cash flow registers for the Financial bank
uint8_t CFn;                     // Number of cash flows currently entered (for financial functions that use cash flow registers)
char excaliburNotes[NOTES_SIZE]; // A small scratchpad for the user to jot down some info (preserved on program exit)
PROG_LONG STOL[MAX_STO];         // Storage registers R0-R99 when in Comp-Sci mode

// ---------------------------------------------------------------------------------------------
// This is the mapping from unique index to function for playback and macro recording purposes.
// This is used to determine which function to call when playing back a macro or executing a
// program line that was recorded with a unique index. It also contains some info about whether
// the function uses floats or longs, whether it should be allowed to be recorded in macros, etc.
// ---------------------------------------------------------------------------------------------
struct playbackStruct playBackMap[MAX_FUNCTIONS + 1];

extern void RPN_digit0(void);
extern void RPN_digit1(void);
extern void RPN_digit2(void);
extern void RPN_digit3(void);
extern void RPN_digit4(void);
extern void RPN_digit5(void);
extern void RPN_digit6(void);
extern void RPN_digit7(void);
extern void RPN_digit8(void);
extern void RPN_digit9(void);

extern void callButtonFunc(void (*routine)(void), char useFloatsLongs,
                           char allowRecord, uint16_t uniqueIndex, char saveLastX, char newXedit, int updateSpareBar);
extern void mapButtonFuncs(void);
void DoMacroSaveRecall(void);

struct funcStruct *currentFuncs = (struct funcStruct *)&Scientific_funcs;
struct funcStruct *lastFuncs = (struct funcStruct *)&Scientific_funcs;

uint32_t slowTimer = 0; // Ticks at roughly 300ms intervals

#define MIN_WINDOW_WIDTH    5
#define MIN_WINDOW_HEIGHT   5
#define MAX_WINDOW_X        2000
#define MAX_WINDOW_Y        2000
#define MIN_WINDOW_X        0
#define MIN_WINDOW_Y        0

// This will force a "default" size...
int32_t main_x  = -1;
int32_t main_y  = 50;
int32_t main_cx = 100;
int32_t main_cy = 100;

HWND toolTipWnd;              // window handle from CreateWindow
HFONT holdsfont;              // handle of original font
HFONT hMainFont;              // handle of new font for most of the UI
HFONT hNumberFont;            // handle of new font for the Stack Display (bigger, bolder)
HFONT hFixedFont;             // handle of the new font for dialogs that need fixed pitch
DLGPROC lpfnMainWndProc;      // Main window procedure/handler
HINSTANCE hExcaliburInstance; // The global instance of Excalibur (assigned by the OS)
HWND calcMainWindow;          // A handle to the Main Window
HMENU hMainMenu;              // A handle to the Main Menu

// Brushes that we need for this application to paint backgrounds
static HBRUSH backgroundBrush;
static HBRUSH helpWindowBackgroundBrush;

void HelpAbout(void);
void ShowUsageStats(void);
void ProcessCustomSave(void);
void SetLastMenuType(int type);
WORD GetMouseHelp(WORD xPos, WORD yPos);
void init_key_pos(void); // for tool tips

// ---------------------------------------------------------------------------------------------------------
// This is the classic Win32 entry point (think of this like main() for traditional C console programs).
// This routine will setup the main Excalibur dialog window - display it and then sit in a loop processing
// various windows related events and dispatching them to our dialog box, button handlers, etc.
// ---------------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "EXCALIBUR";
    MSG msg;
    WNDCLASSEX wndclass;
    int id;
    HMENU tmpMenuHandle;

    hExcaliburInstance = hInstance; // Set the global handle - we use this in a number of places

    wndclass.cbSize = sizeof(wndclass);
    wndclass.hIconSm = LoadIcon(hInstance, szAppName);

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = DLGWINDOWEXTRA;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, szAppName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClassEx(&wndclass);

    MemoryInit();
    ReadFromDisk(); // We need to know the footprint... so we need to read the config before we create the window.

    // This is the main Excalibur dialog window!
    if (footPrint == 1)
    {
        calcMainWindow = CreateDialog(hInstance, "DIALOG_4BANGER", 0, NULL);
        progMode = PROG_FLOAT;
        lastConstBank = 0;
        currentFuncs = (struct funcStruct *)&Scientific_funcs;
        lastFuncs = (struct funcStruct *)&Scientific_funcs;
    }
    else
    {
        calcMainWindow = CreateDialog(hInstance, "DIALOG_EXCALIBUR", 0, NULL);
    }

    hMainMenu = GetMenu(calcMainWindow);

    ShowWindow(calcMainWindow, SW_HIDE);

    SetTimer(calcMainWindow, TIMER_ONE_MINUTE, 60000, NULL); // 1 Minute Timer
    SetTimer(calcMainWindow, TIMER_SLOW, 300, NULL);         // 300ms Timer
    SetTimer(calcMainWindow, TIMER_FAST, 100, NULL);         // 100ms Timer

    srand((unsigned)time(NULL)); // Ensure random numbers are somewhat random!

    // Add the Excalibur Settings into the main menu
    tmpMenuHandle = GetSystemMenu(calcMainWindow, FALSE);
    AppendMenu(tmpMenuHandle, MF_SEPARATOR, 0, NULL);
    AppendMenu(tmpMenuHandle, MF_STRING, IDM_SETTINGS, "Excalibur Settings...");

    // The X Y Z T stack values get a slightly larger/bolder font...
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_X), WM_SETFONT, (WPARAM)hNumberFont, FALSE);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_Y), WM_SETFONT, (WPARAM)hNumberFont, FALSE);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_Z), WM_SETFONT, (WPARAM)hNumberFont, FALSE);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_T), WM_SETFONT, (WPARAM)hNumberFont, FALSE);

    // -------------------------------------------------------------------------------------------
    // Every other control gets the standard hMainFont by default... Note, many of these control
    // IDs don't actually exist... but there is no harm in running through the list this way.
    // -------------------------------------------------------------------------------------------
    for (id = RPN_START_OF_LIST; id < RPN_END_OF_LIST; id++)
    {
        SendMessage(GetDlgItem(calcMainWindow, id), WM_SETFONT, (WPARAM)hMainFont, FALSE);
    }

    // -----------------------------------------------------
    // All of the 40 function buttons get the same font...
    // -----------------------------------------------------
    for (id = FN1; id <= FN40; id++)
    {
        SendMessage(GetDlgItem(calcMainWindow, id), WM_SETFONT, (WPARAM)hMainFont, FALSE);
    }

    Init();
    ShowWindow(calcMainWindow, iCmdShow);           // Now show the window the way it was asked to be displayed...
    CreateToolTipWindow(calcMainWindow, hInstance); // Create the tool-tip window that goes with buttons...
    CreateDebugWindow(calcMainWindow, hInstance);   // Create the debug window for program traceback, register view, etc.
    SelectNewBank(currentFuncs);                    // Make sure the right bank is selected

    // -------------------------------------------------------------------------------
    // This is our main loop that runs forever... processing and dispatching messages
    // -------------------------------------------------------------------------------
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

int CreateToolTipWindow(HWND hwnd, HINSTANCE hInstance)
{
    static TCHAR toolTipWndName[] = TEXT("ToolTipWindow");
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = tooltipWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)helpWindowBackgroundBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = toolTipWndName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Unable to register the Excalibur Class with Windows.\nThis is a fatal error and the program will now exit."), toolTipWndName, MB_ICONERROR);
        return 0;
    }

    toolTipWnd = CreateWindow(toolTipWndName,       // window class name
                              TEXT("ExcalHelp"),    // window caption
                              WS_POPUP | WS_BORDER, // window style
                              CW_USEDEFAULT,        // initial x position
                              CW_USEDEFAULT,        // initial y position
                              100,                  // initial x size (will get auto-resized)
                              50,                   // initial y size (will get auto-resized)
                              hwnd,                 // parent window handle
                              NULL,                 // window menu handle
                              hInstance,            // program instance handle
                              NULL);                // creation parameters

    ShowWindow(toolTipWnd, SW_HIDE);
    toolTipCounter = 0;
    UpdateWindow(toolTipWnd);

    return 0;
}

void sleep_and_peek(int timeMs)
{
    int i;
    MSG msg;

    for (i = 0; i < (timeMs / 50); i++)
    {
        while (PeekMessage(&msg, calcMainWindow, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) // Always check for quit!
            {
                endRunningMacro();
                PostQuitMessage(0);
                return;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(50);
    }
}

// ---------------------------------------------------------------------------------------
// This is our top-level Excalibur window handler... Essentially this handles any
// top-level functionality such as keyboard presses, global timers, window movement, etc.
// ---------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static WORD lastXpos, lastYpos;
    RECT rcWindow;
    DWORD dw;
    WORD xPos, yPos;
    POINT pCursor;
    HDC hdc;
    RECT rc;            // window's screen coordinates
    POINT ptUpperLeft;  // client coordinate of upper left
    POINT ptLowerRight; // client coordinate of lower right
    SIZE lpSize;
    PAINTSTRUCT ps;
    int i;
    UINT flags;

    switch (iMsg)
    {
    case WM_CREATE:
        backgroundBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        helpWindowBackgroundBrush = CreateSolidBrush(GetSysColor(COLOR_INFOBK));
        SetUpFonts(hwnd);
        return 0;

    // We place Settings on the system command box!
    case WM_SYSCOMMAND:
        switch (LOWORD(wParam))
        {
        case (IDM_SETTINGS):
            GetUserSettings();
            break;
        }
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_F10) // F10 is unique... system wants to handle this as a Menu key
        {
            // F10 logic has to be handled here due to system wanting to use it instead...
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN40);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN39);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN38);
            else
                ProcessKeyHit(FN37);

            return 0; // Return 0 to prevent the menu from activating
        }

    case WM_COMMAND:
        if (macroPlayback == TRUE) // If we get a playback over a playback we stop the playback!
        {
            endRunningMacro();
            ShowStatus();
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

        if (!LOWORD(lParam))
        {                   // Process Menu Commands
            switch (wParam) // Determine which Menu ID
            {
            case IDM_SETTINGS:
            {
                GetUserSettings();
            }
            break;
            case IDM_MACRO:
            {
                DoMacroSaveRecall();
            }
            break;
            case IDM_USAGESTATS:
            {
                ShowUsageStats();
            }
            break;
            case IDM_EXIT:
            {
                PostMessage(hwnd, WM_DESTROY, wParam, lParam);
            }
            break;
            case IDM_COPYXTOCLIPBOARD:
            {
                ClipboardCopySelection(hwnd, COPY_X_TO_CLIPBOARD);
            }
            break;
            case IDM_COPYALLTOCLIPBOARD:
            {
                ClipboardCopySelection(hwnd, COPY_ALL_TO_CLIPBOARD);
            }
            break;
            case IDM_COPYCLIPBOARDTOX:
            {
                ClipboardCopySelection(hwnd, COPY_X_FROM_CLIPBOARD);
            }
            break;
            case IDM_COPYMACROTOCLIPBOARD:
            {
                ClipboardCopySelection(hwnd, COPY_MACRO_TO_CLIPBOARD);
            }
            break;
            case IDM_SCIENTIFIC:
                SelectNewBank((struct funcStruct *)&Scientific_funcs);
                break;
            case IDM_FINANCIAL:
                SelectNewBank((struct funcStruct *)&Financial_funcs);
                break;
            case IDM_CONVERSION:
                SelectNewBank((struct funcStruct *)&Conversion_funcs);
                break;
            case IDM_COMPSCI:
                SelectNewBank((struct funcStruct *)&CompSci_funcs);
                break;
            case IDM_CUSTOM:
                SelectNewBank((struct funcStruct *)&Custom_funcs);
                break;
            case IDM_STATS:
                SelectNewBank((struct funcStruct *)&Statistics_funcs);
                break;
            case IDM_SCIENTIFIC2:
                SelectNewBank((struct funcStruct *)&Scientific2_funcs);
                break;
            case IDM_PROGI:
                SelectNewBank((struct funcStruct *)&Program1_funcs);
                break;
            case IDM_PROGII:
                SelectNewBank((struct funcStruct *)&Program2_funcs);
                break;
            case IDM_DEFINECUSTOM:
                cust_define();
                break;
            case IDM_ALWAYSONTOP:
            {
                alwaysOnTop ^= 1;
                if (main_x == -1)
                    flags = SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE;
                else
                    flags = SWP_SHOWWINDOW | SWP_NOSIZE;
                if (alwaysOnTop)
                {
                    CheckMenuItem(hMainMenu, IDM_ALWAYSONTOP, MF_CHECKED);
                    SetWindowPos(calcMainWindow, HWND_TOPMOST, main_x, main_y, main_cx, main_cy, flags);
                }
                else
                {
                    CheckMenuItem(hMainMenu, IDM_ALWAYSONTOP, MF_UNCHECKED);
                    SetWindowPos(calcMainWindow, HWND_NOTOPMOST, main_x, main_y, main_cx, main_cy, flags);
                }
            }
            break;

            case IDM_EXCALIBURHELP:
            {
                ExcaliburHelp();
            }
            break;

            case IDM_ABOUTPROGRAM:
                HelpAbout();
                break;
            }
        }
        else
        {
            ProcessKeyHit(wParam);
            SetFocus(calcMainWindow);
        }
        return 0;
        break;

    case WM_CONTEXTMENU:           // Right mouse button - context sensitive help!
        if (macroPlayback == TRUE) // If we get a playback over a playback we stop the playback!
        {
            endRunningMacro();
            ShowStatus();
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

        xPos = LOWORD(lParam); // horizontal position of cursor
        yPos = HIWORD(lParam); // vertical position of cursor
        GetCursorPos(&pCursor);
        xPos = (WORD)pCursor.x;
        yPos = (WORD)pCursor.y;
        ScreenToClient(hwnd, &pCursor);

        GetWindowRect(hwnd, &rc);
        ptUpperLeft.x = rc.left;
        ptUpperLeft.y = rc.top;
        ptLowerRight.x = rc.right;
        ptLowerRight.y = rc.bottom;
        ScreenToClient(hwnd, &ptUpperLeft);
        ScreenToClient(hwnd, &ptLowerRight);

        if ((pCursor.x >= 0 && pCursor.x <= ptLowerRight.x) && (pCursor.y >= 0 && pCursor.y <= ptLowerRight.y))
        {
            if (GetFocus() == calcMainWindow)
            {
                if (GetMouseHelp((WORD)pCursor.x, (WORD)pCursor.y) == 1)
                {
                    ShowWindow(toolTipWnd, SW_HIDE);
                    toolTipCounter = 0;
                    MessageBox(calcMainWindow, helpMsg, helpTitle, MB_OK | MB_ICONQUESTION);
                }
            }
        }
        return FALSE;
        break;

    case WM_TIMER:
        if (wParam == TIMER_ONE_MINUTE) // 60 second timer
        {
            if (GetFocus() == hwnd)
                inFocusTime++;
        }

        if (wParam == TIMER_SLOW) // 300 ms timer
        {
            slowTimer++;

            if (macroPlayback == FALSE) // Don't bother if we are in the middle of macro playback
            {
                if ((toolTipsOn == 1) && (traceMacroPlayback == FALSE))
                {
                    GetCursorPos(&pCursor);
                    xPos = (WORD)pCursor.x;
                    yPos = (WORD)pCursor.y;
                    ScreenToClient(hwnd, &pCursor);

                    GetWindowRect(hwnd, &rc);
                    ptUpperLeft.x = rc.left;
                    ptUpperLeft.y = rc.top;
                    ptLowerRight.x = rc.right;
                    ptLowerRight.y = rc.bottom;
                    ScreenToClient(hwnd, &ptUpperLeft);
                    ScreenToClient(hwnd, &ptLowerRight);

                    if ((pCursor.x >= 0 && pCursor.x <= ptLowerRight.x) && (pCursor.y >= 0 && pCursor.y <= ptLowerRight.y))
                    {
                        if (!IsWindowVisible(toolTipWnd))
                        {
                            if (xPos == lastXpos && yPos == lastYpos)
                            {
                                if ((toolTipCounter++) >= 2)
                                {
                                    if (GetFocus() == calcMainWindow)
                                    {
                                        if (GetMouseHelp((WORD)pCursor.x, (WORD)pCursor.y) == 1)
                                        {
                                            hdc = GetDC(toolTipWnd);
                                            SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
                                            dw = GetTextExtentPoint32(hdc, helpTitle, strlen(helpTitle), &lpSize);
                                            ReleaseDC(toolTipWnd, hdc);
                                            MoveWindow(toolTipWnd, xPos - 1, yPos + 19, lpSize.cx + 10, lpSize.cy + 4, TRUE);
                                            ShowWindow(toolTipWnd, SW_SHOWNOACTIVATE);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                toolTipCounter = 0;
                            }
                        }
                    }
                    lastXpos = xPos;
                    lastYpos = yPos;
                }
            }
        }

        if (wParam == TIMER_FAST) // 100 ms timer
        {
            ticksUsed = GetTickCount() - lastTickCount;
            lastTickCount += ticksUsed;
            if (userTimer == 1)
            {
                char tmpBuff[16];
                userTicks += ticksUsed;
                sprintf(tmpBuff, "%03d:%02d.%1d", (userTicks / 60000), (userTicks / 1000) % 60, (userTicks / 100) % 10);
                SetDlgItemText(calcMainWindow, TIME_BAR, tmpBuff);
            }

            if (GetFocus() == calcMainWindow)
            {
                if (IsWindowVisible(toolTipWnd))
                {
                    GetCursorPos(&pCursor);
                    xPos = (WORD)pCursor.x;
                    yPos = (WORD)pCursor.y;
                    if (xPos != lastXpos || yPos != lastYpos)
                    {
                        ShowWindow(toolTipWnd, SW_HIDE);
                        toolTipCounter = 0;
                    }
                }
            }
        }

        return 0;
        break;

    case WM_KEYDOWN: // TBD - how to stop macro if playback and what do do about WM char message that will follow!
        if (macroPlayback == TRUE)
            break;
        switch ((int)wParam)
        {
        case (VK_NUMLOCK):
            ShowStatus();
            break;

        case ('0'): // This handles CTRL-0 thru CTRL-9 for auto decimal place setting!
        case ('1'):
        case ('2'):
        case ('3'):
        case ('4'):
        case ('5'):
        case ('6'):
        case ('7'):
        case ('8'):
        case ('9'):
            if (GetKeyState(VK_CONTROL) < 0)
            {
                if (sci_format == 'g')
                    sci_format = 'f'; // Place calculator in FIXed mode if in STD mode
                decimal_places = wParam - '0';
                ShowStack();
                ShowStatus();
            }
            break;

        case ('V'): // Paste
            if (GetKeyState(VK_CONTROL) < 0)
            {
                ClipboardCopySelection(calcMainWindow, COPY_X_FROM_CLIPBOARD);
                blinkXDisplay(TRUE);
            }
            break;

        case ('C'): // Copy
            if (GetKeyState(VK_CONTROL) < 0)
            {
                ClipboardCopySelection(calcMainWindow, COPY_X_TO_CLIPBOARD);
                blinkXDisplay(TRUE);
            }
            break;

        case ('A'): // Copy All
            if (GetKeyState(VK_CONTROL) < 0)
            {
                ClipboardCopySelection(hwnd, COPY_ALL_TO_CLIPBOARD);
                blinkStack(1);
            }
            break;

        case ('S'): // Settings
            if (GetKeyState(VK_CONTROL) < 0)
                GetUserSettings();
            break;

        case ('P'): // Program Manager
            if (GetKeyState(VK_CONTROL) < 0)
                DoMacroSaveRecall();
            break;

        case ('D'): // Define Custom Set
            if (GetKeyState(VK_CONTROL) < 0)
                cust_define();
            break;

        case ('U'): // Program Useage Stats
            if (GetKeyState(VK_CONTROL) < 0)
            {
                // ShowUsageStats();
            }
            break;

        case ('X'): // Exit
            if (GetKeyState(VK_CONTROL) < 0)
                PostMessage(hwnd, WM_DESTROY, wParam, lParam);
            break;

        case (VK_F12): // Recall Window - Center it
        case (VK_HOME):
            main_x = 25;
            main_y = 25;
            flags = SWP_SHOWWINDOW | SWP_NOSIZE;
            if (alwaysOnTop)
            {
                SetWindowPos(calcMainWindow, HWND_TOPMOST, main_x, main_y, main_cx, main_cy, flags);
            }
            else
            {
                SetWindowPos(calcMainWindow, HWND_NOTOPMOST, main_x, main_y, main_cx, main_cy, flags);
            }

        case (VK_UP):
        case (VK_DOWN):
            ProcessDirectKeyHit(wParam);
            break;

        case (VK_F1):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN4);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN3);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN2);
            else
                ProcessKeyHit(FN1);
            break;
        case (VK_F2):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN8);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN7);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN6);
            else
                ProcessKeyHit(FN5);
            break;
        case (VK_F3):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN12);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN11);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN10);
            else
                ProcessKeyHit(FN9);
            break;
        case (VK_F4):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN16);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN15);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN14);
            else
                ProcessKeyHit(FN13);
            break;
        case (VK_F5):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN20);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN19);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN18);
            else
                ProcessKeyHit(FN17);
            break;
        case (VK_F6):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN24);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN23);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN22);
            else
                ProcessKeyHit(FN21);
            break;
        case (VK_F7):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN28);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN27);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN26);
            else
                ProcessKeyHit(FN25);
            break;
        case (VK_F8):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN32);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN31);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN30);
            else
                ProcessKeyHit(FN29);
            break;
        case (VK_F9):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN36);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN35);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN34);
            else
                ProcessKeyHit(FN33);
            break;
        case (VK_F10):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN40);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN39);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN38);
            else
                ProcessKeyHit(FN37);
            break;
        }
        return 0;
        break;

    case WM_CHAR:
        if (macroPlayback == TRUE) // If we get a playback over a playback we stop the playback!
        {
            endRunningMacro();
            ShowStatus();
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }
        ProcessDirectKeyHit((int)wParam);
        break;

    case WM_MOVE:
    case WM_SIZE:
        GetWindowRect(calcMainWindow, &rcWindow);
        if ((rcWindow.left < MAX_WINDOW_X) && (rcWindow.top < MAX_WINDOW_Y))
        {
            main_x = rcWindow.left;
            main_y = rcWindow.top;
            main_cx = rcWindow.right - rcWindow.left;
            main_cy = rcWindow.bottom - rcWindow.top;
        }

        if (showXMinimized == 1)
        {
            if (wParam == SIZE_MINIMIZED)
            {
                GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, 24);
                SetWindowText(calcMainWindow, tmpStr);
            }
            else
            {
                SetWindowText(calcMainWindow, WINDOW_TITLE);
            }
        }

        return 0;
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        init_key_pos();
        ShowStatus();
        EndPaint(hwnd, &ps);
        return 0;
        break;

    case WM_CTLCOLORSTATIC:
        i = GetWindowLong((HWND)LOWORD(lParam), (int)GWL_ID);

        if (i == RPN_STACK || i == RPN_STACK_X || i == RPN_STACK_Y || i == RPN_STACK_Z || i == RPN_STACK_T)
        {
            SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
            SetTextColor((HDC)wParam, GetSysColor(COLOR_WINDOWTEXT));
            return ((DWORD)backgroundBrush);
        }
        return DefWindowProc(hwnd, iMsg, wParam, lParam);

    case WM_DESTROY:
        endRunningMacro();
        SaveToDisk();
        DeleteObject(holdsfont);
        DeleteObject(hMainFont);
        DeleteObject(hNumberFont);
        DeleteObject(hFixedFont);
        DeleteObject(backgroundBrush);
        DestroyWindow(toolTipWnd);
        DestroyWindow(debugTraceWindow);
        KillTimer(calcMainWindow, 1);
        KillTimer(calcMainWindow, 2);
        KillTimer(calcMainWindow, 3);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void SelectNewBank(struct funcStruct *funcs)
{
    // ------------------------------------------------------------------------------------------------
    // If we are switching out of CompSci mode or Program mode we need to convert longs back to floats
    // and reset the display since those modes use the stack differently than the other modes.
    // ------------------------------------------------------------------------------------------------
    if (currentFuncs == (struct funcStruct *)&CompSci_funcs || progMode != PROG_FLOAT)
    {
        if ((funcs != (struct funcStruct *)&Program1_funcs) &&
            (funcs != (struct funcStruct *)&Program2_funcs) &&
            (funcs != (struct funcStruct *)&CompSci_funcs))
        {
            LongsToFloats();
            progMode = PROG_FLOAT;
            ShowStack();
            ShowStatus();
        }
    }

    // ---------------------------------------------------------------------------------
    // Set all menu items to unchecked - we will pick the right one further below.
    // ---------------------------------------------------------------------------------
    CheckMenuItem(hMainMenu, IDM_SCIENTIFIC,    MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_COMPSCI,       MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_FINANCIAL,     MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_CONVERSION,    MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_PROGII,        MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_CUSTOM,        MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_STATS,         MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_SCIENTIFIC2,   MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_PROGI,         MF_UNCHECKED);

    // -----------------------------------------------------------------------------------------
    // Set all radio buttons associated with banking to unchecked. We pick the right one below.
    // -----------------------------------------------------------------------------------------
    SendMessage(GetDlgItem(calcMainWindow, RPN_SCI),        BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STAT),       BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_FIN),        BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_CONV),       BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_COMPSCI),    BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_SCI2),       BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_PROGI),      BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_PROGII),     BM_SETCHECK, (WORD)0, (DWORD)0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_CUST),       BM_SETCHECK, (WORD)0, (DWORD)0L);

    // -------------------------------------------------------------------------------------------------
    // Now determine which of the banks we are switching into... set the radio button and check the
    // menu item for the bank we are switching into and show the appropriate function bar for that bank.
    // -------------------------------------------------------------------------------------------------
    if (funcs == (struct funcStruct *)&Scientific_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_SCIENTIFIC, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_SCI), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_SCI_I);
    }
    else if (funcs == (struct funcStruct *)&CompSci_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_COMPSCI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_COMPSCI), BM_SETCHECK, (WORD)1, (DWORD)0L);
        switch (progMode)
        {
            case PROG_HEX:
                PROG_hex();
                break;
            case PROG_OCT:
                PROG_oct();
                break;
            case PROG_BIN:
                PROG_bin();
                break;
            case PROG_DEC:
            default:
                PROG_dec();
                break;
        }
        ShowFunctionBar(FUNC_BAR_TEXT_COMPSCI);
        FloatsToLongs();
        ShowStack();
        ShowStatus();
    }
    else if (funcs == (struct funcStruct *)&Financial_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_FINANCIAL, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_FIN), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_BUIS);
    }
    else if (funcs == (struct funcStruct *)&Conversion_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_CONVERSION, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CONV), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CONV);
    }
    else if (funcs == (struct funcStruct *)&Custom_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_CUSTOM, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CUST), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CUSTOM);
    }
    else if (funcs == (struct funcStruct *)&Statistics_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_STATS, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_STAT), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_STATS);
    }
    else if (funcs == (struct funcStruct *)&Scientific2_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_SCIENTIFIC2, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_SCI2), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_SCI2);
    }
    else if (funcs == (struct funcStruct *)&Program1_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_PROGI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGI), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG1);
    }
    else if (funcs == (struct funcStruct *)&Program2_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_PROGII, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGII), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG2);
    }

    lastFuncs = currentFuncs;
    currentFuncs = (struct funcStruct *)funcs;
    processBank();
}

LRESULT CALLBACK tooltipWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (message)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
        SetBkColor((HDC)hdc, GetSysColor(COLOR_INFOBK));
        SetTextColor((HDC)hdc, GetSysColor(COLOR_INFOTEXT));
        DrawText(hdc, TEXT(helpTitle), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void CopyTextToClipboard(HWND hwnd, char *text)
{
    HGLOBAL hMem;
    LPSTR lpMem;

    hMem = GlobalAlloc(GHND, (DWORD)(strlen(text) + 1));
    lpMem = GlobalLock(hMem);
    lstrcpy(lpMem, text);
    OpenClipboard(hwnd);
    EmptyClipboard();
    GlobalUnlock(hMem);
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void ClipboardCopySelection(HWND hwnd, uint8_t copytype)
{
    HANDLE hMem;
    LPSTR lpMem;
    char tmp2[32];
    char tmp3[32];
    int i, j;
    unsigned short chksum = 0x0000;

    if (copytype == COPY_X_TO_CLIPBOARD) // Copy X to clipboard
    {
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN); // X register
        trim(tmpStr);
        if (progMode == PROG_DEC)
        {
            if (tmpStr[strlen(tmpStr) - 1] == 'd')
                tmpStr[strlen(tmpStr) - 1] = 0;
        }
        if (tmpStr[strlen(tmpStr) - 1] == '_')
            tmpStr[strlen(tmpStr) - 1] = 0;
        CopyTextToClipboard(hwnd, tmpStr);
        Xedit = X_NEW;
        ShowStack();
    }
    else if (copytype == COPY_ALL_TO_CLIPBOARD) // Copy All to clipboard
    {
        strcpy(clipboardBuffer, "");
        GetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr, MAX_STACK_STRLEN); // T register
        trim(tmpStr);
        if (progMode == PROG_DEC)
        {
            if (tmpStr[strlen(tmpStr) - 1] == 'd')
                tmpStr[strlen(tmpStr) - 1] = 0;
        }
        strcat(clipboardBuffer, tmpStr);
        strcat(clipboardBuffer, "\r\n");

        GetDlgItemText(calcMainWindow, RPN_STACK_Z, tmpStr, MAX_STACK_STRLEN); // Z register
        trim(tmpStr);
        if (progMode == PROG_DEC)
        {
            if (tmpStr[strlen(tmpStr) - 1] == 'd')
                tmpStr[strlen(tmpStr) - 1] = 0;
        }
        strcat(clipboardBuffer, tmpStr);
        strcat(clipboardBuffer, "\r\n");

        GetDlgItemText(calcMainWindow, RPN_STACK_Y, tmpStr, MAX_STACK_STRLEN); // Y register
        trim(tmpStr);
        if (progMode == PROG_DEC)
        {
            if (tmpStr[strlen(tmpStr) - 1] == 'd')
                tmpStr[strlen(tmpStr) - 1] = 0;
        }
        strcat(clipboardBuffer, tmpStr);
        strcat(clipboardBuffer, "\r\n");

        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN); // X register
        trim(tmpStr);
        if (progMode == PROG_DEC)
        {
            if (tmpStr[strlen(tmpStr) - 1] == 'd')
                tmpStr[strlen(tmpStr) - 1] = 0;
        }
        strcat(clipboardBuffer, tmpStr);
        strcat(clipboardBuffer, "\r\n");

        CopyTextToClipboard(hwnd, clipboardBuffer);
        Xedit = X_NEW;
        ShowStack();
    }
    else if (copytype == COPY_MACRO_TO_CLIPBOARD) // Copy current macro to clipboard
    {
        strcpy(clipboardBuffer, "");

        for (i = 0; i < playBackIdx; i++)
        {
            sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
            strcat(clipboardBuffer, tmpStr);
            strcat(clipboardBuffer, "\r\n");
        }
        sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
        strcat(clipboardBuffer, tmpStr);
        strcat(clipboardBuffer, "\r\n");
        chksum = 0x0000;
        for (i = 0; i < playBackIdx; i++)
            chksum += playBack[i];
        sprintf(tmpStr, "Checksum: %04X", chksum);
        strcat(clipboardBuffer, "\r\n");
        strcat(clipboardBuffer, tmpStr);
        strcat(clipboardBuffer, "\r\n");

        CopyTextToClipboard(hwnd, clipboardBuffer);
        Xedit = X_NEW;
        ShowStack();
    }
    else if (copytype == COPY_X_FROM_CLIPBOARD) // Copy from clipboard to X register
    {
        OpenClipboard(hwnd);
        hMem = GetClipboardData(CF_TEXT);
        lpMem = GlobalLock(hMem);
        lstrcpyn(tmp2, lpMem, MAX_STACK_STRLEN);
        GlobalUnlock(hMem);
        CloseClipboard();
        tmp2[20] = '\0';
        j = 0;
        for (i = 0; i < (int)strlen(tmp2); i++)
        {
            if (numberDisplayMode == NONINTERNATIONAL)
            {
                if (tmp2[i] != '.') // Strip decimal points out....
                {
                    tmp3[j] = tmp2[i];
                    j++;
                }
            }
            else
            {
                if (tmp2[i] != ',') // Otherwise, Strip commas out...
                {
                    tmp3[j] = tmp2[i];
                    j++;
                }
            }
        }
        tmp3[j] = (char)NULL;
        if (numberDisplayMode == NONINTERNATIONAL) // Turn comma into dp
        {
            for (i = 0; i < (int)strlen(tmp3); i++)
            {
                if (tmp3[i] == ',') // If we have a comma, make dp
                    tmp3[i] = '.';
            }
        }
        Xedit = X_NEW;
        if (progMode == PROG_FLOAT)
        {
            StackPush((double)atof(tmp3));
        }
        else
        {
            if (progMode == PROG_HEX)
            {
                StackPushL((PROG_LONG)strtou64(tmp3, NULL, 16));
            }
            else
            {
                StackPushL((PROG_LONG)strtou64(tmp3, NULL, 0));
            }
        }
        ShowStack();
    }
}

HFONT GetMainNumberFont(void)
{
    HDC dc;
    int nHeight;
    HFONT font;
    TEXTMETRIC tm;
    dc = GetDC(NULL);

    nHeight = -MulDiv(12, GetDeviceCaps(dc, LOGPIXELSY), 72);

    font = CreateFont(nHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE | FIXED_PITCH, "Consolas");

    font = SelectObject(dc, font);
    GetTextMetrics(dc, &tm);

    // ------------------------------------------------------------
    // Check to make sure we got Consolas... if not, we need to
    // format the right-alignment a bit more conservatively.
    // ------------------------------------------------------------
    {
        char lpszFacename[256 + 1];
        GetTextFaceA(dc, 256, lpszFacename);
        if (strncmp(lpszFacename, "Consolas", 8) == 0)
        {
            bExactFont = TRUE;
        }
        else
        {
            bExactFont = FALSE;
        }
    }

    font = SelectObject(dc, font);
    ReleaseDC(NULL, dc);
    return (font);
}

HFONT GetSystemFont(void)
{
    HDC dc;
    int nHeight;
    HFONT font;
    TEXTMETRIC tm;
    dc = GetDC(NULL);

    nHeight = -MulDiv(10, GetDeviceCaps(dc, LOGPIXELSY), 72);

    font = CreateFont(nHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS | VARIABLE_PITCH, "System");

    font = SelectObject(dc, font);
    GetTextMetrics(dc, &tm);

    font = SelectObject(dc, font);
    ReleaseDC(NULL, dc);
    return (font);
}

HFONT GetSystemFontFixed(void)
{
    HDC dc;
    int nHeight;
    HFONT font;
    TEXTMETRIC tm;
    dc = GetDC(NULL);

    nHeight = -MulDiv(12, GetDeviceCaps(dc, LOGPIXELSY), 72);

    font = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, "System");

    font = SelectObject(dc, font);
    GetTextMetrics(dc, &tm);

    font = SelectObject(dc, font);
    ReleaseDC(NULL, dc);
    return (font);
}

void SetUpFonts(HWND hwnd)
{
    // Get the display context.
    HDC hDC = GetDC(hwnd);

    hMainFont = GetSystemFont();       // Get the font used for buttons and most UI elements (proportional font)
    hNumberFont = GetMainNumberFont(); // Get the font used for the main stack display of numbers(slightly bigger/bolder)
    hFixedFont = GetSystemFontFixed(); // Get the font used for various dialog boxes where the text needs to be aligned in columns

    // Install the font in the current display context.
    holdsfont = SelectObject(hDC, hMainFont);

    // Release the display context.
    ReleaseDC(calcMainWindow, hDC);
}

void NotImp(void)
{
    MessageBox(calcMainWindow, "Function is not implemented yet...", "Excalibur", MB_OK | MB_ICONINFORMATION);
}

__inline void RPN_ClearModifiers(int updateSpare)
{
    modifiers = 0x00;
    rpnStoreRecall = 0x00;
    if (updateSpare)
        UpdateSpareBar(" ");
}

void UpdateSpareBar_StoreRecall(void)
{
    char tmpStr[16];
    if (rpnStoreRecall && !macroPlayback)
    {
        if (rpnStoreRecall & REG_STORE)
        {
            strcpy(tmpStr, "STO");
        }
        else if (rpnStoreRecall & REG_RECALL)
        {
            strcpy(tmpStr, "RCL");
        }
        else if (rpnStoreRecall & REG_EXCHANGE)
        {
            strcpy(tmpStr, "EXCH");
        }
        if (rpnStoreRecall & REG_PLUS)
            strcat(tmpStr, "+");
        if (rpnStoreRecall & REG_MINUS)
            strcat(tmpStr, "-");
        if (rpnStoreRecall & REG_MULTIPLY)
            strcat(tmpStr, "×");
        if (rpnStoreRecall & REG_DIVIDE)
            strcat(tmpStr, "÷");

        if (rpnStoreRecall & REG_DP)
            strcat(tmpStr, " ·");

        UpdateSpareBar(tmpStr);
    }
}


// --------------------------------------------------------
// Used when we switch into one of the 'Comp Sci' modes...
// --------------------------------------------------------
void FloatsToLongs(void)
{
    lastFloat = STACK[STK_X];
    if (STACK[STK_X] <= (float)INT64_MAX)
        STACKL[STK_X] = (PROG_SIGNEDLONG)STACK[STK_X];
    else
        STACKL[STK_X] = wordSizeMask;

    if (STACK[STK_Y] <= (float)INT64_MAX)
        STACKL[STK_Y] = (PROG_SIGNEDLONG)STACK[STK_Y];
    else
        STACKL[STK_Y] = wordSizeMask;

    if (STACK[STK_Z] <= (float)INT64_MAX)
        STACKL[STK_Z] = (PROG_SIGNEDLONG)STACK[STK_Z];
    else
        STACKL[STK_Z] = wordSizeMask;

    if (STACK[STK_T] <= (float)INT64_MAX)
        STACKL[STK_T] = (PROG_SIGNEDLONG)STACK[STK_T];
    else
        STACKL[STK_T] = wordSizeMask;

    if (extendedStack)
    {
        if (STACK[STK_A] <= (float)INT64_MAX)
            STACKL[STK_A] = (PROG_SIGNEDLONG)STACK[STK_A];
        else
            STACKL[STK_A] = wordSizeMask;

        if (STACK[STK_B] <= (float)INT64_MAX)
            STACKL[STK_B] = (PROG_SIGNEDLONG)STACK[STK_B];
        else
            STACKL[STK_B] = wordSizeMask;

        if (STACK[STK_C] <= (float)INT64_MAX)
            STACKL[STK_C] = (PROG_SIGNEDLONG)STACK[STK_C];
        else
            STACKL[STK_C] = wordSizeMask;

        if (STACK[STK_D] <= (float)INT64_MAX)
            STACKL[STK_D] = (PROG_SIGNEDLONG)STACK[STK_D];
        else
            STACKL[STK_D] = wordSizeMask;
    }

    if (LASTX <= (float)INT64_MAX)
        LASTXL = (PROG_SIGNEDLONG)LASTX;
    else
        LASTXL = wordSizeMask;

    if (LASTY <= (float)INT64_MAX)
        LASTYL = (PROG_SIGNEDLONG)LASTY;
    else
        LASTYL = wordSizeMask;
}

// --------------------------------------------------------
// Used when we switch out one of the 'Comp Sci' modes...
// --------------------------------------------------------
void LongsToFloats(void)
{
    if (wordMode == COMPSCI_SIGNED)
    {
        STACK[STK_X] = (double)(PROG_SIGNEDLONG)STACKL[STK_X];
        STACK[STK_Y] = (double)(PROG_SIGNEDLONG)STACKL[STK_Y];
        STACK[STK_Z] = (double)(PROG_SIGNEDLONG)STACKL[STK_Z];
        STACK[STK_T] = (double)(PROG_SIGNEDLONG)STACKL[STK_T];
        STACK[STK_A] = (double)(PROG_SIGNEDLONG)STACKL[STK_A];
        STACK[STK_B] = (double)(PROG_SIGNEDLONG)STACKL[STK_B];
        STACK[STK_C] = (double)(PROG_SIGNEDLONG)STACKL[STK_C];
        STACK[STK_D] = (double)(PROG_SIGNEDLONG)STACKL[STK_D];
        LASTX = (double)(PROG_SIGNEDLONG)LASTXL;
        LASTY = (double)(PROG_SIGNEDLONG)LASTYL;
    }
    else
    {
        STACK[STK_X] = (double)(PROG_SIGNEDLONG)STACKL[STK_X];
        STACK[STK_Y] = (double)(PROG_SIGNEDLONG)STACKL[STK_Y];
        STACK[STK_Z] = (double)(PROG_SIGNEDLONG)STACKL[STK_Z];
        STACK[STK_T] = (double)(PROG_SIGNEDLONG)STACKL[STK_T];
        STACK[STK_A] = (double)(PROG_SIGNEDLONG)STACKL[STK_A];
        STACK[STK_B] = (double)(PROG_SIGNEDLONG)STACKL[STK_B];
        STACK[STK_C] = (double)(PROG_SIGNEDLONG)STACKL[STK_C];
        STACK[STK_D] = (double)(PROG_SIGNEDLONG)STACKL[STK_D];
        LASTX = (double)(PROG_SIGNEDLONG)LASTXL;
        LASTY = (double)(PROG_SIGNEDLONG)LASTYL;
    }
}

void MemoryInit(void)
{
    int i, j, k;

    STACK[STK_X] = 0.0;
    STACK[STK_Y] = 0.0;
    STACK[STK_Z] = 0.0;
    STACK[STK_T] = 0.0;
    STACK[STK_A] = 0.0;
    STACK[STK_B] = 0.0;
    STACK[STK_C] = 0.0;
    STACK[STK_D] = 0.0;

    for (i = 0; i < MAX_STO; i++)
    {
        STO[i] = 0.0;
        STOL[i] = 0L;
    }

    for (i = 0; i < FIN_REG_MAX; i++)
    {
        FIN[i] = 0.0;
    }

    for (i = 0; i < MAX_MACROS; i++)
    {
        strcpy(macroName[i], "Not Currently Defined");
        sprintf(macro_short_names[i], "P%02d", i + 1);
        playBackIdxSave[i] = 0;
    }

    playBackIdx = 0;
    currentMacroPlaybackIdx = 0;
    indirectRegister = 0;
    
    // TBD - check this...
    for (k = 0; k < MAX_CONST_BANKS; k++)
    {
        for (i = 0; i < MAX_CONSTS; i++)
        {
            if (constants[k][i].includeInList == FALSE)
            {
                for (j = i; j < MAX_CONSTS; j++)
                {
                    constants[k][j].includeInList = FALSE;
                }
                break;
            }
        }
    }

    // Get regional time setting for 24-hour format!
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, tmpStr, 5);
    tmpStr[4] = CNULL;
    if (atoi(tmpStr) == 1)
    {
        showTime24HourFormat = TRUE;
    }
    // Get regional comma format...
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, tmpStr, 5);
    if (tmpStr[0] == ',')
    {
        numberDisplayMode = NONINTERNATIONAL;
    }
}

int Init(void)
{
    UINT flags;

    UpdateVersionBar();
    UpdateSpareBar("    ");
    ShowFunctionBar(FUNC_BAR_TEXT_SCI_I);

    mapButtonFuncs();
    ShowStack();
    processBank();
    ShowStatus();

    if (main_x == -1)
        flags = SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE;
    else
        flags = SWP_HIDEWINDOW | SWP_NOSIZE;

    if (alwaysOnTop)
    {
        CheckMenuItem(hMainMenu, IDM_ALWAYSONTOP, MF_CHECKED);
        SetWindowPos(calcMainWindow, HWND_TOPMOST, main_x, main_y, main_cx, main_cy, flags);
    }
    else
    {
        CheckMenuItem(hMainMenu, IDM_ALWAYSONTOP, MF_UNCHECKED);
        SetWindowPos(calcMainWindow, HWND_NOTOPMOST, main_x, main_y, main_cx, main_cy, flags);
    }

    // --------------------------------------------------------
    // Turn on the num lock automatically for this application.
    // --------------------------------------------------------
    if (numLockMode == 1)
    {
        turnOnNumLock();
    }

    // --------------------------------------------------------
    // If we need to remap the +/- and the E keys, do so now...
    // --------------------------------------------------------
    if (eexMode == 1)
    {
        SetDlgItemText(calcMainWindow, RPN_NEGATE, "CHS");
        SetDlgItemText(calcMainWindow, RPN_E, "EEX");
    }
    if (numberDisplayMode == NONINTERNATIONAL)
    {
        SetDlgItemText(calcMainWindow, RPN_DIGIT_DP, ",");
    }
    return (0);
}

void ShowStatus(void)
{
    if (progMode != PROG_FLOAT)
    {
        if (progModecarry == 1)
            sprintf(tmpStr, "C");
        else
            sprintf(tmpStr, "  ");
        SetDlgItemText(calcMainWindow, RPN_CARRY, tmpStr);
    }

    if (recModeON == 0)
        sprintf(tmpStr, "   ");
    else
        sprintf(tmpStr, "REC ");
    SetDlgItemText(calcMainWindow, REC_BAR, tmpStr);

    if (progMode != PROG_FLOAT)
    {
        if (wordMode == COMPSCI_SIGNED)
            sprintf(tmpStr, "S");
        else
            sprintf(tmpStr, "U");

        if (wordSize == 8)
            strcat(tmpStr, "8  ");
        else if (wordSize == 16)
            strcat(tmpStr, "16 ");
        else if (wordSize == 32)
            strcat(tmpStr, "32 ");
        else
            strcat(tmpStr, "64 ");

        if (progMode == PROG_BIN)
        {
            if (binMode == 0)
                strcat(tmpStr, "bin LO");
            else
                strcat(tmpStr, "bin HI");
        }
        else
        {
            strcat(tmpStr, "       ");
        }

        SetDlgItemText(calcMainWindow, PROG_BAR, tmpStr);
    }
    else
    {
        sprintf(tmpStr, "    ");
        SetDlgItemText(calcMainWindow, PROG_BAR, tmpStr);
    }

    if (sci_format == 'g')
        sprintf(statusBar, "STD");
    if (sci_format == 'E')
        sprintf(statusBar, "SCI %2d", decimal_places);
    if (sci_format == 'Z')
        sprintf(statusBar, "ENG %2d", decimal_places);
    if (sci_format == 'f')
        sprintf(statusBar, "FIX %2d", decimal_places);
    SetDlgItemText(calcMainWindow, STATUS_BAR, statusBar);

    if (AngleMode == 0)
        sprintf(tmpStr, "DEG");
    if (AngleMode == 1)
        sprintf(tmpStr, "RAD");
    if (AngleMode == 2)
        sprintf(tmpStr, "GRA");
    SetDlgItemText(calcMainWindow, ANGLE_BAR, tmpStr);
}

void UpdateSpareBar(char *msg)
{
    SetDlgItemText(calcMainWindow, SPARE_BAR, msg);
}

void UpdateVersionBar()
{
    SetDlgItemText(calcMainWindow, TIME_BAR, VERSION_STR);
}

void ShowFunctionBar(char *msg)
{
    sprintf(functionBar, "%s", msg);
    SetDlgItemText(calcMainWindow, FUNC_BAR, functionBar);
}

void RPN_error(char *msg)
{
    MessageBox(calcMainWindow, msg, "Excalibur Calculation Error", MB_OK | MB_ICONEXCLAMATION);
}

void RPN_fact(void)
{
    if (progMode == PROG_FLOAT)
    {
        int fact;
        double temp;

        if (STACK[STK_X] >= 0.0 && STACK[STK_X] <= 170.0)
        {
            temp = 1.0;
            for (fact = (int) STACK[STK_X]; fact > 0; fact--)
            {
                temp = (double) temp *(double) fact;
            }
            StackPop();
            StackPush(temp);
        }
        else
        {
            RPN_error("X! Out Of Range(0-170)");
        }
    }
    else
    {
        PROG_LONG fact;
        PROG_LONG temp;

        if (STACKL[STK_X] >= 0 && STACKL[STK_X] <= 20)
        {
            temp = 1;
            for (fact = STACKL[STK_X]; fact > 0; fact--)
            {
                temp = temp * fact;
            }
            StackPopL();
            StackPushL(temp);
        }
        else
        {
            RPN_error("X! Out Of Range(0-20 in Comp-Sci mode)");
        }
    }
}

double MakeAccurate(double val)
{
    unsigned char str[64];

    sprintf(str, "%.14g", val);
    val = atof(str);
    return (val);
}

// clang-format off
struct funcStruct RPNkeys[] = {
    {RPN_DIGIT_0,   UNI_DIG0,   USES_FL, ALLOWREC, '0', "", NO_L,   X_NULL,     RPN_digit0,         "Digit 0",              "Used in keypad number entry."},
    {RPN_DIGIT_1,   UNI_DIG1,   USES_FL, ALLOWREC, '1', "", NO_L,   X_NULL,     RPN_digit1,         "Digit 1",              "Used in keypad number entry."},
    {RPN_DIGIT_2,   UNI_DIG2,   USES_FL, ALLOWREC, '2', "", NO_L,   X_NULL,     RPN_digit2,         "Digit 2",              "Used in keypad number entry."},
    {RPN_DIGIT_3,   UNI_DIG3,   USES_FL, ALLOWREC, '3', "", NO_L,   X_NULL,     RPN_digit3,         "Digit 3",              "Used in keypad number entry."},
    {RPN_DIGIT_4,   UNI_DIG4,   USES_FL, ALLOWREC, '4', "", NO_L,   X_NULL,     RPN_digit4,         "Digit 4",              "Used in keypad number entry."},
    {RPN_DIGIT_5,   UNI_DIG5,   USES_FL, ALLOWREC, '5', "", NO_L,   X_NULL,     RPN_digit5,         "Digit 5",              "Used in keypad number entry."},
    {RPN_DIGIT_6,   UNI_DIG6,   USES_FL, ALLOWREC, '6', "", NO_L,   X_NULL,     RPN_digit6,         "Digit 6",              "Used in keypad number entry."},
    {RPN_DIGIT_7,   UNI_DIG7,   USES_FL, ALLOWREC, '7', "", NO_L,   X_NULL,     RPN_digit7,         "Digit 7",              "Used in keypad number entry."},
    {RPN_DIGIT_8,   UNI_DIG8,   USES_FL, ALLOWREC, '8', "", NO_L,   X_NULL,     RPN_digit8,         "Digit 8",              "Used in keypad number entry."},
    {RPN_DIGIT_9,   UNI_DIG9,   USES_FL, ALLOWREC, '9', "", NO_L,   X_NULL,     RPN_digit9,         "Digit 9",              "Used in keypad number entry."},
    {RPN_DIGIT_DP,  UNI_DIGDP,  USES_FL, ALLOWREC, '.', "", NO_L,   X_NULL,     RPN_dp,             "Decimal Point",        "Used in keypad number entry and can also be used to enter fractions into the X display (press twice)"},
    {RPN_CLEAR_X,   UNI_CLX,    USES_FL, ALLOWREC,  9,  "", NO_L,   X_NULL,     RPN_clearX,         "Clear X",              "Used to clear the X Register contents."},
    {RPN_DIVIDE,    UNI_DIV,    USES_FL, ALLOWREC, '/', "", YES_L,  X_NULL,     RPN_divide,         "Divide",               "Division of Y by X(Y/X)"},
    {RPN_MULTIPLY,  UNI_MUL,    USES_FL, ALLOWREC, '*', "", YES_L,  X_NULL,     RPN_multiply,       "Multiply",             "Multiplication of Y and X"},
    {RPN_SUBTRACT,  UNI_SUB,    USES_FL, ALLOWREC, '-', "", YES_L,  X_NULL,     RPN_minus,          "Minus",                "Subtraction of X from Y(Y-X)"},
    {RPN_PLUS,      UNI_PLUS,   USES_FL, ALLOWREC, '+', "", YES_L,  X_NULL,     RPN_plus,           "Plus",                 "Addition of X and Y"},
    {RPN_ENTER,     UNI_ENT,    USES_FL, ALLOWREC, 13,  "", NO_L,   X_NULL,     RPN_enter,          "Enter",                "Used to separate numbers in the RPN stack"},

    {RPN_EXCH_X_Y,  UNI_XCH,    USES_FL, ALLOWREC, 'x', "", NO_L,   X_NEW,      RPN_exchange_x_y,   "Exchange X and Y",     "Exchanges the contents of the X and Y registers"},
    {RPN_NEGATE,    UNI_CHS,    USES_FL, ALLOWREC, 'n', "", NO_L,   X_NULL,     RPN_negate_x,       "Change Sign",          "Used to change the sign of X"},
    {RPN_E,         UNI_E,      USES_FL, ALLOWREC, 'e', "", NO_L,   X_NULL,     RPN_Ex,             "Exponent",             "Used to produce an exponential number(e.g. 3.45e+12)"},
    {RPN_STO,       UNI_STO,    USES_FL, ALLOWREC, 's', "", NO_L,   X_NULL,     RPN_store,          "Store Register",       "Used to store X to one of the registers (next digit/dp selects R0-R19). Register Arithmetic is also supported."},
    {RPN_RCL,       UNI_RCL,    USES_FL, ALLOWREC, 'r', "", NO_L,   X_NULL,     RPN_recall,         "Recall Register",      "Used to recall one of registers to X (next digit/dp selects R0-R19). Register Arithmetic is also supported."},
    {RPN_R_UP,      UNI_RUP,    USES_FL, ALLOWREC, 38,  "", NO_L,   X_NEW,      RPN_rotateStackUp,  "Rotate Stack Up",      "Rotates the contents of the stack up"},
    {RPN_R_DN,      UNI_RDN,    USES_FL, ALLOWREC, 40,  "", NO_L,   X_NEW,      RPN_rotateStackDn,  "Rotate Stack Down",    "Rotates the contents of the stack down"},
    {RPN_LASTX,     UNI_LSTX,   USES_FL, ALLOWREC, 'l', "", NO_L,   X_NEW,      RPN_lastX,          "Last X",               "Retrieves the last value of X before the last operation occurred"},
    {RPN_MODE,      UNI_MODE,   USES_FL, ALLOWREC, 'm', "", NO_L,   X_NEW,      RPN_mode,           "Select Mode",          "Used to select number format mode"},
    {RPN_BKSP,      UNI_BKSP,   USES_FL, ALLOWREC,  8,  "", NO_L,   X_NULL,     RPN_backspace,      "Backspace",            "Used to correct mistakes in number entry"},
    {RPN_CLR_STACK, UNI_CLRSTK, USES_FL, ALLOWREC, 'c', "", YES_L,  X_ENTER,    RPN_clearStack,     "Clear Stack",          "Used to clear the entire stack contents. Press twice to clear all registers as well."},
    {RPN_FACT,      UNI_FACT,   USES_FL, ALLOWREC, '!', "", YES_L,  X_NEW,      RPN_fact,           "Factorial X",          "Compute the Factorial of X"},
    {RPN_PLAYBACK,  UNI_PLAY,   USES_FL, NORECORD, 'p', "", NO_L,   X_NEW,      RPN_Playback,       "Run Program",          "Run the the currently loaded program."},
    {RPN_DROP,      UNI_DROP,   USES_FL, ALLOWREC, 'd', "", YES_L,  X_NEW,      RPN_drop,           "Drop Stack",           "Drops the X register and the rest of stack shifts down."},
    {RPN_LARG,      UNI_LARG,   USES_FL, ALLOWREC, ' ', "", NO_L,   X_NEW,      RPN_larg,           "Last Arguments",       "Retrieves the last X and Y pair before last operation."},
    {RPN_SHOW,      UNI_SHOW,   USES_FL, NORECORD, 's', "", NO_L,   X_NULL,     RPN_show,           "Show Values",          "Show Full Stack, Registers, Statistics, etc."},
    {RPN_EDIT,      UNI_EDIT,   USES_FL, ALLOWREC, ' ', "", NO_L,   X_NULL,     RPN_edit,           "Edit X Register",      "Used to place the X register back in edit mode if it is not already."},
    {RPN_POW,       UNI_POW,    USES_FL, ALLOWREC, '^', "", YES_L,  X_NEW,      RPN_pow,            "Raise to Power",       "Raise Y to the power of X"},
    {RPN_NOTES,     UNI_NOTES,  USES_FL, ALLOWREC, ' ', "", NO_L,   X_NULL,     RPN_Notes,          "Excalibur Notepad",    "Allows some simple notes to be stored/saved."},
    {RPN_INV,       UNI_INVX,   USES_FL, ALLOWREC, 'i', "", YES_L,  X_NEW,      RPN_inverse,        "Inverse X",            "Computes the inverse of X"},
    {RPN_REC,       UNI_REC,    USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_Record,         "Record Mode On/Off",   "When ON - Records button presses for playback."},
    {RPN_EXREG,     UNI_EXREG,  USES_FL, ALLOWREC, ' ', "", NO_L,   X_NULL,     RPN_ExchangeReg,    "Exchange X with Reg",  "Exchange X with one of the Registers (next digit/dp selects R0-R19)"},
    {RPN_COPY,      UNI_COPY,   USES_FL, ALLOWREC, ' ', "", NO_L,   X_NULL,     RPN_Copy,           "Copy X Register",      "Copy X register to the clipboard"},
    {RPN_PASTE,     UNI_PASTE,  USES_FL, ALLOWREC, ' ', "", NO_L,   X_NULL,     RPN_Paste,          "Paste X Register",     "Paste X register from the clipboard"},
    {RPN_SQRT,      UNI_SQRT,   USES_FL, ALLOWREC, ' ', "", YES_L,  X_NEW,      SCI_sqrt,           "Square Root",          "Computes the Square Root of the value in X"},
    {RPN_LN,        UNI_LN,     USES_FL, ALLOWREC, ' ', "", YES_L,  X_NEW,      SCI_ln,             "Natural Logarithm",    "Computes the natural logarithm (base e) of X"},
    {RPN_LOG,       UNI_LOG,    USES_FL, ALLOWREC, ' ', "", YES_L,  X_NEW,      SCI_log,            "Base 10 Logarithm",    "Raises the base 10 logarithm of X"},

    {RPN_SCI,       UNI_SCI,    USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectSci,      "Select Scientific I",  "Selects the Scientific I Layout"},
    {RPN_SCI2,      UNI_SCI2,   USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectSci2,     "Select Scientific II", "Selects the Scientific II Layout"},
    {RPN_COMPSCI,   UNI_COMPSCI,USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectCompSci,  "Select Comp Sci",      "Selects the Computer Science Layout"},
    {RPN_FIN,       UNI_FIN,    USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectFin,      "Select Financial",     "Selects the Financial Layout"},
    {RPN_CONV,      UNI_CONV,   USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectConv,     "Select Conversion",    "Selects the Conversion Layout"},
    {RPN_STAT,      UNI_STAT,   USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectStat,     "Select Statistics",    "Selects the Statistical Layout"},
    {RPN_PROGI,     UNI_PROG1,  USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectProgI,    "Select Program I",     "Selects Program Bank I"},
    {RPN_PROGII,    UNI_PROG2,  USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectProgII,   "Select Program II",    "Selects Program Bank II"},
    {RPN_CUST,      UNI_CUST,   USES_FL, NORECORD, ' ', "", NO_L,   X_NULL,     RPN_SelectCustom,   "Select Custom",        "Selects the Custom Layout"},

    {RPN_LAST_KEY,  UNI_UNUSED, USES_FL, ALLOWREC, ' ', "", NO_L,   X_NEW,      NULL,               "Unused",               "Unused"}
};

struct keyPosStruct
{
    int16_t  controlID;
    int32_t x;
    int32_t y;
    int32_t h;
    int32_t w;
};

// -------------------------------------------------------------------------------------------------------
// We don't bother with tooltips on any of the basic RPN keys up through RPN_ENTER so this table starts
// with the Exchange of X/Y key and goes through the rest of the RPN keys and then the function bank keys.
// -------------------------------------------------------------------------------------------------------
struct keyPosStruct RPNkeyPos[] = {
    {RPN_EXCH_X_Y   ,0,     0},
    {RPN_NEGATE     ,0,     0},
    {RPN_E          ,0,     0},
    {RPN_STO        ,0,     0},
    {RPN_RCL        ,0,     0},
    {RPN_R_UP       ,0,     0},
    {RPN_R_DN       ,0,     0},
    {RPN_LASTX      ,0,     0},
    {RPN_MODE       ,0,     0},
    {RPN_BKSP       ,0,     0},
    {RPN_CLR_STACK  ,0,     0},
    {RPN_FACT       ,0,     0},
    {RPN_PLAYBACK   ,0,     0},
    {RPN_DROP       ,0,     0},
    {RPN_LARG       ,0,     0},
    {RPN_SHOW       ,0,     0},
    {RPN_EDIT       ,0,     0},
    {RPN_POW        ,0,     0},
    {RPN_NOTES      ,0,     0},
    {RPN_INV        ,0,     0},
    {RPN_REC        ,0,     0},
    {RPN_EXREG      ,0,     0},
    {RPN_COPY       ,0,     0},
    {RPN_PASTE      ,0,     0},
    {RPN_SQRT       ,0,     0},
    {RPN_LN         ,0,     0},
    {RPN_LOG        ,0,     0},
    {RPN_LAST_KEY,   0,     0}
};

struct keyPosStruct FunctionBankKeyPos[] = {
    {FN1,           0,          0},
    {FN2,           0,          0},
    {FN3,           0,          0},
    {FN4,           0,          0},
    {FN5,           0,          0},
    {FN6,           0,          0},
    {FN7,           0,          0},
    {FN8,           0,          0},
    {FN9,           0,          0},
    {FN10,          0,          0},
    {FN11,          0,          0},
    {FN12,          0,          0},
    {FN13,          0,          0},
    {FN14,          0,          0},
    {FN15,          0,          0},
    {FN16,          0,          0},
    {FN17,          0,          0},
    {FN18,          0,          0},
    {FN19,          0,          0},
    {FN20,          0,          0},
    {FN21,          0,          0},
    {FN22,          0,          0},
    {FN23,          0,          0},
    {FN24,          0,          0},
    {FN25,          0,          0},
    {FN26,          0,          0},
    {FN27,          0,          0},
    {FN28,          0,          0},
    {FN29,          0,          0},
    {FN30,          0,          0},
    {FN31,          0,          0},
    {FN32,          0,          0},
    {FN33,          0,          0},
    {FN34,          0,          0},
    {FN35,          0,          0},
    {FN36,          0,          0},
    {FN37,          0,          0},
    {FN38,          0,          0},
    {FN39,          0,          0},
    {FN40,          0,          0},
    {RPN_LAST_KEY,  0,          0}
};
// clang-format on

int selectFuncs(WPARAM key)
{
    int i;
    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (key == (WPARAM)currentFuncs[i].controlID)
        {
            if (currentFuncs[i].routine != NULL)
            {
                callButtonFunc(currentFuncs[i].routine, currentFuncs[i].useFloatsLongs, currentFuncs[i].allowRecord,
                               currentFuncs[i].uniqueIndex, currentFuncs[i].saveLastX, currentFuncs[i].newXedit, TRUE);
            }
            break;
        }
    }
    return (0);
}

int processBank(void)
{
    int i;

    for (i = 0; i < MAX_FUNCS; i++)
    {
        SendMessage(GetDlgItem(calcMainWindow, currentFuncs[i].controlID), WM_SETFONT, (WPARAM)hMainFont, FALSE);
        SetDlgItemText(calcMainWindow, currentFuncs[i].controlID, currentFuncs[i].desc);
    }

    UpdateWindow(calcMainWindow);
    return (0);
}

int ProcessDirectKeyHit(WPARAM key)
{
    char keyStroke;
    int i, found;

    keyStroke = LOBYTE(key);

    if (numberDisplayMode == NONINTERNATIONAL && keyStroke == ',') // Allow comma as DP separator on keyboard...
        keyStroke = '.';

    found = 0;
    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (footPrint == 0)
        {
            if (toupper(keyStroke) == toupper(currentFuncs[i].op) && keyStroke != ' ')
            {
                found = 1;
                if (currentFuncs[i].routine != NULL)
                {
                    callButtonFunc(currentFuncs[i].routine,
                                   currentFuncs[i].useFloatsLongs,
                                   currentFuncs[i].allowRecord,
                                   currentFuncs[i].uniqueIndex, currentFuncs[i].saveLastX, currentFuncs[i].newXedit, TRUE);
                }
                break;
            }
        }
    }

    if (!found) // Now check the RPN keys themselves...
    {
        i = 0;
        while (RPNkeys[i].controlID != RPN_LAST_KEY)
        {
            if (toupper(keyStroke) == toupper(RPNkeys[i].op) && toupper(keyStroke) != ' ')
            {
                if (RPNkeys[i].routine != NULL)
                {
                    callButtonFunc(RPNkeys[i].routine,
                                   RPNkeys[i].useFloatsLongs,
                                   RPNkeys[i].allowRecord,
                                   RPNkeys[i].uniqueIndex, RPNkeys[i].saveLastX, RPNkeys[i].newXedit, TRUE);
                }
                break;
            }
            i++;
        }
    }

    ShowStack();
    ShowStatus();
    return (0);
}

int ProcessHelp(WPARAM key)
{
    int i;

    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (key == (WPARAM)currentFuncs[i].controlID)
        {
            MessageBox(calcMainWindow, currentFuncs[i].keyHelp, currentFuncs[i].keyTitle, MB_OK | MB_ICONQUESTION);
            break;
        }
    }
    i = 0;
    while (RPNkeys[i].controlID != RPN_LAST_KEY)
    {
        if (key == (WPARAM)RPNkeys[i].controlID)
        {
            MessageBox(calcMainWindow, RPNkeys[i].keyHelp, RPNkeys[i].keyTitle, MB_OK | MB_ICONQUESTION);
            break;
        }
        i++;
    }
    UpdateSpareBar(" ");
    return (0);
}

int ProcessKeyHit(WPARAM key)
{
    int i;

    i = 0;
    if (key >= FN1 && key <= FN40)
        selectFuncs(key);
    else
        while (RPNkeys[i].controlID != RPN_LAST_KEY)
        {
            if (key == (WPARAM)RPNkeys[i].controlID)
            {
                if (RPNkeys[i].routine != NULL)
                {
                    callButtonFunc(RPNkeys[i].routine,
                                   RPNkeys[i].useFloatsLongs,
                                   RPNkeys[i].allowRecord,
                                   RPNkeys[i].uniqueIndex, RPNkeys[i].saveLastX, RPNkeys[i].newXedit, TRUE);
                }
            }
            i++;
        }
    ShowStack();
    ShowStatus();
    return (0);
}

char Radix(int progM)
{
    if (progM == PROG_DEC)
        return ('d');
    if (progM == PROG_HEX)
        return ('h');
    if (progM == PROG_BIN)
        return ('b');
    if (progM == PROG_OCT)
        return ('o');
    return ('*');
}

char RadixBIN(int progM) // Shows bin HI arrow!
{
    if (progM == PROG_BIN)
    {
        if (binMode == 1)
            return ('»'); // Right Arrow!
        else
            return (' ');
    }

    return (' ');
}

void makeInternational(char *str)
{
    int j;
    if (numberDisplayMode == NONINTERNATIONAL)
    {
        for (j = 0; j < (int)strlen(str); j++)
        {
            if (str[j] == '.')
                str[j] = ',';
            else if (str[j] == ',')
                str[j] = '.';
        }
    }
}

void PutCommas(char *str)
{
    char commaStr[64];
    char *orgp;
    char *orgp2;
    int i, j, k, dpCount;

    if ((commaMode != 0) && (strchr(str, '/') == NULL)) // No commas for fractions
    {
        orgp = strchr(str, '.');
        if (orgp == NULL)
        {
            orgp = strchr(&str[1], ' ');
            orgp2 = strchr(&str[1], 'e');
            if (orgp2 != NULL)
                orgp = orgp2;
            if (orgp == NULL)
                orgp = &str[strlen(str)];
        }
        dpCount = 0;
        while (orgp != &str[0])
        {
            orgp--;
            if (*orgp >= '0' && *orgp <= '9')
                dpCount++;
        }
        j = 0;
        k = 0;
        for (i = 0; i <= (int)strlen(str); i++)
        {
            commaStr[j] = str[i];
            j++;
            if (str[i] >= '0' && str[i] <= '9')
            {
                k++;
                if ((k < dpCount) && ((dpCount - k) % 3) == 0)
                {
                    commaStr[j] = ',';
                    j++;
                }
            }
        }
        strcpy(str, commaStr);
    }
    makeInternational(str); // To swap commas and DPs if needed
}

void MakeEngineeringFormat(double val, char *Fstr)
{
    char engStr[64];
    char *sp;
    int exponent, shiftDP;

    if (decimal_places >= 2)
        sprintf(engStr, "%%- 21.%dE", decimal_places);
    else
        sprintf(engStr, "%%- 21.%dE", 2);
    sprintf(Fstr, engStr, val);
    sp = strchr(Fstr, 'E');
    sp++;
    exponent = atoi(sp);
    shiftDP = (exponent % 3);
    if (shiftDP < 0)
        shiftDP += 3;
    while (shiftDP != 0)
    {
        sp = strchr(Fstr, '.');
        *sp = *(sp + 1);
        sp++;
        *sp = '.';
        shiftDP--;
        exponent--;
    }
    sp = strchr(Fstr, 'E');
    sp++;
    sprintf(sp, "%d", exponent);
}

void FormatNumberForStack(double val, char *Fstr)
{
    char sciStr[64];
    char str[64];
    int i;

    val = MakeAccurate(val);
    if (sci_format != 'g')
    {
        if (sci_format == 'Z') // ENGINEERING FORMAT
        {
            MakeEngineeringFormat(val, Fstr);
        }
        else
        {
            sprintf(sciStr, "%%- .%d%c", decimal_places, sci_format);
            sprintf(str, sciStr, val);
            if (strlen(str) > MAX_DIGITS)
                sprintf(str, "%%- 21.%dg", decimal_places + 1);
            else
                sprintf(str, "%%- 21.%d%c", decimal_places, sci_format);
        }
    }
    else
    {
        strcpy(str, "%- 21.14g");
    }
    if (sci_format != 'Z') // ENGINEERING FORMAT
    {
        sprintf(Fstr, str, val);
        PutCommas(Fstr);
        for (i = strlen(Fstr) - 1; i > 0; i--) // Remove trailing spaces...
        {
            if (Fstr[i] == ' ')
                Fstr[i] = CNULL;
            else
                break;
        }
    }
}

void ShowStack(void)
{
    char stackStr[64];

    if (recModeON == 1) // Special record mode - show current program step in Z register!
    {
        sprintf(tmpStr, "%03d-Unknown!", currentMacroPlaybackIdx);
        if (currentMacroPlaybackIdx == 0)
        {
            sprintf(tmpStr, "%03d-<Start Of Program>", currentMacroPlaybackIdx);
        }
        else
        {
            sprintf(tmpStr, "%03d-%s", currentMacroPlaybackIdx, playBackMap[playBack[currentMacroPlaybackIdx - 1]].funcText);
        }
        tmpStr[MAX_STACK_STRLEN] = CNULL;
        SetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr);
        SetDlgItemText(calcMainWindow, RPN_STACK_Z, " ");
    }
    else if (showTrace == TRUE) // Are we showing a trace playback - repurpose Z register area
    {
        if (currentMacroPlaybackIdx == playBackIdx)
        {
            sprintf(tmpStr, "%03d-<End Of Program>", currentMacroPlaybackIdx);
        }
        else
        {
            sprintf(tmpStr, "%03d-%s", currentMacroPlaybackIdx, playBackMap[playBack[currentMacroPlaybackIdx]].funcText);
        }

        tmpStr[MAX_STACK_STRLEN] = CNULL;
        SetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr);
        SetDlgItemText(calcMainWindow, RPN_STACK_Z, " ");
    }

    if (progMode == PROG_FLOAT)
    {
        if (Xedit == X_EDIT)
        {
            sprintf(tmpStr, " %s_", Xstr);
            PutCommas(tmpStr);
        }
        else
        {
            FormatNumberForStack(STACK[STK_X], tmpStr);
        }

        if (rightAlignStack == 1)
        {
            sprintf(stackStr, (bExactFont ? "%24s" : "%22s"), tmpStr);
            SetDlgItemText(calcMainWindow, RPN_STACK_X, stackStr);
        }
        else
        {
            SetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr);
        }

        FormatNumberForStack(STACK[STK_Y], tmpStr);
        if (rightAlignStack == 1)
        {
            sprintf(stackStr, (bExactFont ? "%24s" : "%22s"), tmpStr);
            SetDlgItemText(calcMainWindow, RPN_STACK_Y, stackStr);
        }
        else
        {
            SetDlgItemText(calcMainWindow, RPN_STACK_Y, tmpStr);
        }

        if (recModeON == 0 && showTrace == FALSE)
        {
            FormatNumberForStack(STACK[STK_Z], tmpStr);
            if (rightAlignStack == 1)
            {
                sprintf(stackStr, (bExactFont ? "%24s" : "%22s"), tmpStr);
                SetDlgItemText(calcMainWindow, RPN_STACK_Z, stackStr);
            }
            else
            {
                SetDlgItemText(calcMainWindow, RPN_STACK_Z, tmpStr);
            }

            FormatNumberForStack(STACK[STK_T], tmpStr);
            if (rightAlignStack == 1)
            {
                sprintf(stackStr, (bExactFont ? "%24s" : "%22s"), tmpStr);
                SetDlgItemText(calcMainWindow, RPN_STACK_T, stackStr);
            }
            else
            {
                SetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr);
            }
        }
    }
    else
    {
        if (Xedit == X_EDIT)
        {
            STACKL[STK_X] = ConvertCompSciStrTo64(Xstr);
        }

        MakeCompSciStr(STACKL[STK_X], stackStr);
        sprintf(tmpStr, (bExactFont ? "%23s%c%c" : "%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
        SetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr);

        MakeCompSciStr(STACKL[STK_Y], stackStr);
        sprintf(tmpStr, (bExactFont ? "%23s%c%c" : "%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
        SetDlgItemText(calcMainWindow, RPN_STACK_Y, tmpStr);

        // Show Z and T registers provided we haven't repurposed them above...
        if (recModeON == 0 && traceMacroPlayback == FALSE)
        {
            MakeCompSciStr(STACKL[STK_Z], stackStr);
            sprintf(tmpStr, (bExactFont ? "%23s%c%c" : "%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
            SetDlgItemText(calcMainWindow, RPN_STACK_Z, tmpStr);

            MakeCompSciStr(STACKL[STK_T], stackStr);
            sprintf(tmpStr, (bExactFont ? "%23s%c%c" : "%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
            SetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr);
        }
    }

    if (IsWindowVisible(debugTraceWindow))
    {
        UpdateDebugRegs();

        if (recModeON == 1)
            UpdateDebugProgram(1);
        else
            UpdateDebugProgram(0);
    }
}

void StackPush(double temp)
{
    stackPushes++;

    // This checks the number for NAN and INF, etc...
    if (_finite(temp) == 0)
    {
        MessageBox(calcMainWindow, "Overflow:  Result is too large to be represented.", "Excalibur Floating Point", MB_OK);
        temp = 0.0;
        endRunningMacro();
    }
    if (_isnan(temp) != 0)
    {
        MessageBox(calcMainWindow, "Result cannot be represented (NAN).", "Excalibur Floating Point", MB_OK);
        temp = 0.0;
        endRunningMacro();
    }

    if (extendedStack)
    {
        STACK[STK_D] = STACK[STK_C];
        STACK[STK_C] = STACK[STK_B];
        STACK[STK_B] = STACK[STK_A];
        STACK[STK_A] = STACK[STK_T];
    }
    STACK[STK_T] = STACK[STK_Z];
    STACK[STK_Z] = STACK[STK_Y];
    STACK[STK_Y] = STACK[STK_X];
    STACK[STK_X] = temp;

    FloatsToLongs();
}

double StackPop(void)
{
    double temp;

    stackPops++;

    temp = STACK[STK_X];
    STACK[STK_X] = STACK[STK_Y];
    STACK[STK_Y] = STACK[STK_Z];
    STACK[STK_Z] = STACK[STK_T];
    if (extendedStack)
    {
        STACK[STK_T] = STACK[STK_A];
        STACK[STK_A] = STACK[STK_B];
        STACK[STK_B] = STACK[STK_C];
        STACK[STK_C] = STACK[STK_D];
        if (popFillZero != 0)
            STACK[STK_D] = 0.0;
    }
    else
    {
        if (popFillZero != 0)
            STACK[STK_T] = 0.0;
    }

    FloatsToLongs();
    return (temp);
}

// -----------------------
// Basic keypad keypresses
// -----------------------
void RPN_clearStack(void)
{
    // Check if we should clear all registers
    if (rpnStoreRecall & 0x03)
    {
        memset(STO, 0x00, sizeof(STO));
        memset(STOL, 0x00, sizeof(STOL));
    }
    else
    {
        // A double press of CLS in a row will clear register memory as well...
        if (lastUniqueIndex == UNI_CLRSTK)
        {
            char savedStr[64];

            memset(STO, 0x00, sizeof(STO));
            memset(STOL, 0x00, sizeof(STOL));
            memset(cashFlow, 0x00, sizeof(cashFlow));
            memset(SUM, 0x00, sizeof(SUM));
            memset(FIN, 0x00, sizeof(FIN));
            CFn = 0;

            GetDlgItemText(calcMainWindow, RPN_STACK_X, savedStr, MAX_STACK_STRLEN);
            SetDlgItemText(calcMainWindow, RPN_STACK_X, "  ...MEMORY CLEAR...  ");
            sleep_and_peek(500);
            GetDlgItemText(calcMainWindow, RPN_STACK_X, savedStr, MAX_STACK_STRLEN);
        }

        if (progMode)
        {
            RPN_clearL();
        }
        STACK[STK_X] = 0.0;
        STACK[STK_Y] = 0.0;
        STACK[STK_Z] = 0.0;
        STACK[STK_T] = 0.0;
        STACK[STK_A] = 0.0;
        STACK[STK_B] = 0.0;
        STACK[STK_C] = 0.0;
        STACK[STK_D] = 0.0;
        LASTX = 0.0;
        LASTY = 0.0;
        progModecarry = 0;
        strcpy(Xstr, "");
    }
    RPN_ClearModifiers(!macroPlayback);
}

void RPN_clearL(void)
{
    STACKL[STK_X] = 0L;
    STACKL[STK_Y] = 0L;
    STACKL[STK_Z] = 0L;
    STACKL[STK_T] = 0L;
    STACKL[STK_A] = 0L;
    STACKL[STK_B] = 0L;
    STACKL[STK_C] = 0L;
    STACKL[STK_D] = 0L;
    LASTXL = 0L;
    LASTYL = 0L;
    progModecarry = 0;
    strcpy(Xstr, "");
}

void RPN_enter(void)
{
    if (eRPN)
    {
        if (Xedit == X_EDIT)
        {
            if (progMode == PROG_FLOAT)
                StackPush(StackPop());
            else
                StackPushL(StackPopL());
        }
        else
        {
            if (progMode == PROG_FLOAT)
                StackPush(STACK[STK_X]);
            else
                StackPushL(STACKL[STK_X]);
        }
        Xedit = X_NEW;
    }
    else
    {
        if (progMode == PROG_FLOAT)
            StackPush(STACK[STK_X]);
        else
            StackPushL(STACKL[STK_X]);
        Xedit = X_ENTER;
    }

    RPN_ClearModifiers(!macroPlayback);
}

void RPN_dp(void)
{
    int i;

    if (rpnStoreRecall)
    {
        rpnStoreRecall |= REG_DP;
        UpdateSpareBar_StoreRecall();
        return;
    }

    if (progMode == PROG_FLOAT)
    {
        if (Xedit == X_NEW)
        {
            StackPush(0.0);
            strcpy(Xstr, "0.");
        }
        else if (Xedit == X_ENTER)
        {
            strcpy(Xstr, "0.");
        }
        else if ((strchr(Xstr, '.') == NULL) && (strchr(Xstr, '/') == NULL))
        {
            strcat(Xstr, ".");
        }
        else
        {
            if (strchr(Xstr, '/') == NULL)
            {
                if (Xstr[strlen(Xstr) - 1] == '.')
                    Xstr[strlen(Xstr) - 1] = '/';
                else
                {
                    for (i = 0; i < (int)strlen(Xstr); i++)
                    {
                        if (Xstr[i] == '.')
                            Xstr[i] = '~';
                    }
                    Xstr[strlen(Xstr) + 1] = (char)NULL;
                    Xstr[strlen(Xstr)] = '/';
                }
            }
        }
    }

    RPN_ClearModifiers(!macroPlayback);
    Xedit = X_EDIT;
}

void RPN_Ex(void)
{
    char *ptr;

    if (progMode == PROG_FLOAT && strchr(Xstr, 'E') == NULL)
    {
        if (Xedit == X_EDIT)
        {
            ptr = strchr(Xstr, 'e'); // Don't allow 'e' over existing 'e'
            if (ptr == NULL)
            {
                if (STACK[STK_X] == 0.0)
                {
                    strcpy(Xstr, "1e+");
                }
                else
                {
                    strcat(Xstr, "e+");
                }
            }
        }
        else
        {
            if (Xedit == X_NEW)
                StackPush(1.0);
            strcpy(Xstr, "1e+");
            Xedit = X_EDIT;
        }
    }
}

void RPN_digit(WPARAM key)
{
    double tmp1, tmp2, tmp3;

    // ---------------------------------------------------------
    // Handle Store and Recall of Registers. This also handles
    // the possibility of Store and Recall Arithmetic.
    // ---------------------------------------------------------
    if (rpnStoreRecall)
    {
        uint8_t reg = (key - RPN_DIGIT_0) + (rpnStoreRecall & REG_DP ? 10 : 0);

        if (rpnStoreRecall & REG_STORE)
        {
            if (Xedit == X_EDIT)
                Xedit = X_NEW;

            // See if we are doing any sort of STO Arithmetic...
            if (rpnStoreRecall & REG_PLUS)
            {
                if (progMode == PROG_FLOAT)
                    STO[reg] += STACK[STK_X];
                else
                    STOL[reg] += STACKL[STK_X];
            }
            else if (rpnStoreRecall & REG_MINUS)
            {
                if (progMode == PROG_FLOAT)
                    STO[reg] -= STACK[STK_X];
                else
                    STOL[reg] -= STACKL[STK_X];
            }
            else if (rpnStoreRecall & REG_MULTIPLY)
            {
                if (progMode == PROG_FLOAT)
                    STO[reg] *= STACK[STK_X];
                else
                    STOL[reg] *= STACKL[STK_X];
            }
            else if (rpnStoreRecall & REG_DIVIDE)
            {
                if (progMode == PROG_FLOAT)
                {
                    if (STACK[STK_X] != 0.0)
                    {
                        STO[reg] /= STACK[STK_X];
                    }
                }
                else
                {
                    if (STACKL[STK_X] != 0L)
                    {
                        STOL[reg] /= STACKL[STK_X];
                    }
                }
            }
            else
            {
                if (progMode == PROG_FLOAT)
                    STO[reg] = STACK[STK_X];
                else
                    STOL[reg] = STACKL[STK_X];
            }

            blinkXDisplay(FALSE);
        }
        else if (rpnStoreRecall & REG_RECALL)
        {
            // See if we are doing any sort of RCL Arithmetic...
            // In this case the stack does NOT lift.
            if (rpnStoreRecall & REG_PLUS)
            {
                if (progMode == PROG_FLOAT)
                    STACK[STK_X] = STACK[STK_X] + STO[reg];
                else
                    STACKL[STK_X] = STACKL[STK_X] + STOL[reg];
            }
            else if (rpnStoreRecall & REG_MINUS)
            {
                if (progMode == PROG_FLOAT)
                    STACK[STK_X] = STACK[STK_X] - STO[reg];
                else
                    STACKL[STK_X] = STACKL[STK_X] - STOL[reg];
            }
            else if (rpnStoreRecall & REG_MULTIPLY)
            {
                if (progMode == PROG_FLOAT)
                    STACK[STK_X] = STACK[STK_X] * STO[reg];
                else
                    STACKL[STK_X] = STACKL[STK_X] * STOL[reg];
            }
            else if (rpnStoreRecall & REG_DIVIDE)
            {
                if (progMode == PROG_FLOAT)
                {
                    if (STO[reg] != 0.0) STACK[STK_X] = STACK[STK_X] / STO[reg];
                }
                else
                {
                    if (STOL[reg] != 0L) STACKL[STK_X] = STACKL[STK_X] / STOL[reg];
                }
            }
            else // Normal RCL will lift the stack
            {
                if (Xedit == X_EDIT)
                {
                    RPN_enter();
                }
                else if (Xedit == X_NEW)
                {
                    if (progMode == PROG_FLOAT)
                        StackPush(0.0);
                    else
                        StackPushL(0L);
                }
                else
                    Xedit = X_NEW;

                if (progMode == PROG_FLOAT)
                    STACK[STK_X] = STO[reg];
                else
                    STACKL[STK_X] = STOL[reg];
            }
        }
        else if (rpnStoreRecall & REG_EXCHANGE)
        {
            if (progMode == PROG_FLOAT)
            {
                double exch = STACK[STK_X];
                STACK[STK_X] = STO[reg];
                STO[reg] = exch;
            }
            else
            {
                PROG_LONG exch = STACKL[STK_X];
                STACKL[STK_X] = STOL[reg];
                STOL[reg] = exch;
            }

            if (Xedit == X_EDIT)
                Xedit = X_NEW;
        }

        STACKL[STK_X] = MaskStack(STACKL[STK_X]);
        RPN_ClearModifiers(!macroPlayback);
        return;
    }

    if (progMode == PROG_BIN && key > RPN_DIGIT_1)
        return;
    if (progMode == PROG_OCT && key > RPN_DIGIT_7)
        return;

    if (Xedit == X_NEW)
    {
        if (progMode != PROG_FLOAT)
            StackPushL(0L);
        else
            StackPush(0.0);

        Xstr[0] = '0' + (key - RPN_DIGIT_0);
        Xstr[1] = CNULL;
    }
    else if (Xedit == X_ENTER)
    {
        Xstr[0] = '0' + (key - RPN_DIGIT_0);
        Xstr[1] = CNULL;
    }
    else if (Xedit == X_EDIT)
    {
        if (allowDigitBasedOnMaxStringSize(Xstr, (char)('0' + (key - RPN_DIGIT_0))))
        {
            int len = strlen(Xstr);
            Xstr[len] = '0' + (key - RPN_DIGIT_0);
            Xstr[len + 1] = CNULL;
        }
    }

    if (strchr(Xstr, '/') == NULL)
    {
        STACK[STK_X] = atof(Xstr);
    }
    else
    {
        // This handles the .. to allow fractions to be entered!
        if (strchr(Xstr, '~') == NULL)
        {
            tmp1 = atof(Xstr);
            tmp2 = atof(strchr(Xstr, '/') + 1);
            tmp3 = 0.0;
        }
        else
        {
            tmp3 = atof(Xstr);
            tmp1 = atof(strchr(Xstr, '~') + 1);
            tmp2 = atof(strchr(Xstr, '/') + 1);
        }

        if (tmp2 != 0.0)
            STACK[STK_X] = tmp3 + (tmp1 / tmp2);
        else
            STACK[STK_X] = 0.0;
    }
    
    STACKL[STK_X] = ConvertCompSciStrTo64(Xstr);
    RPN_ClearModifiers(!macroPlayback);
    Xedit = X_EDIT;
}

int allowDigitBasedOnMaxStringSize(char *Xstr, char digit)
{
    int status = FALSE;
    int maxDigits;
    char temp[64];
    char *tmpPtr;
    int i, significantDigits;

    maxDigits = 14;
    significantDigits = 0;
    for (i = 0; i < (int)strlen(Xstr); i++)
    {
        if ((Xstr[i] >= '0') && (Xstr[i] <= '9'))
        {
            significantDigits++;
        }
        if ((Xstr[i] == 'e') || (Xstr[i] == 'E'))
        {
            maxDigits = 15;
        }
    }

    if (progMode == PROG_FLOAT)
    {
        if (significantDigits < maxDigits)
            status = TRUE;
    }
    else
    {
        if (progMode == PROG_BIN)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtou64(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int)strlen(Xstr) >= 32)
                status = FALSE;
        }
        else if (progMode == PROG_DEC)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtou64(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int)strlen(Xstr) >= 20)
                status = FALSE;
        }
        else if (progMode == PROG_HEX)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtou64(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int)strlen(Xstr) >= 16)
                status = FALSE;
        }
        else if (progMode == PROG_OCT)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtou64(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int)strlen(Xstr) >= 22)
                status = FALSE;
        }
    }

    return status;
}

extern BOOL CALLBACK fnDIALOG_DisplayModeProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void RPN_mode2(HWND hDlg)
{
    DLGPROC lpfnDIALOG_DisplayModeProc;

    lpfnDIALOG_DisplayModeProc = (DLGPROC)MakeProcInstance((FARPROC)fnDIALOG_DisplayModeProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_DISPLAY_MODE", hDlg, lpfnDIALOG_DisplayModeProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpfnDIALOG_DisplayModeProc);
    ShowStatus();
}

void RPN_mode(void)
{
    DLGPROC lpfnDIALOG_DisplayModeProc;

    lpfnDIALOG_DisplayModeProc = (DLGPROC)MakeProcInstance((FARPROC)fnDIALOG_DisplayModeProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_DISPLAY_MODE", calcMainWindow, lpfnDIALOG_DisplayModeProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpfnDIALOG_DisplayModeProc);
    ShowStatus();
}

BOOL CALLBACK fnDIALOG_DisplayModeProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch (wMessage)
    {
    case WM_INITDIALOG:
        if (decimal_places == 0)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO0), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 1)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 2)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 3)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 4)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO4), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 5)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO5), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 6)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO6), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 7)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO7), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 8)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO8), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 9)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO9), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 10)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO10), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 11)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO11), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else if (decimal_places == 12)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO12), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, IDC_RADIO13), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (sci_format == 'g')
            SendMessage(GetDlgItem(hDlg, 101), BM_SETCHECK, (WORD)1, (DWORD)0L);
        if (sci_format == 'E')
            SendMessage(GetDlgItem(hDlg, 102), BM_SETCHECK, (WORD)1, (DWORD)0L);
        if (sci_format == 'f')
            SendMessage(GetDlgItem(hDlg, 103), BM_SETCHECK, (WORD)1, (DWORD)0L);
        if (sci_format == 'Z')
            SendMessage(GetDlgItem(hDlg, 104), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (rightAlignStack == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_SETCHECK, (WORD)1, (DWORD)0L);

        return TRUE;

    case WM_COMMAND:
        switch (wParam)
        {
        case (108): // OK was pressed
            if (SendMessage(GetDlgItem(hDlg, 101), BM_GETCHECK, 0, 0L))
            {
                sci_format = 'g';
            }
            if (SendMessage(GetDlgItem(hDlg, 102), BM_GETCHECK, 0, 0L))
            {
                sci_format = 'E';
            }
            if (SendMessage(GetDlgItem(hDlg, 103), BM_GETCHECK, 0, 0L))
            {
                sci_format = 'f';
            }
            if (SendMessage(GetDlgItem(hDlg, 104), BM_GETCHECK, 0, 0L))
            {
                sci_format = 'Z';
            }

            if (SendMessage(GetDlgItem(hDlg, IDC_RADIO0), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 0;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 1;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 2;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO3), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 3;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO4), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 4;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO5), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 5;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO6), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 6;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO7), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 7;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO8), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 8;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO9), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 9;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO10), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 10;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO11), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 11;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO12), BM_GETCHECK, (WORD)0, (DWORD)0L) != 0)
                decimal_places = 12;
            else
                decimal_places = 13;

            if (SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_GETCHECK, (WORD)0, (DWORD)0L))
                rightAlignStack = 1;
            else
                rightAlignStack = 0;

            EndDialog(hDlg, FALSE);
            return TRUE;
        case (109): // Cancel was pressed
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

void RPN_backspace(void)
{
    if (Xedit == X_EDIT)
    {
        if (strlen(Xstr) > 0)
        {
            Xstr[strlen(Xstr) - 1] = CNULL;
            STACK[STK_X] = atof(Xstr);
        }
        else
        {
            STACK[STK_X] = 0.0;
            STACKL[STK_X] = 0;
        }
    }
    else
    {
        RPN_clearX();
    }
}

void RPN_plus(void)
{
    PROG_LONG xl, yl;

    Xedit = X_NEW;

    if (rpnStoreRecall & 0x03)
    {
        rpnStoreRecall &= 0x0F;
        rpnStoreRecall |= REG_PLUS;
        UpdateSpareBar_StoreRecall();
        return;
    }

    if (progMode == PROG_FLOAT)
    {
        StackPush(StackPop() + StackPop());
    }
    else
    {
        xl = StackPopL();
        yl = StackPopL();
        StackPushL(xl + yl);
    }
}

void RPN_multiply(void)
{
    PROG_LONG xl, yl;

    Xedit = X_NEW;

    if (rpnStoreRecall & 0x03)
    {
        rpnStoreRecall &= 0x0F;
        rpnStoreRecall |= REG_MULTIPLY;
        UpdateSpareBar_StoreRecall();
        return;
    }

    if (progMode == PROG_FLOAT)
        StackPush(StackPop() * StackPop());
    else
    {
        xl = StackPopL();
        yl = StackPopL();
        StackPushL(xl * yl);
    }
}

void RPN_divide(void)
{
    double x, y;
    PROG_LONG xl, yl;
    PROG_SIGNEDLONG sxl, syl;

    Xedit = X_NEW;

    if (rpnStoreRecall & 0x03)
    {
        rpnStoreRecall &= 0x0F;
        rpnStoreRecall |= REG_DIVIDE;
        UpdateSpareBar_StoreRecall();
        return;
    }

    if (STACK[STK_X] == 0.0)
    {
        RPN_error("Divide By Zero");
    }
    else
    {
        if (progMode == PROG_FLOAT)
        {
            x = StackPop();
            y = StackPop();
            StackPush(y / x);
        }
        else
        {
            if (wordMode == COMPSCI_SIGNED)
            {
                sxl = (PROG_SIGNEDLONG)StackPopL();
                syl = (PROG_SIGNEDLONG)StackPopL();
                StackPushL(syl / sxl);
            }
            else
            {
                xl = StackPopL();
                yl = StackPopL();
                StackPushL(yl / xl);
            }
        }
    }
}

void RPN_minus(void)
{
    double x, y;
    PROG_LONG xl, yl;

    Xedit = X_NEW;

    if (rpnStoreRecall & 0x03)
    {
        rpnStoreRecall &= 0x0F;
        rpnStoreRecall |= REG_MINUS;
        UpdateSpareBar_StoreRecall();
        return;
    }

    if (progMode == PROG_FLOAT)
    {
        x = StackPop();
        y = StackPop();
        StackPush(y - x);
    }
    else
    {
        xl = StackPopL();
        yl = StackPopL();
        StackPushL(yl - xl);
    }
}

void RPN_exchange_x_y(void)
{
    double x, y;
    PROG_LONG xl, yl;

    Xedit = X_NEW;
    if (progMode == PROG_FLOAT)
    {
        x = StackPop();
        y = StackPop();
        StackPush(x);
        StackPush(y);
    }
    else
    {
        xl = StackPopL();
        yl = StackPopL();
        StackPushL(xl);
        StackPushL(yl);
    }
}

void RPN_negate_x(void)
{
    char *ptr;

    ptr = strchr(Xstr, 'e');
    if ((ptr != NULL) && !(Xedit == X_NEW || Xedit == X_ENTER))
    {
        ptr++;
        if (*ptr == '+')
            *ptr = '-';
        else
            *ptr = '+';
        STACK[STK_X] = atof(Xstr);
    }
    else
    {
        if ((Xedit == X_EDIT) && (progMode == PROG_FLOAT))
        {
            if (Xstr[0] == '-')
                Xstr[0] = '+';
            else if (Xstr[0] == '+')
                Xstr[0] = '-';
            else
            {
                sprintf(tmpStr, "-%s", Xstr);
                strcpy(Xstr, tmpStr);
            }
            STACK[STK_X] = atof(Xstr);
        }
        else
        {
            Xedit = X_NEW;
            if (progMode == PROG_FLOAT)
                StackPush(StackPop() * -1.0);
            else
                StackPushL(StackPopL() * -1L);
        }
    }
}

void RPN_rotateStackUp(void)
{
    double temp;
    PROG_LONG tempL;

    Xedit = X_NEW;
    if (progMode == PROG_FLOAT)
    {
        if (extendedStack)
        {
            temp = STACK[STK_D];
            STACK[STK_D] = STACK[STK_C];
            STACK[STK_C] = STACK[STK_B];
            STACK[STK_B] = STACK[STK_A];
            STACK[STK_A] = STACK[STK_T];
            STACK[STK_T] = STACK[STK_Z];
            STACK[STK_Z] = STACK[STK_Y];
            STACK[STK_Y] = STACK[STK_X];
            STACK[STK_X] = temp;
        }
        else
        {
            temp = STACK[STK_T];
            STACK[STK_T] = STACK[STK_Z];
            STACK[STK_Z] = STACK[STK_Y];
            STACK[STK_Y] = STACK[STK_X];
            STACK[STK_X] = temp;
        }
    }
    else
    {
        if (extendedStack)
        {
            tempL = STACKL[STK_D];
            STACKL[STK_D] = STACKL[STK_C];
            STACKL[STK_C] = STACKL[STK_B];
            STACKL[STK_B] = STACKL[STK_A];
            STACKL[STK_A] = STACKL[STK_T];
            STACKL[STK_T] = STACKL[STK_Z];
            STACKL[STK_Z] = STACKL[STK_Y];
            STACKL[STK_Y] = STACKL[STK_X];
            STACKL[STK_X] = tempL;
        }
        else
        {
            tempL = STACKL[STK_T];
            STACKL[STK_T] = STACKL[STK_Z];
            STACKL[STK_Z] = STACKL[STK_Y];
            STACKL[STK_Y] = STACKL[STK_X];
            STACKL[STK_X] = tempL;
        }
    }
}

void RPN_rotateStackDn(void)
{
    double temp;
    PROG_LONG tempL;

    Xedit = X_NEW;
    if (progMode == PROG_FLOAT)
    {
        if (extendedStack)
        {
            temp = STACK[STK_X];
            STACK[STK_X] = STACK[STK_Y];
            STACK[STK_Y] = STACK[STK_Z];
            STACK[STK_Z] = STACK[STK_T];
            STACK[STK_T] = STACK[STK_A];
            STACK[STK_A] = STACK[STK_B];
            STACK[STK_B] = STACK[STK_C];
            STACK[STK_C] = STACK[STK_D];            
            STACK[STK_D] = temp;
        }
        else
        {
            temp = STACK[STK_X];
            STACK[STK_X] = STACK[STK_Y];
            STACK[STK_Y] = STACK[STK_Z];
            STACK[STK_Z] = STACK[STK_T];
            STACK[STK_T] = temp;
        }
    }
    else
    {
        if (extendedStack)
        {
            tempL = STACKL[STK_X];
            STACKL[STK_X] = STACKL[STK_Y];
            STACKL[STK_Y] = STACKL[STK_Z];
            STACKL[STK_Z] = STACKL[STK_T];
            STACKL[STK_T] = STACKL[STK_A];
            STACKL[STK_A] = STACKL[STK_B];
            STACKL[STK_B] = STACKL[STK_C];
            STACKL[STK_C] = STACKL[STK_D];            
            STACKL[STK_D] = tempL;
        }
        else
        {
            tempL = STACKL[STK_X];
            STACKL[STK_X] = STACKL[STK_Y];
            STACKL[STK_Y] = STACKL[STK_Z];
            STACKL[STK_Z] = STACKL[STK_T];
            STACKL[STK_T] = tempL;
        }
    }
}

void RPN_lastX(void)
{
    Xedit = X_NEW;
    if (progMode == PROG_FLOAT)
        StackPush(LASTX);
    else
        StackPushL(LASTXL);
}

// --------------
// Misc functions
// --------------

double ToRadians(double t)
{
    double temp;

    if (AngleMode == 0)
        temp = t / (180.0 / M_PI);
    else if (AngleMode == 2)
        temp = t / ((180.0 / M_PI) * (400.0 / 360.0));
    else
        temp = t;

    return (temp);
}

double FromRadians(double t)
{
    double temp;

    if (AngleMode == 0)
        temp = t * (180.0 / M_PI);
    else if (AngleMode == 2)
        temp = t * ((180.0 / M_PI) * (400.0 / 360.0));
    else
        temp = t;

    return (temp);
}

// -------------------------
// STOre and ReCaL functions
// -------------------------

void RPN_store(void)
{
    rpnStoreRecall &= ~REG_EXCHANGE;
    if (rpnStoreRecall & REG_STORE)
    {
        if (!macroPlayback)
            UpdateSpareBar(" ");
        rpnStoreRecall = 0x00;
    }
    else
    {
        rpnStoreRecall ^= REG_STORE;
        UpdateSpareBar_StoreRecall();
    }
}

void RPN_recall(void)
{
    rpnStoreRecall &= ~REG_EXCHANGE;
    if (rpnStoreRecall & REG_RECALL)
    {
        if (!macroPlayback)
            UpdateSpareBar(" ");
        rpnStoreRecall = 0x00;
    }
    else
    {
        rpnStoreRecall ^= REG_RECALL;
        UpdateSpareBar_StoreRecall();
    }
}

void RPN_ExchangeReg(void)
{
    rpnStoreRecall &= ~(REG_STORE | REG_RECALL);
    if (rpnStoreRecall & REG_EXCHANGE)
    {
        if (!macroPlayback)
            UpdateSpareBar(" ");
        rpnStoreRecall = 0x00;
    }
    else
    {
        rpnStoreRecall ^= REG_EXCHANGE;
        UpdateSpareBar_StoreRecall();
    }
}

// ------------------------------------------------------------------------
//                           SAVE STUFF TO DISK
// ------------------------------------------------------------------------
int GetMenuType(struct funcStruct *cFunc)
{
    int retVal = 1;

    if (cFunc == (struct funcStruct *)&Scientific_funcs)
        retVal = 1;
    if (cFunc == (struct funcStruct *)&Financial_funcs)
        retVal = 2;
    if (cFunc == (struct funcStruct *)&CompSci_funcs)
        retVal = 3;
    if (cFunc == (struct funcStruct *)&Conversion_funcs)
        retVal = 4;
    if (cFunc == (struct funcStruct *)&Program1_funcs)
        retVal = 5;
    if (cFunc == (struct funcStruct *)&Program2_funcs)
        retVal = 6;
    if (cFunc == (struct funcStruct *)&Statistics_funcs)
        retVal = 7;
    if (cFunc == (struct funcStruct *)&Scientific2_funcs)
        retVal = 8;
    if (cFunc == (struct funcStruct *)&Custom_funcs)
        retVal = 9;

    return (retVal);
}

void SetMenuType(int type)
{
    switch (type)
    {
    case (1):
        currentFuncs = (struct funcStruct *)&Scientific_funcs;
        CheckMenuItem(hMainMenu, IDM_SCIENTIFIC, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_SCI), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_SCI_I);
        break;
    case (2):
        currentFuncs = (struct funcStruct *)&Financial_funcs;
        CheckMenuItem(hMainMenu, IDM_FINANCIAL, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_FIN), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_BUIS);
        break;
    case (3):
        currentFuncs = (struct funcStruct *)&CompSci_funcs;
        CheckMenuItem(hMainMenu, IDM_COMPSCI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_COMPSCI), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_COMPSCI);
        break;
    case (4):
        currentFuncs = (struct funcStruct *)&Conversion_funcs;
        CheckMenuItem(hMainMenu, IDM_CONVERSION, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CONV), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CONV);
        break;
    case (5):
        currentFuncs = (struct funcStruct *)&Program1_funcs;
        CheckMenuItem(hMainMenu, IDM_PROGI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGI), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG1);
        break;
    case (6):
        currentFuncs = (struct funcStruct *)&Program2_funcs;
        CheckMenuItem(hMainMenu, IDM_PROGII, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGII), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG2);
        break;
    case (7):
        currentFuncs = (struct funcStruct *)&Statistics_funcs;
        CheckMenuItem(hMainMenu, IDM_STATS, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_STAT), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_STATS);
        break;
    case (8):
        currentFuncs = (struct funcStruct *)&Scientific2_funcs;
        CheckMenuItem(hMainMenu, IDM_SCIENTIFIC2, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_SCI2), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_SCI2);
        break;
    case (9):
        currentFuncs = (struct funcStruct *)&Custom_funcs;
        CheckMenuItem(hMainMenu, IDM_CUSTOM, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CUST), BM_SETCHECK, (WORD)1, (DWORD)0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CUSTOM);
        break;
    }
}

void SetLastMenuType(int type)
{
    switch (type)
    {
    case (1):
        lastFuncs = (struct funcStruct *)&Scientific_funcs;
        break;
    case (2):
        lastFuncs = (struct funcStruct *)&Financial_funcs;
        break;
    case (3):
        lastFuncs = (struct funcStruct *)&CompSci_funcs;
        break;
    case (4):
        lastFuncs = (struct funcStruct *)&Conversion_funcs;
        break;
    case (5):
        lastFuncs = (struct funcStruct *)&Program1_funcs;
        break;
    case (6):
        lastFuncs = (struct funcStruct *)&Program2_funcs;
        break;
    case (7):
        lastFuncs = (struct funcStruct *)&Statistics_funcs;
        break;
    case (8):
        lastFuncs = (struct funcStruct *)&Scientific2_funcs;
        break;
    case (9):
        lastFuncs = (struct funcStruct *)&Custom_funcs;
        break;
    }
}

#define RESERVED_SIZE 1024
#define CSIDL_LOCAL_APPDATA 0x001c
char reserved[RESERVED_SIZE];

char *GetConfigurationDirectory(void)
{
    static TCHAR Buffer[MAX_PATH]; // buffer for concatenated string
    TCHAR szPath[MAX_PATH];
    LPITEMIDLIST pidl = NULL;
    LPTSTR lpszSystemInfo; // pointer to system information string

    if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl)))
    {
        if (SHGetPathFromIDList(pidl, szPath)) // Use the applications directory...
        {
            sprintf(Buffer, "%s\\Excal32.cfg", szPath);
            CoTaskMemFree(pidl);
        }
        else // ... but if that fails, use the standard system directory...
        {
            lpszSystemInfo = Buffer;
            GetWindowsDirectory(lpszSystemInfo, MAX_PATH);
            strcat(Buffer, "\\Excal32.cfg");
        }
    }
    else
    {
        lpszSystemInfo = Buffer;
        GetWindowsDirectory(lpszSystemInfo, MAX_PATH);
        strcat(Buffer, "\\Excal32.cfg");
    }

    return Buffer;
}

void SaveToDisk(void)
{
    FILE *outfile;
    uint8_t menuCurrentFuncs, menuLastFuncs;
    uint16_t configVersionMain, configVersionSub;

    outfile = fopen(GetConfigurationDirectory(), "wb+");

    if (ClearStackOnExit)
    {
        lastUniqueIndex = UNI_CLRSTK; // Force register clear as well
        RPN_clearStack();
        RPN_clearL();
    }

    memset(reserved, 0, RESERVED_SIZE);

    configVersionMain = CONFIG_VERSION_MAIN;
    configVersionSub = CONFIG_VERSION_SUB;
    menuCurrentFuncs = GetMenuType(currentFuncs);
    menuLastFuncs = GetMenuType(lastFuncs);

    if (outfile)
    {
        fwrite(&configVersionMain,  sizeof(configVersionMain),  1, outfile);
        fwrite(&configVersionSub,   sizeof(configVersionSub),   1, outfile);

        fwrite(&main_x,             sizeof(main_x),             1, outfile);
        fwrite(&main_y,             sizeof(main_y),             1, outfile);
        fwrite(&main_cx,            sizeof(main_cx),            1, outfile);
        fwrite(&main_cy,            sizeof(main_cy),            1, outfile);

        fwrite(&menuCurrentFuncs,   sizeof(menuCurrentFuncs),   1, outfile);
        fwrite(&menuLastFuncs,      sizeof(menuLastFuncs),      1, outfile);
        fwrite(&progMode,           sizeof(progMode),           1, outfile);
        fwrite(&alwaysOnTop,        sizeof(alwaysOnTop),        1, outfile);
        fwrite(&decimal_places,     sizeof(decimal_places),     1, outfile);
        fwrite(&sci_format,         sizeof(sci_format),         1, outfile);
        fwrite(&numberDisplayMode,  sizeof(numberDisplayMode),  1, outfile);
        fwrite(&lastProgMode,       sizeof(lastProgMode),       1, outfile);
        fwrite(&padZeros,           sizeof(padZeros),           1, outfile);
        fwrite(&wordSize,           sizeof(wordSize),           1, outfile);
        fwrite(&wordMode,           sizeof(wordMode),           1, outfile);
        fwrite(&hexSpacing,         sizeof(hexSpacing),         1, outfile);
        fwrite(&wordSizeMask,       sizeof(wordSizeMask),       1, outfile);

        fwrite(STACK,               sizeof(STACK),              1, outfile);
        fwrite(STACKL,              sizeof(STACKL),             1, outfile);
        fwrite(&LASTX,              sizeof(LASTX),              1, outfile);
        fwrite(&LASTY,              sizeof(LASTY),              1, outfile);
        fwrite(&lastFloat,          sizeof(lastFloat),          1, outfile);
        fwrite(&LASTXL,             sizeof(LASTXL),             1, outfile);
        fwrite(&LASTYL,             sizeof(LASTYL),             1, outfile);

        fwrite(&STO,                sizeof(STO),                1, outfile);
        fwrite(&STOL,               sizeof(STOL),               1, outfile);
        fwrite(&SUM,                sizeof(SUM),                1, outfile);
        fwrite(&FIN,                sizeof(FIN),                1, outfile);
        fwrite(&cashFlow,           sizeof(cashFlow),           1, outfile);
        fwrite(&CFn,                sizeof(CFn),                1, outfile);

        fwrite(&AngleMode,          sizeof(AngleMode),          1, outfile);
        fwrite(&taxConstant,        sizeof(taxConstant),        1, outfile);
        fwrite(&commaMode,          sizeof(commaMode),          1, outfile);
        fwrite(&eexMode,            sizeof(eexMode),            1, outfile);
        fwrite(&numLockMode,        sizeof(numLockMode),        1, outfile);
        fwrite(&toolTipsOn,         sizeof(toolTipsOn),         1, outfile);
        fwrite(&payMode,            sizeof(payMode),            1, outfile);
        fwrite(&dateMode,           sizeof(dateMode),           1, outfile);
        fwrite(&depreciationType,   sizeof(depreciationType),   1, outfile);
        fwrite(&stackPushes,        sizeof(stackPushes),        1, outfile);
        fwrite(&stackPops,          sizeof(stackPops),          1, outfile);
        fwrite(&inFocusTime,        sizeof(inFocusTime),        1, outfile);
        fwrite(customSave,          sizeof(customSave),         1, outfile);
        fwrite(&extendedStack,      sizeof(extendedStack),      1, outfile);
        fwrite(&footPrint,          sizeof(footPrint),          1, outfile);
        fwrite(&popFillZero,        sizeof(popFillZero),        1, outfile);
        fwrite(&rightAlignStack,    sizeof(rightAlignStack),    1, outfile);
        fwrite(&showXMinimized,     sizeof(showXMinimized),     1, outfile);
        fwrite(&eRPN,               sizeof(eRPN),               1, outfile);
        fwrite(&ClearStackOnExit,   sizeof(ClearStackOnExit),   1, outfile);
        fwrite(&reservedOpt1,       sizeof(reservedOpt1),       1, outfile);
        fwrite(&reservedOpt2,       sizeof(reservedOpt2),       1, outfile);
        fwrite(&reservedOpt3,       sizeof(reservedOpt3),       1, outfile);

        fwrite(&playBack,           sizeof(playBack),           1, outfile);
        fwrite(&playBackSave,       sizeof(playBackSave),       1, outfile);
        fwrite(&playBackIdx,        sizeof(playBackIdx),        1, outfile);
        fwrite(&playBackIdxSave,    sizeof(playBackIdxSave),    1, outfile);
        fwrite(&macroName,          sizeof(macroName),          1, outfile);
        fwrite(&macro_short_names,  sizeof(macro_short_names),  1, outfile);

        fwrite(&currency1index,     sizeof(currency1index),     1, outfile);
        fwrite(&currency2index,     sizeof(currency2index),     1, outfile);

        fwrite(&constants,          sizeof(constants),          1, outfile);
        fwrite(&constantBankNames,  sizeof(constantBankNames),  1, outfile);
        fwrite(&CurrencyConv,       sizeof(CurrencyConv),       1, outfile);

        fwrite(&lastChosenConst,    sizeof(lastChosenConst),    1, outfile);
        fwrite(&lastConstBank,      sizeof(lastConstBank),      1, outfile);
        fwrite(&excaliburNotes,     sizeof(excaliburNotes),     1, outfile);
        fwrite(&lastChosenMacro,    sizeof(lastChosenMacro),    1, outfile);
        fwrite(&traceDelayValueMs,  sizeof(traceDelayValueMs),  1, outfile);
        fwrite(&indirectRegister,   sizeof(indirectRegister),   1, outfile);

        fwrite(&reserved, RESERVED_SIZE, 1, outfile);

        fclose(outfile);
    }
    else
    {
        // For now we are silent on not being able to save a config file... it's no real harm and maybe something about the user's system prevents it.
        // MessageBox(NULL, TEXT("Unable to save Excalibur Configuration."), "Excalibur 32-bit", MB_ICONERROR);
    }
}

void ReadFromDisk(void)
{
    FILE *infile;
    uint8_t menuCurrentFuncs, menuLastFuncs;
    uint16_t configVersionMain, configVersionSub;

    infile = fopen(GetConfigurationDirectory(), "rb");
    if (infile != NULL)
    {
        fread(&configVersionMain,   sizeof(configVersionMain),  1, infile);
        fread(&configVersionSub,    sizeof(configVersionSub),   1, infile);

        // --------------------------------------------------------------
        // If main version has changed, we wipe config with defaults...
        // --------------------------------------------------------------
        if (configVersionMain != CONFIG_VERSION_MAIN)
        {
            main_x = -1;
            SetMenuType(1);
            SaveToDisk();
            ProcessCustomSave();
            return;
        }

        fread(&main_x,             sizeof(main_x),             1, infile);
        fread(&main_y,             sizeof(main_y),             1, infile);
        fread(&main_cx,            sizeof(main_cx),            1, infile);
        fread(&main_cy,            sizeof(main_cy),            1, infile);

        fread(&menuCurrentFuncs,   sizeof(menuCurrentFuncs),   1, infile);
        fread(&menuLastFuncs,      sizeof(menuLastFuncs),      1, infile);
        fread(&progMode,           sizeof(progMode),           1, infile);
        fread(&alwaysOnTop,        sizeof(alwaysOnTop),        1, infile);
        fread(&decimal_places,     sizeof(decimal_places),     1, infile);
        fread(&sci_format,         sizeof(sci_format),         1, infile);
        fread(&numberDisplayMode,  sizeof(numberDisplayMode),  1, infile);
        fread(&lastProgMode,       sizeof(lastProgMode),       1, infile);
        fread(&padZeros,           sizeof(padZeros),           1, infile);
        fread(&wordSize,           sizeof(wordSize),           1, infile);
        fread(&wordMode,           sizeof(wordMode),           1, infile);
        fread(&hexSpacing,         sizeof(hexSpacing),         1, infile);
        fread(&wordSizeMask,       sizeof(wordSizeMask),       1, infile);

        fread(STACK,               sizeof(STACK),              1, infile);
        fread(STACKL,              sizeof(STACKL),             1, infile);
        fread(&LASTX,              sizeof(LASTX),              1, infile);
        fread(&LASTY,              sizeof(LASTY),              1, infile);
        fread(&lastFloat,          sizeof(lastFloat),          1, infile);
        fread(&LASTXL,             sizeof(LASTXL),             1, infile);
        fread(&LASTYL,             sizeof(LASTYL),             1, infile);

        fread(&STO,                sizeof(STO),                1, infile);
        fread(&STOL,               sizeof(STOL),               1, infile);
        fread(&SUM,                sizeof(SUM),                1, infile);
        fread(&FIN,                sizeof(FIN),                1, infile);
        fread(&cashFlow,           sizeof(cashFlow),           1, infile);
        fread(&CFn,                sizeof(CFn),                1, infile);

        fread(&AngleMode,          sizeof(AngleMode),          1, infile);
        fread(&taxConstant,        sizeof(taxConstant),        1, infile);
        fread(&commaMode,          sizeof(commaMode),          1, infile);
        fread(&eexMode,            sizeof(eexMode),            1, infile);
        fread(&numLockMode,        sizeof(numLockMode),        1, infile);
        fread(&toolTipsOn,         sizeof(toolTipsOn),         1, infile);
        fread(&payMode,            sizeof(payMode),            1, infile);
        fread(&dateMode,           sizeof(dateMode),           1, infile);
        fread(&depreciationType,   sizeof(depreciationType),   1, infile);
        fread(&stackPushes,        sizeof(stackPushes),        1, infile);
        fread(&stackPops,          sizeof(stackPops),          1, infile);
        fread(&inFocusTime,        sizeof(inFocusTime),        1, infile);
        fread(customSave,          sizeof(customSave),         1, infile);
        fread(&extendedStack,      sizeof(extendedStack),      1, infile);
        fread(&footPrint,          sizeof(footPrint),          1, infile);
        fread(&popFillZero,        sizeof(popFillZero),        1, infile);
        fread(&rightAlignStack,    sizeof(rightAlignStack),    1, infile);
        fread(&showXMinimized,     sizeof(showXMinimized),     1, infile);
        fread(&eRPN,               sizeof(eRPN),               1, infile);
        fread(&ClearStackOnExit,   sizeof(ClearStackOnExit),   1, infile);
        fread(&reservedOpt1,       sizeof(reservedOpt1),       1, infile);
        fread(&reservedOpt2,       sizeof(reservedOpt2),       1, infile);
        fread(&reservedOpt3,       sizeof(reservedOpt3),       1, infile);

        fread(&playBack,           sizeof(playBack),           1, infile);
        fread(&playBackSave,       sizeof(playBackSave),       1, infile);
        fread(&playBackIdx,        sizeof(playBackIdx),        1, infile);
        fread(&playBackIdxSave,    sizeof(playBackIdxSave),    1, infile);
        fread(&macroName,          sizeof(macroName),          1, infile);
        fread(&macro_short_names,  sizeof(macro_short_names),  1, infile);

        fread(&currency1index,     sizeof(currency1index),     1, infile);
        fread(&currency2index,     sizeof(currency2index),     1, infile);

        if (configVersionSub != CONFIG_VERSION_SUB) // Skip these if sub-ver changed
        {
            fseek(infile, sizeof(constants), SEEK_CUR);
            fseek(infile, sizeof(constantBankNames), SEEK_CUR);
            fseek(infile, sizeof(CurrencyConv), SEEK_CUR);
        }
        else
        {
            fread(&constants,       sizeof(constants),           1, infile);
            fread(&constantBankNames, sizeof(constantBankNames), 1, infile);
            fread(&CurrencyConv,    sizeof(CurrencyConv),        1, infile);
        }

        fread(&lastChosenConst,     sizeof(lastChosenConst),     1, infile);
        fread(&lastConstBank,       sizeof(lastConstBank),       1, infile);
        fread(excaliburNotes,       sizeof(excaliburNotes),      1, infile);
        fread(&lastChosenMacro,     sizeof(lastChosenMacro),     1, infile);
        fread(&traceDelayValueMs,   sizeof(traceDelayValueMs),   1, infile);
        fread(&indirectRegister,    sizeof(indirectRegister),    1, infile);

        fread(&reserved, RESERVED_SIZE, 1, infile);

        SetMenuType(menuCurrentFuncs);
        SetLastMenuType(menuLastFuncs);

        // -----------------------------------------------------------------------------------------------
        // If the main window was somehow shifted off screen (maybe due to using on a dock station with
        // multiple monitors), simply force the main window back to the center again and auto-resize it.
        // -----------------------------------------------------------------------------------------------
        if ((main_cx < MIN_WINDOW_WIDTH) || (main_cy < MIN_WINDOW_HEIGHT) ||
            (main_x >= MAX_WINDOW_X) || (main_y >= MAX_WINDOW_Y) || (main_x < MIN_WINDOW_X) || (main_y < MIN_WINDOW_Y))
        {
            main_x = -1; // Force auto-resize!
        }
        if (configVersionSub != CONFIG_VERSION_SUB) // If new version but still supported version
        {
            main_x = -1; // Force auto-resize!
        }

        fclose(infile);
        ProcessCustomSave();
    }
    else
    {
        SetMenuType(1);
        ProcessCustomSave();
        main_cx = -1; // Window will auto resize because main_cx == -1
    }
}

void ProcessCustomSave(void)
{
    int i, index, newIdx, saveIdx;

    for (i = 0; i < MAX_FUNCS; i++)
    {
        newIdx = i;
        index = customSave[newIdx].func_idx;
        saveIdx = Custom_funcs[newIdx].controlID;
        switch (customSave[newIdx].custom_save_idx)
        {
        case (CUSTOM_SAVE_SCI): // Scientific
            memcpy(&Custom_funcs[newIdx], &Scientific_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_SCI2): // Scientific II
            memcpy(&Custom_funcs[newIdx], &Scientific2_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_FIN): // Financial
            memcpy(&Custom_funcs[newIdx], &Financial_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_STATS): // Statistics
            memcpy(&Custom_funcs[newIdx], &Statistics_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_CONV): // Conversion
            memcpy(&Custom_funcs[newIdx], &Conversion_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_COMPSCI): // Computer Science
            memcpy(&Custom_funcs[newIdx], &CompSci_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_PROG1): // Programming Bank I
            memcpy(&Custom_funcs[newIdx], &Program1_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_PROG2): // Programming Bank II
            memcpy(&Custom_funcs[newIdx], &Program2_funcs[index], sizeof(struct funcStruct));
            break;
        case (CUSTOM_SAVE_MAC): // Macros - SPECIAL!
            memcpy(&Custom_funcs[newIdx], &MacroFuncs[index], sizeof(struct funcStruct));
            Custom_funcs[newIdx].desc = macro_short_names[index];
            Custom_funcs[newIdx].keyTitle = macroName[index];
            Custom_funcs[newIdx].keyHelp = PROGRAM_ASSIGNED_KEY_STR;
            break;
        }
        Custom_funcs[newIdx].controlID = saveIdx;
    }
    processBank();
}

BOOL CALLBACK fnDIALOG_MACRONAME(HWND, UINT, WPARAM, LPARAM);
char macName[31];
char macShortName[7];
void GetMacroName(void)
{
    DLGPROC lpfnDIALOG_MACRO;

    lpfnDIALOG_MACRO = (DLGPROC)MakeProcInstance((FARPROC)fnDIALOG_MACRONAME, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_MACRO_NAME", calcMainWindow, lpfnDIALOG_MACRO)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpfnDIALOG_MACRO);
}

BOOL CALLBACK fnDIALOG_MACRONAME(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch (wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, macName);
        SetDlgItemText(hDlg, IDC_EDIT2, macShortName);
        return TRUE;

    case WM_COMMAND:

        switch (wParam)
        {
        case (IDOK): // OK
            GetDlgItemText(hDlg, IDC_EDIT1, macName, 50);
            macName[64] = '\0';
            GetDlgItemText(hDlg, IDC_EDIT2, macShortName, 7);
            macShortName[6] = '\0';
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        case (IDCANCEL): // OK
            strcpy(macShortName, "@@ZY&(");
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            strcpy(macShortName, "@@ZY&(");
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

void CopyBufferToCurrentMacro(char *clipboardBuffer)
{
    int idx;
    char tmpB[64];
    char outputBuffer[200];
    int tmpBidx;
    int i;
    int found;
    int done;
    int checksumFound;
    unsigned short chksum, userChecksum;
    int preambleIndex;
    int maxLen;

    userChecksum = 0xFFAA;
    playBackIdx = 0;
    idx = 0;
    tmpBidx = 0;
    done = 0;
    checksumFound = 0;

    while ((idx < MAX_IMPORT_CLIPBOARD_SIZE) && !done)
    {
        if ((clipboardBuffer[idx] == '\n') || (clipboardBuffer[idx] == CNULL))
        {
            tmpB[tmpBidx++] = CNULL;
            preambleIndex = 0;
            done = 0;
            while (!done)
            {
                switch (tmpB[preambleIndex])
                {
                case ('0'):
                case ('1'):
                case ('2'):
                case ('3'):
                case ('4'):
                case ('5'):
                case ('6'):
                case ('7'):
                case ('8'):
                case ('9'):
                case (' '):
                case ('-'):
                    preambleIndex++; // Skip past any leading line numbers and dashes/spaces
                    break;
                default:
                    done = 1;
                }
            }
            done = 0;
            tmpBidx = 0;
            found = 0;
            for (i = 0; i < totalMappedButtonFuncs; i++) // Look up the macro by name in the mapped buttons table
            {
                maxLen = max(strlen(&tmpB[preambleIndex]), strlen(playBackMap[i].funcText));
                if ((tmpB[preambleIndex] != CNULL) && (strncmp(&tmpB[preambleIndex], playBackMap[i].funcText, maxLen) == 0))
                {
                    playBack[playBackIdx] = i;
                    playBackIdx++;
                    if (playBackIdx >= MAX_REC_PLAYBACK)
                    {
                        MessageBox(calcMainWindow, "Maximum program length has been reached.", "Excalibur Program Error", MB_OK);
                        playBackIdx = 0;
                        done = 1;
                    }
                    found = 1;
                    break;
                }
                if (strncmp(&tmpB[preambleIndex], END_OF_PROGRAM_STR, strlen(END_OF_PROGRAM_STR)) == 0)
                {
                    found = 1;
                    done = 1;
                    break;
                }
            }
            if (!found && (tmpB[preambleIndex] != CNULL))
            {
                sprintf(outputBuffer, "Error importing program from clipboard.\n\nLine: [%s] unknown.", tmpB);
                MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_ICONERROR);
                playBackIdx = 0;
                break;
            }
        }
        else
        {
            if (clipboardBuffer[idx] != '\r')
            {
                tmpB[tmpBidx++] = clipboardBuffer[idx];
            }
        }
        if (clipboardBuffer[idx] == CNULL)
        {
            break;
        }
        idx++;
    }
    if (playBackIdx > 0)
    {
        while ((idx < MAX_IMPORT_CLIPBOARD_SIZE))
        {
            if ((clipboardBuffer[idx] == '\n') || (clipboardBuffer[idx] == CNULL))
            {
                tmpB[tmpBidx++] = CNULL;
                tmpBidx = 0;
                if (strncmp(tmpB, "Checksum:", 9) == 0)
                {
                    sscanf(&tmpB[9], "%X", &userChecksum);
                    sprintf(tmpB, "My checksum %04X", userChecksum);
                    checksumFound = 1;
                    break;
                }
            }
            else
            {
                if (clipboardBuffer[idx] != '\r')
                {
                    tmpB[tmpBidx++] = clipboardBuffer[idx];
                }
            }
            if (clipboardBuffer[idx] == CNULL)
            {
                break;
            }
            idx++;
        }

        chksum = 0x0000;
        for (i = 0; i < playBackIdx; i++)
            chksum += playBack[i];
        if (checksumFound)
        {
            if (chksum == userChecksum)
            {
                sprintf(outputBuffer, "Successful Import\n\nProgram Checksum Verified\n\nChecksum of this newly imported program is %04X", chksum);
                MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                sprintf(outputBuffer, "Actual checksum of the imported program was %04X\nbut the the import buffer held a checksum of %04X\n\nThe program was imported but you should double check the program steps.", chksum, userChecksum);
                MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_OK | MB_ICONINFORMATION);
            }
        }
        else
        {
            sprintf(outputBuffer, "Successful Import\n\nChecksum of this newly imported program is %04X", chksum);
            MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_OK | MB_ICONINFORMATION);
        }
    }
}

BOOL CALLBACK fnDIALOG_MACRO(HWND, UINT, WPARAM, LPARAM);
void DoMacroSaveRecall(void)
{
    DLGPROC lpfnDIALOG_MACRO;

    lpfnDIALOG_MACRO = (DLGPROC)MakeProcInstance((FARPROC)fnDIALOG_MACRO, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_MACRO_SAVE", calcMainWindow, lpfnDIALOG_MACRO)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpfnDIALOG_MACRO);
}

BOOL CALLBACK fnDIALOG_MACRO(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    unsigned short chksum;
    LRESULT item;
    HANDLE hMem;
    LPSTR lpMem;
    HGLOBAL tptr;
    char far *cptr;
    static int lbTabStops[1] = {40};

    switch (wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), LB_SETTABSTOPS, 1, (DWORD)lbTabStops);
        SendMessage(GetDlgItem(hDlg, 101), LB_RESETCONTENT, 0, 0);
        SendMessage(GetDlgItem(hDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);

        chksum = 0x0000;
        for (i = 0; i < playBackIdx; i++)
            chksum += playBack[i];
        sprintf(tmpStr, "Checksum: %04X", chksum);
        SetDlgItemText(hDlg, IDC_CHECKSUM2, tmpStr);

        for (i = 0; i < playBackIdx; i++)
        {
            sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
        }
        sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
        SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));

        for (i = 0; i < MAX_MACROS; i++)
        {
            sprintf(tmpStr, "%-6s:\t%s", macro_short_names[i], macroName[i]);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
        }
        SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
        SetFocus(GetDlgItem(hDlg, 101));
        return TRUE;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case (101): // double click?!?
            if (HIWORD(wParam) != LBN_DBLCLK)
            {
                SendDlgItemMessage(hDlg, IDC_LIST1, LB_RESETCONTENT, 0, 0);
                item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
                lastChosenMacro = item;

                chksum = 0x0000;
                for (i = 0; i < playBackIdxSave[item]; i++)
                    chksum += playBackSave[item][i];
                sprintf(tmpStr, "Checksum: %04X", chksum);
                SetDlgItemText(hDlg, IDC_CHECKSUM1, tmpStr);

                for (i = 0; i < playBackIdxSave[item]; i++)
                {
                    sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBackSave[item][i]].funcText);
                    SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
                }
                sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
                SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
                return FALSE;
            }
            return TRUE;
            break;
        case (102): // Save
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT)LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
                return FALSE;
            }
            else
            {
                strcpy(macName, macroName[item]);
                strcpy(macShortName, macro_short_names[item]);
                GetMacroName();
                if (strcmp(macShortName, "@@ZY&(") != 0)
                {
                    strcpy(macroName[item], macName);
                    strcpy(macro_short_names[item], macShortName);
                    memcpy(playBackSave[item], playBack, sizeof(playBack));
                    playBackIdxSave[item] = playBackIdx;
                    lastChosenMacro = item;
                    SendMessage(GetDlgItem(hDlg, 101), LB_RESETCONTENT, 0, 0);
                    chksum = 0x0000;
                    for (i = 0; i < playBackIdx; i++)
                        chksum += playBack[i];
                    sprintf(tmpStr, "Checksum: %04X", chksum);
                    SetDlgItemText(hDlg, IDC_CHECKSUM2, tmpStr);

                    for (i = 0; i < MAX_MACROS; i++)
                    {
                        sprintf(tmpStr, "%-6s:\t%s", macro_short_names[i], macroName[i]);
                        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
                    }
                    SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
                    SetFocus(GetDlgItem(hDlg, 101));
                }
            }
            return TRUE;
            break;

        case (103): // Recall
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT)LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
                return FALSE;
            }
            else
            {
                memcpy(playBack, playBackSave[item], sizeof(playBack));
                playBackIdx = playBackIdxSave[item];
            }
            lastChosenMacro = item;
            SendMessage(GetDlgItem(hDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);
            chksum = 0x0000;
            for (i = 0; i < playBackIdx; i++)
                chksum += playBack[i];
            sprintf(tmpStr, "Checksum: %04X", chksum);
            SetDlgItemText(hDlg, IDC_CHECKSUM2, tmpStr);
            for (i = 0; i < playBackIdx; i++)
            {
                sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
                SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
            }
            sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));

            return TRUE;
            break;

        case (104): // Clear All
            if (MessageBox(hDlg, "Are you sure you wish to clear all programs?", "Excalibur For Windows", MB_ICONQUESTION | MB_YESNO) == IDYES)
            {
                for (i = 0; i < MAX_MACROS; i++)
                    SendDlgItemMessage(hDlg, 101, LB_DELETESTRING, 0, 0);
                for (i = 0; i < MAX_MACROS; i++)
                {
                    playBackIdxSave[i] = 0;
                    strcpy(macroName[i], "Not Currently Defined");
                    sprintf(macro_short_names[i], "P%02d", i + 1);
                    sprintf(tmpStr, "%-6s:\t%s", macro_short_names[i], macroName[i]);
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
                }
                chksum = 0x0000;
                sprintf(tmpStr, "Checksum: %04X", chksum);
                SetDlgItemText(hDlg, IDC_CHECKSUM1, tmpStr);
                lastChosenMacro = 0;
                SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
                SetFocus(GetDlgItem(hDlg, 101));
            }
            return TRUE;

        case (106): // Delete Macro
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT)LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
                return FALSE;
            }
            else
            {
                playBackIdxSave[item] = 0;
                strcpy(macroName[item], "Not Currently Defined");
                sprintf(macro_short_names[item], "P%02d", item + 1);
                SendMessage(GetDlgItem(hDlg, 101), LB_RESETCONTENT, 0, 0);
                for (i = 0; i < MAX_MACROS; i++)
                {
                    sprintf(tmpStr, "%-6s:\t%s", macro_short_names[i], macroName[i]);
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
                }
                chksum = 0x0000;
                for (i = 0; i < playBackIdxSave[item]; i++)
                    chksum += playBackSave[item][i];
                sprintf(tmpStr, "Checksum: %04X", chksum);
                SetDlgItemText(hDlg, IDC_CHECKSUM1, tmpStr);

                SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
                SetFocus(GetDlgItem(hDlg, 101));
            }
            return TRUE;

        case (107): // Paste/Import from Clipboard
            if (MessageBox(hDlg, "Import Program from Clipboard?\n\nThis will overwrite the currently loaded program\nbut does not affect stored programs.", "Excalibur For Windows", MB_ICONQUESTION | MB_YESNO) == IDYES)
            {
                OpenClipboard(calcMainWindow);
                hMem = GetClipboardData(CF_TEXT);
                lpMem = GlobalLock(hMem);
                lstrcpyn(clipboardBuffer, lpMem, MAX_IMPORT_CLIPBOARD_SIZE);
                GlobalUnlock(hMem);
                CloseClipboard();
                clipboardBuffer[MAX_IMPORT_CLIPBOARD_SIZE] = '\0';
                CopyBufferToCurrentMacro(clipboardBuffer);
                lastChosenMacro = 0;
                SendMessage(GetDlgItem(hDlg, IDC_LIST2), LB_RESETCONTENT, 0, 0);
                chksum = 0x0000;
                for (i = 0; i < playBackIdx; i++)
                    chksum += playBack[i];
                sprintf(tmpStr, "Checksum: %04X", chksum);
                SetDlgItemText(hDlg, IDC_CHECKSUM2, tmpStr);
                for (i = 0; i < playBackIdx; i++)
                {
                    sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
                    SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
                }
                sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
                SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG)((LPSTR)tmpStr));
            }
            return TRUE;

        case (108): // Copy Selected Program to Clipboard
            tptr = GlobalAlloc(GHND, (DWORD)MAX_IMPORT_CLIPBOARD_SIZE);
            cptr = GlobalLock(tptr);
            lstrcpy(cptr, "");

            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);

            sprintf(tmpStr, "Macro Name:  %s", macro_short_names[item]);
            lstrcat(cptr, (LPSTR)tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");

            sprintf(tmpStr, "Description: %s", macroName[item]);
            lstrcat(cptr, (LPSTR)tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");
            lstrcat(cptr, (LPSTR) "\r\n");

            for (i = 0; i < playBackIdxSave[item]; i++)
            {
                sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBackSave[item][i]].funcText);
                lstrcat(cptr, (LPSTR)tmpStr);
                lstrcat(cptr, (LPSTR) "\r\n");
            }
            sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
            lstrcat(cptr, (LPSTR)tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");
            chksum = 0x0000;
            for (i = 0; i < playBackIdxSave[item]; i++)
                chksum += playBackSave[item][i];
            sprintf(tmpStr, "Checksum: %04X", chksum);
            lstrcat(cptr, (LPSTR) "\r\n");
            lstrcat(cptr, (LPSTR)tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");

            OpenClipboard(calcMainWindow);
            EmptyClipboard();
            GlobalUnlock(tptr);
            SetClipboardData(CF_TEXT, tptr);
            CloseClipboard();
            sprintf(tmpStr, "The currently selected program [%s] has been saved to the clipboard.", macro_short_names[item]);
            MessageBox(hDlg, tmpStr, "Excalibur For Windows", MB_ICONINFORMATION | MB_OK);
            return TRUE;

        case (109): // Copy Loaded to Clipboard
            ClipboardCopySelection(calcMainWindow, COPY_MACRO_TO_CLIPBOARD);
            MessageBox(hDlg, "The currently loaded program has been saved to the clipboard.", "Excalibur For Windows", MB_ICONINFORMATION | MB_OK);
            return TRUE;

        case (105): // Cancel
            EndDialog(hDlg, FALSE);
            return TRUE;

        default:
            return FALSE;
        }

    case WM_DESTROY:
    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        ProcessCustomSave(); // Always update the keypad for custom layout...
        break;

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CALLBACK fnDIALOG_SettingsProc(HWND, UINT, WPARAM, LPARAM);
void GetUserSettings(void)
{
    DLGPROC lpfnDIALOG_SettingsProc;

    lpfnDIALOG_SettingsProc = (DLGPROC)MakeProcInstance((FARPROC)fnDIALOG_SettingsProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_SETTINGS", calcMainWindow, lpfnDIALOG_SettingsProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpfnDIALOG_SettingsProc);
}

BOOL CALLBACK fnDIALOG_SettingsProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    DWORD bs; // Button Status

    switch (wMessage)
    {
    case WM_INITDIALOG:
        sprintf(tmpStr, "%3.4f", taxConstant);
        SetDlgItemText(hDlg, 103, tmpStr);

        sprintf(tmpStr, "%d", traceDelayValueMs);
        SetDlgItemText(hDlg, 123, tmpStr);

        if (footPrint == 1) // 4-Banger mode
            SendMessage(GetDlgItem(hDlg, 119), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 118), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (extendedStack)
            SendMessage(GetDlgItem(hDlg, 125), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 124), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (popFillZero != 0)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_SETCHECK, (WORD)1, (DWORD)0L);

        SendMessage(GetDlgItem(hDlg, 104 + AngleMode), BM_SETCHECK, (WORD)1, (DWORD)0L);
        if (commaMode == 0)
            SendMessage(GetDlgItem(hDlg, 110), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 109), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (eexMode == 0)
            SendMessage(GetDlgItem(hDlg, 111), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 112), BM_SETCHECK, (WORD)1, (DWORD)0L);
        if (numLockMode == 1)
            SendMessage(GetDlgItem(hDlg, 107), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 108), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (toolTipsOn == TRUE)
            SendMessage(GetDlgItem(hDlg, 114), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 115), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (numberDisplayMode == INTERNATIONAL)
            SendMessage(GetDlgItem(hDlg, 116), BM_SETCHECK, (WORD)1, (DWORD)0L);
        else
            SendMessage(GetDlgItem(hDlg, 117), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (showXMinimized == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK2), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (eRPN == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK3), BM_SETCHECK, (WORD)1, (DWORD)0L);

        if (ClearStackOnExit == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK4), BM_SETCHECK, (WORD)1, (DWORD)0L);

        return TRUE;
    case WM_COMMAND:
        switch (wParam)
        {
        case (101): // OK

            GetDlgItemText(hDlg, 103, tmpStr, 8);
            taxConstant = atof(tmpStr);
            if (taxConstant == 0.0)
                taxConstant = 1.05;

            GetDlgItemText(hDlg, 123, tmpStr, 9);
            traceDelayValueMs = atoi(tmpStr);
            if (traceDelayValueMs < 100)
                traceDelayValueMs = 100; // Can't really handle much below this anyway

            bs = SendMessage(GetDlgItem(hDlg, 125), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                extendedStack = 1;
            else
                extendedStack = 0;

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0)
                popFillZero = 1;
            else
                popFillZero = 0;

            bs = SendMessage(GetDlgItem(hDlg, 118), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                if (footPrint != 0)
                {
                    MessageBox(hDlg,
                               "Changing the footprint size requires you to close and restart Excalibur for the new setting to take place.",
                               "Excalibur Footprint Change", MB_OK);
                }
                footPrint = 0;
            }
            bs = SendMessage(GetDlgItem(hDlg, 119), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                if (footPrint != 1)
                {
                    MessageBox(hDlg,
                               "Changing the footprint size requires you to close and restart Excalibur for the new setting to take place.",
                               "Excalibur Footprint Change", MB_OK);
                }
                footPrint = 1;
            }

            bs = SendMessage(GetDlgItem(hDlg, 104), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                AngleMode = 0; // Degrees
            }
            bs = SendMessage(GetDlgItem(hDlg, 105), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                AngleMode = 1; // Radians
            }
            bs = SendMessage(GetDlgItem(hDlg, 106), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                AngleMode = 2; // Gradients
            }
            bs = SendMessage(GetDlgItem(hDlg, 109), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                commaMode = 1;
            else
                commaMode = 0;

            bs = SendMessage(GetDlgItem(hDlg, 107), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                numLockMode = 1;
            else
                numLockMode = 0;

            bs = SendMessage(GetDlgItem(hDlg, 114), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                toolTipsOn = 1;
            else
                toolTipsOn = 0;

            bs = SendMessage(GetDlgItem(hDlg, 116), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                numberDisplayMode = INTERNATIONAL;
                SetDlgItemText(calcMainWindow, RPN_DIGIT_DP, ".");
            }
            else
            {
                numberDisplayMode = NONINTERNATIONAL;
                SetDlgItemText(calcMainWindow, RPN_DIGIT_DP, ",");
            }

            bs = SendMessage(GetDlgItem(hDlg, 111), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
            {
                eexMode = 0;
                SetDlgItemText(calcMainWindow, RPN_NEGATE, "±");
                SetDlgItemText(calcMainWindow, RPN_E, "E");
            }
            else
            {
                eexMode = 1;
                SetDlgItemText(calcMainWindow, RPN_NEGATE, "CHS");
                SetDlgItemText(calcMainWindow, RPN_E, "EEX");
            }

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK2), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                showXMinimized = 1;
            else
                showXMinimized = 0;

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK3), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                eRPN = 1;
            else
                eRPN = 0;

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK4), BM_GETCHECK, (WORD)0, (DWORD)0L);
            if (bs != 0L)
                ClearStackOnExit = 1;
            else
                ClearStackOnExit = 0;

            EndDialog(hDlg, FALSE);
            ShowStatus();
            ShowStack();
            return TRUE;
        case (102): // CANCEL
            EndDialog(hDlg, FALSE);
            return TRUE;

        case (113): // DISPLAY MODES
            RPN_mode2(hDlg);
            break;

        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

void HelpAbout(void)
{
    MessageBox(calcMainWindow, ABOUT_MSG, "About Excalibur", MB_OK | MB_ICONINFORMATION);
}

void ShowUsageStats(void)
{
    char stackPushesStr[64];
    char stackPopsStr[64];

    sprintf(stackPushesStr, "%I64u", stackPushes);
    PutCommas(stackPushesStr);
    sprintf(stackPopsStr, "%I64u", stackPops);
    PutCommas(stackPopsStr);
    sprintf(tmpStr, "Stack Pushes:  %s\nStack Pops:      %s\nTime In Focus:  %lu mins", stackPushesStr, stackPopsStr, inFocusTime);
    MessageBox(calcMainWindow, tmpStr, "System Usage Statistics", MB_ICONINFORMATION | MB_OK);
}

WORD GetMouseHelp(WORD xPos, WORD yPos)
{
    int i;
    WORD status = 0;

    if (footPrint == 0)
    {
        i = 0;
        while (FunctionBankKeyPos[i].controlID != RPN_LAST_KEY)
        {
            if (xPos >= FunctionBankKeyPos[i].x && xPos <= FunctionBankKeyPos[i].x + FunctionBankKeyPos[i].w)
                if (yPos >= FunctionBankKeyPos[i].y && yPos <= FunctionBankKeyPos[i].y + FunctionBankKeyPos[i].h)
                {
                    strcpy(helpTitle, currentFuncs[i].keyTitle);
                    strcpy(helpMsg, currentFuncs[i].keyHelp);
                    status = 1;
                    break;
                }
            i++;
        }
    }

    i = 0;
    if (status == 0)
    {
        while (RPNkeyPos[i].controlID != RPN_LAST_KEY)
        {
            if (xPos >= RPNkeyPos[i].x && xPos <= RPNkeyPos[i].x + RPNkeyPos[i].w)
                if (yPos >= RPNkeyPos[i].y && yPos <= RPNkeyPos[i].y + RPNkeyPos[i].h)
                {
                    strcpy(helpTitle, RPNkeys[i + 17].keyTitle); // +17 gets us past RPN_ENTER where we start using tooltips
                    strcpy(helpMsg, RPNkeys[i + 17].keyHelp);
                    status = 1;
                    break;
                }
            i++;
        }
    }

    return (status);
}

void init_key_pos(void) // for tool tips
{
    int i;
    RECT rc;
    POINT pt;
    int fnButtonX;
    int fnButtonY;
    int fnButtonH;
    int fnButtonW;

    GetWindowRect(GetDlgItem(calcMainWindow, FN1), &rc);
    pt.x = rc.left;
    pt.y = rc.top;
    ScreenToClient(calcMainWindow, &pt);
    fnButtonX = pt.x;
    fnButtonY = pt.y;

    GetWindowRect(GetDlgItem(calcMainWindow, FN2), &rc);
    pt.x = rc.left;
    pt.y = rc.top;
    ScreenToClient(calcMainWindow, &pt);
    fnButtonW = pt.x - fnButtonX;

    GetWindowRect(GetDlgItem(calcMainWindow, FN5), &rc);
    pt.x = rc.left;
    pt.y = rc.top;
    ScreenToClient(calcMainWindow, &pt);
    fnButtonH = pt.y - fnButtonY;

    i = 0;
    while (RPNkeyPos[i].controlID != RPN_LAST_KEY)
    {
        GetWindowRect(GetDlgItem(calcMainWindow, RPNkeyPos[i].controlID), &rc);
        pt.x = rc.left;
        pt.y = rc.top;
        ScreenToClient(calcMainWindow, &pt);
        RPNkeyPos[i].x = pt.x;
        RPNkeyPos[i].y = pt.y;
        RPNkeyPos[i].h = rc.bottom - rc.top;
        RPNkeyPos[i].w = rc.right - rc.left;
        i++;
    }

    i = 0;
    while (FunctionBankKeyPos[i].controlID != RPN_LAST_KEY)
    {
        GetWindowRect(GetDlgItem(calcMainWindow, FunctionBankKeyPos[i].controlID), &rc);
        pt.x = rc.left;
        pt.y = rc.top;
        ScreenToClient(calcMainWindow, &pt);
        FunctionBankKeyPos[i].x = pt.x;
        FunctionBankKeyPos[i].y = pt.y;
        FunctionBankKeyPos[i].h = rc.bottom - rc.top;
        FunctionBankKeyPos[i].w = rc.right - rc.left;
        i++;
    }
}

void RPN_clearX(void)
{
    STACK[STK_X] = 0.0;
    STACKL[STK_X] = 0L;
    progModecarry = 0;
    Xedit = X_ENTER;
    RPN_ClearModifiers(!macroPlayback);
}

void RPN_drop(void) // drop the stack
{
    Xedit = X_NEW;
    if (progMode == PROG_FLOAT)
        StackPop();
    else
        StackPopL();
}

void RPN_larg(void) // drop the stack
{
    Xedit = X_NEW;
    if (progMode == PROG_FLOAT)
    {
        StackPush(LASTY);
        StackPush(LASTX);
    }
    else
    {
        StackPushL(LASTYL);
        StackPushL(LASTXL);
    }
}


BOOL CALLBACK fnDIALOG_ShowMemory(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void RPN_show(void)
{
    DLGPROC lpfnDIALOG_ShowMemory;

    lpfnDIALOG_ShowMemory = (DLGPROC)MakeProcInstance((FARPROC)fnDIALOG_ShowMemory, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_SHOW", calcMainWindow, lpfnDIALOG_ShowMemory)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpfnDIALOG_ShowMemory);
    ShowStatus();
}

BOOL CALLBACK fnDIALOG_ShowMemory(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[64];
    WORD i;

    switch (wMessage)
    {
    case WM_INITDIALOG:
        for (i=101; i<250; i++) // Set fixed font for all number outputs...
        {
            SendMessage(GetDlgItem(hDlg, i), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        }

        // Main and Extended Stack - might be floats or longs
        if (progMode == PROG_FLOAT)
        {
            for (i=0; i<MAX_STACK; i++)
            {
                sprintf(tmp, "%-.14g", STACK[i]);
                makeInternational(tmp);
                SetDlgItemText(hDlg, IDC_SHOW_X+i, tmp);
            }
        }
        else
        {
            for (i=0; i<MAX_STACK; i++)
            {
                MakeCompSciStr(STACKL[i], tmp);
                SetDlgItemText(hDlg, IDC_SHOW_X+i, tmp);
            }
        }

        // Registers (R00-R20)
        for (i=0; i<=20; i++)
        {
            if (progMode == PROG_FLOAT)
            {
                sprintf(tmp, "R%02d", i);
                SetDlgItemText(hDlg, IDC_SHOW_LBL_R00+i, tmp);

                sprintf(tmp, "%-.14g", STO[i]);
                makeInternational(tmp);
            }
            else
            {
                sprintf(tmp, "L%02d", i);
                SetDlgItemText(hDlg, IDC_SHOW_LBL_R00+i, tmp);
                MakeCompSciStr(STOL[i], tmp);
            }
            SetDlgItemText(hDlg, IDC_SHOW_R00+i, tmp);
        }

        // Financial Registers (at least the TVM ones)
        for (i=0; i<=5; i++)
        {
            sprintf(tmp, "%-.14g", FIN[i]);
            makeInternational(tmp);
            SetDlgItemText(hDlg, IDC_SHOW_FIN_N+i, tmp);
        }

        // Statistics Registers
        for (i=0; i<=5; i++)
        {
            sprintf(tmp, "%-.14g", SUM[i]);
            makeInternational(tmp);
            SetDlgItemText(hDlg, IDC_SHOW_STATS_N+i, tmp);
        }

        return TRUE;

    case WM_COMMAND:
        switch (wParam)
        {
        case (IDOK): // OK was pressed
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}


void RPN_edit(void)
{
    int i, j;

    if (Xedit != X_EDIT)
    {
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, 40);
        j = 0;
        for (i = 0; i < (int)strlen(tmpStr); i++)
        {
            if (tmpStr[i] != ' ' && tmpStr[i] != ',' && tmpStr[i] != 'h' && tmpStr[i] != 'b' && tmpStr[i] != 'o' && tmpStr[i] != 'd')
            {
                Xstr[j] = tmpStr[i];
                j++;
            }
        }
        Xstr[j] = (char)NULL;
        Xedit = X_EDIT;
    }
    else
    {
        Xedit = X_NEW;
        StackPush(StackPop());
    }
}

void RPN_SelectSci(void)
{
    SelectNewBank((struct funcStruct *)&Scientific_funcs);
}

void RPN_SelectSci2(void)
{
    SelectNewBank((struct funcStruct *)&Scientific2_funcs);
}

void RPN_SelectStat(void)
{
    SelectNewBank((struct funcStruct *)&Statistics_funcs);
}

void RPN_SelectFin(void)
{
    SelectNewBank((struct funcStruct *)&Financial_funcs);
}

void RPN_SelectConv(void)
{
    SelectNewBank((struct funcStruct *)&Conversion_funcs);
}

void RPN_SelectCompSci(void)
{
    SelectNewBank((struct funcStruct *)&CompSci_funcs);
}

void RPN_SelectCustom(void)
{
    SelectNewBank((struct funcStruct *)&Custom_funcs);
}

void RPN_SelectProgI(void)
{
    SelectNewBank((struct funcStruct *)&Program1_funcs);
}

void RPN_SelectProgII(void)
{
    SelectNewBank((struct funcStruct *)&Program2_funcs);
}

// -----------------------------------------------------------------------------
// Good old bubble sort... not the most efficient but this is only done once at
// startup and the list is small and nearly sorted so this will be plenty fast.
// -----------------------------------------------------------------------------
void sortPlaybackList(void)
{
    int i, j;
    uint8_t exchangeMade;
    struct playbackStruct tmpPlayBack;

    for (i = 0; i < totalMappedButtonFuncs; i++)
    {
        exchangeMade = FALSE;
        for (j = 0; j < (totalMappedButtonFuncs - i - 1); j++)
        {
            if (playBackMap[j].uniqueIndex > playBackMap[j + 1].uniqueIndex)
            {
                // Need to swap...
                memcpy(&tmpPlayBack, &playBackMap[j], sizeof(tmpPlayBack));
                memcpy(&playBackMap[j], &playBackMap[j + 1], sizeof(tmpPlayBack));
                memcpy(&playBackMap[j + 1], &tmpPlayBack, sizeof(tmpPlayBack));
                exchangeMade = TRUE; // At least one swap was performed
            }
        }
        if (exchangeMade == FALSE)
            break; // We are Sorted!
    }
}

void mapButtonFuncs(void)
{
    int i, j;

    j = 0;
    i = 0;

    do // We do a do-while so we include RPN_LAST_KEY (and a single UNI_UNUSED controlID) in the map.
    {
        playBackMap[j].saveLastX = RPNkeys[i].saveLastX;
        playBackMap[j].newXedit = RPNkeys[i].newXedit;
        playBackMap[j].routine = RPNkeys[i].routine;
        playBackMap[j].funcText = RPNkeys[i].keyTitle;
        playBackMap[j].uniqueIndex = RPNkeys[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = RPNkeys[i].useFloatsLongs;
        playBackMap[j].allowRecord = RPNkeys[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
        i++;
    } while (RPNkeys[i - 1].controlID != RPN_LAST_KEY);

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Scientific_funcs[i].saveLastX;
        playBackMap[j].newXedit = Scientific_funcs[i].newXedit;
        playBackMap[j].routine = Scientific_funcs[i].routine;
        playBackMap[j].funcText = Scientific_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = Scientific_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Scientific_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Scientific_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Scientific2_funcs[i].saveLastX;
        playBackMap[j].newXedit = Scientific2_funcs[i].newXedit;
        playBackMap[j].routine = Scientific2_funcs[i].routine;
        playBackMap[j].funcText = Scientific2_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = Scientific2_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Scientific2_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Scientific2_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Financial_funcs[i].saveLastX;
        playBackMap[j].newXedit = Financial_funcs[i].newXedit;
        playBackMap[j].routine = Financial_funcs[i].routine;
        playBackMap[j].funcText = Financial_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = Financial_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Financial_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Financial_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Conversion_funcs[i].saveLastX;
        playBackMap[j].newXedit = Conversion_funcs[i].newXedit;
        playBackMap[j].routine = Conversion_funcs[i].routine;
        playBackMap[j].funcText = Conversion_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = Conversion_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Conversion_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Conversion_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (Statistics_funcs[i].uniqueIndex != UNI_UNUSED)
        {
            playBackMap[j].saveLastX = Statistics_funcs[i].saveLastX;
            playBackMap[j].newXedit = Statistics_funcs[i].newXedit;
            playBackMap[j].routine = Statistics_funcs[i].routine;
            playBackMap[j].funcText = Statistics_funcs[i].keyTitle;
            playBackMap[j].uniqueIndex = Statistics_funcs[i].uniqueIndex;
            playBackMap[j].useFloatsLongs = Statistics_funcs[i].useFloatsLongs;
            playBackMap[j].allowRecord = Statistics_funcs[i].allowRecord;
            if (j < MAX_FUNCTIONS)
                j++;
        }
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = CompSci_funcs[i].saveLastX;
        playBackMap[j].newXedit = CompSci_funcs[i].newXedit;
        playBackMap[j].routine = CompSci_funcs[i].routine;
        playBackMap[j].funcText = CompSci_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = CompSci_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = CompSci_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = CompSci_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Program1_funcs[i].saveLastX;
        playBackMap[j].newXedit = Program1_funcs[i].newXedit;
        playBackMap[j].routine = Program1_funcs[i].routine;
        playBackMap[j].funcText = Program1_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = Program1_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Program1_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Program1_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Program2_funcs[i].saveLastX;
        playBackMap[j].newXedit = Program2_funcs[i].newXedit;
        playBackMap[j].routine = Program2_funcs[i].routine;
        playBackMap[j].funcText = Program2_funcs[i].keyTitle;
        playBackMap[j].uniqueIndex = Program2_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Program2_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Program2_funcs[i].allowRecord;
        if (j < MAX_FUNCTIONS)
            j++;
    }

    totalMappedButtonFuncs = j;

    if (totalMappedButtonFuncs >= MAX_FUNCTIONS)
    {
        MessageBox(calcMainWindow, "Error - Maximum number of mapped functions exists!", "Excalibur Fatal Error", MB_OK);
    }
    else // Sort the list based on unique index for fast lookup during recording and playback.
    {
        sortPlaybackList();
    }
}

void SaveProgramStep(uint16_t uniqueIndex)
{
    int i, k;

    i = 0;
    while (i < totalMappedButtonFuncs)
    {
        if (uniqueIndex == playBackMap[i].uniqueIndex) // We always look up the function based on unique index!
        {
            if (playBackIdx < (MAX_REC_PLAYBACK - 1))
            {
                if (currentMacroPlaybackIdx != playBackIdx) // Insert at current position!
                {
                    for (k = playBackIdx; k >= currentMacroPlaybackIdx; k--)
                    {
                        playBack[k + 1] = playBack[k];
                    }
                    playBackIdx++;
                    playBack[currentMacroPlaybackIdx] = i;
                    currentMacroPlaybackIdx++;
                }
                else // Insert at end!
                {
                    playBack[currentMacroPlaybackIdx] = i;
                    playBackIdx++;
                    currentMacroPlaybackIdx++;
                }
            }
            else
            {
                MessageBox(calcMainWindow, "Maximum program length has been reached.", "Excalibur Program Error", MB_OK);
            }

            break;
        }
        i++;
    }
}

void callButtonFunc(void (*routine)(void), char useFloatsLongs, char allowRecord,
                    uint16_t uniqueIndex, char saveLastX, char newXedit, int updateSpareBar)
{
    if (IsWindowVisible(toolTipWnd)) // A button press reset's the window!
    {
        ShowWindow(toolTipWnd, SW_HIDE);
        toolTipCounter = 0;
    }

    if (progMode != PROG_FLOAT) // Always ensure floats are "in-sync" with longs before any press!
    {
        LongsToFloats();
    }

    if (saveLastX == YES_L)
    {
        LASTX = STACK[STK_X];
        LASTXL = STACKL[STK_X];
        LASTY = STACK[STK_Y];
        LASTYL = STACKL[STK_Y];
    }

    // Before we call the button function we need to ensure both stacks look right.
    // This will help with Macro programming between stacks!
    if (useFloatsLongs == USES_L && progMode == PROG_FLOAT)
    {
        progMode = PROG_DEC;
        FloatsToLongs();
        ShowStack();
        ShowStatus();
    }
    else if (useFloatsLongs == USES_F && progMode != PROG_FLOAT)
    {
        progMode = PROG_FLOAT;
        LongsToFloats();
        ShowStack();
        ShowStatus();
    }

    routine(); // This calls the actual button routine to perform things like SIN, COS, CLX, etc

    lastUniqueIndex = uniqueIndex;

    if ((recModeON == 1) && (allowRecord == ALLOWREC))
    {
        SaveProgramStep(uniqueIndex);
    }

    if (newXedit != X_NULL)
    {
        Xedit = newXedit;
        RPN_ClearModifiers(updateSpareBar);
    }
}

// -------------------------------------------------------------------------------------------------
// This version is streamlined for use when a macro is playing back for relatively blazing speed...
// -------------------------------------------------------------------------------------------------
void callButtonFunc_fast(void (*routine)(void), char useFloatsLongs, uint16_t uniqueIndex, char saveLastX, char newXedit)
{
    if (progMode != PROG_FLOAT) // Always ensure floats are "in-sync" with longs before any press!
    {
        LongsToFloats();
    }

    if (saveLastX == YES_L)
    {
        LASTX = STACK[STK_X];
        LASTXL = STACKL[STK_X];
        LASTY = STACK[STK_Y];
        LASTYL = STACKL[STK_Y];
    }

    // -----------------------------------------------------------------------------
    // Before we call the button function we need to ensure both stacks look right.
    // This will help with Macro programming between stacks!
    // -----------------------------------------------------------------------------
    if (useFloatsLongs == USES_L && progMode == PROG_FLOAT)
    {
        progMode = PROG_DEC;
        FloatsToLongs();
        ShowStack();
        ShowStatus();
    }
    else if (useFloatsLongs == USES_F && progMode != PROG_FLOAT)
    {
        progMode = PROG_FLOAT;
        LongsToFloats();
        ShowStack();
        ShowStatus();
    }

    routine(); // This calls the actual button routine to perform things like SIN, COS, CLX, etc

    lastUniqueIndex = uniqueIndex;

    if (newXedit != X_NULL)
    {
        Xedit = newXedit;
        RPN_ClearModifiers(FALSE);
    }
}

void RPN_Record(void)
{
    if (recModeON == 0)
    {
        recModeON = 1;
        playBackIdx = 0;
        currentMacroPlaybackIdx = 0;
        Xedit = X_NEW;
    }
    else
    {
        recModeON = 0;
    }
    ShowStatus();
}

// -------------------------------------------------------------------------------
// This is the core driver for when we playback a macro. We loop through the
// recorded macro steps and call the appropriate button functions. We also check
// for user input and allow the user to stop the macro at any time by pressing
// the Escape key. The macro playback loop is optimized to push through the
// recorded keystrokes as fast as possible. On a typical i5 computer from
// around 2018, this will run about 1 million 'Excalibur Instructions' per second.
// -------------------------------------------------------------------------------
void RPN_Playback(void)
{
    int idx;
    int flashRunningDsp = 0;
    MSG msg;
    DWORD lastSlowTimer = 0;

    if (recModeON == 1) // Always turn off rec mode before playback!
    {
        recModeON = 0;
        ShowStatus();
    }

    Xedit = X_NEW;
    MacroStackIdx = 0;

    macroPlayback = TRUE;

    GetAsyncKeyState(VK_ESCAPE); // Get one reading at least!
    SetFocus(calcMainWindow);    // For long macros this will "release" the Play key depression...

    if (IsWindowVisible(toolTipWnd)) // Macro running... hide tool tip window if it was visible.
    {
        ShowWindow(toolTipWnd, SW_HIDE);
        toolTipCounter = 0;
    }

    SetWindowText(GetDlgItem(calcMainWindow, RPN_PLAYBACK), "Stop");

    // ------------------------------------------------------------------------------------------------
    // This is the main macro playback loop... it has been somewhat optimized so that we push through
    // as many recorded keystrokes as possible. On a fairly pedestrian i5 computer (circa 2018), this
    // will run about 1 million 'Excalibur Instructions' per second. Good enough.
    // ------------------------------------------------------------------------------------------------
    for (currentMacroPlaybackIdx = 0; currentMacroPlaybackIdx < playBackIdx; currentMacroPlaybackIdx++)
    {
        static int dampenSystemProcessing = 0;

        // Don't need to peek THAT often... allows macro to run faster
        if ((!(++dampenSystemProcessing & 0x3F)) || traceMacroPlayback)
        {
            while (PeekMessage(&msg, calcMainWindow, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT) // Always check for quit!
                {
                    endRunningMacro();
                    PostQuitMessage(0);
                    return;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
                break;

            if (currentMacroPlaybackIdx == playBackIdx) // We may have ended the macro!!
                break;
        }

        if (traceMacroPlayback == FALSE)
        {
            if (slowTimer != lastSlowTimer)
            {
                lastSlowTimer = slowTimer;
                if (++flashRunningDsp & 1)
                    UpdateSpareBar(" ");
                else
                    UpdateSpareBar("Run...");
            }
        }

        idx = playBack[currentMacroPlaybackIdx];

        if (playBackMap[idx].routine != NULL)
        {
            callButtonFunc_fast(playBackMap[idx].routine, playBackMap[idx].useFloatsLongs,
                                playBackMap[idx].uniqueIndex, playBackMap[idx].saveLastX, playBackMap[idx].newXedit);
        }

        if (traceMacroPlayback == TRUE)
        {
            sleep_and_peek(traceDelayValueMs);

            showTrace = TRUE;
            ShowStack();
            showTrace = FALSE;

            while (PeekMessage(&msg, debugTraceWindow, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    SetWindowText(GetDlgItem(calcMainWindow, RPN_PLAYBACK), "Run"); // Reset button text

    UpdateSpareBar(" ");
    macroPlayback = FALSE;
}

void RPN_SingleStep(void)
{
    int idx;

    if (recModeON == 1) // Always turn off rec mode before playback!
    {
        recModeON = 0;
        ShowStatus();
    }

    macroPlayback = TRUE;
    traceMacroPlayback = TRUE;

    showTrace = TRUE;
    ShowStack();
    showTrace = FALSE;

    if (currentMacroPlaybackIdx == playBackIdx) // We may have ended the macro!!
    {
        Xedit = X_NEW;
    }
    else
    {
        idx = playBack[currentMacroPlaybackIdx];
        if (playBackMap[idx].routine != NULL)
        {
            // Always disallow record of playback keystrokes!
            callButtonFunc(playBackMap[idx].routine, playBackMap[idx].useFloatsLongs, NORECORD,
                           playBackMap[idx].uniqueIndex, playBackMap[idx].saveLastX, playBackMap[idx].newXedit, FALSE);
            if (playBackMap[idx].newXedit != X_NULL)
            {
                Xedit = playBackMap[idx].newXedit;
            }
        }
    }

    sleep_and_peek(traceDelayValueMs);
    ShowStack();

    UpdateSpareBar("    ");
    macroPlayback = FALSE;
    traceMacroPlayback = FALSE;
}

BOOL CALLBACK NotesDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch (wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, excaliburNotes);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case (IDOK): // OK - Close and save notes
            GetDlgItemText(hDlg, IDC_EDIT1, excaliburNotes, NOTES_SIZE - 1);
            excaliburNotes[NOTES_SIZE] = CNULL;
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

void RPN_Notes(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_NOTES", calcMainWindow, NotesDlgProc);
}

void RPN_Copy(void)
{
    ClipboardCopySelection(calcMainWindow, COPY_X_TO_CLIPBOARD);
    blinkXDisplay(FALSE);
}

void RPN_Paste(void)
{
    ClipboardCopySelection(calcMainWindow, COPY_X_FROM_CLIPBOARD);
    blinkXDisplay(FALSE);
}

void RPN_inverse(void)
{
    if (STACK[STK_X] == 0.0)
    {
        RPN_error("Divide By Zero");
    }
    else
    {
        StackPush(1.0 / StackPop());
    }
}

void RPN_pow(void)
{
    if (progMode == PROG_FLOAT)
    {
        double xtemp, ytemp;
        xtemp = StackPop();
        ytemp = StackPop();
        if (ytemp == 0.0 && xtemp < 0.0)
            RPN_error("Power:  Y=0 and X < 0");
        else
            StackPush(pow(ytemp, xtemp));
    }
    else
    {
        PROG_LONG i, xtemp64, ytemp64;

        xtemp64 = StackPopL();
        ytemp64 = StackPopL();
        if (ytemp64 == 0 && xtemp64 < 0)
		{
            RPN_error("Power:  Y=0 and X < 0");
		}
        else if (xtemp64 == 0)
		{
			StackPushL(1);
		}
		else
        {
			PROG_LONG sum = ytemp64;
            for (i = 0; i < xtemp64-1; i++)
            {
                sum = sum * ytemp64;
            }

            StackPushL(sum);
        }
    }
}

void blinkXDisplay(uint8_t no_peek)
{
    if (!macroPlayback)
    {
        HWND hControl;
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN);
        SetDlgItemText(calcMainWindow, RPN_STACK_X, " ");

        hControl = GetDlgItem(calcMainWindow, RPN_STACK_X);
        InvalidateRect(hControl, NULL, TRUE);
        UpdateWindow(hControl);

        if (no_peek) Sleep(250);
        else sleep_and_peek(250);
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN);

        hControl = GetDlgItem(calcMainWindow, RPN_STACK_X);
        InvalidateRect(hControl, NULL, TRUE);
        UpdateWindow(hControl);
        Sleep(10);
    }
}

void blinkStack(uint8_t no_peek)
{
    char tmp1[MAX_STACK_STRLEN + 1];
    char tmp2[MAX_STACK_STRLEN + 1];
    char tmp3[MAX_STACK_STRLEN + 1];
    char tmp4[MAX_STACK_STRLEN + 1];

    if (!macroPlayback)
    {
        HWND hControl;
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmp1, MAX_STACK_STRLEN);
        GetDlgItemText(calcMainWindow, RPN_STACK_Y, tmp2, MAX_STACK_STRLEN);
        GetDlgItemText(calcMainWindow, RPN_STACK_Z, tmp3, MAX_STACK_STRLEN);
        GetDlgItemText(calcMainWindow, RPN_STACK_T, tmp4, MAX_STACK_STRLEN);
        SetDlgItemText(calcMainWindow, RPN_STACK_X, " ");
        SetDlgItemText(calcMainWindow, RPN_STACK_Y, " ");
        SetDlgItemText(calcMainWindow, RPN_STACK_Z, " ");
        SetDlgItemText(calcMainWindow, RPN_STACK_T, " ");

        hControl = GetDlgItem(calcMainWindow, RPN_STACK);
        InvalidateRect(hControl, NULL, TRUE);
        UpdateWindow(hControl);

        if (no_peek) Sleep(250);
        else sleep_and_peek(250);
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmp1, MAX_STACK_STRLEN);
        GetDlgItemText(calcMainWindow, RPN_STACK_Y, tmp2, MAX_STACK_STRLEN);
        GetDlgItemText(calcMainWindow, RPN_STACK_Z, tmp3, MAX_STACK_STRLEN);
        GetDlgItemText(calcMainWindow, RPN_STACK_T, tmp4, MAX_STACK_STRLEN);

        hControl = GetDlgItem(calcMainWindow, RPN_STACK);
        InvalidateRect(hControl, NULL, TRUE);
        UpdateWindow(hControl);
        Sleep(10);
    }
}

void RPN_digit0(void)
{
    RPN_digit(101);
}

void RPN_digit1(void)
{
    RPN_digit(102);
}

void RPN_digit2(void)
{
    RPN_digit(103);
}

void RPN_digit3(void)
{
    RPN_digit(104);
}

void RPN_digit4(void)
{
    RPN_digit(105);
}

void RPN_digit5(void)
{
    RPN_digit(106);
}

void RPN_digit6(void)
{
    RPN_digit(107);
}

void RPN_digit7(void)
{
    RPN_digit(108);
}

void RPN_digit8(void)
{
    RPN_digit(109);
}

void RPN_digit9(void)
{
    RPN_digit(110);
}

void RPN_endConst(void)
{
    // Simply so that constant numbers can lift stack appropriately.
}

#define EXCAL_HELP_STR "TAB        Clear X                       Key UP     Rotate Stack Up\n"      \
                       "Key C      Clear Stack (twice=all)       Key DN     Rotate Stack Down\n"    \
                       "Key L      Last X Register               Key E      Exponent\n"             \
                       "Key X      Exchange X and Y              Key S      Store Register\n"       \
                       "Key +      Add X and Y registers         Key R      Recall Register\n"      \
                       "Key -      Subtract X from Y             Key M      Display Mode\n"         \
                       "Key *      Multiply X and Y              Key H      Help Key\n"             \
                       "Key /      Divide X into Y               Key F      Last Function Bank\n"   \
                       "Key !      Factorial of X                Key ^      Raise Y to X power\n"   \
                       "ENTER      Enter number to Stack         Key N      Change Sign (Negate)\n" \
                       "\n"                                                                         \
                       "DP         Decimal Point. Press twice for fraction.\n"                      \
                       "BKSP       Erases last character, otherwise clears X.\n"                    \
                       "F1-F10     First column of the currently selected function bank.\n"         \
                       "SHIFT  FN  Second column of the currently selected function bank.\n"        \
                       "CTRL   FN  Second column of the currently selected function bank.\n"        \
                       "CTLSFT FN  Fourth column of the currently selected function bank.\n"        \
                       "CTRL 0-9   Quick selection of the number of decimal places to show.\n"      \
                       "CTRL S     Brings up the Settings Dialog.\n"                                \
                       "CTRL P     Brings up the Program Manager Dialog.\n"                         \
                       "CTRL D     Brings up the Define Custom Button Set Dialog.\n"                \
                       "CTRL D     Brings up the Define Custom Button Set Dialog.\n"                \
                       "CTRL C     Copy X Register to the clipboard.\n"                             \
                       "CTRL A     Copy All Registers to the clipboard.\n"                          \
                       "CTRL V     Paste X Register from the clipboard.\n"                          \
                       "CTRL X     Exits Excalibur (same as pressing the Close icon).\n"            \
                       "\n"                                                                         \
                       "Right Click on any key to provide short context help for that function.\n"  \
                       ""

extern BOOL CALLBACK HelpDialog(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void ExcaliburHelp(void)
{
    DLGPROC lpHelpDialog;

    lpHelpDialog = (DLGPROC)MakeProcInstance((FARPROC)HelpDialog, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_HELP", calcMainWindow, lpHelpDialog)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC)lpHelpDialog);
}

BOOL CALLBACK HelpDialog(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    SIZE lpSize;
    DWORD dw;

    switch (wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM)hFixedFont, FALSE);
        SetDlgItemText(hDlg, 101, EXCAL_HELP_STR);

        hdc = GetDC(hDlg);
        SelectObject(hdc, hFixedFont);
        dw = GetTextExtentPoint32(hdc, "WWWWW88888OOOOOXXXXXWWWWW88888OOOOOXXXXXWWWWW88888OOOOOXXXXXOOOOOXXXXXXX", 72, &lpSize);
        ReleaseDC(hDlg, hdc);
        MoveWindow(hDlg, main_x + 100, main_y + 5, lpSize.cx + 60, (lpSize.cy * 31) + 12, TRUE);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case (102):
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

// Handle several math errors caused by passing a negative argument
// to log or log10(_DOMAIN errors). When this happens, _matherr
// returns the natural or base-10 logarithm of the absolute value
// of the argument and suppresses the usual error message.
int _matherr(struct _exception *except)
{
    // Handle _OVERFLOW and _UNDERFLOW
    if (except->type == _OVERFLOW)
    {
        MessageBox(calcMainWindow, "Excalibur Function Overflow...", "Excalibur Floating Point", MB_OK);
        except->retval = 0.0;
        endRunningMacro();
        return 1;
    }
    else if (except->type == _UNDERFLOW)
    {
        MessageBox(calcMainWindow, "Excalibur Function Underflow...", "Excalibur Floating Point", MB_OK);
        except->retval = 0.0;
        endRunningMacro();
        return 1;
    }
    else if (except->type == _DOMAIN)
    {
        MessageBox(calcMainWindow, "Excalibur Function Argument Domain Error...", "Excalibur Floating Point", MB_OK);
        except->retval = 0.0;
        endRunningMacro();
        return 1;
    }
    else if (except->type == _TLOSS)
    {
        MessageBox(calcMainWindow,
                   "Excalibur Function - Total Loss of Significance...", "Excalibur Floating Point", MB_OK);
        except->retval = 0.0;
        endRunningMacro();
        return 1;
    }
    else
    {
        return 0; // Else use the default actions
    }
}

void SetNumLock(BOOL bState)
{
    GetKeyboardState((LPBYTE)&keyState);
    if ((bState && !(keyState[VK_NUMLOCK] & 1)) ||
        (!bState && (keyState[VK_NUMLOCK] & 1)))
    {
        // Simulate a key press
        keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);

        // Simulate a key release
        keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}

void turnOnNumLock(void)
{
    OSVERSIONINFO ver;
    int retVal;

    ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    retVal = GetVersionEx((LPOSVERSIONINFO)&ver);
    if ((retVal == 0) || ver.dwMajorVersion > 4) // Windows 2k or XP, etc.
    {
        SetNumLock(TRUE);
    }
    else // Windows 9x
    {
        GetKeyboardState(keyState);
        keyState[VK_NUMLOCK] = (char)0x81;
        SetKeyboardState(keyState);
    }
}

void trim(char *str)
{
    char *start = str;
    char *end;

    // 1. Move 'start' pointer forward to skip leading whitespace
    while (isspace((unsigned char)*start))
    {
        start++;
    }

    // 2. If the string is all whitespace, null-terminate and return
    if (*start == 0)
    {
        *str = '\0';
        return;
    }

    // 3. Find the end of the string and move backward to skip trailing whitespace
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }

    // 4. Write new null terminator after the last non-space character
    *(end + 1) = '\0';

    // 5. Shift the trimmed string back to the beginning of the original buffer
    if (start != str)
    {
        memmove(str, start, end - start + 2);
    }
}

#define INT64_MAX_PRE_DIV (INT64_MAX / 10)
#define UINT64_MAX_PRE_DIV (UINT64_MAX / 10)

// Helper function to convert a character to its integer value based on base
static int char_to_val(char c, int base)
{
    int val = -1;
    if (isdigit((unsigned char)c))
    {
        val = c - '0';
    }
    else if (isalpha((unsigned char)c))
    {
        val = tolower((unsigned char)c) - 'a' + 10;
    }
    return (val < base) ? val : -1;
}

// Custom strtou64 implementation
uint64_t strtou64(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    uint64_t acc = 0;
    int any = 0;
    int neg = 0;
    uint64_t cutoff;
    uint64_t cutlim;

    if (base < 0 || base == 1 || base > 36)
    {
        if (endptr)
            *endptr = (char *)nptr;
        errno = EINVAL;
        return 0;
    }

    // Skip leading whitespace
    while (isspace((unsigned char)*s))
    {
        s++;
    }

    // Handle optional sign
    if (*s == '-')
    {
        neg = 1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    // Determine or validate base
    if ((base == 0 || base == 16) && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
    {
        s += 2;
        base = 16;
    }
    else if (base == 0)
    {
        if (*s == '0')
        {
            base = 8;
        }
        else
        {
            base = 10;
        }
    }

    // Calculate overflow thresholds
    cutoff = UINT64_MAX / (uint64_t)base;
    cutlim = UINT64_MAX % (uint64_t)base;

    // Parse the digits
    for (;; s++)
    {
        int c = char_to_val(*s, base);
        if (c < 0)
        {
            break;
        }

        if (any < 0)
        {
            // Already overflowed, just skip remaining valid digits
            continue;
        }

        // Check for overflow
        if (acc > cutoff || (acc == cutoff && (uint64_t)c > cutlim))
        {
            any = -1;
            acc = UINT64_MAX;
            errno = ERANGE;
        }
        else
        {
            any = 1;
            acc = acc * base + c;
        }
    }

    // Handle negative unsigned mapping (standard C behavior)
    if (neg && any > 0)
    {
        acc = (uint64_t)(-((int64_t)acc));
    }

    if (endptr)
    {
        *endptr = (char *)(any ? s : nptr);
    }

    return acc;
}

// Custom strtoi64 implementation
int64_t strtoi64(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    uint64_t acc = 0;
    int any = 0;
    int neg = 0;
    uint64_t limit;
    uint64_t cutoff;
    uint64_t cutlim;

    if (base < 0 || base == 1 || base > 36)
    {
        if (endptr)
            *endptr = (char *)nptr;
        errno = EINVAL;
        return 0;
    }

    while (isspace((unsigned char)*s))
    {
        s++;
    }

    if (*s == '-')
    {
        neg = 1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    if ((base == 0 || base == 16) && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
    {
        s += 2;
        base = 16;
    }
    else if (base == 0)
    {
        if (*s == '0')
        {
            base = 8;
        }
        else
        {
            base = 10;
        }
    }

    // Use absolute limit calculations depending on the sign to prevent signed overflow
    limit = neg ? (uint64_t)-(INT64_MIN + 1) + 1 : (uint64_t)INT64_MAX;
    cutoff = limit / (uint64_t)base;
    cutlim = limit % (uint64_t)base;

    for (;; s++)
    {
        int c = char_to_val(*s, base);
        if (c < 0)
        {
            break;
        }

        if (any < 0)
        {
            continue;
        }

        if (acc > cutoff || (acc == cutoff && (uint64_t)c > cutlim))
        {
            any = -1;
            acc = neg ? (uint64_t)INT64_MIN : (uint64_t)INT64_MAX;
            errno = ERANGE;
        }
        else
        {
            any = 1;
            acc = acc * base + c;
        }
    }

    if (endptr)
    {
        *endptr = (char *)(any ? s : nptr);
    }

    if (any < 0)
    {
        return (int64_t)acc;
    }

    return neg ? -(int64_t)acc : (int64_t)acc;
}
