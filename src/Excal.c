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
#include <dos.h>
#include <mmsystem.h>
#include <ctype.h>
#include <float.h>
#include <signal.h>
#include <shlobj.h>
#include "EXCAL.h"

#define WINDOW_TITLE "Excalibur 32-bit"

#define ABOUT_MSG "Excalibur for Windows 32-bit\n"                      \
                  "Version 3.XX-01  -  April 26, 2026\n\n"              \
                  "Copyright 1994-2026 David Bernazzani\n\n"            \
                  "Please read the disclaimer and understand the\n"     \
                  "accuracy and precision issues before using.\n\n"     \
                  "Excalibur is freeware - but see the github page\n"   \
                  "if you want to donate to support the effort.\n\n"    \
                  "https://github.com/wavemotion-dave/Excalibur"

#define CONFIG_VERSION_MAIN     0xF002      // If this changes, we wipe EVERYTHING
#define CONFIG_VERSION_SUB      0xF001      // If this changes, we reset x,y window position and reset constant tables (currency, physics constants, etc)

#define END_OF_PROGRAM_STR "<End Of Program>"

int16_t  playBackSave[MAX_MACROS][MAX_REC_PLAYBACK + 1];
int16_t  playBackIdxSave[MAX_MACROS];
uint8_t  recModeON = 0;
uint8_t  macroPlayback = FALSE;      /* TRUE if macro playback in progress */

int16_t  playBack[MAX_REC_PLAYBACK + 1];
int16_t  playBackIdx = 0;
int16_t  currentMacroPlaybackIdx = 0;
uint8_t  showTrace = FALSE;
uint8_t  toolTipCounter = 0;
uint8_t  bExactFont = TRUE;
int16_t  MacroStack[MAX_MACRO_STACK];
int16_t  MacroStackIdx = 0;
uint32_t macroFlags = 0x00000000;

uint32_t wordSize = 32;
uint32_t wordSizeMask = 0xFFFFFFFF;
uint8_t  padZeros = PROG_NOPADZEROS;
uint8_t  wordMode = PROG_SIGNED;
uint8_t  numberDisplayMode = INTERNATIONAL;

uint16_t progDelayValue = 1000;

int16_t  totalMappedButtonFuncs = 0;
uint32_t lastTickCount = 0;
uint32_t ticksUsed = 0;

uint8_t  ClearStackOnExit = 0;
uint8_t  RPL_entry = 0;
uint8_t  progModecarry = 0;
uint8_t  rightAlignStack = 0;
uint8_t  showXMinimized = 0;
uint8_t  traceMacroPlayback = FALSE;
int32_t  lastChosenMacro = 0;
uint8_t  showTime24HourFormat = FALSE;
uint8_t  alwaysOnTop = 0;

char     macroName[MAX_MACROS][51];
char     macro_short_names[MAX_MACROS][7];
char     clipboardBuffer[MAX_IMPORT_CLIPBOARD_SIZE+1];
char     helpTitle[64];
char     helpMsg[256];
char     statusBar[32];
char     tmpStr[256];
BYTE     keyState[256];

// -------------------------------------
// Buffers for editing the X register
// -------------------------------------
uint8_t  Xedit = X_NEW;
char     Xstr[50];

/* ---------------- */
/* Global registers */
/* ---------------- */
double X;                       /* Main register X */
double Y;                       /* Main register Y */
double Z;                       /* Main register Z */
double T;                       /* Main Register T */
double A;                       /* Extended Stack A */
double B;                       /* Extended Stack B */
double C;                       /* Extended Stack C */
double D;                       /* Extended Stack D */
double LASTX;                   /* LAST X register */
double LASTY;                   /* LAST Y register */

PROG_LONG XL;                   /* The main register X when in Comp-Sci mode */
PROG_LONG YL;                   /* The main register Y when in Comp-Sci mode */
PROG_LONG ZL;                   /* The main register Z when in Comp-Sci mode */
PROG_LONG TL;                   /* The main register T when in Comp-Sci mode */
PROG_LONG AL;                   /* The extended stack A when in Comp-Sci mode */
PROG_LONG BL;                   /* The extended stack B when in Comp-Sci mode */
PROG_LONG CL;                   /* The extended stack C when in Comp-Sci mode */
PROG_LONG DL;                   /* The extended stack D when in Comp-Sci mode */
PROG_LONG LASTXL;               /* LAST X when in Comp-Sci mode */
PROG_LONG LASTYL;               /* LAST Y when in Comp-Sci mode */

// Some statistics registers
uint64_t stackPushes = 0;           // Total number of Stack Pushes
uint64_t stackPops = 0;             // Total number of Stack Pops
uint32_t inFocusTime = 0;           // Number of minutes Excalibur window in 'focus'

uint8_t  AngleMode = 0;             // 0=Degrees, 1=Radians, 2=Gradients
uint8_t  commaMode = 1;             // 0=International, 1=American
uint8_t  eexMode = 1;               // 0=EEX, 1=E
uint8_t  numLockMode = 1;           // Turn on NumLock when program starts?
uint8_t  toolTipsOn = 1;            // Enable tooltips?
uint8_t  extendedStack = 0;         // Standard Stack is 4 deep. Extended is 8 deep.
uint8_t  popFillZero = 0;           // T register fills with zero?
int32_t  lastChosenConst = 0;       // Last chosen constant
int32_t  lastConstBank = 0;         // Last chosen constant bank
int32_t  decimal_places = 12;       // Default decimal places
uint8_t  sci_format = 'g';          // Default scientific display format
uint32_t indirectRegister = 0;      // For programming - (i) register
uint8_t  progMode = PROG_NORMAL;    // Normal floating-point mode
uint8_t  helpMode = 0;              // Used to determine if next key or button hit is for help

double STO[MAX_STO];                // Storage registers R0-R25
char STOlabels[MAX_STO][9];         // Labels associated with the R0-R25 registers
double SUM[SUM_MAX];                // Statistics registers for the Financial bank
char excaliburNotes[NOTES_SIZE];    // A small scratchpad for the user to jot down some info

struct playbackStruct playBackMap[MAX_FUNCTIONS + 1];

extern void Macro_BST(void);
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

extern void callButtonFunc(void(*routine) (void), char useFloatsLongs,
                            char allowRecord, int uniqueIndex, char saveLastX, char newXedit, int updateSpareBar);
extern void mapButtonFuncs(void);
void DoMacroSaveRecall(void);

struct funcStruct *currentFuncs = (struct funcStruct *) &Scientific_funcs;
struct funcStruct *lastFuncs = (struct funcStruct *) &Scientific_funcs;

uint32_t halfSecTimer = 0;

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

HWND        toolTipWnd;             // window handle from CreateWindow
HFONT       holdsfont;              // handle of original font
HFONT       hMainFont;              // handle of new font for most of the UI
HFONT       hNumberFont;            // handle of new font for the Stack Display (bigger, bolder)
HFONT       hFixedFont;             // handle of the new font for dialogs that need fixed pitch
DLGPROC     lpfnMainWndProc;        // Main window procedure/handler
HINSTANCE   hExcaliburInstance;     // The global instance of Excalibur (assigned by the OS)
HWND        calcMainWindow;         // A handle to the Main Window
HMENU       hMainMenu;              // A handle to the Main Menu

//Brushes that we need for this application.
static HBRUSH backgroundBrush;
static HBRUSH helpWindowBackgroundBrush;

void HelpAbout(void);
void ShowUsageStats(void);
void ProcessCusomSave(void);
void SetLastMenuType(int type);
WORD GetMouseHelp(WORD xPos, WORD yPos);
void init_key_pos(void);        // for tool tips

#ifdef __cplusplus
typedef void(*fptr) (int);
#else
typedef void(*fptr) ();
#endif

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

    hExcaliburInstance = hInstance;  // Set the global handle - we use this in a number of places

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

    PreInit();

    // This is the main Excalibur dialog window!
    calcMainWindow = CreateDialog(hInstance, "EXCALIBUR", 0, NULL);

    hMainMenu = GetMenu(calcMainWindow);

    ShowWindow(calcMainWindow, SW_HIDE);

    SetTimer(calcMainWindow, 1, 60000, NULL);   // 1 Minute Timer
    SetTimer(calcMainWindow, 2, 300,   NULL);   // 300ms Timer
    SetTimer(calcMainWindow, 3, 100,   NULL);   // 100ms Timer
    
    srand((unsigned) time(NULL));   // Ensure random numbers are somewhat random!

    // Add the Excalibur Settings into the main menu
    tmpMenuHandle = GetSystemMenu(calcMainWindow, FALSE);
    AppendMenu(tmpMenuHandle, MF_SEPARATOR, 0, NULL);
    AppendMenu(tmpMenuHandle, MF_STRING, IDM_SETTINGS, "Excalibur Settings...");

    // The X Y Z T stack values get a slightly larger/bolder font...
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_X), WM_SETFONT, (WPARAM) hNumberFont, FALSE);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_Y), WM_SETFONT, (WPARAM) hNumberFont, FALSE);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_Z), WM_SETFONT, (WPARAM) hNumberFont, FALSE);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STACK_T), WM_SETFONT, (WPARAM) hNumberFont, FALSE);

    // -------------------------------------------------------------------------------------------
    // Every other control gets the standard hMainFont by default... Note, many of these control
    // IDs don't actually exist... but there is no harm in running through the list this way.
    // -------------------------------------------------------------------------------------------
    for (id = RPN_START_OF_LIST; id < RPN_END_OF_LIST; id++)
    {
        SendMessage(GetDlgItem(calcMainWindow, id), WM_SETFONT, (WPARAM) hMainFont, FALSE);
    }

    // -----------------------------------------------------
    // All of the 40 function buttons get the same font...
    // -----------------------------------------------------
    for (id = FN1; id <= FN40; id++)
    {
        SendMessage(GetDlgItem(calcMainWindow, id), WM_SETFONT, (WPARAM) hMainFont, FALSE);
    }

    Init();
    ShowWindow(calcMainWindow, iCmdShow);           // Now show the window the way it was asked to be displayed...
    CreateToolTipWindow(calcMainWindow, hInstance); // Create the tool-tip window that goes with buttons...
    CreateDebugWindow(calcMainWindow, hInstance);   // Create the debug window for program traceback, register view, etc.

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
    WNDCLASS     wndclass;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW;
     wndclass.lpfnWndProc   = helpWndProc;
     wndclass.cbClsExtra    = 0;
     wndclass.cbWndExtra    = 0;
     wndclass.hInstance     = hInstance;
     wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
     wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
     wndclass.hbrBackground = (HBRUSH) helpWindowBackgroundBrush;
     wndclass.lpszMenuName  = NULL;
     wndclass.lpszClassName = toolTipWndName;

     if (!RegisterClass(&wndclass))
     {
          MessageBox(NULL, TEXT("Unable to register the Excalibur Class with Windows.\nThis is a fatal error and the program will now exit."), toolTipWndName, MB_ICONERROR);
          return 0;
     }

     toolTipWnd = CreateWindow(toolTipWndName,        // window class name
                          TEXT("ExcalHelp"),          // window caption
                          WS_POPUP | WS_BORDER,       // window style
                          CW_USEDEFAULT,              // initial x position
                          CW_USEDEFAULT,              // initial y position
                          100,                        // initial x size -- tbd can probably make zero
                          50,                         // initial y size -- tbd same here...
                          hwnd,                       // parent window handle
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL);                      // creation parameters


     ShowWindow(toolTipWnd, SW_HIDE);
     toolTipCounter = 0;
     UpdateWindow(toolTipWnd);

    return 0;
}

// ---------------------------------------------------------------------------------------
// This is our top-level Excalibur window handler... Essentially this handles any 
// top-level functionality such as keyboard presses, global timers, window movement, etc.
// ---------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rcWindow;
    DWORD dw;
    WORD xPos, yPos;
    static WORD lastXpos, lastYpos;
    POINT pCursor;
    HDC hdc;
    RECT rc;                    /* window's screen coordinates      */
    POINT ptUpperLeft;          /* client coordinate of upper left  */
    POINT ptLowerRight;         /* client coordinate of lower right */
    SIZE lpSize;
    PAINTSTRUCT ps;
    int i;
    UINT flags;

    switch(iMsg)
    {
    case WM_CREATE:
        backgroundBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        helpWindowBackgroundBrush = CreateSolidBrush(GetSysColor(COLOR_INFOBK));
        SetUpFonts(hwnd);
        return 0;

    // We place Settings on the system command box!
    case WM_SYSCOMMAND:
        switch(LOWORD(wParam))
        {
            case(IDM_SETTINGS):
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
        if (macroPlayback == TRUE)      // If we get a playback over a playback we stop the playback!
        {
            endRunningMacro();
            ShowStatus();
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

        if (!LOWORD(lParam))
        {                       // Process Menu Commands
            switch(wParam)     // Determine which Menu ID
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
                    ClipboardCopySelection(hwnd, 0);
                }
                break;
            case IDM_COPYALLTOCLIPBOARD:
                {
                    ClipboardCopySelection(hwnd, 1);
                }
                break;
            case IDM_COPYCLIPBOARDTOX:
                {
                    ClipboardCopySelection(hwnd, 2);
                }
                break;
            case IDM_COPYMACROTOCLIPBOARD:
                {
                    ClipboardCopySelection(hwnd, 3);
                }
                break;
            case IDM_SCIENTIFIC:
                SelectNewFunc((struct funcStruct *) &Scientific_funcs);
                break;
            case IDM_FINANCIAL:
                SelectNewFunc((struct funcStruct *) &Financial_funcs);
                break;
            case IDM_CONVERSION:
                SelectNewFunc((struct funcStruct *) &Conversion_funcs);
                break;
            case IDM_COMPSCI:
                SelectNewFunc((struct funcStruct *) &CompSci_funcs);
                break;
            case IDM_GEOMETRY:
                SelectNewFunc((struct funcStruct *) &Geometry_funcs);
                break;
            case IDM_CUSTOM:
                SelectNewFunc((struct funcStruct *) &Custom_funcs);
                break;
            case IDM_STATS:
                SelectNewFunc((struct funcStruct *) &Statistics_funcs);
                break;
            case IDM_PHYSICS:
                SelectNewFunc((struct funcStruct *) &Physics_funcs);
                break;
            case IDM_PROGI:
                SelectNewFunc((struct funcStruct *) &Program1_funcs);
                break;
            case IDM_PROGII:
                SelectNewFunc((struct funcStruct *) &Program2_funcs);
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
            if (helpMode == 0)
            {
                ProcessKeyHit(wParam);
            }
            else
            {
                ProcessHelp(wParam);
            }
            SetFocus(calcMainWindow);
        }
        return 0;
        break;

    case WM_CONTEXTMENU:            // Right mouse button - context sensitive help!
        if (macroPlayback == TRUE)  // If we get a playback over a playback we stop the playback!
        {
            endRunningMacro();
            ShowStatus();
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

        xPos = LOWORD(lParam); // horizontal position of cursor
        yPos = HIWORD(lParam); // vertical position of cursor
        GetCursorPos(&pCursor);
        xPos = (WORD) pCursor.x;
        yPos = (WORD) pCursor.y;
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
                if (GetMouseHelp((WORD) pCursor.x, (WORD) pCursor.y) == 1)
                {
                    ShowWindow(toolTipWnd, SW_HIDE);
                    toolTipCounter = 0;
                    MessageBox(calcMainWindow, helpMsg, helpTitle, MB_OK | MB_ICONQUESTION);
                }
        }
        return FALSE;
        break;


    case WM_TIMER:
        if (wParam == 1)        // 60 second timer
        {
            if (GetFocus() == hwnd)
                inFocusTime++;
            UpdateTimeBar();
        }
        if (wParam == 3)        // 100 ms timer
        {
            ticksUsed = GetTickCount() - lastTickCount;
            lastTickCount += ticksUsed;
            if (userTimer == 1)
            {
                char tmpBuff[16];
                userTicks += ticksUsed;
                sprintf(tmpBuff, "%03d:%02d.%1d", (userTicks / 60000),
                         (userTicks / 1000) % 60, (userTicks / 100) % 10);
                SetDlgItemText(calcMainWindow, TIME_BAR, tmpBuff);
            }
            
            if (GetFocus() == calcMainWindow)
                if (IsWindowVisible(toolTipWnd))
                {
                    GetCursorPos(&pCursor);
                    xPos = (WORD) pCursor.x;
                    yPos = (WORD) pCursor.y;
                    if (xPos != lastXpos || yPos != lastYpos)
                    {
                        ShowWindow(toolTipWnd, SW_HIDE);
                        toolTipCounter = 0;
                    }
                }
        }
        if (wParam == 2)        // 500 ms timer
        {
            halfSecTimer++;
            GetCursorPos(&pCursor);
            xPos = (WORD) pCursor.x;
            yPos = (WORD) pCursor.y;
            ScreenToClient(hwnd, &pCursor);

            GetWindowRect(hwnd, &rc);
            ptUpperLeft.x = rc.left;
            ptUpperLeft.y = rc.top;
            ptLowerRight.x = rc.right;
            ptLowerRight.y = rc.bottom;
            ScreenToClient(hwnd, &ptUpperLeft);
            ScreenToClient(hwnd, &ptLowerRight);

            if ((pCursor.x >= 0 && pCursor.x <= ptLowerRight.x) && (pCursor.y >= 0 && pCursor.y <= ptLowerRight.y))
                if (!IsWindowVisible(toolTipWnd))
                    if (xPos == lastXpos && yPos == lastYpos)
                    {
                        if ((toolTipCounter++) >= 2)
                        {
                            if (GetFocus() == calcMainWindow)
                                if (GetMouseHelp((WORD) pCursor.x, (WORD) pCursor.y) == 1)
                                {
                                    hdc = GetDC(toolTipWnd);
                                    SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
                                    dw = GetTextExtentPoint32(hdc, helpTitle, strlen(helpTitle), &lpSize);
                                    ReleaseDC(toolTipWnd, hdc);
                                    MoveWindow(toolTipWnd, xPos - 1, yPos + 19, lpSize.cx + 10, lpSize.cy + 4, TRUE);
                                    if ((toolTipsOn == 1) && (traceMacroPlayback == FALSE) && (macroPlayback == FALSE))
                                    {
                                        ShowWindow(toolTipWnd, SW_SHOWNOACTIVATE);
                                    }
                                }
                        }
                    }
                    else
                    {
                        toolTipCounter = 0;
                    }
            lastXpos = xPos;
            lastYpos = yPos;
        }
        return 0;
        break;

    case WM_KEYDOWN:           //TBD - how to stop macro if playback and what do do about WM char message that will follow!
        if (macroPlayback == TRUE)
            break;
        switch((int) wParam)
        {
        case(VK_NUMLOCK):
            ShowStatus();
            break;

        case('0'):            // This handles CTRL-0 thru CTRL-9 for auto decimal place setting!
        case('1'):
        case('2'):
        case('3'):
        case('4'):
        case('5'):
        case('6'):
        case('7'):
        case('8'):
        case('9'):
            if (GetKeyState(VK_CONTROL) < 0)
            {
                if (sci_format == 'g')
                    sci_format = 'f';   // Place calculator in FIXed mode if in STD mode
                decimal_places = wParam - '0';
                ShowStack();
                ShowStatus();
            }
            break;

        case('V'):            // Paste
            if (GetKeyState(VK_CONTROL) < 0)
                ClipboardCopySelection(hwnd, 2);
            break;

        case('C'):            // Copy
            if (GetKeyState(VK_CONTROL) < 0)
                ClipboardCopySelection(hwnd, 0);
            break;

        case('A'):            // Copy All
            if (GetKeyState(VK_CONTROL) < 0)
                ClipboardCopySelection(hwnd, 1);
            break;

        case('S'):            // Settings
            if (GetKeyState(VK_CONTROL) < 0)
                GetUserSettings();
            break;

        case('P'):            // Program Manager
            if (GetKeyState(VK_CONTROL) < 0)
                DoMacroSaveRecall();
            break;

        case('D'):            // Define Custom Set
            if (GetKeyState(VK_CONTROL) < 0)
                cust_define();
            break;

        case('U'):            // Program Useage Stats
            if (GetKeyState(VK_CONTROL) < 0)
            {
                //ShowUsageStats();
            }
            break;

        case('X'):            // Exit
            if (GetKeyState(VK_CONTROL) < 0)
                PostMessage(hwnd, WM_DESTROY, wParam, lParam);
            break;

        case(VK_F12):  // Recall Window - Center it
        case(VK_HOME):
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

        case(VK_UP):
        case(VK_DOWN):
            ProcessDirectKeyHit(wParam);
            break;


        case(VK_F1):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN4);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN3);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN2);
            else
                ProcessKeyHit(FN1);
            break;
        case(VK_F2):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN8);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN7);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN6);
            else
                ProcessKeyHit(FN5);
            break;
        case(VK_F3):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN12);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN11);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN10);
            else
                ProcessKeyHit(FN9);
            break;
        case(VK_F4):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN16);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN15);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN14);
            else
                ProcessKeyHit(FN13);
            break;
        case(VK_F5):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN20);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN19);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN18);
            else
                ProcessKeyHit(FN17);
            break;
        case(VK_F6):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN24);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN23);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN22);
            else
                ProcessKeyHit(FN21);
            break;
        case(VK_F7):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN28);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN27);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN26);
            else
                ProcessKeyHit(FN25);
            break;
        case(VK_F8):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN32);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN31);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN30);
            else
                ProcessKeyHit(FN29);
            break;
        case(VK_F9):
            if ((GetKeyState(VK_SHIFT) < 0) && (GetKeyState(VK_CONTROL) < 0))
                ProcessKeyHit(FN36);
            else if (GetKeyState(VK_CONTROL) < 0)
                ProcessKeyHit(FN35);
            else if (GetKeyState(VK_SHIFT) < 0)
                ProcessKeyHit(FN34);
            else
                ProcessKeyHit(FN33);
            break;
        case(VK_F10):
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
        if (macroPlayback == TRUE)      // If we get a playback over a playback we stop the playback!
        {
            endRunningMacro();
            ShowStatus();
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }
        ProcessDirectKeyHit((int) wParam);
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
        i = GetWindowLong((HWND) LOWORD(lParam), (int) GWL_ID);

        if (i == RPN_STACK || i == RPN_STACK_X || i == RPN_STACK_Y || i == RPN_STACK_Z || i == RPN_STACK_T)
        {
            SetBkColor((HDC) wParam, GetSysColor(COLOR_WINDOW));
            SetTextColor((HDC) wParam, GetSysColor(COLOR_WINDOWTEXT));
            return((DWORD) backgroundBrush);
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


void SelectNewFunc(struct funcStruct *funcs)
{
    if (currentFuncs == (struct funcStruct *) &CompSci_funcs || progMode != PROG_NORMAL)
    {
        if ( (funcs != (struct funcStruct *) &Program1_funcs) && (funcs != (struct funcStruct *) &Program2_funcs) )
        {
            LongsToFloats();
            progMode = PROG_NORMAL;
            ShowStack();
            ShowStatus();
        }
    }
    CheckMenuItem(hMainMenu, IDM_SCIENTIFIC,    MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_COMPSCI,       MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_FINANCIAL,     MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_CONVERSION,    MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_PROGII,        MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_GEOMETRY,      MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_CUSTOM,        MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_STATS,         MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_PHYSICS,       MF_UNCHECKED);
    CheckMenuItem(hMainMenu, IDM_PROGI,         MF_UNCHECKED);

    SendMessage(GetDlgItem(calcMainWindow, RPN_SCI),     BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_STAT),    BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_FIN),     BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_CONV),    BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_GEOM),    BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_COMPSCI), BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_PHY),     BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_PROGI),   BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_PROGII),  BM_SETCHECK, (WORD) 0, (DWORD) 0L);
    SendMessage(GetDlgItem(calcMainWindow, RPN_CUST),    BM_SETCHECK, (WORD) 0, (DWORD) 0L);

    if (funcs == (struct funcStruct *) &Scientific_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_SCIENTIFIC, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_SCI), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_SCI_I);
    }
    if (funcs == (struct funcStruct *) &CompSci_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_COMPSCI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_COMPSCI), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_COMPSCI);
        FloatsToLongs();
        PROG_dec();
        ShowStack();
        ShowStatus();
    }
    if (funcs == (struct funcStruct *) &Financial_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_FINANCIAL, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_FIN), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_BUIS);
    }
    if (funcs == (struct funcStruct *) &Conversion_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_CONVERSION, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CONV), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CONV);
    }
    if (funcs == (struct funcStruct *) &Geometry_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_GEOMETRY, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_GEOM), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_GEOM);
    }
    if (funcs == (struct funcStruct *) &Custom_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_CUSTOM, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CUST), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CUSTOM);
    }
    if (funcs == (struct funcStruct *) &Statistics_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_STATS, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_STAT), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_STATS);
    }
    if (funcs == (struct funcStruct *) &Physics_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_PHYSICS, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PHY), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PHYSICS);
    }
    if (funcs == (struct funcStruct *) &Program1_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_PROGI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGI), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG1);
    }
    if (funcs == (struct funcStruct *) &Program2_funcs)
    {
        CheckMenuItem(hMainMenu, IDM_PROGII, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGII), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG2);
    }
    lastFuncs = currentFuncs;
    currentFuncs = (struct funcStruct *) funcs;
    processFuncs();
}


