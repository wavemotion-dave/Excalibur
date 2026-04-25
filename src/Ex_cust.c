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
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <mmsystem.h>
#include <ctype.h>
#include "EXCAL.h"

struct funcStruct Custom_funcsTmp[MAX_FUNCS];   // so that cancel can be
                                                // pressed
struct funcStruct Custom_funcs[MAX_FUNCS] = {
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN2,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN3,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN4,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN5,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN6,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN7,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN8,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN9,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN10, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN11, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN12, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN13, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN14, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN15, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN16, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN17, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN18, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN19, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN20, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN21, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN22, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN23, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN24, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN25, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN26, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN27, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN28, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN29, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN30, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN31, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN32, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN33, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN34, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN35, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN36, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN37, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN38, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN39, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL},
    {FN40, 0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", YES_L, X_NEW, NULL,    T_NULL, H_NULL}
};

extern void Cust_MacroPlayback1(void);
extern void Cust_MacroPlayback2(void);
extern void Cust_MacroPlayback3(void);
extern void Cust_MacroPlayback4(void);
extern void Cust_MacroPlayback5(void);
extern void Cust_MacroPlayback6(void);
extern void Cust_MacroPlayback7(void);
extern void Cust_MacroPlayback8(void);
extern void Cust_MacroPlayback9(void);
extern void Cust_MacroPlayback10(void);
extern void Cust_MacroPlayback11(void);
extern void Cust_MacroPlayback12(void);
extern void Cust_MacroPlayback13(void);
extern void Cust_MacroPlayback14(void);
extern void Cust_MacroPlayback15(void);
extern void Cust_MacroPlayback16(void);
extern void Cust_MacroPlayback17(void);
extern void Cust_MacroPlayback18(void);
extern void Cust_MacroPlayback19(void);
extern void Cust_MacroPlayback20(void);
extern void Cust_MacroPlayback21(void);
extern void Cust_MacroPlayback22(void);
extern void Cust_MacroPlayback23(void);
extern void Cust_MacroPlayback24(void);
extern void Cust_MacroPlayback25(void);
extern void Cust_MacroPlayback26(void);
extern void Cust_MacroPlayback27(void);
extern void Cust_MacroPlayback28(void);
extern void Cust_MacroPlayback29(void);
extern void Cust_MacroPlayback30(void);
extern void Cust_MacroPlayback31(void);
extern void Cust_MacroPlayback32(void);
extern void Cust_MacroPlayback33(void);
extern void Cust_MacroPlayback34(void);
extern void Cust_MacroPlayback35(void);
extern void Cust_MacroPlayback36(void);
extern void Cust_MacroPlayback37(void);
extern void Cust_MacroPlayback38(void);
extern void Cust_MacroPlayback39(void);
extern void Cust_MacroPlayback40(void);
extern void Cust_MacroPlayback41(void);
extern void Cust_MacroPlayback42(void);
extern void Cust_MacroPlayback43(void);
extern void Cust_MacroPlayback44(void);
extern void Cust_MacroPlayback45(void);
extern void Cust_MacroPlayback46(void);
extern void Cust_MacroPlayback47(void);
extern void Cust_MacroPlayback48(void);
extern void Cust_MacroPlayback49(void);
extern void Cust_MacroPlayback50(void);
extern void Cust_MacroPlayback51(void);
extern void Cust_MacroPlayback52(void);
extern void Cust_MacroPlayback53(void);
extern void Cust_MacroPlayback54(void);
extern void Cust_MacroPlayback55(void);
extern void Cust_MacroPlayback56(void);
extern void Cust_MacroPlayback57(void);
extern void Cust_MacroPlayback58(void);
extern void Cust_MacroPlayback59(void);
extern void Cust_MacroPlayback60(void);
extern void Cust_MacroPlayback61(void);
extern void Cust_MacroPlayback62(void);
extern void Cust_MacroPlayback63(void);
extern void Cust_MacroPlayback64(void);


struct funcStruct MacroFuncs[MAX_MACROS] = {
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback1,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback2,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback3,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback4,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback5,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback6,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback7,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback8,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback9,     T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback10,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback11,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback12,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback13,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback14,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback15,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback16,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback17,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback18,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback19,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback20,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback21,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback22,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback23,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback24,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback25,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback26,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback27,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback28,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback29,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback30,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback31,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback32,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback33,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback34,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback35,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback36,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback37,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback38,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback39,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback40,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback41,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback42,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback43,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback44,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback45,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback46,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback47,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback48,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback49,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback50,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback51,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback52,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback53,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback54,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback55,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback56,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback57,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback58,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback59,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback60,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback61,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback62,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback63,    T_NULL,  H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback64,    T_NULL,  H_NULL},
};