LRESULT CALLBACK helpWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc;
     PAINTSTRUCT ps;
     RECT        rect;

     switch(message)
     {
     case WM_CREATE:
          return 0;

     case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
        SetBkColor((HDC) hdc, GetSysColor(COLOR_INFOBK));
        SetTextColor((HDC) hdc, GetSysColor(COLOR_INFOTEXT));
        DrawText(hdc, TEXT(helpTitle), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &ps);
        return 0;

     case WM_DESTROY:
        return 0;
     }
     return DefWindowProc(hwnd, message, wParam, lParam);
}


int ClipboardCopySelection(HWND hwnd, int copytype)
{
    HGLOBAL tptr;
    HANDLE hMem;
    LPSTR lpMem;
    char far *cptr;
    char tmp2[32];
    char tmp3[32];
    int i, j, k;
    unsigned short chksum = 0x0000;

    if (copytype == 0)          // Copy X to clipboard
    {
        tptr = GlobalAlloc(GHND, (DWORD) 64L);
        cptr = GlobalLock(tptr);
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN);  // X register
        k = 0;                  // Strip leading spaces!
        while ((tmpStr[k] == ' ') && k < 20)
            k++;
        lstrcpy(cptr, (LPSTR) & tmpStr[k]);
        OpenClipboard(hwnd);
        EmptyClipboard();
        GlobalUnlock(tptr);
        SetClipboardData(CF_TEXT, tptr);
        CloseClipboard();
        Xedit = X_NEW;
        ShowStack();
        return(0);
    }
    if (copytype == 2)          //  Copy to X register!!!
    {
        OpenClipboard(hwnd);
        hMem = GetClipboardData(CF_TEXT);
        lpMem = GlobalLock(hMem);
        lstrcpyn(tmp2, lpMem, MAX_STACK_STRLEN);
        GlobalUnlock(hMem);
        CloseClipboard();
        tmp2[20] = '\0';
        j = 0;
        for (i = 0; i < (int) strlen(tmp2); i++)
        {
            if (numberDisplayMode == NONINTERNATIONAL)
            {
                if (tmp2[i] != '.') // Stip decimal points out....
                {
                    tmp3[j] = tmp2[i];
                    j++;
                }
            }
            else
            {
                if (tmp2[i] != ',') // Otherwise, Stip commas out...
                {
                    tmp3[j] = tmp2[i];
                    j++;
                }
            }
        }
        tmp3[j] = (char) NULL;
        if (numberDisplayMode == NONINTERNATIONAL) // Turn comma into dp
        {
            for (i=0; i<(int)strlen(tmp3); i++)
            {
                if (tmp3[i] == ',') // If we have a comma, make dp
                    tmp3[i] = '.';
            }
        }
        Xedit = X_NEW;
        if (progMode == PROG_NORMAL)
        {
            StackPush((double) atof(tmp3));
        }
        else
        {
            if (progMode == PROG_HEX)
            {
                StackPushL((PROG_LONG) strtol(tmp3, NULL, 16));
            }
            else
            {
                StackPushL((PROG_LONG) strtol(tmp3, NULL, 0));
            }
        }
        ShowStack();
        return(0);
    }
    if (copytype == 1)          // Copy All to clipboard
    {
        tptr = GlobalAlloc(GHND, (DWORD) 128L);
        cptr = GlobalLock(tptr);
        lstrcpy(cptr, "");
        GetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr, MAX_STACK_STRLEN);  // T register
        k = 0;                  // Strip leading spaces!
        while ((tmpStr[k] == ' ') && k < 20)
            k++;
        lstrcat(cptr, (LPSTR) & tmpStr[k]);
        lstrcat(cptr, (LPSTR) "\r\n");
        GetDlgItemText(calcMainWindow, RPN_STACK_Z, tmpStr, MAX_STACK_STRLEN);  // Z register
        k = 0;                  // Strip leading spaces!
        while ((tmpStr[k] == ' ') && k < 20)
            k++;
        lstrcat(cptr, (LPSTR) & tmpStr[k]);
        lstrcat(cptr, (LPSTR) "\r\n");
        GetDlgItemText(calcMainWindow, RPN_STACK_Y, tmpStr, MAX_STACK_STRLEN);  // Y register
        k = 0;                  // Strip leading spaces!
        while ((tmpStr[k] == ' ') && k < 20)
            k++;
        lstrcat(cptr, (LPSTR) & tmpStr[k]);
        lstrcat(cptr, (LPSTR) "\r\n");
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN);  // X register
        k = 0;                  // Strip leading spaces!
        while ((tmpStr[k] == ' ') && k < 20)
            k++;
        lstrcat(cptr, (LPSTR) & tmpStr[k]);
        lstrcat(cptr, (LPSTR) "\r\n");
        OpenClipboard(hwnd);
        EmptyClipboard();
        GlobalUnlock(tptr);
        SetClipboardData(CF_TEXT, tptr);
        CloseClipboard();
        Xedit = X_NEW;
        ShowStack();
        return(0);
    }
    if (copytype == 3)          // Copy current macro to clipboard
    {
        tptr = GlobalAlloc(GHND, (DWORD) MAX_IMPORT_CLIPBOARD_SIZE);
        cptr = GlobalLock(tptr);
        lstrcpy(cptr, "");

        for (i = 0; i < playBackIdx; i++)
        {
            sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
            lstrcat(cptr, (LPSTR) tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");
        }
        sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
        lstrcat(cptr, (LPSTR) tmpStr);
        lstrcat(cptr, (LPSTR) "\r\n");
        chksum = 0x0000;
        for (i = 0; i < playBackIdx; i++)
            chksum += playBack[i];
        sprintf(tmpStr, "Checksum: %04X", chksum);
        lstrcat(cptr, (LPSTR) "\r\n");
        lstrcat(cptr, (LPSTR) tmpStr);
        lstrcat(cptr, (LPSTR) "\r\n");

        OpenClipboard(hwnd);
        EmptyClipboard();
        GlobalUnlock(tptr);
        SetClipboardData(CF_TEXT, tptr);
        CloseClipboard();
        Xedit = X_NEW;
        ShowStack();
        return(0);
    }
    return(0);
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
        char lpszFacename[256+1];
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
    return(font);
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
    return(font);
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
    return(font);
}

void SetUpFonts(HWND hwnd)
{
    HDC hDC;

    // Get the display context.
    hDC = GetDC(hwnd);

    hMainFont     = GetSystemFont();        // Get the font used for buttons and most UI elements (proportional font)
    hNumberFont   = GetMainNumberFont();    // Get the font used for the main stack display of numbers(slightly bigger/bolder)
    hFixedFont    = GetSystemFontFixed();   // Get the font used for various dialog boxes where the text needs to be aligned in columns

    // Install the font in the current display context.
    holdsfont = SelectObject(hDC, hMainFont);

    // Release the display context.
    ReleaseDC(calcMainWindow, hDC);
}

void NotImp(void)
{
    MessageBox(calcMainWindow, "Function is not implemented yet...", "Excalibur", MB_OK | MB_ICONINFORMATION);
}

// --------------------------------------------------------
// Used when we switch into one of the 'Comp Sci' modes...
// --------------------------------------------------------
void FloatsToLongs(void)
{
    if (X <= (float) 0xFFFFFFFF)
        XL = (PROG_LONG) X;
    else
        XL = 0xFFFFFFFF;

    if (Y <= (float) 0xFFFFFFFF)
        YL = (PROG_LONG) Y;
    else
        YL = 0xFFFFFFFF;

    if (Z <= (float) 0xFFFFFFFF)
        ZL = (PROG_LONG) Z;
    else
        ZL = 0xFFFFFFFF;

    if (T <= (float) 0xFFFFFFFF)
        TL = (PROG_LONG) T;
    else
        TL = 0xFFFFFFFF;

    if (extendedStack)
    {
        if (A <= (float) 0xFFFFFFFF)
            AL = (PROG_LONG) A;
        else
            AL = 0xFFFFFFFF;

        if (B <= (float) 0xFFFFFFFF)
            BL = (PROG_LONG) B;
        else
            BL = 0xFFFFFFFF;

        if (C <= (float) 0xFFFFFFFF)
            CL = (PROG_LONG) C;
        else
            CL = 0xFFFFFFFF;

        if (D <= (float) 0xFFFFFFFF)
            DL = (PROG_LONG) D;
        else
            DL = 0xFFFFFFFF;
    }

    if (LASTX <= (float) 0xFFFFFFFF)
        LASTXL = (PROG_LONG) LASTX;
    else
        LASTXL = 0xFFFFFFFF;

    if (LASTY <= (float) 0xFFFFFFFF)
        LASTYL = (PROG_LONG) LASTY;
    else
        LASTYL = 0xFFFFFFFF;
}

// --------------------------------------------------------
// Used when we switch out one of the 'Comp Sci' modes...
// --------------------------------------------------------
void LongsToFloats(void)
{
    if (wordMode == PROG_SIGNED)
    {
        X = (double) (PROG_SIGNEDLONG) XL;
        Y = (double) (PROG_SIGNEDLONG) YL;
        Z = (double) (PROG_SIGNEDLONG) ZL;
        T = (double) (PROG_SIGNEDLONG) TL;
        A = (double) (PROG_SIGNEDLONG) AL;
        B = (double) (PROG_SIGNEDLONG) BL;
        C = (double) (PROG_SIGNEDLONG) CL;
        D = (double) (PROG_SIGNEDLONG) DL;
        LASTX = (double) (PROG_SIGNEDLONG) LASTXL;
        LASTY = (double) (PROG_SIGNEDLONG) LASTYL;
    }
    else
    {
        X = (double) XL;
        Y = (double) YL;
        Z = (double) ZL;
        T = (double) TL;
        A = (double) AL;
        B = (double) BL;
        C = (double) CL;
        D = (double) DL;
        LASTX = (double) LASTXL;
        LASTY = (double) LASTYL;
    }
}

int PreInit(void)
{
    int i, j, k;

    X = 0.0;
    Y = 0.0;
    Z = 0.0;
    T = 0.0;
    A = 0.0;
    B = 0.0;
    C = 0.0;
    D = 0.0;

    for (i = 0; i < MAX_STO; i++)
    {
        STO[i] = 0.0;
        strcpy(STOlabels[i], "no label");
    }

    for (i = 0; i < MAX_MACROS; i++)
    {
        strcpy(macroName[i], "Not Currently Defined");
        sprintf(macro_short_names[i], "P%02d", i + 1);
        playBackIdxSave[i] = 0;
    }
    playBackIdx = 0;
    currentMacroPlaybackIdx = 0;

    // Since we don't want to pre-init the whole constants table we look for first FALSE entry and mark the rest FALSE!
    // We read them in from disk again below so this won't harm anything to do it here!
    for (k = 0; k < MAX_CONST_BANKS; k++)
    {
        for (i = 0; i < MAX_CONSTS; i++)
        {
            if (constants[k][i].includeInList == FALSE)
            {
                for (j = i; j < MAX_CONSTS; j++)
                    constants[k][j].includeInList = FALSE;
                break;
            }
        }
    }

    // Get regional time setting for 24-hour format!
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, tmpStr, 5);
    tmpStr[4] = '\0';
    if (atoi(tmpStr) == 1)
        showTime24HourFormat = TRUE;

    // Get regional comma format...
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, tmpStr, 5);
    if (tmpStr[0] == ',')
        numberDisplayMode = NONINTERNATIONAL;
    return(0);
}

int Init(void)
{
    UINT flags;

    UpdateTimeBar();
    UpdateSpareBar("    ");
    ShowFunctionBar(FUNC_BAR_TEXT_SCI_I);
    ReadFromDisk();

    mapButtonFuncs();
    ShowStack();
    processFuncs();
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

/* -------------------------------------------------------- */
/* Turn on the num lock automatically for this application. */
/* -------------------------------------------------------- */
    if (numLockMode == 1)
    {
        turnOnNumLock();
    }

/* -------------------------------------------------------- */
/* If we need to remap the +/- and the E keys, do so now... */
/* -------------------------------------------------------- */
    if (eexMode == 1)
    {
        SetDlgItemText(calcMainWindow, RPN_NEGATE, "CHS");
        SetDlgItemText(calcMainWindow, RPN_E, "EEX");
    }
    if (numberDisplayMode == NONINTERNATIONAL)
    {
        SetDlgItemText(calcMainWindow, RPN_DIGIT_DP, ",");
    }
    return(0);
}

int ShowStatus(void)
{
    if (progMode != PROG_NORMAL)
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



    if (progMode != PROG_NORMAL)
    {
        if (wordMode == PROG_SIGNED)
            sprintf(tmpStr, "S");
        else
            sprintf(tmpStr, "U");

        if (wordSize == 8)
            strcat(tmpStr, "8  ");
        else if (wordSize == 16)
            strcat(tmpStr, "16 ");
        else
            strcat(tmpStr, "32 ");

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

    return(0);
}

char functionBar[60];

int UpdateSpareBar(char *msg)
{
    SetDlgItemText(calcMainWindow, SPARE_BAR, msg);
    return(0);
}

int UpdateTimeBar()
{
    int temp_hour, temp_min;

    _strtime(tmpStr);
    tmpStr[5] = CNULL;
    tmpStr[2] = CNULL;
    temp_hour = atoi(tmpStr);
    temp_min = atoi(&tmpStr[3]);

    if (showTime24HourFormat == TRUE)
    {
        sprintf(tmpStr, "%2d:%02d ", temp_hour, temp_min);
    }
    else
    {
        if (temp_hour < 12)
        {
            if (temp_hour == 0)
                temp_hour = 12; //12AM!!
            sprintf(tmpStr, "%2d:%02d AM", temp_hour, temp_min);
        }
        else if (temp_hour == 12)
            sprintf(tmpStr, "%2d:%02d PM", temp_hour, temp_min);
        else
            sprintf(tmpStr, "%2d:%02d PM", temp_hour - 12, temp_min);
    }

    if (userTimer == 0)         // Only if we don't have stopwatch!
        SetDlgItemText(calcMainWindow, TIME_BAR, tmpStr);

    return(0);
}

int ShowFunctionBar(char *msg)
{
    sprintf(functionBar, "%s", msg);
    SetDlgItemText(calcMainWindow, FUNC_BAR, functionBar);
    return(0);
}

void RPN_error(char *msg)
{
    MessageBox(calcMainWindow, msg, "Excalibur Calculation Error", MB_OK | MB_ICONEXCLAMATION);
}

void RPN_help(void)
{
    helpMode ^= 1;
    if (helpMode == 0)
        UpdateSpareBar("    ");
    else
        UpdateSpareBar("HELP");
}

double MakeAccurate(double val)
{
    unsigned char str[50];

    sprintf(str, "%.13g", val);
    val = atof(str);
    return(val);
}

// clang-format off
struct keypadStruct RPNkeys[] = {
    {RPN_DIGIT_0,   UNI_DIG0,   USES_FL, ALLOWREC, '0', NO_L,   X_EDIT,     RPN_digit0,         "Digit 0",              "Used in keypad number entry."},
    {RPN_DIGIT_1,   UNI_DIG1,   USES_FL, ALLOWREC, '1', NO_L,   X_EDIT,     RPN_digit1,         "Digit 1",              "Used in keypad number entry."},
    {RPN_DIGIT_2,   UNI_DIG2,   USES_FL, ALLOWREC, '2', NO_L,   X_EDIT,     RPN_digit2,         "Digit 2",              "Used in keypad number entry."},
    {RPN_DIGIT_3,   UNI_DIG3,   USES_FL, ALLOWREC, '3', NO_L,   X_EDIT,     RPN_digit3,         "Digit 3",              "Used in keypad number entry."},
    {RPN_DIGIT_4,   UNI_DIG4,   USES_FL, ALLOWREC, '4', NO_L,   X_EDIT,     RPN_digit4,         "Digit 4",              "Used in keypad number entry."},
    {RPN_DIGIT_5,   UNI_DIG5,   USES_FL, ALLOWREC, '5', NO_L,   X_EDIT,     RPN_digit5,         "Digit 5",              "Used in keypad number entry."},
    {RPN_DIGIT_6,   UNI_DIG6,   USES_FL, ALLOWREC, '6', NO_L,   X_EDIT,     RPN_digit6,         "Digit 6",              "Used in keypad number entry."},
    {RPN_DIGIT_7,   UNI_DIG7,   USES_FL, ALLOWREC, '7', NO_L,   X_EDIT,     RPN_digit7,         "Digit 7",              "Used in keypad number entry."},
    {RPN_DIGIT_8,   UNI_DIG8,   USES_FL, ALLOWREC, '8', NO_L,   X_EDIT,     RPN_digit8,         "Digit 8",              "Used in keypad number entry."},
    {RPN_DIGIT_9,   UNI_DIG9,   USES_FL, ALLOWREC, '9', NO_L,   X_EDIT,     RPN_digit9,         "Digit 9",              "Used in keypad number entry."},
    {RPN_DIGIT_DP,  UNI_DIGDP,  USES_FL, ALLOWREC, '.', NO_L,   X_EDIT,     RPN_dp,             "Decimal Point",        "Used in keypad number entry and can also be used to enter fractions into the X display (press twice)"},
    {RPN_CLEAR_X,   UNI_CLX,    USES_FL, ALLOWREC,  9,  NO_L,   X_NULL,     RPN_clearX,         "Clear X",              "Used to clear the X Register contents."},
    {RPN_DIVIDE,    UNI_DIV,    USES_FL, ALLOWREC, '/', YES_L,  X_NEW,      RPN_divide,         "Divide",               "Division of Y by X(Y/X)"},
    {RPN_MULTIPLY,  UNI_MUL,    USES_FL, ALLOWREC, '*', YES_L,  X_NEW,      RPN_multiply,       "Multiply",             "Multiplication of Y and X"},
    {RPN_SUBTRACT,  UNI_SUB,    USES_FL, ALLOWREC, '-', YES_L,  X_NEW,      RPN_minus,          "Minus",                "Subtraction of X from Y(Y-X)"},
    {RPN_PLUS,      UNI_PLUS,   USES_FL, ALLOWREC, '+', YES_L,  X_NEW,      RPN_plus,           "Plus",                 "Addition of X and Y"},
    {RPN_ENTER,     UNI_ENT,    USES_FL, ALLOWREC, 13,  NO_L,   X_NULL,     RPN_enter,          "Enter",                "Used to separate numbers in the RPN stack"},
    {RPN_EXCH_X_Y,  UNI_XCH,    USES_FL, ALLOWREC, 'x', NO_L,   X_NEW,      RPN_exchange_x_y,   "Exchange X and Y",     "Exchanges the contents of the X and Y registers"},
    {RPN_NEGATE,    UNI_CHS,    USES_FL, ALLOWREC, 'n', NO_L,   X_NULL,     RPN_negate_x,       "Change Sign",          "Used to change the sign of X"},
    {RPN_E,         UNI_E,      USES_FL, ALLOWREC, 'E', NO_L,   X_NULL,     RPN_Ex,             "Exponent",             "Used to produce an exponential number(e.g. 3.45e+12)"},
    {RPN_STO,       UNI_STO,    USES_FL, NORECORD, 'S', NO_L,   X_NEW,      RPN_store,          "Store Number",         "Used to store the current X number to one of 26 registers"},
    {RPN_RCL,       UNI_RCL,    USES_FL, NORECORD, 'R', NO_L,   X_NEW,      RPN_recall,         "Recall Number",        "Used to recall a number from one of 26 registers to the X register"},
    {RPN_R_UP,      UNI_RUP,    USES_FL, ALLOWREC, 38,  NO_L,   X_NEW,      RPN_rotateStackUp,  "Rotate Stack Up",      "Rotates the contents of the stack up"},
    {RPN_R_DN,      UNI_RDN,    USES_FL, ALLOWREC, 40,  NO_L,   X_NEW,      RPN_rotateStackDn,  "Rotate Stack Down",    "Rotates the contents of the stack down"},
    {RPN_LASTX,     UNI_LSTX,   USES_FL, ALLOWREC, 'L', NO_L,   X_NEW,      RPN_lastX,          "Last X",               "Retrieves the last value of X before the last operation occurred"},
    {RPN_MODE,      UNI_MODE,   USES_FL, ALLOWREC, 'M', NO_L,   X_NEW,      RPN_mode,           "Select Mode",          "Used to select number format mode"},
    {RPN_BKSP,      UNI_BKSP,   USES_FL, ALLOWREC,  8,  NO_L,   X_NULL,     RPN_backspace,      "Backspace",            "Used to correct mistakes in number entry"},
    {RPN_HELP,      UNI_HELP,   USES_FL, ALLOWREC, 'h', NO_L,   X_NULL,     RPN_help,           "Help",                 "After clicking this key, select another key for individual key help.\nSame as right-click of the mouse on any key."},
    {RPN_PLAYBACK,  UNI_PLAY,   USES_FL, NORECORD, 'p', NO_L,   X_NEW,      RPN_Playback,       "Playback",             "Plays back the last recorded sequence of button presses."},
    {RPN_CLEAR_ALL, UNI_CLRA,   USES_FL, ALLOWREC, 'C', YES_L,  X_ENTER,    RPN_clear,          "Clear Stack",          "Used to clear the entire stack contents."},
    {RPN_RCL0,      UNI_RCLA,   USES_FL, NORECORD, 'a', YES_L,  X_NEW,      RPN_recallA,        "Recall R0",            "Recalls the R0 register stored using the STO key."},
    {RPN_EDIT,      UNI_EDIT,   USES_FL, ALLOWREC, ' ', NO_L,   X_NULL,     RPN_edit,           "Edit X Register",      "Used to place the X register back in edit mode if it is not already."},
    {RPN_RCL1,      UNI_RCLB,   USES_FL, NORECORD, 'b', YES_L,  X_NEW,      RPN_recallB,        "Recall R1",            "Recalls the R1 register stored using the STO key."},
    {RPN_STO0,      UNI_STOA,   USES_FL, NORECORD, ' ', YES_L,  X_NEW,      RPN_storeA,         "Store R0",             "Stores the R0 register shortcut for the STO key."},
    {RPN_STO1,      UNI_STOB,   USES_FL, NORECORD, ' ', YES_L,  X_NEW,      RPN_storeB,         "Store R1",             "Stores the R1 register shortcut for the STO key."},
    {RPN_DROP,      UNI_DROP,   USES_FL, ALLOWREC, 'd', YES_L,  X_NEW,      RPN_drop,           "Drop Stack",           "Drops the X register and the rest of stack shifts down."},
    {RPN_LARG,      UNI_LARG,   USES_FL, ALLOWREC, ' ', NO_L,   X_NEW,      RPN_larg,           "Last Arguments",       "Retrieves the last X and Y pair before last operation."},
    {RPN_FRAC,      UNI_FRAC,   USES_FL, ALLOWREC, ' ', NO_L,   X_EDIT,     RPN_frac,           "Fraction Bar",         "Insert Fraction to current X edit"},
    {RPN_REC,       UNI_REC,    USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_Record,         "Record Mode On/Off",   "When ON - Records button presses for playback."},
    {RPN_NOTES,     UNI_NOTES,  USES_FL, ALLOWREC, ' ', NO_L,   X_NULL,     RPN_Notes,          "Excalibur Notepad",    "Allows some simple notes to be stored/saved."},
    {RPN_INV,       UNI_INVX,   USES_FL, ALLOWREC, ' ', YES_L,  X_NEW,      RPN_inverse,        "Inverse X",            "Computes the inverse of X"},
    {RPN_SCI,       UNI_SCI,    USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectSci,      "Select Scientific I",  "Selects the Scientific I Layout"},
    {RPN_PHY,       UNI_PHY,    USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectPhysics,  "Select Scientific II", "Selects the Scientific II Layout"},
    {RPN_FIN,       UNI_BUSI,   USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectBusi,     "Select Financial",     "Selects the Financial Layout"},
    {RPN_STAT,      UNI_STAT,   USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectStat,     "Select Statistics",    "Selects the Statistical Layout"},
    {RPN_CONV,      UNI_CONV,   USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectConv,     "Select Conversion",    "Selects the Conversion Layout"},
    {RPN_GEOM,      UNI_GEOM,   USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectGeom,     "Select Geometry",      "Selects the Geometry Layout"},
    {RPN_COMPSCI,   UNI_PROG1,  USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectProg,     "Select Comp Sci",      "Selects the Computer Science Layout"},
    {RPN_PROGI,     UNI_MAC,    USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectMacro,    "Select Program I",     "Selects Program Bank I"},
    {RPN_PROGII,    UNI_PROG2,  USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectProg2,    "Select Program II",    "Selects Program Bank II"},
    {RPN_CUST,      UNI_CUST,   USES_FL, NORECORD, ' ', NO_L,   X_NULL,     RPN_SelectCust,     "Select Custom",        "Selects the Custom Layout"},

    {RPN_LAST_KEY,  UNI_UNUSED, USES_FL, ALLOWREC, ' ', NO_L,   X_NEW,      NULL,               "Unused",               "Unused"}
};

struct keyPosStruct
{
    int index;
    int x;
    int y;
    int h;
    int w;
};

struct keyPosStruct RPNkeyPos[] = {
#if 0
    {RPN_DIGIT_0,   0,          0},
    {RPN_DIGIT_1,   0,          0},
    {RPN_DIGIT_2,   0,          0},
    {RPN_DIGIT_3,   0,          0},
    {RPN_DIGIT_4,   0,          0},
    {RPN_DIGIT_5,   0,          0},
    {RPN_DIGIT_6,   0,          0},
    {RPN_DIGIT_7,   0,          0},
    {RPN_DIGIT_8,   0,          0},
    {RPN_DIGIT_9,   0,          0},
    {RPN_DIGIT_DP,  0,          0},
    {RPN_CLEAR_X,   0,          0},
    {RPN_DIVIDE,    0,          0},
    {RPN_MULTIPLY,  0,          0},
    {RPN_SUBTRACT,  0,          0},
    {RPN_PLUS,      0,          0},
    {RPN_ENTER,     0,          0},
#endif
    {RPN_EXCH_X_Y,  0,          0},
    {RPN_NEGATE,    0,          0},
    {RPN_E,         0,          0},
    {RPN_STO,       0,          0},
    {RPN_RCL,       0,          0},
    {RPN_R_UP,      0,          0},
    {RPN_R_DN,      0,          0},
    {RPN_LASTX,     0,          0},
    {RPN_MODE,      0,          0},
    {RPN_BKSP,      0,          0},
    {RPN_HELP,      0,          0},
    {RPN_PLAYBACK,  0,          0},
    {RPN_CLEAR_ALL, 0,          0},
    {RPN_RCL0,      0,          0},
    {RPN_EDIT,      0,          0},
    {RPN_RCL1,      0,          0},
    {RPN_STO0,      0,          0},
    {RPN_STO1,      0,          0},
    {RPN_DROP,      0,          0},
    {RPN_LARG,      0,          0},
    {RPN_FRAC,      0,          0},
    {RPN_REC,       0,          0},
    {RPN_NOTES,     0,          0},
    {RPN_INV,       0,          0},
    {RPN_SCI,       0,          0},
    {RPN_STAT,      0,          0},
    {RPN_FIN,       0,          0},
    {RPN_CONV,      0,          0},
    {RPN_GEOM,      0,          0},
    {RPN_COMPSCI,   0,          0},
    {RPN_PROGII,    0,          0},
    {RPN_CUST,      0,          0},
    {RPN_PHY,       0,          0},
    {RPN_PROGI,     0,          0},
    {RPN_LAST_KEY,  0,          0}
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
        if (key == (WPARAM) currentFuncs[i].index)
        {
            if (currentFuncs[i].routine != NULL)
            {
                callButtonFunc(currentFuncs[i].routine,
                                currentFuncs[i].useFloatsLongs,
                                currentFuncs[i].allowRecord,
                                currentFuncs[i].uniqueIndex, currentFuncs[i].saveLastX, currentFuncs[i].newXedit, TRUE);
            }
        }
    }
    return(0);
}

int processFuncs()
{
    int i;

    for (i = 0; i < MAX_FUNCS; i++)
    {
        SendMessage(GetDlgItem(calcMainWindow, currentFuncs[i].index), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        SetDlgItemText(calcMainWindow, currentFuncs[i].index, currentFuncs[i].desc);
    }

    UpdateWindow(calcMainWindow);
    return(0);
}


int ProcessDirectKeyHit(WPARAM key)
{
    char keyStroke;
    int i, found;

    keyStroke = LOBYTE(key);

    if (numberDisplayMode == NONINTERNATIONAL && keyStroke == ',')      // Allow comma as DP seperator on keyboard...
        keyStroke = '.';

    found = 0;
    for (i = 0; i < MAX_FUNCS; i++)
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
        }
    }
    if (!found)                 // Now check the RPN keys themselves...
    {
        i = 0;
        while (RPNkeys[i].index != RPN_LAST_KEY)
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
            }
            i++;
        }
    }

    ShowStack();
    ShowStatus();
    return(0);
}

int ProcessHelp(WPARAM key)
{
    int i;
    char helpMsg[250];
    char helpTitle[55];

    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (key == (WPARAM) currentFuncs[i].index)
        {
            LoadString(hExcaliburInstance, currentFuncs[i].keyTitle, helpTitle, sizeof(helpTitle) - 1);
            LoadString(hExcaliburInstance, currentFuncs[i].keyHelp, helpMsg, sizeof(helpMsg) - 1);
            MessageBox(calcMainWindow, helpMsg, helpTitle, MB_OK | MB_ICONQUESTION);
            break;
        }
    }
    i = 0;
    while (RPNkeys[i].index != RPN_LAST_KEY)
    {
        if (key == (WPARAM) RPNkeys[i].index)
        {
            MessageBox(calcMainWindow, RPNkeys[i].keyHelp, RPNkeys[i].keyTitle, MB_OK | MB_ICONQUESTION);
            break;
        }
        i++;
    }
    helpMode = 0;
    UpdateSpareBar(" ");
    return(0);
}