struct funcStruct *customPullFuncs = (struct funcStruct *) &Scientific_funcs;
int savedIndexFunc = 0;
struct customSaveStruct customSave[MAX_FUNCS];

extern BOOL CALLBACK CustomDefDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void cust_define(void)
{
    DLGPROC lpCustomDefDlgProc;

    lpCustomDefDlgProc = (DLGPROC) MakeProcInstance((FARPROC) CustomDefDlgProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_CUSTOMDEF", calcMainWindow, lpCustomDefDlgProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpCustomDefDlgProc);
}

extern int processCustomFuncs(HWND hDlg);
extern int processCustomFuncsSolo(HWND hDlg, int i);
BOOL CALLBACK CustomDefDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[60];
    int saveIdx;
    int newIdx;
    int i;
    static LRESULT macroItem;
    static int index;
    static int lbTabStops[1] = { 40 };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), LB_SETTABSTOPS, 1, (DWORD) lbTabStops);
        customPullFuncs = (struct funcStruct *) &Scientific_funcs;
        processCustomFuncs(hDlg);
        savedIndexFunc = CUSTOM_SAVE_SCI;
        SendMessage(GetDlgItem(hDlg, 190), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        memcpy(Custom_funcsTmp, Custom_funcs, sizeof(Custom_funcs));
        for (i = 0; i < MAX_MACROS; i++)
        {
            sprintf(tmp, "%-6s:\t%s", macro_short_names[i], macroName[i]);
            SendDlgItemMessage(hDlg, MACRO_LIST, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        }

        SetFocus(hDlg);

        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(MACRO_LIST):     // click on program list?!?
            macroItem = SendDlgItemMessage(hDlg, MACRO_LIST, LB_GETCURSEL, 0, 0L);
            strcpy(tmp, macroName[macroItem]);
            SetDlgItemText(hDlg, 200, tmp);
            break;
        }

        if (wParam >= 101 && wParam <= 140)
        {
            macroItem = (LRESULT) - 1;
            index = wParam - 101;
            LoadString(hExcaliburInstance, customPullFuncs[index].keyTitle, tmp, sizeof(tmp) - 1);
            SetDlgItemText(hDlg, 200, tmp);
            SetFocus(hDlg);
        }
        else if (wParam >= 141 && wParam <= 180)
        {
            newIdx = wParam - 141;
            saveIdx = Custom_funcs[newIdx].index;
            if (macroItem != (LRESULT) - 1)     // should we copy the macro instead...
            {
                memcpy(&Custom_funcs[newIdx], &MacroFuncs[macroItem], sizeof(struct funcStruct));
                strcpy(Custom_funcs[newIdx].desc, macro_short_names[macroItem]);
                customSave[newIdx].custom_save_idx = CUSTOM_SAVE_MAC;
                customSave[newIdx].func_idx = (int16_t)macroItem;
                Custom_funcs[newIdx].keyTitle = 9000 + macroItem;
            }
            else
            {
                memcpy(&Custom_funcs[newIdx], &customPullFuncs[index], sizeof(struct funcStruct));
                customSave[newIdx].custom_save_idx = savedIndexFunc;
                customSave[newIdx].func_idx = (int16_t)index;
            }
            Custom_funcs[newIdx].index = saveIdx;
            processCustomFuncsSolo(hDlg, newIdx);
            SetFocus(hDlg);
        }
        else
            switch(wParam)
            {
            case(IDC_CUSTOM_OK):      // OK
                EndDialog(hDlg, FALSE);
                processFuncs();
                SaveToDisk();
                break;

            case(IDC_CUSTOM_CANCEL):  // Cancel
                EndDialog(hDlg, FALSE);
                memcpy(Custom_funcs, Custom_funcsTmp, sizeof(Custom_funcs));
                processFuncs();
                SaveToDisk();
                break;

            case(190):        // Sci funcs
                savedIndexFunc = CUSTOM_SAVE_SCI;
                customPullFuncs = (struct funcStruct *) &Scientific_funcs;
                processCustomFuncs(hDlg);
                break;
            case(191):        // Buisness funcs
                savedIndexFunc = CUSTOM_SAVE_BUS;
                customPullFuncs = (struct funcStruct *) &Financial_funcs;
                processCustomFuncs(hDlg);
                break;
            case(192):        // Programming funcs
                savedIndexFunc = CUSTOM_SAVE_PRO;
                customPullFuncs = (struct funcStruct *) &CompSci_funcs;
                processCustomFuncs(hDlg);
                break;
            case(193):        // Stats funcs
                savedIndexFunc = CUSTOM_SAVE_STA;
                customPullFuncs = (struct funcStruct *) &Statistics_funcs;
                processCustomFuncs(hDlg);
                break;
            case(194):        // Geom funcs
                savedIndexFunc = CUSTOM_SAVE_GEO;
                customPullFuncs = (struct funcStruct *) &Geometry_funcs;
                processCustomFuncs(hDlg);
                break;
            case(196):        // Conversion funcs
                savedIndexFunc = CUSTOM_SAVE_CON;
                customPullFuncs = (struct funcStruct *) &Conversion_funcs;
                processCustomFuncs(hDlg);
                break;
            case(197):        // Conversion funcs
                savedIndexFunc = CUSTOM_SAVE_COM;
                customPullFuncs = (struct funcStruct *) &Complex_funcs;
                processCustomFuncs(hDlg);
                break;
            case(198):        // Physics funcs
                savedIndexFunc = CUSTOM_SAVE_PHY;
                customPullFuncs = (struct funcStruct *) &Physics_funcs;
                processCustomFuncs(hDlg);
                break;
            case(199):        // Program Bank 1 funcs
                savedIndexFunc = CUSTOM_SAVE_MACBANK;
                customPullFuncs = (struct funcStruct *) &Program1_funcs;
                processCustomFuncs(hDlg);
                break;
            case(201):        // Program Bank II funcs
                savedIndexFunc = CUSTOM_SAVE_MACBANK2;
                customPullFuncs = (struct funcStruct *) &Program2_funcs;
                processCustomFuncs(hDlg);
                break;
            }
        break;

    case WM_DESTROY:
    case WM_CLOSE:
        EndDialog(hDlg, FALSE);
        processFuncs();
        break;
    }
    return FALSE;
}

int processCustomFuncs(HWND hDlg)
{
    int i;

//TODO: clean up - only one font now
    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (customPullFuncs[i].fontType == 0)   // Sans Serif font
            SendMessage(GetDlgItem(hDlg, 101 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        else                    // Symbol font
        if (customPullFuncs[i].fontType == 1)   // Symbol
            SendMessage(GetDlgItem(hDlg, 101 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        else                    // Line Draw Font
            SendMessage(GetDlgItem(hDlg, 101 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        SetDlgItemText(hDlg, 101 + i, customPullFuncs[i].desc);
    }

    for (i = 0; i < MAX_FUNCS; i++)
    {
        if (Custom_funcs[i].fontType == 0)      // Sans Serif font
            SendMessage(GetDlgItem(hDlg, 141 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        else                    // Symbol font
        if (Custom_funcs[i].fontType == 1)      // Symbol
            SendMessage(GetDlgItem(hDlg, 141 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        else                    // Line Draw Font
            SendMessage(GetDlgItem(hDlg, 141 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        SetDlgItemText(hDlg, 141 + i, Custom_funcs[i].desc);
    }

    UpdateWindow(hDlg);
    return(0);
}

// TODO: only one font now
int processCustomFuncsSolo(HWND hDlg, int i)
{
    if (Custom_funcs[i].fontType == 0)  // Sans Serif font
        SendMessage(GetDlgItem(hDlg, 141 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
    else                        // Symbol font
    if (Custom_funcs[i].fontType == 1)  // Symbol
        SendMessage(GetDlgItem(hDlg, 141 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
    else                        // Line Draw Font
        SendMessage(GetDlgItem(hDlg, 141 + i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
    SetDlgItemText(hDlg, 141 + i, Custom_funcs[i].desc);

    UpdateWindow(hDlg);
    return(0);
}


int playBackTmp[MAX_REC_PLAYBACK + 1];
int playBackIdxTmp;
void CustMacroPlayback(int item)
{
    // Save original macro...
    playBackIdxTmp = playBackIdx;
    memcpy(playBackTmp, playBack, sizeof(playBack));

    memcpy(playBack, playBackSave[item], sizeof(playBack));
    playBackIdx = playBackIdxSave[item];
    RPN_Playback();

    // Restore original macro...
    playBackIdx = playBackIdxTmp;
    memcpy(playBack, playBackTmp, sizeof(playBack));
}

void Cust_MacroPlayback1(void)
{
    CustMacroPlayback(0);
}

void Cust_MacroPlayback2(void)
{
    CustMacroPlayback(1);
}

void Cust_MacroPlayback3(void)
{
    CustMacroPlayback(2);
}

void Cust_MacroPlayback4(void)
{
    CustMacroPlayback(3);
}

void Cust_MacroPlayback5(void)
{
    CustMacroPlayback(4);
}

void Cust_MacroPlayback6(void)
{
    CustMacroPlayback(5);
}

void Cust_MacroPlayback7(void)
{
    CustMacroPlayback(6);
}

void Cust_MacroPlayback8(void)
{
    CustMacroPlayback(7);
}

void Cust_MacroPlayback9(void)
{
    CustMacroPlayback(8);
}

void Cust_MacroPlayback10(void)
{
    CustMacroPlayback(9);
}

void Cust_MacroPlayback11(void)
{
    CustMacroPlayback(10);
}

void Cust_MacroPlayback12(void)
{
    CustMacroPlayback(11);
}

void Cust_MacroPlayback13(void)
{
    CustMacroPlayback(12);
}

void Cust_MacroPlayback14(void)
{
    CustMacroPlayback(13);
}

void Cust_MacroPlayback15(void)
{
    CustMacroPlayback(14);
}

void Cust_MacroPlayback16(void)
{
    CustMacroPlayback(15);
}

void Cust_MacroPlayback17(void)
{
    CustMacroPlayback(16);
}

void Cust_MacroPlayback18(void)
{
    CustMacroPlayback(17);
}

void Cust_MacroPlayback19(void)
{
    CustMacroPlayback(18);
}

void Cust_MacroPlayback20(void)
{
    CustMacroPlayback(19);
}

void Cust_MacroPlayback21(void)
{
    CustMacroPlayback(20);
}

void Cust_MacroPlayback22(void)
{
    CustMacroPlayback(21);
}

void Cust_MacroPlayback23(void)
{
    CustMacroPlayback(22);
}

void Cust_MacroPlayback24(void)
{
    CustMacroPlayback(23);
}

void Cust_MacroPlayback25(void)
{
    CustMacroPlayback(24);
}

void Cust_MacroPlayback26(void)
{
    CustMacroPlayback(25);
}

void Cust_MacroPlayback27(void)
{
    CustMacroPlayback(26);
}

void Cust_MacroPlayback28(void)
{
    CustMacroPlayback(27);
}

void Cust_MacroPlayback29(void)
{
    CustMacroPlayback(28);
}

void Cust_MacroPlayback30(void)
{
    CustMacroPlayback(29);
}

void Cust_MacroPlayback31(void)
{
    CustMacroPlayback(30);
}

void Cust_MacroPlayback32(void)
{
    CustMacroPlayback(31);
}

void Cust_MacroPlayback33(void)
{
    CustMacroPlayback(32);
}

void Cust_MacroPlayback34(void)
{
    CustMacroPlayback(33);
}

void Cust_MacroPlayback35(void)
{
    CustMacroPlayback(34);
}

void Cust_MacroPlayback36(void)
{
    CustMacroPlayback(35);
}

void Cust_MacroPlayback37(void)
{
    CustMacroPlayback(36);
}

void Cust_MacroPlayback38(void)
{
    CustMacroPlayback(37);
}

void Cust_MacroPlayback39(void)
{
    CustMacroPlayback(38);
}

void Cust_MacroPlayback40(void)
{
    CustMacroPlayback(39);
}

void Cust_MacroPlayback41(void)
{
    CustMacroPlayback(40);
}

void Cust_MacroPlayback42(void)
{
    CustMacroPlayback(41);
}

void Cust_MacroPlayback43(void)
{
    CustMacroPlayback(42);
}

void Cust_MacroPlayback44(void)
{
    CustMacroPlayback(43);
}

void Cust_MacroPlayback45(void)
{
    CustMacroPlayback(44);
}

void Cust_MacroPlayback46(void)
{
    CustMacroPlayback(45);
}

void Cust_MacroPlayback47(void)
{
    CustMacroPlayback(46);
}

void Cust_MacroPlayback48(void)
{
    CustMacroPlayback(47);
}

void Cust_MacroPlayback49(void)
{
    CustMacroPlayback(48);
}

void Cust_MacroPlayback50(void)
{
    CustMacroPlayback(49);
}

void Cust_MacroPlayback51(void)
{
    CustMacroPlayback(50);
}

void Cust_MacroPlayback52(void)
{
    CustMacroPlayback(51);
}

void Cust_MacroPlayback53(void)
{
    CustMacroPlayback(52);
}

void Cust_MacroPlayback54(void)
{
    CustMacroPlayback(53);
}

void Cust_MacroPlayback55(void)
{
    CustMacroPlayback(54);
}

void Cust_MacroPlayback56(void)
{
    CustMacroPlayback(55);
}

void Cust_MacroPlayback57(void)
{
    CustMacroPlayback(56);
}

void Cust_MacroPlayback58(void)
{
    CustMacroPlayback(57);
}

void Cust_MacroPlayback59(void)
{
    CustMacroPlayback(58);
}

void Cust_MacroPlayback60(void)
{
    CustMacroPlayback(59);
}

void Cust_MacroPlayback61(void)
{
    CustMacroPlayback(60);
}

void Cust_MacroPlayback62(void)
{
    CustMacroPlayback(61);
}

void Cust_MacroPlayback63(void)
{
    CustMacroPlayback(62);
}

void Cust_MacroPlayback64(void)
{
    CustMacroPlayback(63);
}