int ProcessKeyHit(WPARAM key)
{
    int i;

    i = 0;
    if (key >= FN1 && key <= FN40)
        selectFuncs(key);
    else
        while (RPNkeys[i].index != RPN_LAST_KEY)
        {
            if (key == (WPARAM) RPNkeys[i].index)
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
    return(0);
}


char Radix(int progM)
{
    if (progM == PROG_DEC)
        return('d');
    if (progM == PROG_HEX)
        return('h');
    if (progM == PROG_BIN)
        return('b');
    if (progM == PROG_OCT)
        return('o');
    return('*');
}

char RadixBIN(int progM)         // Shows bin HI arrow!
{
    if (progM == PROG_BIN)
    {
        if (binMode == 1)
            return('»');    // Right Arrow!
        else
            return(' ');
    }

    return(' ');
}


void makeInternational(char *str)
{
    int j;
    if (numberDisplayMode == NONINTERNATIONAL)
    {
        for (j = 0; j < (int) strlen(str); j++)
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

    if ((commaMode != 0) && (strchr(str, '/') == NULL))    // No commas for fractions
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
        for (i = 0; i <= (int) strlen(str); i++)
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
    makeInternational(str);    // To swap commas and DPs if needed
}

void MakeEngineeringFormat(double val, char *Fstr)
{
    char engStr[50];
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

void MakeSciFormat(double val, char *Fstr)
{
    char sciStr[50];
    char str[50];
    int i;

    val = MakeAccurate(val);
    if (sci_format != 'g')
    {
        if (sci_format == 'Z')  // ENGINEERING FORMAT
        {
            MakeEngineeringFormat(val, Fstr);
        }
        else
        {
            sprintf(sciStr, "%%- .%d%c", decimal_places, sci_format);
            sprintf(str, sciStr, val);
            if (strlen(str) > MAX_DIGITS)
                sprintf(str, "%%- 21.%dg", decimal_places+1);
            else
                sprintf(str, "%%- 21.%d%c", decimal_places, sci_format);
        }
    }
    else
    {
        strcpy(str, "%- 21.13g");
    }
    if (sci_format != 'Z')      // ENGINEERING FORMAT
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
    char stackStr[50];

    if (recModeON == 1)    // Special record mode - show current program step in Z register!
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
    else
    if (showTrace == TRUE)    // Special record mode - show current program step in Z register!
    {
        if (currentMacroPlaybackIdx == playBackIdx)
        {
            sprintf(tmpStr, "%03d-<End Of Program>", currentMacroPlaybackIdx);
        }
        else
        {
            sprintf(tmpStr, "%03d-%s", currentMacroPlaybackIdx,
                     playBackMap[playBack[currentMacroPlaybackIdx]].funcText);
        }

        tmpStr[MAX_STACK_STRLEN] = CNULL;
        SetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr);
        SetDlgItemText(calcMainWindow, RPN_STACK_Z, " ");
    }

    if (progMode == PROG_NORMAL)
    {
        if (Xedit == X_EDIT)
        {
            sprintf(tmpStr, " %s_", Xstr);
            PutCommas(tmpStr);
        }
        else
            MakeSciFormat(X, tmpStr);

        if (rightAlignStack == 1)
        {
            sprintf(stackStr, (bExactFont ? "%24s":"%22s"), tmpStr);
            SetDlgItemText(calcMainWindow, RPN_STACK_X, stackStr);
        }
        else
            SetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr);


        MakeSciFormat(Y, tmpStr);
        if (rightAlignStack == 1)
        {
            sprintf(stackStr, (bExactFont ? "%24s":"%22s"), tmpStr);
            SetDlgItemText(calcMainWindow, RPN_STACK_Y, stackStr);
        }
        else
            SetDlgItemText(calcMainWindow, RPN_STACK_Y, tmpStr);

        if (recModeON == 0 && showTrace == FALSE)
        {
            MakeSciFormat(Z, tmpStr);
            if (rightAlignStack == 1)
            {
                sprintf(stackStr, (bExactFont ? "%24s":"%22s"), tmpStr);
                SetDlgItemText(calcMainWindow, RPN_STACK_Z, stackStr);
            }
            else
                SetDlgItemText(calcMainWindow, RPN_STACK_Z, tmpStr);

            MakeSciFormat(T, tmpStr);
            if (rightAlignStack == 1)
            {
                sprintf(stackStr, (bExactFont ? "%24s":"%22s"), tmpStr);
                SetDlgItemText(calcMainWindow, RPN_STACK_T, stackStr);
            }
            else
                SetDlgItemText(calcMainWindow, RPN_STACK_T, tmpStr);
        }
    }
    else
    {
        if (Xedit == X_EDIT)
            XL = MakeProgStr(Xstr);

        MakeRadixStr(XL, stackStr);
        sprintf(tmpStr, (bExactFont ? "%23s%c%c":"%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
        SetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr);

        MakeRadixStr(YL, stackStr);
        sprintf(tmpStr, (bExactFont ? "%23s%c%c":"%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
        SetDlgItemText(calcMainWindow, RPN_STACK_Y, tmpStr);

        if (recModeON == 0 && traceMacroPlayback == FALSE)
        {
            MakeRadixStr(ZL, stackStr);
            sprintf(tmpStr, (bExactFont ? "%23s%c%c":"%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
            SetDlgItemText(calcMainWindow, RPN_STACK_Z, tmpStr);

            MakeRadixStr(TL, stackStr);
            sprintf(tmpStr, (bExactFont ? "%23s%c%c":"%20s%c%c"), stackStr, Radix(progMode), RadixBIN(progMode));
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
        MessageBox(calcMainWindow, "Overflow:  Result is too large or too small to be represented.", "Excalibur Floating Point", MB_OK);
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
        D = C;
        C = B;
        B = A;
        A = T;
    }
    T = Z;
    Z = Y;
    Y = X;
    X = temp;

    FloatsToLongs();
}


double StackPop(void)
{
    double temp;

    stackPops++;

    temp = X;
    X = Y;
    Y = Z;
    Z = T;
    if (extendedStack)
    {
        T = A;
        A = B;
        B = C;
        C = D;
        if (popFillZero != 0)
            D = 0.0;
    }
    else
    {
        if (popFillZero != 0)
            T = 0.0;
    }

    FloatsToLongs();
    return(temp);
}


/* ----------------------- */
/* Basic keypad keypresses */
/* ----------------------- */
void RPN_clear(void)
{
    if (progMode > 0) RPN_clearL();
    X = 0.0;
    Y = 0.0;
    Z = 0.0;
    T = 0.0;
    A = 0.0;
    B = 0.0;
    C = 0.0;
    D = 0.0;
    LASTX = 0.0;
    LASTY = 0.0;
    progModecarry = 0;
    strcpy(Xstr, "");
}

void RPN_clearL(void)
{
    XL = 0L;
    YL = 0L;
    ZL = 0L;
    TL = 0L;
    AL = 0L;
    BL = 0L;
    CL = 0L;
    DL = 0L;
    LASTXL = 0L;
    LASTYL = 0L;
    progModecarry = 0;
    strcpy(Xstr, "");
}


void RPN_enter(void)
{
    if (RPL_entry)
    {
        if (Xedit == X_EDIT)
        {
            if (progMode == PROG_NORMAL)
                StackPush(StackPop());
            else
                StackPushL(StackPopL());
        }
        else
        {
            if (progMode == PROG_NORMAL)
                StackPush(X);
            else
                StackPushL(XL);
        }
        Xedit = X_NEW;
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(X);
        else
            StackPushL(XL);
        Xedit = X_ENTER;
    }
}

void RPN_dp(void)
{
    int i;

    if (progMode == PROG_NORMAL)
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
                    for (i = 0; i < (int) strlen(Xstr); i++)
                    {
                        if (Xstr[i] == '.')
                            Xstr[i] = '~';
                    }
                    Xstr[strlen(Xstr) + 1] = (char) NULL;
                    Xstr[strlen(Xstr)] = '/';
                }
            }
        }
    }
}

void RPN_Ex(void)
{
    char *ptr;

    if (progMode == PROG_NORMAL && strchr(Xstr, 'E') == NULL)
    {
        if (Xedit == X_EDIT)
        {
            ptr = strchr(Xstr, 'e');   // Don't allow 'e' over existing 'e'
            if (ptr == NULL)
            {
                if (X == 0.0)
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

    if (progMode == PROG_BIN && key > RPN_DIGIT_1)
        return;
    if (progMode == PROG_OCT && key > RPN_DIGIT_7)
        return;

    if (Xedit == X_NEW)
    {
        if (progMode != PROG_NORMAL)
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
        if (allowDigitBasedOnMaxStringSize(Xstr, (char) ('0' + (key - 101))))
        {
            int len = strlen(Xstr);
            Xstr[len] = '0' + (key - 101);
            Xstr[len+1] = CNULL;
        }
    }

    if (strchr(Xstr, '/') == NULL)
    {
        X = atof(Xstr);
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
            X = tmp3 + (tmp1 / tmp2);
        else
            X = 0.0;
    }
    XL = MakeProgStr(Xstr);
    Xedit = X_EDIT;
}


int allowDigitBasedOnMaxStringSize(char *Xstr, char digit)
{
    int status = FALSE;
    int maxDigits;
    char temp[40];
    char *tmpPtr;
    int i, significantDigits;

    maxDigits = 12;
    significantDigits = 0;
    for (i=0; i<(int)strlen(Xstr); i++)
    {
        if ( (Xstr[i] >= '0') && (Xstr[i] <= '9') )
        {
            significantDigits++;
        }
        if ( (Xstr[i] == 'e') || (Xstr[i] == 'E') )
        {
            maxDigits = 15;
        }
    }

    if (progMode == PROG_NORMAL)
    {
        if (significantDigits < maxDigits)
            status = TRUE;
    }
    else
    {
        if (progMode == PROG_BIN)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtoul(temp, &tmpPtr, progMode) < wordSizeMask)
                status = TRUE;
            if ((int) strlen(Xstr) >= 32)
                status = FALSE;
        }
        else if (progMode == PROG_DEC)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtoul(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int) strlen(Xstr) >= 10)
                status = FALSE;
        }
        else if (progMode == PROG_HEX)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtoul(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int) strlen(Xstr) >= 8)
                status = FALSE;
        }
        else if (progMode == PROG_OCT)
        {
            sprintf(temp, "%s%c", Xstr, digit);
            if (strtoul(temp, &tmpPtr, progMode) <= wordSizeMask)
                status = TRUE;
            if ((int) strlen(Xstr) >= 11)
                status = FALSE;
        }
    }

    return status;
}

extern BOOL CALLBACK fnDIALOG_DisplayModeProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void RPN_mode2(HWND hDlg)
{
    DLGPROC lpfnDIALOG_DisplayModeProc;

    lpfnDIALOG_DisplayModeProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_DisplayModeProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_DISPLAY_MODE", hDlg, lpfnDIALOG_DisplayModeProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_DisplayModeProc);
    ShowStatus();
}

void RPN_mode(void)
{
    DLGPROC lpfnDIALOG_DisplayModeProc;

    lpfnDIALOG_DisplayModeProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_DisplayModeProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_DISPLAY_MODE", calcMainWindow, lpfnDIALOG_DisplayModeProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_DisplayModeProc);
    ShowStatus();
}


BOOL CALLBACK fnDIALOG_DisplayModeProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch(wMessage)
    {
    case WM_INITDIALOG:
        if (decimal_places == 0)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO0), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 1)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 2)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 3)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 4)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO4), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 5)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO5), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 6)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO6), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 7)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO7), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 8)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO8), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 9)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO9), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 10)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO10), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (decimal_places == 11)
            SendMessage(GetDlgItem(hDlg, IDC_RADIO11), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, IDC_RADIO12), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (sci_format == 'g')
            SendMessage(GetDlgItem(hDlg, 101), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        if (sci_format == 'E')
            SendMessage(GetDlgItem(hDlg, 102), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        if (sci_format == 'f')
            SendMessage(GetDlgItem(hDlg, 103), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        if (sci_format == 'Z')
            SendMessage(GetDlgItem(hDlg, 104), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (rightAlignStack == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(108):            /* OK was pressed */
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

            if (SendMessage(GetDlgItem(hDlg, IDC_RADIO0), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 0;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 1;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 2;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO3), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 3;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO4), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 4;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO5), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 5;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO6), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 6;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO7), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 7;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO8), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 8;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO9), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 9;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO10), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 10;
            else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO11), BM_GETCHECK, (WORD) 0, (DWORD) 0L) != 0)
                decimal_places = 11;
            else
                decimal_places = 12;

            if (SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_GETCHECK, (WORD) 0, (DWORD) 0L))
                rightAlignStack = 1;
            else
                rightAlignStack = 0;

            EndDialog(hDlg, FALSE);
            return TRUE;
        case(109):            /* Cancel was pressed */
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
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
            X = atof(Xstr);
        }
        if (strlen(Xstr) == 0)
        {
            X = 0.0;
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
    if (progMode == PROG_NORMAL)
        StackPush(StackPop() + StackPop());
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
    if (progMode == PROG_NORMAL)
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

    if (X == 0.0)
    {
        RPN_error("Divide By Zero");
    }
    else
    {
        Xedit = X_NEW;
        if (progMode == PROG_NORMAL)
        {
            x = StackPop();
            y = StackPop();
            StackPush(y / x);
        }
        else
        {
            if (wordMode == PROG_SIGNED)
            {
                sxl = (PROG_SIGNEDLONG) StackPopL();
                syl = (PROG_SIGNEDLONG) StackPopL();
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
    if (progMode == PROG_NORMAL)
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
    long xl, yl;

    Xedit = X_NEW;
    if (progMode == PROG_NORMAL)
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
        X = atof(Xstr);
    }
    else
    {
        if ((Xedit == X_EDIT) && (progMode == PROG_NORMAL))
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
            X = atof(Xstr);
        }
        else
        {
            Xedit = X_NEW;
            if (progMode == PROG_NORMAL)
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
    if (progMode == PROG_NORMAL)
    {
        temp = T;
        T = Z;
        Z = Y;
        Y = X;
        X = temp;
    }
    else
    {
        tempL = TL;
        TL = ZL;
        ZL = YL;
        YL = XL;
        XL = tempL;
    }
}

void RPN_rotateStackDn(void)
{
    double temp;
    PROG_LONG tempL;

    Xedit = X_NEW;
    if (progMode == PROG_NORMAL)
    {
        temp = X;
        X = Y;
        Y = Z;
        Z = T;
        T = temp;
    }
    else
    {
        tempL = XL;
        XL = YL;
        YL = ZL;
        ZL = TL;
        TL = tempL;
    }
}

void RPN_lastX(void)
{
    Xedit = X_NEW;
    if (progMode == PROG_NORMAL)
        StackPush(LASTX);
    else
        StackPushL(LASTXL);
}



/* -------------- */
/* Misc functions */
/* -------------- */

double ToRadians(double t)
{
    double temp;

    if (AngleMode == 0)
        temp = t / (180.0 / M_PI);
    else if (AngleMode == 2)
        temp = t / ((180.0 / M_PI) * (400.0 / 360.0));
    else
        temp = t;

    return(temp);
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

    return(temp);
}


/* ------------------------- */
/* STOre and ReCaL functions */
/* ------------------------- */
char stoTmpStr[10];
BOOL CALLBACK StoNameDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, stoTmpStr);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(IDOK):           /* OK - save value! */
            GetDlgItemText(hDlg, IDC_EDIT1, stoTmpStr, 9);     /* 8 chars plus NULL */
            EndDialog(hDlg, FALSE);
            return TRUE;
        case(IDCANCEL):       /* Close/Cancel */
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CALLBACK fnDIALOG_StoreProc(HWND, UINT, WPARAM, LPARAM);

void RPN_store(void)
{

    DLGPROC lpfnDIALOG_StoreProc;

    lpfnDIALOG_StoreProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_StoreProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_STORE", calcMainWindow, lpfnDIALOG_StoreProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_StoreProc);
}

BOOL CALLBACK fnDIALOG_StoreProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    LRESULT item;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        for (i = 0; i < MAX_STO; i++)
        {
            sprintf(tmpStr, "R%02d = %-18.11g {%-8s}", i, STO[i], STOlabels[i]);
            makeInternational(tmpStr);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
        }
        SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, 0, 0);
        SetFocus(GetDlgItem(hDlg, 101));
        return TRUE;
    case WM_COMMAND:

        switch(LOWORD(wParam))
        {
        case(101):            // double click?!?
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;
        case(102):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
                return FALSE;
            }
            else
            {
                SendDlgItemMessage(hDlg, 101, LB_GETTEXT, item, (LPARAM) ((LPSTR) tmpStr));
                RPN_storeReg(item);
                Xedit = X_NEW;
            }
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(103):
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(104):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                SendDlgItemMessage(hDlg, 101, LB_GETTEXT, item, (LPARAM) ((LPSTR) tmpStr));
                RPN_storeAddReg(item);
                Xedit = X_NEW;
            }
            EndDialog(hDlg, FALSE);
            return TRUE;
        case(105):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                SendDlgItemMessage(hDlg, 101, LB_GETTEXT, item, (LPARAM) ((LPSTR) tmpStr));
                RPN_storeSubReg(item);
                Xedit = X_NEW;
            }
            EndDialog(hDlg, FALSE);
            return TRUE;
        case(106):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                SendDlgItemMessage(hDlg, 101, LB_GETTEXT, item, (LPARAM) ((LPSTR) tmpStr));
                RPN_storeMulReg(item);
                Xedit = X_NEW;
            }
            EndDialog(hDlg, FALSE);
            return TRUE;
        case(107):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
                return FALSE;
            }
            else
            {
                SendDlgItemMessage(hDlg, 101, LB_GETTEXT, item, (LPARAM) ((LPSTR) tmpStr));
                RPN_storeDivReg(item);
                Xedit = X_NEW;
            }
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(108):
            for (i = 0; i < MAX_STO; i++)
                SendDlgItemMessage(hDlg, 101, LB_DELETESTRING, 0, 0);
            for (i = 0; i < MAX_STO; i++)
            {
                STO[i] = 0.0;
                strcpy(STOlabels[i], "no label");
                sprintf(tmpStr, "R%02d = %-18.11g {%-8s}", i, STO[i], STOlabels[i]);
                makeInternational(tmpStr);
                SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
            }
            SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, 0, 0);
            return TRUE;

        case(109):            // Edit label
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
                return FALSE;
            }
            else
            {
                strcpy(stoTmpStr, STOlabels[item]);
                DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_STO_NAME", calcMainWindow, StoNameDlgProc);
                strcpy(STOlabels[item], stoTmpStr);
                SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
                for (i = 0; i < MAX_STO; i++)
                {
                    sprintf(tmpStr, "R%02d = %-18.11g {%-8s}", i, STO[i], STOlabels[i]);
                    makeInternational(tmpStr);
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
                }
                SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, item, 0);
            }
            return TRUE;

        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CALLBACK fnDIALOG_RecallProc(HWND, UINT, WPARAM, LPARAM);
void RPN_recall(void)
{
    DLGPROC lpfnDIALOG_RecallProc;

    lpfnDIALOG_RecallProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_RecallProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_RECALL", calcMainWindow, lpfnDIALOG_RecallProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_RecallProc);
}

BOOL CALLBACK fnDIALOG_RecallProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    LRESULT item;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        for (i = 0; i < MAX_STO; i++)
        {
            sprintf(tmpStr, "R%02d = %-18.11g {%-8s}", i, STO[i], STOlabels[i]);
            makeInternational(tmpStr);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
        }
        SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, 0, 0);
        SetFocus(GetDlgItem(hDlg, 101));
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(101):
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;
        case(102):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                RPN_recallReg(item);
            }
            EndDialog(hDlg, FALSE);
            return TRUE;
        case(103):
            EndDialog(hDlg, FALSE);
            return TRUE;

        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}


/* ------------------------------------------------------------------------ */
/*                           SAVE STUFF TO DISK                             */
/* ------------------------------------------------------------------------ */
int GetMenuType(struct funcStruct *cFunc)
{
    int retVal = 1;

    if (cFunc == (struct funcStruct *) &Scientific_funcs)  retVal = 1;
    if (cFunc == (struct funcStruct *) &Financial_funcs)   retVal = 2;
    if (cFunc == (struct funcStruct *) &CompSci_funcs)     retVal = 3;
    if (cFunc == (struct funcStruct *) &Conversion_funcs)  retVal = 4;
    if (cFunc == (struct funcStruct *) &Geometry_funcs)    retVal = 5;
    if (cFunc == (struct funcStruct *) &Program1_funcs)    retVal = 6;
    if (cFunc == (struct funcStruct *) &Program2_funcs)    retVal = 7;
    if (cFunc == (struct funcStruct *) &Statistics_funcs)  retVal = 8;
    if (cFunc == (struct funcStruct *) &Physics_funcs)     retVal = 9;
    if (cFunc == (struct funcStruct *) &Custom_funcs)      retVal = 10;

    return(retVal);
}

void SetMenuType(int type)
{
    switch(type)
    {
    case(1):
        currentFuncs = (struct funcStruct *) &Scientific_funcs;
        CheckMenuItem(hMainMenu, IDM_SCIENTIFIC, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_SCI), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_SCI_I);
        break;
    case(2):
        currentFuncs = (struct funcStruct *) &Financial_funcs;
        CheckMenuItem(hMainMenu, IDM_FINANCIAL, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_FIN), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_BUIS);
        break;
    case(3):
        currentFuncs = (struct funcStruct *) &CompSci_funcs;
        CheckMenuItem(hMainMenu, IDM_COMPSCI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_COMPSCI), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_COMPSCI);
        break;
    case(4):
        currentFuncs = (struct funcStruct *) &Conversion_funcs;
        CheckMenuItem(hMainMenu, IDM_CONVERSION, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CONV), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CONV);
        break;
    case(5):
        currentFuncs = (struct funcStruct *) &Geometry_funcs;
        CheckMenuItem(hMainMenu, IDM_GEOMETRY, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_GEOM), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_GEOM);
        break;
    case(6):
        currentFuncs = (struct funcStruct *) &Program1_funcs;
        CheckMenuItem(hMainMenu, IDM_PROGI, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGI), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG1);
        break;
    case(7):
        currentFuncs = (struct funcStruct *) &Program2_funcs;
        CheckMenuItem(hMainMenu, IDM_PROGII, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PROGII), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PROG2);
        break;
    case(8):
        currentFuncs = (struct funcStruct *) &Statistics_funcs;
        CheckMenuItem(hMainMenu, IDM_STATS, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_STAT), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_STATS);
        break;
    case(9):
        currentFuncs = (struct funcStruct *) &Physics_funcs;
        CheckMenuItem(hMainMenu, IDM_PHYSICS, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_PHY), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_PHYSICS);
        break;
    case(10):
        currentFuncs = (struct funcStruct *) &Custom_funcs;
        CheckMenuItem(hMainMenu, IDM_CUSTOM, MF_CHECKED);
        SendMessage(GetDlgItem(calcMainWindow, RPN_CUST), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        ShowFunctionBar(FUNC_BAR_TEXT_CUSTOM);
        break;
    }
}

void SetLastMenuType(int type)
{
    switch(type)
    {
    case(1):
        lastFuncs = (struct funcStruct *) &Scientific_funcs;
        break;
    case(2):
        lastFuncs = (struct funcStruct *) &Financial_funcs;
        break;
    case(3):
        lastFuncs = (struct funcStruct *) &CompSci_funcs;
        break;
    case(4):
        lastFuncs = (struct funcStruct *) &Conversion_funcs;
        break;
    case(5):
        lastFuncs = (struct funcStruct *) &Geometry_funcs;
        break;
    case(6):
        lastFuncs = (struct funcStruct *) &Program1_funcs;
        break;
    case(7):
        lastFuncs = (struct funcStruct *) &Program2_funcs;
        break;
    case(8):
        lastFuncs = (struct funcStruct *) &Statistics_funcs;
        break;
    case(9):
        lastFuncs = (struct funcStruct *) &Physics_funcs;
        break;
    case(10):
        lastFuncs = (struct funcStruct *) &Custom_funcs;
        break;
    }
}

#define RESERVED_SIZE           1024
#define CSIDL_LOCAL_APPDATA     0x001c
char reserved[RESERVED_SIZE];

char *GetConfigurationDirectory(void)
{
    static TCHAR Buffer[MAX_PATH];     // buffer for concatenated string
    static TCHAR szPath[MAX_PATH];
    LPITEMIDLIST pidl = NULL;
    LPTSTR lpszSystemInfo;     // pointer to system information string

    if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl)))
    {
        if (SHGetPathFromIDList(pidl, szPath))  // Use the applications directory...
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
    uint8_t  menuCurrentFuncs, menuLastFuncs;
    uint16_t configVersionMain, configVersionSub;

    outfile = fopen(GetConfigurationDirectory(), "wb+");

    if (ClearStackOnExit)
    {
        RPN_clear();
        RPN_clearL();
    }

    memset(reserved, 0, RESERVED_SIZE);

    configVersionMain = CONFIG_VERSION_MAIN;
    configVersionSub  = CONFIG_VERSION_SUB;
    menuCurrentFuncs  = GetMenuType(currentFuncs);
    menuLastFuncs     = GetMenuType(lastFuncs);

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
        fwrite(&padZeros,           sizeof(padZeros),           1, outfile);
        fwrite(&wordSize,           sizeof(wordSize),           1, outfile);
        fwrite(&wordMode,           sizeof(wordMode),           1, outfile);
        fwrite(&wordSizeMask,       sizeof(wordSizeMask),       1, outfile);

        fwrite(&X,                  sizeof(X),                  1, outfile);
        fwrite(&Y,                  sizeof(Y),                  1, outfile);
        fwrite(&Z,                  sizeof(Z),                  1, outfile);
        fwrite(&T,                  sizeof(T),                  1, outfile);
        fwrite(&LASTX,              sizeof(LASTX),              1, outfile);
        fwrite(&LASTY,              sizeof(LASTY),              1, outfile);

        fwrite(&XL,                 sizeof(XL),                 1, outfile);
        fwrite(&YL,                 sizeof(YL),                 1, outfile);
        fwrite(&ZL,                 sizeof(ZL),                 1, outfile);
        fwrite(&TL,                 sizeof(TL),                 1, outfile);
        fwrite(&LASTXL,             sizeof(LASTXL),             1, outfile);
        fwrite(&LASTYL,             sizeof(LASTYL),             1, outfile);

        fwrite(&A,                  sizeof(A),                  1, outfile);
        fwrite(&B,                  sizeof(B),                  1, outfile);
        fwrite(&C,                  sizeof(C),                  1, outfile);
        fwrite(&D,                  sizeof(D),                  1, outfile);
        fwrite(&AL,                 sizeof(AL),                 1, outfile);
        fwrite(&BL,                 sizeof(BL),                 1, outfile);
        fwrite(&CL,                 sizeof(CL),                 1, outfile);
        fwrite(&DL,                 sizeof(DL),                 1, outfile);

        fwrite(STO,                 sizeof(STO),                1, outfile);
        fwrite(SUM,                 sizeof(SUM),                1, outfile);
        fwrite(fin_reg,             sizeof(fin_reg),            1, outfile);
        fwrite(cashFlow,            sizeof(cashFlow),           1, outfile);
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
        fwrite(&popFillZero,        sizeof(popFillZero),        1, outfile);
        fwrite(&rightAlignStack,    sizeof(rightAlignStack),    1, outfile);
        fwrite(&showXMinimized,     sizeof(showXMinimized),     1, outfile);
        fwrite(&RPL_entry,          sizeof(RPL_entry),          1, outfile);
        fwrite(&ClearStackOnExit,   sizeof(ClearStackOnExit),   1, outfile);

        fwrite(playBack,            sizeof(playBack),           1, outfile);
        fwrite(playBackSave,        sizeof(playBackSave),       1, outfile);
        fwrite(&playBackIdx,        sizeof(playBackIdx),        1, outfile);
        fwrite(&playBackIdxSave,    sizeof(playBackIdxSave),    1, outfile);
        fwrite(&macroName,          sizeof(macroName),          1, outfile);
        fwrite(&macro_short_names,  sizeof(macro_short_names),  1, outfile);

        fwrite(&currency1index,     sizeof(currency1index),     1, outfile);
        fwrite(&currency2index,     sizeof(currency2index),     1, outfile);

        fwrite(constants,           sizeof(constants),          1, outfile);
        fwrite(constantBankNames,   sizeof(constantBankNames),  1, outfile);
        fwrite(CurrencyConv,        sizeof(CurrencyConv),       1, outfile);

        fwrite(&lastChosenConst,    sizeof(lastChosenConst),    1, outfile);
        fwrite(&lastConstBank,      sizeof(lastConstBank),      1, outfile);
        fwrite(excaliburNotes,      sizeof(excaliburNotes),     1, outfile);
        fwrite(&lastChosenMacro,    sizeof(lastChosenMacro),    1, outfile);
        fwrite(STOlabels,           sizeof(STOlabels),          1, outfile);
        fwrite(&progDelayValue,     sizeof(int),                1, outfile);
        fwrite(&indirectRegister,   sizeof(indirectRegister),   1, outfile);

        fwrite(reserved,            RESERVED_SIZE,              1, outfile);

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
    uint8_t  menuCurrentFuncs, menuLastFuncs;
    uint16_t configVersionMain, configVersionSub;

    infile = fopen(GetConfigurationDirectory(), "rb");
    if (infile != NULL)
    {
        fread(&configVersionMain,   sizeof(configVersionMain),      1, infile);
        fread(&configVersionSub,    sizeof(configVersionSub),       1, infile);

        // --------------------------------------------------------------
        // If main version has changed, we wipe config with defaults...
        // --------------------------------------------------------------
        if (configVersionMain != CONFIG_VERSION_MAIN)
        {
            main_x = -1;
            SetMenuType(1);
            SaveToDisk();
            ProcessCusomSave();
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
        fread(&padZeros,           sizeof(padZeros),           1, infile);
        fread(&wordSize,           sizeof(wordSize),           1, infile);
        fread(&wordMode,           sizeof(wordMode),           1, infile);
        fread(&wordSizeMask,       sizeof(wordSizeMask),       1, infile);

        fread(&X,                  sizeof(X),                  1, infile);
        fread(&Y,                  sizeof(Y),                  1, infile);
        fread(&Z,                  sizeof(Z),                  1, infile);
        fread(&T,                  sizeof(T),                  1, infile);
        fread(&LASTX,              sizeof(LASTX),              1, infile);
        fread(&LASTY,              sizeof(LASTY),              1, infile);

        fread(&XL,                 sizeof(XL),                 1, infile);
        fread(&YL,                 sizeof(YL),                 1, infile);
        fread(&ZL,                 sizeof(ZL),                 1, infile);
        fread(&TL,                 sizeof(TL),                 1, infile);
        fread(&LASTXL,             sizeof(LASTXL),             1, infile);
        fread(&LASTYL,             sizeof(LASTYL),             1, infile);

        fread(&A,                  sizeof(A),                  1, infile);
        fread(&B,                  sizeof(B),                  1, infile);
        fread(&C,                  sizeof(C),                  1, infile);
        fread(&D,                  sizeof(D),                  1, infile);
        fread(&AL,                 sizeof(AL),                 1, infile);
        fread(&BL,                 sizeof(BL),                 1, infile);
        fread(&CL,                 sizeof(CL),                 1, infile);
        fread(&DL,                 sizeof(DL),                 1, infile);

        fread(STO,                 sizeof(STO),                1, infile);
        fread(SUM,                 sizeof(SUM),                1, infile);
        fread(fin_reg,             sizeof(fin_reg),            1, infile);
        fread(cashFlow,            sizeof(cashFlow),           1, infile);
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
        fread(&popFillZero,        sizeof(popFillZero),        1, infile);
        fread(&rightAlignStack,    sizeof(rightAlignStack),    1, infile);
        fread(&showXMinimized,     sizeof(showXMinimized),     1, infile);
        fread(&RPL_entry,          sizeof(RPL_entry),          1, infile);
        fread(&ClearStackOnExit,   sizeof(ClearStackOnExit),   1, infile);

        fread(playBack,            sizeof(playBack),           1, infile);
        fread(playBackSave,        sizeof(playBackSave),       1, infile);
        fread(&playBackIdx,        sizeof(playBackIdx),        1, infile);
        fread(&playBackIdxSave,    sizeof(playBackIdxSave),    1, infile);
        fread(&macroName,          sizeof(macroName),          1, infile);
        fread(&macro_short_names,  sizeof(macro_short_names),  1, infile);

        fread(&currency1index,     sizeof(currency1index),     1, infile);
        fread(&currency2index,     sizeof(currency2index),     1, infile);

        if (configVersionSub != CONFIG_VERSION_SUB) // Skip these if sub-ver changed
        {
            fseek(infile, sizeof(constants),           SEEK_CUR);
            fseek(infile, sizeof(constantBankNames),   SEEK_CUR);
            fseek(infile, sizeof(CurrencyConv),        SEEK_CUR);
        }
        else
        {
            fread(constants,           sizeof(constants),          1, infile);
            fread(constantBankNames,   sizeof(constantBankNames),  1, infile);
            fread(CurrencyConv,        sizeof(CurrencyConv),       1, infile);
        }

        fread(&lastChosenConst,    sizeof(lastChosenConst),    1, infile);
        fread(&lastConstBank,      sizeof(lastConstBank),      1, infile);
        fread(excaliburNotes,      sizeof(excaliburNotes),     1, infile);
        fread(&lastChosenMacro,    sizeof(lastChosenMacro),    1, infile);
        fread(STOlabels,           sizeof(STOlabels),          1, infile);
        fread(&progDelayValue,     sizeof(int),                1, infile);
        fread(&indirectRegister,   sizeof(indirectRegister),   1, infile);

        fread(reserved,            RESERVED_SIZE,              1, infile);

        SetMenuType(menuCurrentFuncs);
        SetLastMenuType(menuLastFuncs);

        // -----------------------------------------------------------------------------------------------
        // If the main window was somehow shifted off screen (maybe due to using on a dock station with
        // multiple monitors), simply force the main window back to the center again and auto-resize it.
        // -----------------------------------------------------------------------------------------------
        if ((main_cx < MIN_WINDOW_WIDTH) || (main_cy < MIN_WINDOW_HEIGHT) ||
            (main_x >= MAX_WINDOW_X) || (main_y >= MAX_WINDOW_Y) || (main_x < MIN_WINDOW_X) || (main_y < MIN_WINDOW_Y))
        {
            main_x = -1;        // Force auto-resize!
        }
        if (configVersionSub != CONFIG_VERSION_SUB)       /* If new version but still supported version */
        {
            main_x = -1;        // Force auto-resize!
        }

        fclose(infile);
        ProcessCusomSave();
    }
    else
    {
        SetMenuType(1);
        ProcessCusomSave();
        main_cx = -1;       // Window will auto resize because main_cx == -1
    }
}

void ProcessCusomSave(void)
{
    int i, index, newIdx, saveIdx;

    for (i = 0; i < MAX_FUNCS; i++)
    {
        newIdx = i;
        index = customSave[newIdx].func_idx;
        saveIdx = Custom_funcs[newIdx].index;
        switch(customSave[newIdx].custom_save_idx)
        {
        case(CUSTOM_SAVE_SCI):        // Scientific
            memcpy(&Custom_funcs[newIdx], &Scientific_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_BUS):        // Financial
            memcpy(&Custom_funcs[newIdx], &Financial_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_PRO):        // Programming
            memcpy(&Custom_funcs[newIdx], &CompSci_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_STA):        // Statistics
            memcpy(&Custom_funcs[newIdx], &Statistics_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_GEO):        // Geometry
            memcpy(&Custom_funcs[newIdx], &Geometry_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_CON):        // Conversion
            memcpy(&Custom_funcs[newIdx], &Conversion_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_PHY):        // Physics
            memcpy(&Custom_funcs[newIdx], &Physics_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_MACBANK):    // Programming Bank I
            memcpy(&Custom_funcs[newIdx], &Program1_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_MACBANK2):    // Programming Bank II
            memcpy(&Custom_funcs[newIdx], &Program2_funcs[index], sizeof(struct funcStruct));
            break;
        case(CUSTOM_SAVE_MAC):        // Macros - SPECIAL!
            memcpy(&Custom_funcs[newIdx], &MacroFuncs[index], sizeof(struct funcStruct));
            strcpy(Custom_funcs[newIdx].desc, macro_short_names[index]);
            Custom_funcs[newIdx].keyTitle = 9000 + index;
            break;

        }
        Custom_funcs[newIdx].index = saveIdx;
    }
    processFuncs();
}

BOOL CALLBACK fnDIALOG_MACRONAME(HWND, UINT, WPARAM, LPARAM);
char macName[31];
char macShortName[7];
void GetMacroName(void)
{
    DLGPROC lpfnDIALOG_MACRO;

    lpfnDIALOG_MACRO = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_MACRONAME, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_MACRO_NAME", calcMainWindow, lpfnDIALOG_MACRO)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_MACRO);
}

BOOL CALLBACK fnDIALOG_MACRONAME(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, macName);
        SetDlgItemText(hDlg, IDC_EDIT2, macShortName);
        return TRUE;

    case WM_COMMAND:

        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, macName, 50);
            macName[50] = '\0';
            GetDlgItemText(hDlg, IDC_EDIT2, macShortName, 7);
            macShortName[6] = '\0';
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        case(IDCANCEL):       // OK
            strcpy(macShortName, "@@ZY&(");
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
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
    char tmpB[50];
    char outputBuffer[200];
    int  tmpBidx;
    int  i;
    int  found;
    int  done;
    int  checksumFound;
    unsigned short chksum, userChecksum;
    int preambleIndex;
    int maxLen;
    char *ptr;

    userChecksum = 0xFFAA;
    playBackIdx = 0;
    idx=0;
    tmpBidx=0;
    done = 0;
    checksumFound = 0;

    while ((idx < MAX_IMPORT_CLIPBOARD_SIZE) && !done)
    {
        if ((clipboardBuffer[idx] == '\n') || (clipboardBuffer[idx] == CNULL))
        {
            tmpB[tmpBidx++] = CNULL;

            // ---------------------------------------------------------------
            // Map old Recall/StoreA-Z to new R0-R26 so that we are friendly
            // to older users of Excalibur when importing programs...
            // ---------------------------------------------------------------
            ptr = strstr(tmpB, "Store Plus ");
            if (ptr != 0) // Does the word 'Store Plus' appear here?
            {
                if ((ptr[11] >= 'A') && (ptr[11] <= 'Z') && (!isdigit(ptr[12])))
                {
                    char tmp2[5];
                    sprintf(tmp2, "R%d", ptr[11]-'A');
                    ptr[11] = CNULL;
                    strcat(ptr, tmp2);
                }
            }
            else
            {
                ptr = strstr(tmpB, "Store Mult ");
                if (ptr != 0) // Does the word 'Store Plus' appear here?
                {
                    if ((ptr[11] >= 'A') && (ptr[11] <= 'Z') && (!isdigit(ptr[12])))
                    {
                        char tmp2[5];
                        sprintf(tmp2, "R%d", ptr[11]-'A');
                        ptr[11] = CNULL;
                        strcat(ptr, tmp2);
                    }
                }
                else
                {
                    ptr = strstr(tmpB, "Store Minus ");
                    if (ptr != 0) // Does the word 'Store Minus' appear here?
                    {
                        if ((ptr[12] >= 'A') && (ptr[12] <= 'Z') && (!isdigit(ptr[13])))
                        {
                            char tmp2[5];
                            sprintf(tmp2, "R%d", ptr[12]-'A');
                            ptr[12] = CNULL;
                            strcat(ptr, tmp2);
                        }
                    }
                    else // Look for basic Store/Recall A-Z
                    {
                        ptr = strstr(tmpB, "Store ");
                        if (ptr != 0) // Does the word Store appear here?
                        {
                            if ((ptr[6] >= 'A') && (ptr[6] <= 'Z') && (!isdigit(ptr[7])))
                            {
                                char tmp2[5];
                                sprintf(tmp2, "R%d", ptr[6]-'A');
                                ptr[6] = CNULL;
                                strcat(ptr, tmp2);
                            }
                        }
                        else
                        {
                            ptr = strstr(tmpB, "Recall ");
                            if (ptr != 0) // Does the word Recall appear here?
                            {
                                if ((ptr[7] >= 'A') && (ptr[7] <= 'Z') && (!isdigit(ptr[8])))
                                {
                                    char tmp2[5];
                                    sprintf(tmp2, "R%d", ptr[7]-'A');
                                    ptr[7] = CNULL;
                                    strcat(ptr, tmp2);
                                }
                            }
                        }
                    }
                }
            }

            preambleIndex=0;
            done = 0;
            while (!done)
            {
                switch(tmpB[preambleIndex])
                {
                case('0'):
                case('1'):
                case('2'):
                case('3'):
                case('4'):
                case('5'):
                case('6'):
                case('7'):
                case('8'):
                case('9'):
                case(' '):
                case('-'):
                    preambleIndex++;  // Skip past any leading line numbers and dashes/spaces
                    break;
                default:
                    done = 1;
                }
            }
            done = 0;
            tmpBidx=0;
            found = 0;
            for (i=0; i < totalMappedButtonFuncs; i++) // Look up the macro by name in the mapped buttons table
            {
                maxLen = max(strlen(&tmpB[preambleIndex]), strlen(playBackMap[i].funcText));
                if ((tmpB[preambleIndex] != CNULL) && (strncmp(&tmpB[preambleIndex], playBackMap[i].funcText, maxLen) == 0))
                {
                    playBack[playBackIdx] = i;
                    playBackIdx++;
                    if (playBackIdx >= MAX_REC_PLAYBACK)
                    {
                        MessageBox(calcMainWindow,"Maximum program length has been reached.", "Excalibur Program Error", MB_OK);
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
                playBackIdx=0;
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
                tmpBidx=0;
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
                sprintf(outputBuffer,"Successful Import\n\nProgram Checksum Verified\n\nChecksum of this newly imported program is %04X", chksum);
                MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                sprintf(outputBuffer,"Actual checksum of the imported program was %04X\nbut the the import buffer held a checksum of %04X\n\nThe program was imported but you should double check the program steps.", chksum, userChecksum);
                MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_OK | MB_ICONINFORMATION);
            }
        }
        else
        {
            sprintf(outputBuffer,"Successful Import\n\nChecksum of this newly imported program is %04X", chksum);
            MessageBox(calcMainWindow, outputBuffer, "Excalibur For Windows", MB_OK | MB_ICONINFORMATION);
        }
    }
}


BOOL CALLBACK fnDIALOG_MACRO(HWND, UINT, WPARAM, LPARAM);
void DoMacroSaveRecall(void)
{
    DLGPROC lpfnDIALOG_MACRO;

    lpfnDIALOG_MACRO = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_MACRO, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_MACRO_SAVE", calcMainWindow, lpfnDIALOG_MACRO)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_MACRO);
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
    static int lbTabStops[1] = { 40 };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), LB_SETTABSTOPS, 1, (DWORD) lbTabStops);
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
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
        }
        sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
        SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));

        for (i = 0; i < MAX_MACROS; i++)
        {
            sprintf(tmpStr, "%-6s:\t%s", macro_short_names[i], macroName[i]);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
        }
        SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
        SetFocus(GetDlgItem(hDlg, 101));
        return TRUE;

    case WM_COMMAND:

        switch(LOWORD(wParam))
        {
        case(101):            // double click?!?
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
                    SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
                }
                sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
                SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
                return FALSE;
            }
            return TRUE;
            break;
        case(102):            // Save
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
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
                        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
                    }
                    SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
                    SetFocus(GetDlgItem(hDlg, 101));
                }
            }
            return TRUE;
            break;

        case(103):            // Recall
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
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
                SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
            }
            sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));

            return TRUE;
            break;

        case(104):            // Clear All
            if (MessageBox
                (hDlg, "Are you sure you wish to clear all programs?", "Excalibur For Windows", MB_ICONQUESTION | MB_YESNO) == IDYES)
            {
                for (i = 0; i < MAX_MACROS; i++)
                    SendDlgItemMessage(hDlg, 101, LB_DELETESTRING, 0, 0);
                for (i = 0; i < MAX_MACROS; i++)
                {
                    playBackIdxSave[i] = 0;
                    strcpy(macroName[i], "Not Currently Defined");
                    sprintf(macro_short_names[i], "P%02d", i + 1);
                    sprintf(tmpStr, "%-6s:\t%s", macro_short_names[i], macroName[i]);
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
                }
                chksum = 0x0000;
                sprintf(tmpStr, "Checksum: %04X", chksum);
                SetDlgItemText(hDlg, IDC_CHECKSUM1, tmpStr);
                lastChosenMacro = 0;
                SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenMacro, 0);
                SetFocus(GetDlgItem(hDlg, 101));
            }
            return TRUE;

        case(106):            // Delete Macro
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
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
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
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

        case(107):            // Paste/Import from Clipboard
            if (MessageBox
                (hDlg, "Import Program from Clipboard?\n\nThis will overwrite the currently loaded program\nbut does not affect stored programs.", "Excalibur For Windows", MB_ICONQUESTION | MB_YESNO) == IDYES)
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
                    SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
                }
                sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
                SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmpStr));
            }
            return TRUE;

        case(108):            // Copy Selected Program to Clipboard
            tptr = GlobalAlloc(GHND, (DWORD) MAX_IMPORT_CLIPBOARD_SIZE);
            cptr = GlobalLock(tptr);
            lstrcpy(cptr, "");

            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);

            sprintf(tmpStr, "Macro Name:  %s", macro_short_names[item]);
            lstrcat(cptr, (LPSTR) tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");

            sprintf(tmpStr, "Description: %s", macroName[item]);
            lstrcat(cptr, (LPSTR) tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");
            lstrcat(cptr, (LPSTR) "\r\n");

            for (i = 0; i < playBackIdxSave[item]; i++)
            {
                sprintf(tmpStr, "%03d - %s", i + 1, playBackMap[playBackSave[item][i]].funcText);
                lstrcat(cptr, (LPSTR) tmpStr);
                lstrcat(cptr, (LPSTR) "\r\n");
            }
            sprintf(tmpStr, "%03d - <End Of Program>", i + 1);
            lstrcat(cptr, (LPSTR) tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");
            chksum = 0x0000;
            for (i = 0; i < playBackIdxSave[item]; i++)
                chksum += playBackSave[item][i];
            sprintf(tmpStr, "Checksum: %04X", chksum);
            lstrcat(cptr, (LPSTR) "\r\n");
            lstrcat(cptr, (LPSTR) tmpStr);
            lstrcat(cptr, (LPSTR) "\r\n");

            OpenClipboard(calcMainWindow);
            EmptyClipboard();
            GlobalUnlock(tptr);
            SetClipboardData(CF_TEXT, tptr);
            CloseClipboard();
            sprintf(tmpStr, "The currently selected program [%s] has been saved to the clipboard.", macro_short_names[item]);
            MessageBox(hDlg, tmpStr, "Excalibur For Windows", MB_ICONINFORMATION | MB_OK);
            return TRUE;

        case(109):            // Copy Loaded to Clipboard
            ClipboardCopySelection(calcMainWindow, 3);
            MessageBox(hDlg, "The currently loaded program has been saved to the clipboard.", "Excalibur For Windows", MB_ICONINFORMATION | MB_OK);
            return TRUE;

        case(105):            // Cancel
            EndDialog(hDlg, FALSE);
            return TRUE;

        default:
            return FALSE;
        }

    case WM_DESTROY:
    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        ProcessCusomSave();    // Always update the keypad for custom layout...
        break;

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
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

    lpfnDIALOG_SettingsProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_SettingsProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_SETTINGS", calcMainWindow, lpfnDIALOG_SettingsProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_SettingsProc);
}

BOOL CALLBACK fnDIALOG_SettingsProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    DWORD bs;                   // Button Status

    switch(wMessage)
    {
    case WM_INITDIALOG:
        sprintf(tmpStr, "%3.4f", taxConstant);
        SetDlgItemText(hDlg, 103, tmpStr);

        sprintf(tmpStr, "%d", progDelayValue);
        SetDlgItemText(hDlg, 123, tmpStr);

        if (extendedStack)
            SendMessage(GetDlgItem(hDlg, 125), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 124), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (popFillZero != 0)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        SendMessage(GetDlgItem(hDlg, 104 + AngleMode), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        if (commaMode == 0)
            SendMessage(GetDlgItem(hDlg, 110), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 109), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (eexMode == 0)
            SendMessage(GetDlgItem(hDlg, 111), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 112), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        if (numLockMode == 1)
            SendMessage(GetDlgItem(hDlg, 107), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 108), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (toolTipsOn == TRUE)
            SendMessage(GetDlgItem(hDlg, 114), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 115), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (numberDisplayMode == INTERNATIONAL)
            SendMessage(GetDlgItem(hDlg, 116), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 117), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (showXMinimized == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK2), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (RPL_entry == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK3), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (ClearStackOnExit == 1)
            SendMessage(GetDlgItem(hDlg, IDC_CHECK4), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        return TRUE;
    case WM_COMMAND:
        switch(wParam)
        {
        case(101):            // OK

            GetDlgItemText(hDlg, 103, tmpStr, 8);
            taxConstant = atof(tmpStr);
            if (taxConstant == 0.0)
                taxConstant = 1.05;

            GetDlgItemText(hDlg, 123, tmpStr, 9);
            progDelayValue = atoi(tmpStr);

            bs = SendMessage(GetDlgItem(hDlg, 125), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                extendedStack = 1;
            else
                extendedStack = 0;

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK1), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0)
                popFillZero = 1;
            else
                popFillZero = 0;

            bs = SendMessage(GetDlgItem(hDlg, 104), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                AngleMode = 0;  // Degrees
            }
            bs = SendMessage(GetDlgItem(hDlg, 105), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                AngleMode = 1;  // Radians
            }
            bs = SendMessage(GetDlgItem(hDlg, 106), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                AngleMode = 2;  // Gradients
            }
            bs = SendMessage(GetDlgItem(hDlg, 109), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                commaMode = 1;
             else
                commaMode = 0;

            bs = SendMessage(GetDlgItem(hDlg, 107), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                numLockMode = 1;
            else
                numLockMode = 0;

            bs = SendMessage(GetDlgItem(hDlg, 114), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                toolTipsOn = 1;
            else
                toolTipsOn = 0;

            bs = SendMessage(GetDlgItem(hDlg, 116), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
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

            bs = SendMessage(GetDlgItem(hDlg, 111), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                eexMode = 0;
                SetDlgItemText(calcMainWindow, RPN_NEGATE, "+/-");
                SetDlgItemText(calcMainWindow, RPN_E, "E");
            }
            else
            {
                eexMode = 1;
                SetDlgItemText(calcMainWindow, RPN_NEGATE, "CHS");
                SetDlgItemText(calcMainWindow, RPN_E, "EEX");
            }

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK2), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                showXMinimized = 1;
            else
                showXMinimized = 0;

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK3), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                RPL_entry = 1;
            else
                RPL_entry = 0;

            bs = SendMessage(GetDlgItem(hDlg, IDC_CHECK4), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                ClearStackOnExit = 1;
            else
                ClearStackOnExit = 0;


            EndDialog(hDlg, FALSE);
            ShowStatus();
            ShowStack();
            return TRUE;
        case(102):            // CANCEL
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(113):            // DISPLAY MODES
            RPN_mode2(hDlg);
            break;

        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
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

    i = 0;
    while (FunctionBankKeyPos[i].index != RPN_LAST_KEY)
    {
        if (xPos >= FunctionBankKeyPos[i].x && xPos <= FunctionBankKeyPos[i].x + FunctionBankKeyPos[i].w)
            if (yPos >= FunctionBankKeyPos[i].y && yPos <= FunctionBankKeyPos[i].y + FunctionBankKeyPos[i].h)
            {
                if (currentFuncs[i].keyTitle >= 9000)
                {
                    strcpy(helpTitle, macroName[currentFuncs[i].keyTitle - 9000]);
                    strcpy(helpMsg, "Program Assigned Key");
                }
                else
                {
                    LoadString(hExcaliburInstance, currentFuncs[i].keyTitle, helpTitle, sizeof(helpTitle) - 1);
                    LoadString(hExcaliburInstance, currentFuncs[i].keyHelp, helpMsg, sizeof(helpMsg) - 1);
                }
                status = 1;
                break;
            }
        i++;
    }

    i = 0;
    if (status == 0)
    {
        while (RPNkeyPos[i].index != RPN_LAST_KEY)
        {
            if (xPos >= RPNkeyPos[i].x && xPos <= RPNkeyPos[i].x + RPNkeyPos[i].w)
                if (yPos >= RPNkeyPos[i].y && yPos <= RPNkeyPos[i].y + RPNkeyPos[i].h)
                {
                    strcpy(helpTitle, RPNkeys[i + 17].keyTitle);       // tbd, dont use 18
                    // const as offset
                    strcpy(helpMsg, RPNkeys[i + 17].keyHelp);
                    status = 1;
                    break;
                }
            i++;
        }
    }

    return(status);
}


void init_key_pos(void)        // for tool tips
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
    while (RPNkeyPos[i].index != RPN_LAST_KEY)
    {
        GetWindowRect(GetDlgItem(calcMainWindow, RPNkeyPos[i].index), &rc);
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
    while (FunctionBankKeyPos[i].index != RPN_LAST_KEY)
    {
        GetWindowRect(GetDlgItem(calcMainWindow, FunctionBankKeyPos[i].index), &rc);
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
    X = 0.0;
    XL = 0L;
    progModecarry = 0;
    Xedit = X_ENTER;
}

void RPN_drop(void)            // drop the stack
{
    Xedit = X_NEW;
    if (progMode == PROG_NORMAL)
        StackPop();
    else
        StackPopL();
}

void RPN_larg(void)            // drop the stack
{
    Xedit = X_NEW;
    if (progMode == PROG_NORMAL)
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

void RPN_frac(void)
{
    int i;

    if (progMode == PROG_NORMAL)
    {
        if (Xedit == X_NEW)
        {
            StackPush(0.0);
            strcpy(Xstr, "0/");
        }
        else if (Xedit == X_ENTER)
        {
            strcpy(Xstr, "0/");
        }
        else
        {
            if (strchr(Xstr, '~') == NULL)
            {
                for (i = 0; i < (int) strlen(Xstr); i++)
                {
                    if ((Xstr[i] == '.') || (Xstr[i] == '/'))
                    {
                        Xstr[i] = '~';
                    }
                }
                Xstr[strlen(Xstr) + 1] = (char) NULL;
                Xstr[strlen(Xstr)] = '/';
            }
        }
    }
}

void RPN_edit(void)
{
    int i, j;

    if (Xedit != X_EDIT)
    {
        GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, 40);
        j = 0;
        for (i = 0; i < (int) strlen(tmpStr); i++)
        {
            if (tmpStr[i] != ' ' && tmpStr[i] != ',' && tmpStr[i] != 'h' && tmpStr[i] != 'b' && tmpStr[i] != 'o' && tmpStr[i] != 'd')
            {
                Xstr[j] = tmpStr[i];
                j++;
            }
        }
        Xstr[j] = (char) NULL;
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
    SelectNewFunc((struct funcStruct *) &Scientific_funcs);
}

void RPN_SelectStat(void)
{
    SelectNewFunc((struct funcStruct *) &Statistics_funcs);
}

void RPN_SelectBusi(void)
{
    SelectNewFunc((struct funcStruct *) &Financial_funcs);
}

void RPN_SelectConv(void)
{
    SelectNewFunc((struct funcStruct *) &Conversion_funcs);
}

void RPN_SelectGeom(void)
{
    SelectNewFunc((struct funcStruct *) &Geometry_funcs);
}

void RPN_SelectProg(void)
{
    SelectNewFunc((struct funcStruct *) &CompSci_funcs);
}

void RPN_SelectProg2(void)
{
    SelectNewFunc((struct funcStruct *) &Program2_funcs);
}

void RPN_SelectCust(void)
{
    SelectNewFunc((struct funcStruct *) &Custom_funcs);
}

void RPN_SelectPhysics(void)
{
    SelectNewFunc((struct funcStruct *) &Physics_funcs);
}

void RPN_SelectMacro(void)
{
    SelectNewFunc((struct funcStruct *) &Program1_funcs);
}

void mapButtonFuncs(void)
{
    int i, j;
    uint8_t exchangeMade;
    struct playbackStruct tmpPlayBack;

    j = 0;
    i = 0;

    while (RPNkeys[i].index != RPN_LAST_KEY)
    {
        playBackMap[j].saveLastX = RPNkeys[i].saveLastX;
        playBackMap[j].newXedit = RPNkeys[i].newXedit;
        playBackMap[j].routine = RPNkeys[i].routine;
        strcpy(playBackMap[j].funcText, RPNkeys[i].keyTitle);
        playBackMap[j].uniqueIndex = RPNkeys[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = RPNkeys[i].useFloatsLongs;
        playBackMap[j].allowRecord = RPNkeys[i].allowRecord;
        j++;
        i++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Scientific_funcs[i].saveLastX;
        playBackMap[j].newXedit = Scientific_funcs[i].newXedit;
        playBackMap[j].routine = Scientific_funcs[i].routine;
        LoadString(hExcaliburInstance, Scientific_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Scientific_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Scientific_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Scientific_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = CompSci_funcs[i].saveLastX;
        playBackMap[j].newXedit = CompSci_funcs[i].newXedit;
        playBackMap[j].routine = CompSci_funcs[i].routine;
        LoadString(hExcaliburInstance, CompSci_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = CompSci_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = CompSci_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = CompSci_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Financial_funcs[i].saveLastX;
        playBackMap[j].newXedit = Financial_funcs[i].newXedit;
        playBackMap[j].routine = Financial_funcs[i].routine;
        LoadString(hExcaliburInstance, Financial_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Financial_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Financial_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Financial_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Conversion_funcs[i].saveLastX;
        playBackMap[j].newXedit = Conversion_funcs[i].newXedit;
        playBackMap[j].routine = Conversion_funcs[i].routine;
        LoadString(hExcaliburInstance, Conversion_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Conversion_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Conversion_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Conversion_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Geometry_funcs[i].saveLastX;
        playBackMap[j].newXedit = Geometry_funcs[i].newXedit;
        playBackMap[j].routine = Geometry_funcs[i].routine;
        LoadString(hExcaliburInstance, Geometry_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Geometry_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Geometry_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Geometry_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Statistics_funcs[i].saveLastX;
        playBackMap[j].newXedit = Statistics_funcs[i].newXedit;
        playBackMap[j].routine = Statistics_funcs[i].routine;
        LoadString(hExcaliburInstance, Statistics_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Statistics_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Statistics_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Statistics_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Physics_funcs[i].saveLastX;
        playBackMap[j].newXedit = Physics_funcs[i].newXedit;
        playBackMap[j].routine = Physics_funcs[i].routine;
        LoadString(hExcaliburInstance, Physics_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Physics_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Physics_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Physics_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Program1_funcs[i].saveLastX;
        playBackMap[j].newXedit = Program1_funcs[i].newXedit;
        playBackMap[j].routine = Program1_funcs[i].routine;
        LoadString(hExcaliburInstance, Program1_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Program1_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Program1_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Program1_funcs[i].allowRecord;
        j++;
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        playBackMap[j].saveLastX = Program2_funcs[i].saveLastX;
        playBackMap[j].newXedit = Program2_funcs[i].newXedit;
        playBackMap[j].routine = Program2_funcs[i].routine;
        LoadString(hExcaliburInstance, Program2_funcs[i].keyTitle, playBackMap[j].funcText, MAX_MACRO_FUNC_TEXT-1);
        playBackMap[j].uniqueIndex = Program2_funcs[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = Program2_funcs[i].useFloatsLongs;
        playBackMap[j].allowRecord = Program2_funcs[i].allowRecord;
        j++;
    }

    i = 0;
    while (STOkeys[i].index != RPN_LAST_KEY) // Put all of the STO/RCL functions into the map
    {
        playBackMap[j].saveLastX = STOkeys[i].saveLastX;
        playBackMap[j].newXedit = STOkeys[i].newXedit;
        playBackMap[j].routine = STOkeys[i].routine;
        strcpy(playBackMap[j].funcText, STOkeys[i].keyTitle);
        playBackMap[j].uniqueIndex = STOkeys[i].uniqueIndex;
        playBackMap[j].useFloatsLongs = STOkeys[i].useFloatsLongs;
        playBackMap[j].allowRecord = STOkeys[i].allowRecord;
        j++;
        i++;
    }

    totalMappedButtonFuncs = j;

    if (totalMappedButtonFuncs > MAX_FUNCTIONS)
    {
        MessageBox(calcMainWindow, "Error - Maximum number of mapped functions exists!", "Excalibur Fatal Error", MB_OK);
    }
    else  // Bubble sort on unique Index so that we can move buttons around and not have this all break! Bubble sort fast enough - this list is almost sorted anyway...
    {
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
                break;          // We are Sorted!
        }
    }
}

void SaveProgramStep(int uniqueIndex)
{
    int i, k;

    i = 0;
    while (i < totalMappedButtonFuncs)
    {
        if (uniqueIndex == playBackMap[i].uniqueIndex)      // We always look up the function based on unique index!
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
                else        // Insert at end!
                {
                    playBack[currentMacroPlaybackIdx] = i;
                    playBackIdx++;
                    currentMacroPlaybackIdx++;
                }
            }
            else
                MessageBox(calcMainWindow, "Maximum program length has been reached.", "Excalibur Program Error", MB_OK);
            break;
        }
        i++;
    }
}

void callButtonFunc(void(*routine) (void), char useFloatsLongs, char allowRecord,
                    int uniqueIndex, char saveLastX, char newXedit, int updateSpareBar)
{
    if (IsWindowVisible(toolTipWnd))      // A button press reset's the window!
    {
        ShowWindow(toolTipWnd, SW_HIDE);
        toolTipCounter = 0;
    }

    if (progMode != PROG_NORMAL)        // Always ensure floats are "in-sync" with longs before any press!
    {
        LongsToFloats();
    }

    if (saveLastX == YES_L)
    {
        LASTX  = X;
        LASTXL = XL;
        LASTY  = Y;
        LASTYL = YL;
    }


    // Before we call the button function we need to ensure both stacks look right.
    // This will help with Macro programming between stacks!
    if (useFloatsLongs == USES_L && progMode == PROG_NORMAL)
    {
        progMode = PROG_DEC;
        FloatsToLongs();
        ShowStack();
        ShowStatus();
    }
    else if (useFloatsLongs == USES_F && progMode != PROG_NORMAL)
    {
        progMode = PROG_NORMAL;
        LongsToFloats();
        ShowStack();
        ShowStatus();
    }

    routine();                 // This calls the actual button routine to perform things like SIN, COS, CLX, etc

    if ((recModeON == 1) && (allowRecord == ALLOWREC))
    {
        SaveProgramStep(uniqueIndex);
    }

    if (newXedit != X_NULL)
    {
        Xedit = newXedit;
        hyperbolic = 0;
        finStore = 0;
        finRecall = 0;
        convInverse = 0;
        if (updateSpareBar != FALSE)
            UpdateSpareBar(" ");
    }
}

// -------------------------------------------------------------------------------------------------
// This version is streamlined for use when a macro is playing back for relatively blazing speed...
// -------------------------------------------------------------------------------------------------
void callButtonFunc_fast(void(*routine) (void), char useFloatsLongs, int uniqueIndex, char saveLastX, char newXedit)
{
    if (progMode != PROG_NORMAL)        // Always ensure floats are "in-sync" with longs before any press!
    {
        LongsToFloats();
    }

    if (saveLastX == YES_L)
    {
        LASTX  = X;
        LASTXL = XL;
        LASTY  = Y;
        LASTYL = YL;
    }

    // Before we call the button function we need to ensure both stacks look right.
    // This will help with Macro programming between stacks!
    if (useFloatsLongs == USES_L && progMode == PROG_NORMAL)
    {
        progMode = PROG_DEC;
        FloatsToLongs();
        ShowStack();
        ShowStatus();
    }
    else if (useFloatsLongs == USES_F && progMode != PROG_NORMAL)
    {
        progMode = PROG_NORMAL;
        LongsToFloats();
        ShowStack();
        ShowStatus();
    }

    routine();                 // This calls the actual button routine to perform things like SIN, COS, CLX, etc

    if (newXedit != X_NULL)
    {
        Xedit = newXedit;
        hyperbolic = 0;
        finStore = 0;
        finRecall = 0;
        convInverse = 0;
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


void RPN_Playback(void)
{
    int idx;
    int flashRunning = 499;
    int flashRunningDsp = 0;
    MSG msg;
    int xx = 2;
    DWORD initialTimer = 0;

    if (recModeON == 1)         // Always turn off rec mode before playback!
    {
        recModeON = 0;
        ShowStatus();
    }

    Xedit = X_NEW;
    MacroStackIdx = 0;

    macroPlayback = TRUE;

    GetAsyncKeyState(VK_ESCAPE);        // Get one reading at least!
    SetFocus(calcMainWindow);           // For long macros this will "release" the Play key depresion...

    if (IsWindowVisible(toolTipWnd))    // Macro running... hide tool tip window if it was visible.
    {
        ShowWindow(toolTipWnd, SW_HIDE);
        toolTipCounter = 0;
    }

    // ------------------------------------------------------------------------------------------------
    // This is the main macro playback loop... it has been somewhat optimized so that we push through
    // as many recorded keystrokes as possible. On a fairly pedestrian i5 computer (circa 2018), this
    // will run about 1 million 'Excalibur Instructions' per second. Good enough.
    // ------------------------------------------------------------------------------------------------
    for (currentMacroPlaybackIdx = 0; currentMacroPlaybackIdx < playBackIdx; currentMacroPlaybackIdx++)
    {
        static int dampenSystemProcessing = 0;

        // Don't need to peek THAT often... allows macro to run faster
        if (!(++dampenSystemProcessing & 7))
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

            if (currentMacroPlaybackIdx == playBackIdx)     // We may have ended the macro!!
                break;
        }

        if (traceMacroPlayback == FALSE)
        {
            if (halfSecTimer != initialTimer)
            {
                initialTimer = halfSecTimer;
                if ((++flashRunningDsp % 2) == 0)
                    UpdateSpareBar("    ");
                else
                    UpdateSpareBar("Run...");
            }
        }
        else // traceMacroPlayback is TRUE
        {
            showTrace = TRUE;
            ShowStack();
            showTrace = FALSE;
        }

        idx = playBack[currentMacroPlaybackIdx];

        if (playBackMap[idx].routine != NULL)
        {
            callButtonFunc_fast(playBackMap[idx].routine, playBackMap[idx].useFloatsLongs,
                                playBackMap[idx].uniqueIndex, playBackMap[idx].saveLastX, playBackMap[idx].newXedit);
        }

        if (traceMacroPlayback == TRUE)
        {
            Sleep(progDelayValue);

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

    UpdateSpareBar("    ");
    macroPlayback = FALSE;
}



void RPN_SingleStep(void)
{
    int idx;

    if (recModeON == 1)         // Always turn off rec mode before playback!
    {
        recModeON = 0;
        ShowStatus();
    }

    macroPlayback = TRUE;
    traceMacroPlayback = TRUE;

    showTrace = TRUE;
    ShowStack();
    showTrace = FALSE;

    if (currentMacroPlaybackIdx == playBackIdx)     // We may have ended the macro!!
    {
        Xedit = X_NEW;
    }
    else
    {
        idx = playBack[currentMacroPlaybackIdx];
        if (playBackMap[idx].routine != NULL)
        {
            // Always disallow record of playback keystrokes!
            callButtonFunc(playBackMap[idx].routine,
                            playBackMap[idx].useFloatsLongs, NORECORD,
                            playBackMap[idx].uniqueIndex, playBackMap[idx].saveLastX, playBackMap[idx].newXedit, FALSE);
            if (playBackMap[idx].newXedit != X_NULL)
            {
                Xedit = playBackMap[idx].newXedit;
            }
        }

    }

    Sleep(progDelayValue);
    ShowStack();

    UpdateSpareBar("    ");
    macroPlayback = FALSE;
    traceMacroPlayback = FALSE;
}


BOOL CALLBACK NotesDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, excaliburNotes);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(IDOK):           /* OK - Close and save notes */
            GetDlgItemText(hDlg, IDC_EDIT1, excaliburNotes, NOTES_SIZE-1);
            excaliburNotes[NOTES_SIZE] = CNULL;
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
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

void RPN_CopyX(void)
{
    ClipboardCopySelection(calcMainWindow, 0);
}

void RPN_inverse(void)
{
    if (X == 0.0)
    {
        RPN_error("Divide By Zero");
    }
    else
    {
        StackPush(1.0 / StackPop());
    }
}

void blinkXDisplay(void)
{
    GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN);
    SetDlgItemText(calcMainWindow, RPN_STACK_X, "                      ");
    Sleep(200);
    GetDlgItemText(calcMainWindow, RPN_STACK_X, tmpStr, MAX_STACK_STRLEN);
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
    // Simply so that constant numbers can lift stack appopriately.
}


#define EXCAL_HELP_STR  "TAB        Clear X                       Key UP     Rotate Stack Up\n"\
                        "Key C      Clear Stack                   Key DN     Rotate Stack Down\n"\
                        "Key L      Last X Register               Key E      Exponent\n"\
                        "Key X      Exchange X and Y              Key S      Store Register\n"\
                        "Key +      Add X and Y registers         Key R      Recall Register\n"\
                        "Key -      Subtract Y from X             Key A      Recall R1 Register\n"\
                        "Key *      Multiply X and Y              Key B      Recall R2 Register\n"\
                        "Key /      Divide Y into X               Key M      Display Mode\n"\
                        "ENTER      Enter number to Stack         Key H      Help Key\n"\
                        "Key N      Change Sign (Negate)          Key F      Last Function Bank\n"\
                        "\n"\
                        "DP         Decimal Point. Press twice for fraction.\n"\
                        "BKSP       Erases last character, otherwise clears X.\n"\
                        "F1-F10     First column of the currently selected function bank.\n"\
                        "SHIFT  FN  Second column of the currently selected function bank.\n"\
                        "CTL    FN  Second column of the currently selected function bank.\n"\
                        "CTLSFT FN  Fourth column of the currently selected function bank.\n"\
                        "CTRL 0-9   Quick selection of the number of decimal places to show.\n"\
                        "CTRL S     Brings up the Settings Dialog.\n"\
                        "CTRL P     Brings up the Program Manager Dialog.\n"\
                        "CTRL D     Brings up the Define Custom Button Set Dialog.\n"\
                        "CTRL D     Brings up the Define Custom Button Set Dialog.\n"\
                        "CTRL C     Copy X Register to the clipboard.\n"\
                        "CTRL A     Copy All Registers to the clipboard.\n"\
                        "CTRL V     Paste X Register from the clipboard.\n"\
                        "CTRL X     Exits Excalibur (same as pressing the Close icon).\n"\
                        ""

extern BOOL CALLBACK HelpDialog(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void ExcaliburHelp(void)
{
    DLGPROC lpHelpDialog;

    lpHelpDialog = (DLGPROC) MakeProcInstance((FARPROC) HelpDialog, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_HELP", calcMainWindow, lpHelpDialog)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpHelpDialog);
}

BOOL CALLBACK HelpDialog(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    SIZE lpSize;
    DWORD dw;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        SetDlgItemText(hDlg, 101, EXCAL_HELP_STR);

        hdc = GetDC(hDlg);
        SelectObject(hdc, hFixedFont);
        dw = GetTextExtentPoint32(hdc, "WWWWW88888OOOOOXXXXXWWWWW88888OOOOOXXXXXWWWWW88888OOOOOXXXXXOOOOOXXXXX", 70, &lpSize);
        ReleaseDC(hDlg, hdc);
        MoveWindow(hDlg, main_x+100, main_y+5, lpSize.cx + 50, (lpSize.cy * 29) + 12, TRUE);

        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(102):
            EndDialog(hDlg, FALSE);
            return TRUE;
        default:
            return FALSE;
        }

    case WM_SYSCOMMAND:
        switch(wParam & 0xFFF0)
        {
        case SC_CLOSE:
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
    }
    return FALSE;
}



/* Handle several math errors caused by passing a negative argument
 * to log or log10(_DOMAIN errors). When this happens, _matherr
 * returns the natural or base-10 logarithm of the absolute value
 * of the argument and suppresses the usual error message. */
int _matherr(struct _exception *except)
{
    /* Handle _OVERFLOW and _UNDERFLOW */
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
        return 0;               /* Else use the default actions */
    }
}

void SetNumLock(BOOL bState)
{
    GetKeyboardState((LPBYTE)&keyState);
    if ( (bState && !(keyState[VK_NUMLOCK] & 1)) ||
      (!bState && (keyState[VK_NUMLOCK] & 1)) )
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
    if ((retVal == 0) || ver.dwMajorVersion > 4)  // Windows 2k or XP, etc.
    {
        SetNumLock(TRUE);
    }
    else // Windows 9x
    {
        GetKeyboardState(keyState);
        keyState[VK_NUMLOCK] = (char) 0x81;
        SetKeyboardState(keyState);
    }
}

void PushConstant(double value)
{
    int i, startPos;

    if (Xedit == X_ENTER)
        X = (double) value;
    else
        StackPush((double) value);

    if (recModeON == 1) // If recording, generate key sequence to playback struct
    {
        sprintf(tmpStr, "%.12g", value);
        startPos = 0;
        if (tmpStr[0] == '-')
        {
            startPos=1;
        }
        SaveProgramStep(UNI_STARTCONST);
        for (i=startPos; i<(int)strlen(tmpStr); i++)
        {
            switch(tmpStr[i])
            {
                case('.'): SaveProgramStep(UNI_DIGDP); break;
                case('0'): SaveProgramStep(UNI_DIG0);  break;
                case('1'): SaveProgramStep(UNI_DIG1);  break;
                case('2'): SaveProgramStep(UNI_DIG2);  break;
                case('3'): SaveProgramStep(UNI_DIG3);  break;
                case('4'): SaveProgramStep(UNI_DIG4);  break;
                case('5'): SaveProgramStep(UNI_DIG5);  break;
                case('6'): SaveProgramStep(UNI_DIG6);  break;
                case('7'): SaveProgramStep(UNI_DIG7);  break;
                case('8'): SaveProgramStep(UNI_DIG8);  break;
                case('9'): SaveProgramStep(UNI_DIG9);  break;
                case('e'): //no break...
                case('E'):
                    if (startPos == 1)
                    {
                        startPos = 2;
                        SaveProgramStep(UNI_CHS);
                    }
                    SaveProgramStep(UNI_E);
                    break;
                case('-'): SaveProgramStep(UNI_CHS); break;
            }
        }
        if (startPos == 1)
        {
            startPos = 2;
            SaveProgramStep(UNI_CHS);
        }
        SaveProgramStep(UNI_ENDCONST);
    }
}

//=================================================================================================================
struct keypadStruct STOkeys[] = {
    {0,  UNI_ADDA,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddA,  "Store Plus R0",  ""},
    {0,  UNI_ADDB,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddB,  "Store Plus R1",  ""},
    {0,  UNI_ADDC,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddC,  "Store Plus R2",  ""},
    {0,  UNI_ADDD,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddD,  "Store Plus R3",  ""},
    {0,  UNI_ADDE,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddE,  "Store Plus R4",  ""},
    {0,  UNI_ADDF,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddF,  "Store Plus R5",  ""},
    {0,  UNI_ADDG,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddG,  "Store Plus R6",  ""},
    {0,  UNI_ADDH,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddH,  "Store Plus R7",  ""},
    {0,  UNI_ADDI,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddI,  "Store Plus R8",  ""},
    {0,  UNI_ADDJ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddJ,  "Store Plus R9",  ""},
    {0,  UNI_ADDK,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddK,  "Store Plus R10", ""},
    {0,  UNI_ADDL,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddL,  "Store Plus R11", ""},
    {0,  UNI_ADDM,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddM,  "Store Plus R12", ""},
    {0,  UNI_ADDN,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddN,  "Store Plus R13", ""},
    {0,  UNI_ADDO,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddO,  "Store Plus R14", ""},
    {0,  UNI_ADDP,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddP,  "Store Plus R15", ""},
    {0,  UNI_ADDQ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddQ,  "Store Plus R16", ""},
    {0,  UNI_ADDR,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddR,  "Store Plus R17", ""},
    {0,  UNI_ADDS,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddS,  "Store Plus R18", ""},
    {0,  UNI_ADDT,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddT,  "Store Plus R19", ""},
    {0,  UNI_ADDU,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddU,  "Store Plus R20", ""},
    {0,  UNI_ADDV,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddV,  "Store Plus R21", ""},
    {0,  UNI_ADDW,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddW,  "Store Plus R22", ""},
    {0,  UNI_ADDX,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddX,  "Store Plus R23", ""},
    {0,  UNI_ADDY,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddY,  "Store Plus R24", ""},
    {0,  UNI_ADDZ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeAddZ,  "Store Plus R25", ""},

    {0,  UNI_SUBA,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubA,  "Store Minus R0",  ""},
    {0,  UNI_SUBB,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubB,  "Store Minus R1",  ""},
    {0,  UNI_SUBC,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubC,  "Store Minus R2",  ""},
    {0,  UNI_SUBD,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubD,  "Store Minus R3",  ""},
    {0,  UNI_SUBE,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubE,  "Store Minus R4",  ""},
    {0,  UNI_SUBF,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubF,  "Store Minus R5",  ""},
    {0,  UNI_SUBG,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubG,  "Store Minus R6",  ""},
    {0,  UNI_SUBH,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubH,  "Store Minus R7",  ""},
    {0,  UNI_SUBI,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubI,  "Store Minus R8",  ""},
    {0,  UNI_SUBJ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubJ,  "Store Minus R9",  ""},
    {0,  UNI_SUBK,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubK,  "Store Minus R10", ""},
    {0,  UNI_SUBL,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubL,  "Store Minus R11", ""},
    {0,  UNI_SUBM,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubM,  "Store Minus R12", ""},
    {0,  UNI_SUBN,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubN,  "Store Minus R13", ""},
    {0,  UNI_SUBO,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubO,  "Store Minus R14", ""},
    {0,  UNI_SUBP,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubP,  "Store Minus R15", ""},
    {0,  UNI_SUBQ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubQ,  "Store Minus R16", ""},
    {0,  UNI_SUBR,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubR,  "Store Minus R17", ""},
    {0,  UNI_SUBS,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubS,  "Store Minus R18", ""},
    {0,  UNI_SUBT,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubT,  "Store Minus R19", ""},
    {0,  UNI_SUBU,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubU,  "Store Minus R20", ""},
    {0,  UNI_SUBV,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubV,  "Store Minus R21", ""},
    {0,  UNI_SUBW,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubW,  "Store Minus R22", ""},
    {0,  UNI_SUBX,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubX,  "Store Minus R23", ""},
    {0,  UNI_SUBY,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubY,  "Store Minus R24", ""},
    {0,  UNI_SUBZ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeSubZ,  "Store Minus R25", ""},

    {0,  UNI_MULA,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulA,  "Store Mult R0",   ""},
    {0,  UNI_MULB,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulB,  "Store Mult R1",   ""},
    {0,  UNI_MULC,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulC,  "Store Mult R2",   ""},
    {0,  UNI_MULD,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulD,  "Store Mult R3",   ""},
    {0,  UNI_MULE,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulE,  "Store Mult R4",   ""},
    {0,  UNI_MULF,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulF,  "Store Mult R5",   ""},
    {0,  UNI_MULG,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulG,  "Store Mult R6",   ""},
    {0,  UNI_MULH,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulH,  "Store Mult R7",   ""},
    {0,  UNI_MULI,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulI,  "Store Mult R8",   ""},
    {0,  UNI_MULJ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulJ,  "Store Mult R9",   ""},
    {0,  UNI_MULK,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulK,  "Store Mult R10",  ""},
    {0,  UNI_MULL,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulL,  "Store Mult R11",  ""},
    {0,  UNI_MULM,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulM,  "Store Mult R12",  ""},
    {0,  UNI_MULN,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulN,  "Store Mult R13",  ""},
    {0,  UNI_MULO,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulO,  "Store Mult R14",  ""},
    {0,  UNI_MULP,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulP,  "Store Mult R15",  ""},
    {0,  UNI_MULQ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulQ,  "Store Mult R16",  ""},
    {0,  UNI_MULR,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulR,  "Store Mult R17",  ""},
    {0,  UNI_MULS,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulS,  "Store Mult R18",  ""},
    {0,  UNI_MULT,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulT,  "Store Mult R19",  ""},
    {0,  UNI_MULU,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulU,  "Store Mult R20",  ""},
    {0,  UNI_MULV,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulV,  "Store Mult R21",  ""},
    {0,  UNI_MULW,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulW,  "Store Mult R22",  ""},
    {0,  UNI_MULX,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulX,  "Store Mult R23",  ""},
    {0,  UNI_MULY,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulY,  "Store Mult R24",  ""},
    {0,  UNI_MULZ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeMulZ,  "Store Mult R25",  ""},

    {0,  UNI_DIVA,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivA,  "Store Div R0",    ""},
    {0,  UNI_DIVB,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivB,  "Store Div R1",    ""},
    {0,  UNI_DIVC,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivC,  "Store Div R2",    ""},
    {0,  UNI_DIVD,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivD,  "Store Div R3",    ""},
    {0,  UNI_DIVE,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivE,  "Store Div R4",    ""},
    {0,  UNI_DIVF,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivF,  "Store Div R5",    ""},
    {0,  UNI_DIVG,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivG,  "Store Div R6",    ""},
    {0,  UNI_DIVH,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivH,  "Store Div R7",    ""},
    {0,  UNI_DIVI,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivI,  "Store Div R8",    ""},
    {0,  UNI_DIVJ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivJ,  "Store Div R9",    ""},
    {0,  UNI_DIVK,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivK,  "Store Div R10",   ""},
    {0,  UNI_DIVL,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivL,  "Store Div R11",   ""},
    {0,  UNI_DIVM,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivM,  "Store Div R12",   ""},
    {0,  UNI_DIVN,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivN,  "Store Div R13",   ""},
    {0,  UNI_DIVO,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivO,  "Store Div R14",   ""},
    {0,  UNI_DIVP,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivP,  "Store Div R15",   ""},
    {0,  UNI_DIVQ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivQ,  "Store Div R16",   ""},
    {0,  UNI_DIVR,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivR,  "Store Div R17",   ""},
    {0,  UNI_DIVS,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivS,  "Store Div R18",   ""},
    {0,  UNI_DIVT,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivT,  "Store Div R19",   ""},
    {0,  UNI_DIVU,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivU,  "Store Div R20",   ""},
    {0,  UNI_DIVV,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivV,  "Store Div R21",   ""},
    {0,  UNI_DIVW,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivW,  "Store Div R22",   ""},
    {0,  UNI_DIVX,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivX,  "Store Div R23",   ""},
    {0,  UNI_DIVY,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivY,  "Store Div R24",   ""},
    {0,  UNI_DIVZ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeDivZ,  "Store Div R25",   ""},


    {0,  UNI_STOA,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeA,     "Store R0",        ""},
    {0,  UNI_STOB,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeB,     "Store R1",        ""},
    {0,  UNI_STOC,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeC,     "Store R2",        ""},
    {0,  UNI_STOD,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeD,     "Store R3",        ""},
    {0,  UNI_STOE,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeE,     "Store R4",        ""},
    {0,  UNI_STOF,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeF,     "Store R5",        ""},
    {0,  UNI_STOG,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeG,     "Store R6",        ""},
    {0,  UNI_STOH,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeH,     "Store R7",        ""},
    {0,  UNI_STOI,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeI,     "Store R8",        ""},
    {0,  UNI_STOJ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeJ,     "Store R9",        ""},
    {0,  UNI_STOK,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeK,     "Store R10",       ""},
    {0,  UNI_STOL,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeL,     "Store R11",       ""},
    {0,  UNI_STOM,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeM,     "Store R12",       ""},
    {0,  UNI_STON,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeN,     "Store R13",       ""},
    {0,  UNI_STOO,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeO,     "Store R14",       ""},
    {0,  UNI_STOP,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeP,     "Store R15",       ""},
    {0,  UNI_STOQ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeQ,     "Store R16",       ""},
    {0,  UNI_STOR,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeR,     "Store R17",       ""},
    {0,  UNI_STOS,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeS,     "Store R18",       ""},
    {0,  UNI_STOT,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeT,     "Store R19",       ""},
    {0,  UNI_STOU,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeU,     "Store R20",       ""},
    {0,  UNI_STOV,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeV,     "Store R21",       ""},
    {0,  UNI_STOW,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeW,     "Store R22",       ""},
    {0,  UNI_STOX,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeX,     "Store R23",       ""},
    {0,  UNI_STOY,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeY,     "Store R24",       ""},
    {0,  UNI_STOZ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_storeZ,     "Store R25",       ""},

    {0,  UNI_RCLA,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallA,    "Recall R0",       ""},
    {0,  UNI_RCLB,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallB,    "Recall R1",       ""},
    {0,  UNI_RCLC,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallC,    "Recall R2",       ""},
    {0,  UNI_RCLD,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallD,    "Recall R3",       ""},
    {0,  UNI_RCLE,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallE,    "Recall R4",       ""},
    {0,  UNI_RCLF,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallF,    "Recall R5",       ""},
    {0,  UNI_RCLG,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallG,    "Recall R6",       ""},
    {0,  UNI_RCLH,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallH,    "Recall R7",       ""},
    {0,  UNI_RCLI,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallI,    "Recall R8",       ""},
    {0,  UNI_RCLJ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallJ,    "Recall R9",       ""},
    {0,  UNI_RCLK,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallK,    "Recall R10",      ""},
    {0,  UNI_RCLL,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallL,    "Recall R11",      ""},
    {0,  UNI_RCLM,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallM,    "Recall R12",      ""},
    {0,  UNI_RCLN,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallN,    "Recall R13",      ""},
    {0,  UNI_RCLO,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallO,    "Recall R14",      ""},
    {0,  UNI_RCLP,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallP,    "Recall R15",      ""},
    {0,  UNI_RCLQ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallQ,    "Recall R16",      ""},
    {0,  UNI_RCLR,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallR,    "Recall R17",      ""},
    {0,  UNI_RCLS,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallS,    "Recall R18",      ""},
    {0,  UNI_RCLT,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallT,    "Recall R19",      ""},
    {0,  UNI_RCLU,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallU,    "Recall R20",      ""},
    {0,  UNI_RCLV,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallV,    "Recall R21",      ""},
    {0,  UNI_RCLW,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallW,    "Recall R22",      ""},
    {0,  UNI_RCLX,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallX,    "Recall R23",      ""},
    {0,  UNI_RCLY,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallY,    "Recall R24",      ""},
    {0,  UNI_RCLZ,   USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_recallZ,    "Recall R25",      ""},

    {0,  UNI_STARTCONST,USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_endConst, "Start Const",    ""},
    {0,  UNI_ENDCONST,  USES_FL, ALLOWREC, ' ', YES_L, X_NEW, RPN_endConst, "End Const",      ""},

    {RPN_LAST_KEY,  UNI_UNUSED, USES_FL, ALLOWREC, ' ', NO_L, X_NEW, NULL,  "Unused",         "Unused"}
};


void RPN_storeReg(int reg)
{
    if (progMode == PROG_NORMAL)
        STO[reg] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[reg] = (PROG_SIGNEDLONG) XL;
        else
            STO[reg] = XL;
    }
    if (recModeON == 1)
    {
        SaveProgramStep(UNI_STOA+reg);
    }
}

void RPN_storeA(void) { RPN_storeReg(0); }
void RPN_storeB(void) { RPN_storeReg(1); }
void RPN_storeC(void) { RPN_storeReg(2); }
void RPN_storeD(void) { RPN_storeReg(3); }
void RPN_storeE(void) { RPN_storeReg(4); }
void RPN_storeF(void) { RPN_storeReg(5); }
void RPN_storeG(void) { RPN_storeReg(6); }
void RPN_storeH(void) { RPN_storeReg(7); }
void RPN_storeI(void) { RPN_storeReg(8); }
void RPN_storeJ(void) { RPN_storeReg(9); }
void RPN_storeK(void) { RPN_storeReg(10); }
void RPN_storeL(void) { RPN_storeReg(11); }
void RPN_storeM(void) { RPN_storeReg(12); }
void RPN_storeN(void) { RPN_storeReg(13); }
void RPN_storeO(void) { RPN_storeReg(14); }
void RPN_storeP(void) { RPN_storeReg(15); }
void RPN_storeQ(void) { RPN_storeReg(16); }
void RPN_storeR(void) { RPN_storeReg(17); }
void RPN_storeS(void) { RPN_storeReg(18); }
void RPN_storeT(void) { RPN_storeReg(19); }
void RPN_storeU(void) { RPN_storeReg(20); }
void RPN_storeV(void) { RPN_storeReg(21); }
void RPN_storeW(void) { RPN_storeReg(22); }
void RPN_storeX(void) { RPN_storeReg(23); }
void RPN_storeY(void) { RPN_storeReg(24); }
void RPN_storeZ(void) { RPN_storeReg(25); }

void RPN_storeAddReg(int reg)
{
    if (progMode == PROG_NORMAL)
        STO[reg] += X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[reg] += (PROG_SIGNEDLONG) XL;
        else
            STO[reg] += XL;
    }
    if (recModeON == 1)
    {
        SaveProgramStep(UNI_ADDA+reg);
    }
}


void RPN_storeAddA(void) { RPN_storeAddReg(0); }
void RPN_storeAddB(void) { RPN_storeAddReg(1); }
void RPN_storeAddC(void) { RPN_storeAddReg(2); }
void RPN_storeAddD(void) { RPN_storeAddReg(3); }
void RPN_storeAddE(void) { RPN_storeAddReg(4); }
void RPN_storeAddF(void) { RPN_storeAddReg(5); }
void RPN_storeAddG(void) { RPN_storeAddReg(6); }
void RPN_storeAddH(void) { RPN_storeAddReg(7); }
void RPN_storeAddI(void) { RPN_storeAddReg(8); }
void RPN_storeAddJ(void) { RPN_storeAddReg(9); }
void RPN_storeAddK(void) { RPN_storeAddReg(10); }
void RPN_storeAddL(void) { RPN_storeAddReg(11); }
void RPN_storeAddM(void) { RPN_storeAddReg(12); }
void RPN_storeAddN(void) { RPN_storeAddReg(13); }
void RPN_storeAddO(void) { RPN_storeAddReg(14); }
void RPN_storeAddP(void) { RPN_storeAddReg(15); }
void RPN_storeAddQ(void) { RPN_storeAddReg(16); }
void RPN_storeAddR(void) { RPN_storeAddReg(17); }
void RPN_storeAddS(void) { RPN_storeAddReg(18); }
void RPN_storeAddT(void) { RPN_storeAddReg(19); }
void RPN_storeAddU(void) { RPN_storeAddReg(20); }
void RPN_storeAddV(void) { RPN_storeAddReg(21); }
void RPN_storeAddW(void) { RPN_storeAddReg(22); }
void RPN_storeAddX(void) { RPN_storeAddReg(23); }
void RPN_storeAddY(void) { RPN_storeAddReg(24); }
void RPN_storeAddZ(void) { RPN_storeAddReg(25); }


void RPN_storeSubReg(int reg)
{
    if (progMode == PROG_NORMAL)
        STO[reg] -= X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[reg] -= (PROG_SIGNEDLONG) XL;
        else
            STO[reg] -= XL;
    }
    if (recModeON == 1)
    {
        SaveProgramStep(UNI_SUBA+reg);
    }
}


void RPN_storeSubA(void) { RPN_storeSubReg(0); }
void RPN_storeSubB(void) { RPN_storeSubReg(1); }
void RPN_storeSubC(void) { RPN_storeSubReg(2); }
void RPN_storeSubD(void) { RPN_storeSubReg(3); }
void RPN_storeSubE(void) { RPN_storeSubReg(4); }
void RPN_storeSubF(void) { RPN_storeSubReg(5); }
void RPN_storeSubG(void) { RPN_storeSubReg(6); }
void RPN_storeSubH(void) { RPN_storeSubReg(7); }
void RPN_storeSubI(void) { RPN_storeSubReg(8); }
void RPN_storeSubJ(void) { RPN_storeSubReg(9); }
void RPN_storeSubK(void) { RPN_storeSubReg(10); }
void RPN_storeSubL(void) { RPN_storeSubReg(11); }
void RPN_storeSubM(void) { RPN_storeSubReg(12); }
void RPN_storeSubN(void) { RPN_storeSubReg(13); }
void RPN_storeSubO(void) { RPN_storeSubReg(14); }
void RPN_storeSubP(void) { RPN_storeSubReg(15); }
void RPN_storeSubQ(void) { RPN_storeSubReg(16); }
void RPN_storeSubR(void) { RPN_storeSubReg(17); }
void RPN_storeSubS(void) { RPN_storeSubReg(18); }
void RPN_storeSubT(void) { RPN_storeSubReg(19); }
void RPN_storeSubU(void) { RPN_storeSubReg(20); }
void RPN_storeSubV(void) { RPN_storeSubReg(21); }
void RPN_storeSubW(void) { RPN_storeSubReg(22); }
void RPN_storeSubX(void) { RPN_storeSubReg(23); }
void RPN_storeSubY(void) { RPN_storeSubReg(24); }
void RPN_storeSubZ(void) { RPN_storeSubReg(25); }


void RPN_storeMulReg(int reg)
{
    if (progMode == PROG_NORMAL)
        STO[reg] *= X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[reg] *= (PROG_SIGNEDLONG) XL;
        else
            STO[reg] *= XL;
    }
    if (recModeON == 1)
    {
        SaveProgramStep(UNI_MULA+reg);
    }
}


void RPN_storeMulA(void) { RPN_storeMulReg(0); }
void RPN_storeMulB(void) { RPN_storeMulReg(1); }
void RPN_storeMulC(void) { RPN_storeMulReg(2); }
void RPN_storeMulD(void) { RPN_storeMulReg(3); }
void RPN_storeMulE(void) { RPN_storeMulReg(4); }
void RPN_storeMulF(void) { RPN_storeMulReg(5); }
void RPN_storeMulG(void) { RPN_storeMulReg(6); }
void RPN_storeMulH(void) { RPN_storeMulReg(7); }
void RPN_storeMulI(void) { RPN_storeMulReg(8); }
void RPN_storeMulJ(void) { RPN_storeMulReg(9); }
void RPN_storeMulK(void) { RPN_storeMulReg(10); }
void RPN_storeMulL(void) { RPN_storeMulReg(11); }
void RPN_storeMulM(void) { RPN_storeMulReg(12); }
void RPN_storeMulN(void) { RPN_storeMulReg(13); }
void RPN_storeMulO(void) { RPN_storeMulReg(14); }
void RPN_storeMulP(void) { RPN_storeMulReg(15); }
void RPN_storeMulQ(void) { RPN_storeMulReg(16); }
void RPN_storeMulR(void) { RPN_storeMulReg(17); }
void RPN_storeMulS(void) { RPN_storeMulReg(18); }
void RPN_storeMulT(void) { RPN_storeMulReg(19); }
void RPN_storeMulU(void) { RPN_storeMulReg(20); }
void RPN_storeMulV(void) { RPN_storeMulReg(21); }
void RPN_storeMulW(void) { RPN_storeMulReg(22); }
void RPN_storeMulX(void) { RPN_storeMulReg(23); }
void RPN_storeMulY(void) { RPN_storeMulReg(24); }
void RPN_storeMulZ(void) { RPN_storeMulReg(25); }

void RPN_storeDivReg(int reg)
{
    if (progMode == PROG_NORMAL)
    {
        if (X != 0.0)
            STO[reg] /= X;
        else
            RPN_error("STO: Divide by Zero");
    }
    else
    {
        if (XL != (PROG_LONG) 0)
        {
            if (wordMode == PROG_SIGNED)
                STO[reg] /= (PROG_SIGNEDLONG) XL;
            else
                STO[reg] /= XL;
        }
        else
            RPN_error("STO: Divide by Zero");
    }

    if (recModeON == 1)
    {
        SaveProgramStep(UNI_DIVA+reg);
    }
}


void RPN_storeDivA(void) { RPN_storeDivReg(0); }
void RPN_storeDivB(void) { RPN_storeDivReg(1); }
void RPN_storeDivC(void) { RPN_storeDivReg(2); }
void RPN_storeDivD(void) { RPN_storeDivReg(3); }
void RPN_storeDivE(void) { RPN_storeDivReg(4); }
void RPN_storeDivF(void) { RPN_storeDivReg(5); }
void RPN_storeDivG(void) { RPN_storeDivReg(6); }
void RPN_storeDivH(void) { RPN_storeDivReg(7); }
void RPN_storeDivI(void) { RPN_storeDivReg(8); }
void RPN_storeDivJ(void) { RPN_storeDivReg(9); }
void RPN_storeDivK(void) { RPN_storeDivReg(10); }
void RPN_storeDivL(void) { RPN_storeDivReg(11); }
void RPN_storeDivM(void) { RPN_storeDivReg(12); }
void RPN_storeDivN(void) { RPN_storeDivReg(13); }
void RPN_storeDivO(void) { RPN_storeDivReg(14); }
void RPN_storeDivP(void) { RPN_storeDivReg(15); }
void RPN_storeDivQ(void) { RPN_storeDivReg(16); }
void RPN_storeDivR(void) { RPN_storeDivReg(17); }
void RPN_storeDivS(void) { RPN_storeDivReg(18); }
void RPN_storeDivT(void) { RPN_storeDivReg(19); }
void RPN_storeDivU(void) { RPN_storeDivReg(20); }
void RPN_storeDivV(void) { RPN_storeDivReg(21); }
void RPN_storeDivW(void) { RPN_storeDivReg(22); }
void RPN_storeDivX(void) { RPN_storeDivReg(23); }
void RPN_storeDivY(void) { RPN_storeDivReg(24); }
void RPN_storeDivZ(void) { RPN_storeDivReg(25); }


void RPN_recallReg(int reg)
{
    if (progMode == PROG_NORMAL)
    {
        if (Xedit == X_ENTER)
            X = STO[reg];
        else
            StackPush(STO[reg]);
    }
    else
    {
        if (Xedit == X_ENTER)
            XL = maskStackStuff((PROG_LONG) STO[reg]);
        else
            StackPushL((PROG_LONG) STO[reg]);
    }

    if (recModeON == 1)
    {
        SaveProgramStep(UNI_RCLA+reg);
    }
}

void RPN_recallA(void) { RPN_recallReg(0); }
void RPN_recallB(void) { RPN_recallReg(1); }
void RPN_recallC(void) { RPN_recallReg(2); }
void RPN_recallD(void) { RPN_recallReg(3); }
void RPN_recallE(void) { RPN_recallReg(4); }
void RPN_recallF(void) { RPN_recallReg(5); }
void RPN_recallG(void) { RPN_recallReg(6); }
void RPN_recallH(void) { RPN_recallReg(7); }
void RPN_recallI(void) { RPN_recallReg(8); }
void RPN_recallJ(void) { RPN_recallReg(9); }
void RPN_recallK(void) { RPN_recallReg(10); }
void RPN_recallL(void) { RPN_recallReg(11); }
void RPN_recallM(void) { RPN_recallReg(12); }
void RPN_recallN(void) { RPN_recallReg(13); }
void RPN_recallO(void) { RPN_recallReg(14); }
void RPN_recallP(void) { RPN_recallReg(15); }
void RPN_recallQ(void) { RPN_recallReg(16); }
void RPN_recallR(void) { RPN_recallReg(17); }
void RPN_recallS(void) { RPN_recallReg(18); }
void RPN_recallT(void) { RPN_recallReg(19); }
void RPN_recallU(void) { RPN_recallReg(20); }
void RPN_recallV(void) { RPN_recallReg(21); }
void RPN_recallW(void) { RPN_recallReg(22); }
void RPN_recallX(void) { RPN_recallReg(23); }
void RPN_recallY(void) { RPN_recallReg(24); }
void RPN_recallZ(void) { RPN_recallReg(25); }

