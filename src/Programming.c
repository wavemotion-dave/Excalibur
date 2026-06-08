// ========================================================================================
// Excalibur RPN Calculator is Copyright(c) 1994-2026 by Dave Bernazzani(wavemotion-dave)
//
// This is legacy code that was created to run under Visual C++ 4.5 and 5.0 circa 1995 and
// was largely developed with Windows 95 through Windows 98SE (some very early portions
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
#include "Excal.h"

#pragma comment(lib, "winmm.lib")   // Needed to playback WAV beep

HWND debugTraceWindow = NULL;

double  debugValue = 0.0;
uint8_t dinputDebugInProgress = 0;
uint8_t RegisterToInput = 0;

#define LOOP_REG_H   0
#define LOOP_REG_HC  1
#define LOOP_REG_I   2
#define LOOP_REG_IC  3
#define LOOP_REG_J   4
#define LOOP_REG_JC  5
int32_t LOOPS[6] = {0,0,0,0,0,0};

extern void Macro_ExchXi(void);
extern void Macro_ExchX2i(void);
extern void Macro_Jump(void);
extern void Macro_LblH(void);
extern void Macro_GotoH(void);
extern void Macro_GsbH(void);
extern void Macro_Halt(void);
extern void Macro_LblI(void);
extern void Macro_GotoI(void);
extern void Macro_GsbI(void);
extern void Macro_LblJ(void);
extern void Macro_GotoJ(void);
extern void Macro_GsbJ(void);
extern void Macro_StoInd(void);
extern void Macro_RclInd(void);
extern void Macro_GotoInd(void);
extern void Macro_GosubInd(void);
extern void Macro_LblA(void);
extern void Macro_LblB(void);
extern void Macro_LblC(void);
extern void Macro_LblD(void);
extern void Macro_LblE(void);
extern void Macro_LblF(void);
extern void Macro_LblG(void);
extern void Macro_GotoA(void);
extern void Macro_GotoB(void);
extern void Macro_GotoC(void);
extern void Macro_GotoD(void);
extern void Macro_GotoE(void);
extern void Macro_GotoF(void);
extern void Macro_GotoG(void);
extern void Macro_XLessY(void);
extern void Macro_XGreaterY(void);
extern void Macro_XLEZero(void);
extern void Macro_XGTZero(void);
extern void Macro_XEY(void);
extern void Macro_XNEY(void);
extern void Macro_XEZero(void);
extern void Macro_XNEZero(void);
extern void Macro_GsbA(void);
extern void Macro_GsbB(void);
extern void Macro_GsbC(void);
extern void Macro_GsbD(void);
extern void Macro_GsbE(void);
extern void Macro_GsbF(void);
extern void Macro_GsbG(void);
extern void Macro_STOH(void);
extern void Macro_STOI(void);
extern void Macro_STOJ(void);
extern void Macro_STOHC(void);
extern void Macro_STOIC(void);
extern void Macro_STOJC(void);
extern void Macro_Return(void);
extern void Macro_SFx(void);
extern void Macro_CFx(void);
extern void Macro_TFx(void);
extern void Macro_ClearF(void);
extern void Macro_XLTR0(void);
extern void Macro_XGTR0(void);
extern void Macro_XLTR1(void);
extern void Macro_XGTR1(void);
extern void Macro_Pause(void);
extern void Macro_InpR0(void);
extern void Macro_InpR1(void);
extern void Macro_InpR2(void);
extern void Macro_Trace(void);
extern void Macro_DEL(void);
extern void Macro_FWD(void);
extern void Macro_REV(void);
extern void Macro_EDIT(void);
extern void Macro_Debug(void);
extern void Macro_LoopH(void);
extern void Macro_LoopI(void);
extern void Macro_LoopJ(void);
extern void Macro_DSZi(void);
extern void Macro_DSZ2i(void);
extern void Macro_Sto2i(void);
extern void Macro_Rcl2i(void);
extern void Macro_Sto2x(void);
extern void Macro_Rcl2x(void);
extern void Macro_Beep(void);
extern void RecallIndirect(int32_t reg);
extern void StoreIndirect(int32_t reg);

struct funcStruct Program1_funcs[MAX_FUNCS] =
{
    {FN1,   UNI_LBLA,       USES_FL,    ALLOWREC,   ' ',    "Lbl A",    YES_L,      X_NEW,   Macro_LblA,     T_LBLA,     H_LABELS},
    {FN2,   UNI_GOTOA,      USES_FL,    ALLOWREC,   ' ',    "Goto A",   YES_L,      X_NEW,   Macro_GotoA,    T_GOTOA,    H_GOTOS},
    {FN3,   UNI_GSBA,       USES_FL,    ALLOWREC,   ' ',    "Gsb A",    YES_L,      X_NEW,   Macro_GsbA,     T_GSBA,     H_GOSUBS},
    {FN4,   UNI_RET,        USES_FL,    ALLOWREC,   ' ',    "Return",   YES_L,      X_NEW,   Macro_Return,   T_RETURN,   H_RETURN},

    {FN5,   UNI_LBLB,       USES_FL,    ALLOWREC,   ' ',    "Lbl B",    YES_L,      X_NEW,   Macro_LblB,     T_LBLB,     H_LABELS},
    {FN6,   UNI_GOTOB,      USES_FL,    ALLOWREC,   ' ',    "Goto B",   YES_L,      X_NEW,   Macro_GotoB,    T_GOTOB,    H_GOTOS},
    {FN7,   UNI_GSBB,       USES_FL,    ALLOWREC,   ' ',    "Gsb B",    YES_L,      X_NEW,   Macro_GsbB,     T_GSBB,     H_GOSUBS},
    {FN8,   UNI_SETH,       USES_FL,    ALLOWREC,   ' ',    "Set H",    YES_L,      X_NEW,   Macro_STOH,     T_SETH,     H_SETH},

    {FN9,   UNI_LBLC,       USES_FL,    ALLOWREC,   ' ',    "Lbl C",    YES_L,      X_NEW,   Macro_LblC,     T_LBLC,     H_LABELS},
    {FN10,  UNI_GOTOC,      USES_FL,    ALLOWREC,   ' ',    "Goto C",   YES_L,      X_NEW,   Macro_GotoC,    T_GOTOC,    H_GOTOS},
    {FN11,  UNI_GSBC,       USES_FL,    ALLOWREC,   ' ',    "Gsb C",    YES_L,      X_NEW,   Macro_GsbC,     T_GSBC,     H_GOSUBS},
    {FN12,  UNI_SETHC,      USES_FL,    ALLOWREC,   ' ',    "Set H'",   YES_L,      X_NEW,   Macro_STOHC,    T_SETHC,    H_SETHC},

    {FN13,  UNI_LBLD,       USES_FL,    ALLOWREC,   ' ',    "Lbl D",    YES_L,      X_NEW,   Macro_LblD,     T_LBLD,     H_LABELS},
    {FN14,  UNI_GOTOD,      USES_FL,    ALLOWREC,   ' ',    "Goto D",   YES_L,      X_NEW,   Macro_GotoD,    T_GOTOD,    H_GOTOS},
    {FN15,  UNI_GSBD,       USES_FL,    ALLOWREC,   ' ',    "Gsb D",    YES_L,      X_NEW,   Macro_GsbD,     T_GSBD,     H_GOSUBS},
    {FN16,  UNI_SETI,       USES_FL,    ALLOWREC,   ' ',    "Set I",    YES_L,      X_NEW,   Macro_STOI,     T_SETI,     H_SETI},

    {FN17,  UNI_LBLE,       USES_FL,    ALLOWREC,   ' ',    "Lbl E",    YES_L,      X_NEW,   Macro_LblE,     T_LBLE,     H_LABELS},
    {FN18,  UNI_GOTOE,      USES_FL,    ALLOWREC,   ' ',    "Goto E",   YES_L,      X_NEW,   Macro_GotoE,    T_GOTOE,    H_GOTOS},
    {FN19,  UNI_GSBE,       USES_FL,    ALLOWREC,   ' ',    "Gsb E",    YES_L,      X_NEW,   Macro_GsbE,     T_GSBE,     H_GOSUBS},
    {FN20,  UNI_SETIC,      USES_FL,    ALLOWREC,   ' ',    "Set I'",   YES_L,      X_NEW,   Macro_STOIC,    T_SETIC,    H_SETIC},

    {FN21,  UNI_LBLF,       USES_FL,    ALLOWREC,   ' ',    "Lbl F",    YES_L,      X_NEW,   Macro_LblF,     T_LBLF,     H_LABELS},
    {FN22,  UNI_GOTOF,      USES_FL,    ALLOWREC,   ' ',    "Goto F",   YES_L,      X_NEW,   Macro_GotoF,    T_GOTOF,    H_GOTOS},
    {FN23,  UNI_GSBF,       USES_FL,    ALLOWREC,   ' ',    "Gsb F",    YES_L,      X_NEW,   Macro_GsbF,     T_GSBF,     H_GOSUBS},
    {FN24,  UNI_SETJ,       USES_FL,    ALLOWREC,   ' ',    "Set J" ,   YES_L,      X_NEW,   Macro_STOJ,     T_SETJ,     H_SETJ},

    {FN25,  UNI_LBLG,       USES_FL,    ALLOWREC,   ' ',    "Lbl G",    YES_L,      X_NEW,   Macro_LblG,     T_LBLG,     H_LABELS},
    {FN26,  UNI_GOTOG,      USES_FL,    ALLOWREC,   ' ',    "Goto G",   YES_L,      X_NEW,   Macro_GotoG,    T_GOTOG,    H_GOTOS},
    {FN27,  UNI_GSBG,       USES_FL,    ALLOWREC,   ' ',    "Gsb G",    YES_L,      X_NEW,   Macro_GsbG,     T_GSBG,     H_GOSUBS},
    {FN28,  UNI_SETJC,      USES_FL,    ALLOWREC,   ' ',    "Set J'",   YES_L,      X_NEW,   Macro_STOJC,    T_SETJC,    H_SETJC},

    {FN29,  UNI_LBLH,       USES_FL,    ALLOWREC,   ' ',    "Lbl H",    YES_L,      X_NEW,   Macro_LblH,     T_LBLH,     H_LABELS},
    {FN30,  UNI_GOTOH,      USES_FL,    ALLOWREC,   ' ',    "Goto H",   YES_L,      X_NEW,   Macro_GotoH,    T_GOTOH,    H_GOTOS},
    {FN31,  UNI_GSBH,       USES_FL,    ALLOWREC,   ' ',    "Gsb H",    YES_L,      X_NEW,   Macro_GsbH,     T_GSBH,     H_GOSUBS},
    {FN32,  UNI_LOOPH,      USES_FL,    ALLOWREC,   ' ',    "Loop H",   YES_L,      X_NEW,   Macro_LoopH,    T_LOOPH,    H_LOOPH},

    {FN33,  UNI_LBLI,       USES_FL,    ALLOWREC,   ' ',    "Lbl I",    YES_L,      X_NEW,   Macro_LblI,     T_LBLI,     H_LABELS},
    {FN34,  UNI_GOTOI,      USES_FL,    ALLOWREC,   ' ',    "Goto I",   YES_L,      X_NEW,   Macro_GotoI,    T_GOTOI,    H_GOTOS},
    {FN35,  UNI_GSBI,       USES_FL,    ALLOWREC,   ' ',    "Gsb I",    YES_L,      X_NEW,   Macro_GsbI,     T_GSBI,     H_GOSUBS},
    {FN36,  UNI_LOOPI,      USES_FL,    ALLOWREC,   ' ',    "Loop I",   YES_L,      X_NEW,   Macro_LoopI,    T_LOOPI,    H_LOOPI},

    {FN37,  UNI_LBLJ,       USES_FL,    ALLOWREC,   ' ',    "Lbl J",    YES_L,      X_NEW,   Macro_LblJ,     T_LBLJ,     H_LABELS},
    {FN38,  UNI_GOTOJ,      USES_FL,    ALLOWREC,   ' ',    "Goto J",   YES_L,      X_NEW,   Macro_GotoJ,    T_GOTOJ,    H_GOTOS},
    {FN39,  UNI_GSBJ,       USES_FL,    ALLOWREC,   ' ',    "Gsb J",    YES_L,      X_NEW,   Macro_GsbJ,     T_GSBJ,     H_GOSUBS},
    {FN40,  UNI_LOOPJ,      USES_FL,    ALLOWREC,   ' ',    "Loop J",   YES_L,      X_NEW,   Macro_LoopJ,    T_LOOPJ,    H_LOOPJ}

};

struct funcStruct Program2_funcs[MAX_FUNCS] =
{
    {FN1,   UNI_GOTOIND,    USES_FL,    ALLOWREC,   ' ',    "Goto(i)",  YES_L,      X_NEW,   Macro_GotoInd,  T_GOTOIND,  H_GOTOIND},
    {FN2,   UNI_GOSUBIND,   USES_FL,    ALLOWREC,   ' ',    "Gsb(i)",   YES_L,      X_NEW,   Macro_GosubInd, T_GSBIND,   H_GSBIND},
    {FN3,   UNI_PAUSE,      USES_FL,    ALLOWREC,   ' ',    "Pause",    YES_L,      X_NEW,   Macro_Pause,    T_PAUSE,    H_PAUSE},
    {FN4,   UNI_HALT,       USES_FL,    ALLOWREC,   ' ',    "Halt",     YES_L,      X_NEW,   Macro_Halt,     T_HALT,     H_HALT},

    {FN5,   UNI_INPR0,      USES_FL,    ALLOWREC,   ' ',    "Inp R0",   YES_L,      X_NEW,   Macro_InpR0,    T_INPR0,    H_INPR0},
    {FN6,   UNI_INPR1,      USES_FL,    ALLOWREC,   ' ',    "Inp R1",   YES_L,      X_NEW,   Macro_InpR1,    T_INPR1,    H_INPR1},
    {FN7,   UNI_INPR2,      USES_FL,    ALLOWREC,   ' ',    "Inp R2",   YES_L,      X_NEW,   Macro_InpR2,    T_INPR2,    H_INPR2},
    {FN8,   UNI_BEEP,       USES_FL,    ALLOWREC,   ' ',    "Beep",     YES_L,      X_NEW,   Macro_Beep,     T_BEEP,     H_BEEP},

    {FN9,   UNI_SFX,        USES_FL,    ALLOWREC,   ' ',    "SFx",      YES_L,      X_NEW,   Macro_SFx,      T_SFX,      H_SFX},
    {FN10,  UNI_CFX,        USES_FL,    ALLOWREC,   ' ',    "CFx",      YES_L,      X_NEW,   Macro_CFx,      T_CFX,      H_CFX},
    {FN11,  UNI_TFX,        USES_FL,    ALLOWREC,   ' ',    "TFx?",     YES_L,      X_NEW,   Macro_TFx,      T_TFX,      H_TFX},
    {FN12,  UNI_CLRFL,      USES_FL,    ALLOWREC,   ' ',    "CFlags",   YES_L,      X_NEW,   Macro_ClearF,   T_CLEARF,   H_CLEARF},

    {FN13,  UNI_XLTR0,      USES_FL,    ALLOWREC,   ' ',    "X<R0?",    YES_L,      X_NEW,   Macro_XLTR0,    T_XLTR0,    H_XLTR0},
    {FN14,  UNI_XGTR0,      USES_FL,    ALLOWREC,   ' ',    "X>R0?",    YES_L,      X_NEW,   Macro_XGTR0,    T_XGTR0,    H_XGTR0},
    {FN15,  UNI_XLTR1,      USES_FL,    ALLOWREC,   ' ',    "X<R1?",    YES_L,      X_NEW,   Macro_XLTR1,    T_XLTR1,    H_XLTR1},
    {FN16,  UNI_XGTR1,      USES_FL,    ALLOWREC,   ' ',    "X>R1?",    YES_L,      X_NEW,   Macro_XGTR1,    T_XGTR1,    H_XGTR1},

    {FN17,  UNI_XLEY,       USES_FL,    ALLOWREC,   ' ',    "X<=Y?",    YES_L,      X_NEW,   Macro_XLessY,   T_MACXLY,   H_MACXLY},
    {FN18,  UNI_XGY,        USES_FL,    ALLOWREC,   ' ',    "X>Y?",     YES_L,      X_NEW,   Macro_XGreaterY,T_MACXGY,   H_MACXGY},
    {FN19,  UNI_XEY,        USES_FL,    ALLOWREC,   ' ',    "X=Y?",     YES_L,      X_NEW,   Macro_XEY,      T_XEY,      H_XEY},
    {FN20,  UNI_XNEY,       USES_FL,    ALLOWREC,   ' ',    "X!=Y?",    YES_L,      X_NEW,   Macro_XNEY,     T_XNEY,     H_XNEY},

    {FN21,  UNI_XLEZ,       USES_FL,    ALLOWREC,   ' ',    "X<=0?",    YES_L,      X_NEW,   Macro_XLEZero,  T_XLEZERO,  H_XLEZERO},
    {FN22,  UNI_XGZ,        USES_FL,    ALLOWREC,   ' ',    "X>0?",     YES_L,      X_NEW,   Macro_XGTZero,  T_XGTZERO,  H_XGTZERO},
    {FN23,  UNI_XEZ,        USES_FL,    ALLOWREC,   ' ',    "X=0?",     YES_L,      X_NEW,   Macro_XEZero,   T_XEZERO,   H_XEZERO},
    {FN24,  UNI_XNEZ,       USES_FL,    ALLOWREC,   ' ',    "X!=0?",    YES_L,      X_NEW,   Macro_XNEZero,  T_XNEZERO,  H_XNEZERO},

    {FN25,  UNI_STOIND,     USES_FL,    ALLOWREC,   ' ',    "Sto i",    YES_L,      X_NEW,   Macro_StoInd,   T_STOIND,   H_STOIND},
    {FN26,  UNI_RCLIND,     USES_FL,    ALLOWREC,   ' ',    "Rcl i",    YES_L,      X_NEW,   Macro_RclInd,   T_RCLIND,   H_RCLIND},
    {FN27,  UNI_DSZI,       USES_FL,    ALLOWREC,   ' ',    "DSZ i",    YES_L,      X_NEW,   Macro_DSZi,     T_DSZ,      H_DSZ},
    {FN28,  UNI_DSZ2I,      USES_FL,    ALLOWREC,   ' ',    "DSZ(i)",   YES_L,      X_NEW,   Macro_DSZ2i,    T_DSZ2I,    H_DSZ2I},

    {FN29,  UNI_STO2I,      USES_FL,    ALLOWREC,   ' ',    "Sto(i)",   YES_L,      X_NEW,   Macro_Sto2i,    T_STO2I,    H_STO2I},
    {FN30,  UNI_RCL2I,      USES_FL,    ALLOWREC,   ' ',    "Rcl(i)",   YES_L,      X_NEW,   Macro_Rcl2i,    T_RCL2I,    H_RCL2I},
    {FN31,  UNI_STO2X,      USES_FL,    ALLOWREC,   ' ',    "Sto(x)",   YES_L,      X_NEW,   Macro_Sto2x,    T_STO2X,    H_STO2X},
    {FN32,  UNI_RCL2X,      USES_FL,    ALLOWREC,   ' ',    "Rcl(x)",   YES_L,      X_NEW,   Macro_Rcl2x,    T_RCL2X,    H_RCL2X},

    {FN33,  UNI_EXCHXI,     USES_FL,    ALLOWREC,   ' ',    "X«»i",     YES_L,      X_NEW,   Macro_ExchXi,   T_EXCHXI,   H_EXCHXI},
    {FN34,  UNI_EXCHX2I,    USES_FL,    ALLOWREC,   ' ',    "X«»(i)",   YES_L,      X_NEW,   Macro_ExchX2i,  T_EXCHX2I,  H_EXCHX2I},
    {FN35,  UNI_TRACE,      USES_FL,    NORECORD,   ' ',    "TRACE",    YES_L,      X_NULL,  Macro_Trace,    T_TRACE,    H_TRACE},
    {FN36,  UNI_DEBUG,      USES_FL,    NORECORD,   ' ',    "DEBUG",    YES_L,      X_NULL,  Macro_Debug,    T_DEBUG,    H_DEBUG},

    {FN37,  UNI_MEDIT,      USES_FL,    NORECORD,   ' ',    "PEDIT",    YES_L,      X_NULL,  Macro_EDIT,     T_MEDIT,    H_MEDIT},
    {FN38,  UNI_FWD,        USES_FL,    NORECORD,   ' ',    "FWD",      YES_L,      X_NULL,  Macro_FWD,      T_FWD,      H_FWD},
    {FN39,  UNI_REV,        USES_FL,    NORECORD,   ' ',    "REV",      YES_L,      X_NULL,  Macro_REV,      T_REV,      H_REV},
    {FN40,  UNI_DEL,        USES_FL,    NORECORD,   ' ',    "DEL",      YES_L,      X_NULL,  Macro_DEL,      T_DEL,      H_DEL}
};


void Macro_LblA(void) {}    // Function does nothing but anchors a label
void Macro_LblB(void) {}    // Function does nothing but anchors a label
void Macro_LblC(void) {}    // Function does nothing but anchors a label
void Macro_LblD(void) {}    // Function does nothing but anchors a label
void Macro_LblE(void) {}    // Function does nothing but anchors a label
void Macro_LblF(void) {}    // Function does nothing but anchors a label
void Macro_LblG(void) {}    // Function does nothing but anchors a label
void Macro_LblH(void) {}    // Function does nothing but anchors a label
void Macro_LblI(void) {}    // Function does nothing but anchors a label
void Macro_LblJ(void) {}    // Function does nothing but anchors a label

int FindProgrammingLabel(uint16_t label, uint8_t forwards)
{
    int j;

    if (forwards)
    {
        // ---------------------------------------------------------------------
        // Scan forwards from the current playpack position and wrap as needed.
        // This is the same way many classic HP calculators do the search.
        // ---------------------------------------------------------------------
        for (j = 0; j < playBackEndIdx; j++)
        {
            int idx = (currentPlaybackIdx + j) % playBackEndIdx;
            if (playBackMap[playBack[idx]].uniqueIndex == label)
            {
                return idx;  // Label found...
            }
        }
    }
    else //Scan Backwards... for improved Looping logic
    {
        // ---------------------------------------------------------------------
        // Scan backwards from the current playpack position and wrap as needed.
        // ---------------------------------------------------------------------
        for (j = 0; j < playBackEndIdx; j++)
        {
            int idx = (currentPlaybackIdx - j) % playBackEndIdx;
            if (playBackMap[playBack[idx]].uniqueIndex == label)
            {
                return idx;  // Label found...
            }
        }
    }
    return -1; // No label found
}

#define BACKWARDS 0
#define FORWARDS  1
void rpn_goto(uint16_t uniqueIdx, uint8_t forwards)
{
    if (macroPlayback == TRUE)
    {
        int label_idx = FindProgrammingLabel(uniqueIdx, forwards);
        if (label_idx >= 0)
        {
            currentPlaybackIdx = label_idx;
            ShowTrace();
        }
        else
        {
            RPN_error("Run Time Error: Label not found - Unable to continue program execution.");
            currentPlaybackIdx = playBackEndIdx; // end program.
        }
    }
}

void Macro_GotoA(void)  {rpn_goto(UNI_LBLA, FORWARDS);}
void Macro_GotoB(void)  {rpn_goto(UNI_LBLB, FORWARDS);}
void Macro_GotoC(void)  {rpn_goto(UNI_LBLC, FORWARDS);}
void Macro_GotoD(void)  {rpn_goto(UNI_LBLD, FORWARDS);}
void Macro_GotoE(void)  {rpn_goto(UNI_LBLE, FORWARDS);}
void Macro_GotoF(void)  {rpn_goto(UNI_LBLF, FORWARDS);}
void Macro_GotoG(void)  {rpn_goto(UNI_LBLG, FORWARDS);}
void Macro_GotoH(void)  {rpn_goto(UNI_LBLH, FORWARDS);}
void Macro_GotoI(void)  {rpn_goto(UNI_LBLI, FORWARDS);}
void Macro_GotoJ(void)  {rpn_goto(UNI_LBLJ, FORWARDS);}

void RPN_gosub(uint16_t uniqueIdx)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentPlaybackIdx;
            rpn_goto(uniqueIdx, FORWARDS);
        }
        else
        {
            RPN_error("Run Time Error: Max Program Function Stack Reached - No more Gosubs");
            currentPlaybackIdx = playBackEndIdx; // end program.
        }
    }
}


void Macro_GsbA(void)   {RPN_gosub(UNI_LBLA);}
void Macro_GsbB(void)   {RPN_gosub(UNI_LBLB);}
void Macro_GsbC(void)   {RPN_gosub(UNI_LBLC);}
void Macro_GsbD(void)   {RPN_gosub(UNI_LBLD);}
void Macro_GsbE(void)   {RPN_gosub(UNI_LBLE);}
void Macro_GsbF(void)   {RPN_gosub(UNI_LBLF);}
void Macro_GsbG(void)   {RPN_gosub(UNI_LBLG);}
void Macro_GsbH(void)   {RPN_gosub(UNI_LBLH);}
void Macro_GsbI(void)   {RPN_gosub(UNI_LBLI);}
void Macro_GsbJ(void)   {RPN_gosub(UNI_LBLJ);}


int PopStackInteger(void)
{
    if (progMode == PROG_FLOAT)
        return (int) StackPop();
    else
        return (int) StackPopL();
}

void CheckMacroCondition(int condition)
{
    if (macroPlayback == TRUE)
    {
        if (condition)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentPlaybackIdx++;
        }
    }
}

void Macro_XLessY(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] <= STACK[STK_Y]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] <= STACKL[STK_Y]);
    }
}

void Macro_XGreaterY(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] > STACK[STK_Y]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] > STACKL[STK_Y]);
    }
}

void Macro_XLEZero(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] <= 0.0);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] <= 0L);
    }
}

void Macro_XEY(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] == STACK[STK_Y]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] == STACKL[STK_Y]);
    }
}


void Macro_XNEY(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] != STACK[STK_Y]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] != STACKL[STK_Y]);
    }
}

void Macro_XEZero(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] == 0.0);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] == 0L);
    }
}

void Macro_XNEZero(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] != 0.0);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] != 0L);
    }
}


void Macro_XGTZero(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] > 0.0);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] > 0L);
    }
}

void Macro_Return(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx > 0)
        {
            currentPlaybackIdx = MacroStack[--MacroStackIdx];
        }
        else
        {
            endRunningMacro();  // End the program if we are at the top layer already
        }
    }
}


void Macro_SFx(void)
{
    uint8_t iX = (uint8_t)PopStackInteger();
    uint32_t mask = (0x00000001 << iX);

    if (iX == 32)
    {
        progModeCarry = 1;
    }
    else if (iX == 33)
    {
        progModeOverflow = 1;
    }
    else
    {
        progFlags = progFlags | mask;
    }
}

void Macro_CFx(void)
{
    uint8_t iX = (uint8_t)PopStackInteger();
    uint32_t mask = (0x00000001 << iX);

    if (iX == 32)
    {
        progModeCarry = 0;
    }
    else if (iX == 33)
    {
        progModeOverflow = 0;
    }
    else
    {
        progFlags = progFlags & ~mask;
    }
}

void Macro_TFx(void)
{
    uint8_t iX = (uint8_t)PopStackInteger();
    uint32_t mask = (0x00000001 << iX);

    if (iX == 32)
    {
        CheckMacroCondition(progModeCarry);
    }
    else if (iX == 33)
    {
        CheckMacroCondition(progModeOverflow);
    }
    else
    {
        CheckMacroCondition(progFlags & mask);
    }
}

void Macro_XLTR0(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] < STO[0]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] < STOL[0]);
    }
}

void Macro_XGTR0(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] > STO[0]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] > STOL[0]);
    }
}

void Macro_XLTR1(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] < STO[1]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] < STOL[1]);
    }

}

void Macro_XGTR1(void)
{
    if (progMode == PROG_FLOAT)
    {
        CheckMacroCondition(STACK[STK_X] > STO[1]);
    }
    else
    {
        CheckMacroCondition(STACKL[STK_X] > STOL[1]);
    }
}

void Macro_ClearF(void)
{
    progFlags = 0x00000000;
    progModeCarry = 0;
    progModeOverflow = 0;
    blinkXDisplay(FALSE);
}

void Macro_Pause(void)
{
    if (macroPlayback == TRUE)
    {
        ShowStack();
        sleep_and_peek(traceDelayValueMs);
    }
}

void Macro_Beep(void)
{
    // Play the sound resource...
    PlaySound(MAKEINTRESOURCE(IDR_BEEP), GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
    sleep_and_peek(250);
}

BOOL CALLBACK inputRegisterProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[64];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        if (progMode == PROG_FLOAT)
        {
            FormatNumberForStack(STO[RegisterToInput], tmp, FALSE);
        }
        else
        {
            if (progMode == PROG_HEX)
            {
                sprintf(tmp, "0x%I64X", STOL[RegisterToInput]);
            }
            else
            {
                sprintf(tmp, "%I64d", STOL[RegisterToInput]);
            }
        }
        SetDlgItemText(hDlg, IDC_INPUT_REG, current_macro_inputs[RegisterToInput]);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 64);
            if (progMode == PROG_FLOAT)
            {
                STO[RegisterToInput] = atof(tmp);
            }
            else
            {
                STOL[RegisterToInput] = ConvertCompSciStrTo64(tmp);
            }
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }
    }
    return FALSE;
}

BOOL CALLBACK inputRegisterPromptProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[32];

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, current_macro_inputs[RegisterToInput]);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 20);
            tmp[20] = CNULL;
            strcpy(current_macro_inputs[RegisterToInput], tmp);
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }
    }
    return FALSE;
}

void InputRegisterPrompt(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG_PROMPT", calcMainWindow, inputRegisterPromptProc);
}

void Macro_InpR0(void)
{
    RegisterToInput = 0;
    if (recModeON) InputRegisterPrompt();
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_InpR1(void)
{
    RegisterToInput = 1;
    if (recModeON) InputRegisterPrompt();
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_InpR2(void)
{
    RegisterToInput = 2;
    if (recModeON) InputRegisterPrompt();
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_DEL(void)
{
    int k;

    if (recModeON == 1 && currentPlaybackIdx > 0)
    {
        for (k = currentPlaybackIdx - 1; k < playBackEndIdx; k++)
            playBack[k] = playBack[k + 1];      // Shift everything down one line!
        if (currentPlaybackIdx == playBackEndIdx)
        {
            playBackEndIdx--;
            currentPlaybackIdx--;
        }
        else
        {
            playBackEndIdx--;
        }
        MacroStackIdx = 0;
    }
}

void Macro_FWD(void)
{
    if (recModeON == 1 && currentPlaybackIdx < playBackEndIdx)
    {
        currentPlaybackIdx++;
        MacroStackIdx = 0;
    }
}

void Macro_REV(void)
{
    if (recModeON == 1 && currentPlaybackIdx > 0)
    {
        currentPlaybackIdx--;
        MacroStackIdx = 0;
    }
}

void Macro_Trace(void)
{
    if (recModeON == 0)
    {
        if (traceMacroPlayback)
        {
            traceMacroPlayback = FALSE;
            endRunningMacro();
        }
        else
        {
            traceMacroPlayback = TRUE;
            RPN_Playback();
            traceMacroPlayback = FALSE;
        }
    }
}

void Macro_EDIT(void)
{
    if (recModeON == 0)
    {
        recModeON = 1;
        currentPlaybackIdx = 0;
        MacroStackIdx = 0;
        Xedit = X_NEW;
    }
    else
        recModeON = 0;
    ShowStatus();
}

void endRunningMacro(void)
{
    currentPlaybackIdx = playBackEndIdx;
    MacroStackIdx = 0;
    macroPlayback = FALSE;
}


void Macro_RecallN(void)
{
    int i = PopStackInteger();

    if (i >= 0 && i < MAX_STO)
    {
        if (progMode == PROG_FLOAT)
        {
            StackPush(STO[i]);
        }
        else
        {
            StackPushL(STOL[i]);
        }
    }
    else
    {
        RPN_error("Register Out Of Range (R0 to R99)");
    }
}

void Macro_ExchXi(void)
{
    int32_t temp = indirectRegister;

    if (progMode == PROG_FLOAT)
    {
        indirectRegister = (int32_t)STACK[STK_X];
        STACK[STK_X] = (double)temp;
    }
    else
    {
        indirectRegister = (int32_t)STACKL[STK_X];
        STACKL[STK_X] = (PROG_LONG)temp;
    }
}

void Macro_ExchX2i(void)
{
    // TODO - extended registers?
    if (indirectRegister >= 0 && indirectRegister < MAX_STO)
    {
        if (progMode == PROG_FLOAT)
        {
            double temp = STACK[STK_X];
            STACK[STK_X] = STO[indirectRegister];
            STO[indirectRegister] = temp;
        }
        else
        {
            PROG_LONG temp = STACKL[STK_X];
            STACKL[STK_X] = STOL[indirectRegister];
            STOL[indirectRegister] = temp;
        }
    }
}


void Macro_StoInd(void)
{
    if (progMode == PROG_FLOAT)
    {
        indirectRegister = (int32_t) STACK[STK_X];
    }
    else
    {
        indirectRegister = (int32_t) STACKL[STK_X];
    }
    blinkXDisplay(FALSE);
}

void Macro_RclInd(void)
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_FLOAT)
            STACK[STK_X] = indirectRegister;
        else
            STACKL[STK_X] = MaskStack((int32_t) indirectRegister);
    }
    else
    {
        if (progMode == PROG_FLOAT)
            StackPush(indirectRegister);
        else
            StackPushL(MaskStack((int32_t) indirectRegister));
    }
}

void Macro_GotoInd(void)
{
    uint16_t uniqueLabel = 0;

    if (macroPlayback == TRUE)
    {
        if ((indirectRegister >= 0) && (indirectRegister < MAX_LABELS))
        {
            int label_idx;

            switch(indirectRegister)
            {
                case 0:  uniqueLabel=UNI_LBLA; break;
                case 1:  uniqueLabel=UNI_LBLB; break;
                case 2:  uniqueLabel=UNI_LBLC; break;
                case 3:  uniqueLabel=UNI_LBLD; break;
                case 4:  uniqueLabel=UNI_LBLE; break;
                case 5:  uniqueLabel=UNI_LBLF; break;
                case 6:  uniqueLabel=UNI_LBLG; break;
                case 7:  uniqueLabel=UNI_LBLH; break;
                case 8:  uniqueLabel=UNI_LBLI; break;
                case 9:  uniqueLabel=UNI_LBLJ; break;
            }

            label_idx = FindProgrammingLabel(uniqueLabel, 1);

            if (label_idx >= 0)
            {
                currentPlaybackIdx = label_idx;
                ShowTrace();
            }
            else
            {
                RPN_error("Run Time Error: Label not found - Unable to continue program execution.");
                currentPlaybackIdx = playBackEndIdx; // end program.
            }
        }
        else
        {
            RPN_error("Run Time Error: Value of indirect Label must point to LabelA=0 through LabelJ=9");
            endRunningMacro();
        }
    }
}

void Macro_GosubInd(void)
{
    uint16_t uniqueLabel = 0;

    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentPlaybackIdx;
            if ((indirectRegister >= 0) && (indirectRegister < MAX_LABELS))
            {
                int label_idx;

                switch(indirectRegister)
                {
                    case 0:  uniqueLabel=UNI_LBLA; break;
                    case 1:  uniqueLabel=UNI_LBLB; break;
                    case 2:  uniqueLabel=UNI_LBLC; break;
                    case 3:  uniqueLabel=UNI_LBLD; break;
                    case 4:  uniqueLabel=UNI_LBLE; break;
                    case 5:  uniqueLabel=UNI_LBLF; break;
                    case 6:  uniqueLabel=UNI_LBLG; break;
                    case 7:  uniqueLabel=UNI_LBLH; break;
                    case 8:  uniqueLabel=UNI_LBLI; break;
                    case 9:  uniqueLabel=UNI_LBLJ; break;
                }

                label_idx = FindProgrammingLabel(uniqueLabel, 1);

                if (label_idx >= 0)
                {
                    currentPlaybackIdx = label_idx;
                    ShowTrace();
                }
                else
                {
                    RPN_error("Run Time Error: Label not found - Unable to continue program execution.");
                    currentPlaybackIdx = playBackEndIdx; // end program.
                }
            }
            else
            {
                RPN_error("Run Time Error: Value of indirect Label must point to LabelA=0 through LabelJ=9");
                endRunningMacro();
            }
        }
        else
        {
            RPN_error("Run Time Error: Max Program Function Stack Reached - No more Gosubs");
            currentPlaybackIdx = playBackEndIdx; // end program.
        }
    }
}


void Macro_Halt(void)
{
    if (macroPlayback == TRUE)
    {
        endRunningMacro();
    }
}

BOOL CALLBACK inputDebugValue(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    static char tmp[64];

    switch(wMessage)
    {
    case WM_INITDIALOG:
        FormatNumberForStack(debugValue, tmp, FALSE);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 63);
            tmp[63] = CNULL;
            trim(tmp);
            if (progMode == PROG_FLOAT)
            {
                debugValue = atof(tmp);
            }
            else
            {
                // Not perfect - it means in Comp-Sci mode we can't enter values bigger than 32-bits
                if (wordMode == COMPSCI_SIGNED)
                    debugValue = (double)(int32_t)ConvertCompSciStrTo64(tmp);
                else
                    debugValue = (double)(uint32_t)ConvertCompSciStrTo64(tmp);                
            }
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        case(IDCANCEL):       // Cancel
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;
        
        default:
            return FALSE;
        }
    }
    return FALSE;
}

double getNewDebugVal(void)
{
    dinputDebugInProgress = 1;
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_DEBUG_INPUT", calcMainWindow, inputDebugValue);
    dinputDebugInProgress = 0;
    return debugValue;
}

static uint32_t last_debug_register_checksum = 0xFFFFBEEF;

BOOL CALLBACK debugWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT item;

     switch(message)
     {
     case WM_INITDIALOG:
          last_debug_register_checksum = 0xDEADBEEF;
          return TRUE;

     case WM_SIZE:
     {
        // Allow the Debug/Trace window to be resized
        RECT rect1, rect2, rect3;
        int newHeight = HIWORD(lParam);

        GetWindowRect(GetDlgItem(hwnd, TRACE_PROGRAM), &rect1);
        GetWindowRect(GetDlgItem(hwnd, TRACE_REGS1),   &rect2);
        GetWindowRect(GetDlgItem(hwnd, TRACE_REGS2),   &rect3);

        MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&rect1, 2);
        MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&rect2, 2);
        MapWindowPoints(HWND_DESKTOP, hwnd, (LPPOINT)&rect3, 2);

        // Resize the listbox to fill the entire window vertically (with a small border at the bottom)
        MoveWindow(GetDlgItem(hwnd, TRACE_PROGRAM), rect1.left, rect1.top, (rect1.right - rect1.left), newHeight - 40, TRUE);
        MoveWindow(GetDlgItem(hwnd, TRACE_REGS1),   rect2.left, rect2.top, (rect2.right - rect2.left), newHeight - 40, TRUE);
        MoveWindow(GetDlgItem(hwnd, TRACE_REGS2),   rect3.left, rect3.top, (rect3.right - rect3.left), newHeight - 40, TRUE);

        return 0;
     }

     case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case TRACE_REGS1:
                    if ((dinputDebugInProgress == 0) && (HIWORD(wParam) == LBN_DBLCLK))
                    {
                        item = SendDlgItemMessage(hwnd, TRACE_REGS1, LB_GETCURSEL, 0, 0L);
                        switch((int)item)
                        {
                            case 1:
                                debugValue = STACK[STK_D];
                                STACK[STK_D] = getNewDebugVal();
                                STACKL[STK_D] = (PROG_LONG) STACK[STK_D];
                                break;
                            case 2:
                                debugValue = STACK[STK_C];
                                STACK[STK_C] = getNewDebugVal();
                                STACKL[STK_C] = (PROG_LONG) STACK[STK_C];
                                break;
                            case 3:
                                debugValue = STACK[STK_B];
                                STACK[STK_B] = getNewDebugVal();
                                STACKL[STK_B] = (PROG_LONG) STACK[STK_B];
                                break;
                            case 4:
                                debugValue = STACK[STK_A];
                                STACK[STK_A] = getNewDebugVal();
                                STACKL[STK_A] = (PROG_LONG) STACK[STK_A];
                                break;

                            case 7:
                                debugValue = STACK[STK_T];
                                STACK[STK_T] = getNewDebugVal();
                                STACKL[STK_T] = (PROG_LONG) STACK[STK_T];
                                break;
                            case 8:
                                debugValue = STACK[STK_Z];
                                STACK[STK_Z] = getNewDebugVal();
                                STACKL[STK_Z] = (PROG_LONG) STACK[STK_Z];
                                break;
                            case 9:
                                debugValue = STACK[STK_Y];
                                STACK[STK_Y] = getNewDebugVal();
                                STACKL[STK_Y] = (PROG_LONG) STACK[STK_Y];
                                break;
                            case 10:
                                debugValue = STACK[STK_X];
                                STACK[STK_X] = getNewDebugVal();
                                STACKL[STK_X] = (PROG_LONG) STACK[STK_X];
                                break;
                            case 12:
                                debugValue = (double) indirectRegister;
                                indirectRegister = (int32_t)getNewDebugVal();
                                break;
                            case 13:
                                debugValue = (double) progFlags;
                                progFlags = (uint32_t)getNewDebugVal();
                                break;
                            case 14:
                                debugValue = (double) LOOPS[LOOP_REG_H];
                                LOOPS[LOOP_REG_H] = (int32_t)getNewDebugVal();
                                break;
                            case 15:
                                debugValue = (double) LOOPS[LOOP_REG_HC];
                                LOOPS[LOOP_REG_HC] = (int32_t)getNewDebugVal();
                                break;
                            case 16:
                                debugValue = (double) LOOPS[LOOP_REG_I];
                                LOOPS[LOOP_REG_I] = (int32_t)getNewDebugVal();
                                break;
                            case 17:
                                debugValue = (double) LOOPS[LOOP_REG_IC];
                                LOOPS[LOOP_REG_IC] = (int32_t)getNewDebugVal();
                                break;
                            case 18:
                                debugValue = (double) LOOPS[LOOP_REG_J];
                                LOOPS[LOOP_REG_J] = (int32_t)getNewDebugVal();
                                break;
                            case 19:
                                debugValue = (double) LOOPS[LOOP_REG_JC];
                                LOOPS[LOOP_REG_JC] = (int32_t)getNewDebugVal();
                                break;
                            case 22:
                                debugValue = FIN[FIN_REG_n];
                                FIN[FIN_REG_n] = getNewDebugVal();
                                break;
                            case 23:
                                debugValue = FIN[FIN_REG_i];
                                FIN[FIN_REG_i] = getNewDebugVal();
                                break;
                            case 24:
                                debugValue = FIN[FIN_REG_PV];
                                FIN[FIN_REG_PV] = getNewDebugVal();
                                break;
                            case 25:
                                debugValue = FIN[FIN_REG_PMT];
                                FIN[FIN_REG_PMT] = getNewDebugVal();
                                break;
                            case 26:
                                debugValue = FIN[FIN_REG_FV];
                                FIN[FIN_REG_FV] = getNewDebugVal();
                                break;
                            case 27:
                                debugValue = FIN[FIN_REG_MUC];
                                FIN[FIN_REG_MUC] = getNewDebugVal();
                                break;
                            case 28:
                                debugValue = FIN[FIN_REG_MUP];
                                FIN[FIN_REG_MUP] = getNewDebugVal();
                                break;
                            case 29:
                                debugValue = FIN[FIN_REG_COST];
                                FIN[FIN_REG_COST] = getNewDebugVal();
                                break;
                            case 30:
                                debugValue = FIN[FIN_REG_PRICE];
                                FIN[FIN_REG_PRICE] = getNewDebugVal();
                                break;
                        }
                        ShowStack();
                        return TRUE;
                    }
                    break;

                case TRACE_REGS2:
                    if ((dinputDebugInProgress == 0) && (HIWORD(wParam) == LBN_DBLCLK))
                    {
                        item = SendDlgItemMessage(hwnd, TRACE_REGS2, LB_GETCURSEL, 0, 0L);
                        if (progMode == PROG_FLOAT)
                        {
                            debugValue = STO[item];
                            STO[item] = getNewDebugVal();
                        }
                        else
                        {
                            debugValue = (double) (PROG_SIGNEDLONG)STOL[item];
                            STOL[item] = (PROG_LONG) getNewDebugVal();
                        }
                        ShowStack();
                        return TRUE;
                    }
                    break;

                case TRACE_PROGRAM:
                    item = SendDlgItemMessage(hwnd, TRACE_PROGRAM, LB_GETCURSEL, 0, 0L);
                    if (recModeON)
                    {
                        currentPlaybackIdx = (int16_t)item;
                        if (currentPlaybackIdx > playBackEndIdx)
                            currentPlaybackIdx = playBackEndIdx;
                    }
                    else
                    {
                        if (item > 0)
                            currentPlaybackIdx = (int16_t) item-1;
                        else
                            currentPlaybackIdx = (int16_t) 0;
                    }
                    ShowStack();
                    return TRUE;
            }
            break;
      case WM_CLOSE:
            Macro_Debug(); // This will toggle the window to hidden...
            return TRUE;
     }

     return FALSE;
}


void CreateDebugWindow(HWND hwnd, HINSTANCE hInstance)
{
    static char szAppNameDbg[] = "DIALOG_PROGTRACE";
    debugTraceWindow = CreateDialog(hInstance, szAppNameDbg, 0, ( DLGPROC )debugWndProc);
    ShowWindow(debugTraceWindow, SW_HIDE);
    UpdateWindow(debugTraceWindow);
    SetFocus(calcMainWindow);
}

void Macro_Debug(void)
{
    if (IsWindowVisible(debugTraceWindow))
    {
        ShowWindow(debugTraceWindow, SW_HIDE);
    }
    else
    {
        ShowWindow(debugTraceWindow, SW_SHOW);
        UpdateDebugRegs();
        UpdateDebugProgram(1);
    }
}

void DebuggerStackVal(char id, enum StackPosition_t pos)
{
    char tmp[64];
    char tmp2[64];
    if (progMode == PROG_FLOAT)
    {
        sprintf(tmp, " %c: %-20.13g", id, STACK[pos]);
        makeInternational(tmp);
    }
    else
    {
        MakeCompSciStr(STACKL[pos], tmp2);
        sprintf(tmp, " %c: %-20s", id, tmp2);
    }
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
}

void UpdateDebugRegs(void)
{
    char tmp[64];
    char tmp2[64];
    int i;

    SendMessage(GetDlgItem(debugTraceWindow, TRACE_REGS1), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
    SendMessage(GetDlgItem(debugTraceWindow, TRACE_REGS2), WM_SETFONT, (WPARAM) hFixedFont, FALSE);

    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_RESETCONTENT, 0, 0);

    sprintf(tmp, " Extended Stack");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    DebuggerStackVal('D', STK_D);
    DebuggerStackVal('C', STK_C);
    DebuggerStackVal('B', STK_B);
    DebuggerStackVal('A', STK_A);

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Main Stack");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    DebuggerStackVal('T', STK_T);
    DebuggerStackVal('Z', STK_Z);
    DebuggerStackVal('Y', STK_Y);
    DebuggerStackVal('X', STK_X);

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " i: %d", indirectRegister);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Flags: 0x%08X", progFlags);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " H : %d", LOOPS[LOOP_REG_H]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " H': %d", LOOPS[LOOP_REG_HC]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " I : %d", LOOPS[LOOP_REG_I]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " I': %d", LOOPS[LOOP_REG_IC]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " J : %d", LOOPS[LOOP_REG_J]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " J': %d", LOOPS[LOOP_REG_JC]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Financial Regs");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, "   n: %-20.13g", FIN[FIN_REG_n]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, "   i: %-20.13g", FIN[FIN_REG_i]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, "  PV: %-20.13g", FIN[FIN_REG_PV]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " PMT: %-20.13g", FIN[FIN_REG_PMT]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, "  FV: %-20.13g", FIN[FIN_REG_FV]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " MkC: %-20.13g", FIN[FIN_REG_MUC]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " MkP: %-20.13g", FIN[FIN_REG_MUP]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " CST: %-20.13g", FIN[FIN_REG_COST]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " PRC: %-20.13g", FIN[FIN_REG_PRICE]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " CFn: %-18d", CFn);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    for (i = 0; i < MAX_CF; i++)
    {
        sprintf(tmp, " CF[%2d]:%-16.11g", i, cashFlow[i]);
        makeInternational(tmp);
        SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
    }

    // ------------------------------------------------------------------------
    // Block for local vars... Here we don't want to update the entire 100
    // registers if they haven't changed. So we do a simple checksum and see.
    // ------------------------------------------------------------------------
    {
        uint32_t checksum = 0;

        if (progMode == PROG_FLOAT)
        {
            uint8_t *ptr = (uint8_t *)STO;
            for (i = 0; i < sizeof(STO); i++)
            {
                checksum += *ptr++;
            }
        }
        else
        {
            uint8_t *ptr = (uint8_t *)STOL;
            for (i = 0; i < sizeof(STOL); i++)
            {
                checksum += *ptr++;
            }
        }

        if (checksum != last_debug_register_checksum)
        {
            last_debug_register_checksum = checksum;

            SendDlgItemMessage(debugTraceWindow, TRACE_REGS2, LB_RESETCONTENT, 0, 0);

            for (i = 0; i < MAX_STO; i++)
            {
                if (progMode == PROG_FLOAT)
                {
                    sprintf(tmp, " R%02d: %-20.13g", i, STO[i]);
                    makeInternational(tmp);
                }
                else
                {
                    MakeCompSciStr(STOL[i], tmp2);
                    sprintf(tmp, " L%02d: %-20s", i, tmp2);
                }
                SendDlgItemMessage(debugTraceWindow, TRACE_REGS2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
            }
        }
    }
}

void UpdateDebugProgram(int resetProgramList)
{
    char tmp[64];
    int i;

    if (resetProgramList != 0)
    {
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_RESETCONTENT, 0, 0);
        sprintf(tmp, "%03d - %s", 0, "<Start Of Program>");
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        for (i = 0; i < playBackEndIdx; i++)
        {
            sprintf(tmp, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
            SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        }
        sprintf(tmp, "%03d - <End Of Program>", i + 1);
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
    }
    if (recModeON)
    {
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_SETCURSEL, currentPlaybackIdx, 0);
    }
    else
    {
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_SETCURSEL, currentPlaybackIdx+1, 0);
    }
}

void Macro_STOH(void)
{
    LOOPS[LOOP_REG_H] = PopStackInteger();
}

void Macro_STOI(void)
{
    LOOPS[LOOP_REG_I] = PopStackInteger();
}

void Macro_STOJ(void)
{
    LOOPS[LOOP_REG_J] = PopStackInteger();
}

void Macro_STOHC(void)
{
    LOOPS[LOOP_REG_HC] = PopStackInteger();
}

void Macro_STOIC(void)
{
    LOOPS[LOOP_REG_IC] = PopStackInteger();
}

void Macro_STOJC(void)
{
    LOOPS[LOOP_REG_JC] = PopStackInteger();
}

void Macro_LoopH(void)
{
    // If we are less than the target, drive towards it
    if (LOOPS[LOOP_REG_H] < LOOPS[LOOP_REG_HC])
    {
        if (++LOOPS[LOOP_REG_H] < LOOPS[LOOP_REG_HC])
        {
            rpn_goto(UNI_LBLH, BACKWARDS); // Jump BACKWARDS
        }
    }
    // If we are greater than the target, drive towards it
    else if (LOOPS[LOOP_REG_H] > LOOPS[LOOP_REG_HC])
    {
        if (--LOOPS[LOOP_REG_H] > LOOPS[LOOP_REG_HC])
        {
            rpn_goto(UNI_LBLH, BACKWARDS); // Jump BACKWARDS
        }
    }
    // else were are at target - do not branch
}

void Macro_LoopI(void)
{
    // If we are less than the target, drive towards it
    if (LOOPS[LOOP_REG_I] < LOOPS[LOOP_REG_IC])
    {
        if (++LOOPS[LOOP_REG_I] < LOOPS[LOOP_REG_IC])
        {
            rpn_goto(UNI_LBLI, BACKWARDS); // Jump BACKWARDS
        }
    }
    // If we are greater than the target, drive towards it
    else if (LOOPS[LOOP_REG_I] > LOOPS[LOOP_REG_IC])
    {
        if (--LOOPS[LOOP_REG_I] > LOOPS[LOOP_REG_IC])
        {
            rpn_goto(UNI_LBLI, BACKWARDS); // Jump BACKWARDS
        }
    }
    // else were are at target - do not branch
}

void Macro_LoopJ(void)
{
    // If we are less than the target, drive towards it
    if (LOOPS[LOOP_REG_J] < LOOPS[LOOP_REG_JC])
    {
        if (++LOOPS[LOOP_REG_J] < LOOPS[LOOP_REG_JC])
        {
            rpn_goto(UNI_LBLJ, BACKWARDS); // Jump BACKWARDS
        }
    }
    // If we are greater than the target, drive towards it
    else if (LOOPS[LOOP_REG_J] > LOOPS[LOOP_REG_JC])
    {
        if (--LOOPS[LOOP_REG_J] > LOOPS[LOOP_REG_JC])
        {
            rpn_goto(UNI_LBLJ, BACKWARDS); // Jump BACKWARDS
        }
    }
    // else were are at target - do not branch
}

void Macro_DSZi(void)
{
    if (indirectRegister > 0)
    {
        // When we reach zero, we skip the next instruction
        if (--indirectRegister == 0)
        {
            if (macroPlayback == TRUE) currentPlaybackIdx++;
        }
    }
}

void Macro_DSZ2i(void)
{
    if ((indirectRegister >= 0) && (indirectRegister < MAX_STO+14))
    {
        if (indirectRegister >= MAX_STO+8) // Loop Counters
        {
            if (--LOOPS[indirectRegister - (MAX_STO+8)] == 0)
            {
                if (macroPlayback == TRUE) currentPlaybackIdx++;
            }
        }
        else if (indirectRegister >= MAX_STO) // Stack Contents
        {
            if (progMode == PROG_FLOAT)
            {
                if ((int64_t)--STACK[indirectRegister-MAX_STO] == 0)
                {
                    if (macroPlayback == TRUE) currentPlaybackIdx++;
                }
            }
            else
            {
                if (STACKL[indirectRegister-MAX_STO] == 0)
                {
                    if (macroPlayback == TRUE) currentPlaybackIdx++;
                }
            }
        }
        else // It's one of the normal R00-R99 registers
        {
            if (progMode == PROG_FLOAT)
            {
                if ((int64_t)--STO[indirectRegister] == 0)
                {
                    if (macroPlayback == TRUE) currentPlaybackIdx++;
                }
            }
            else
            {
                if (STOL[indirectRegister] == 0)
                {
                    if (macroPlayback == TRUE) currentPlaybackIdx++;
                }
            }
        }
    }
}

void StoreIndirect(int32_t reg)
{
    if ((reg >= 0) && (reg < MAX_STO+114))
    {
        if (reg >= MAX_STO+8) // Loop Contents
        {
            if (progMode == PROG_FLOAT)
            {
                LOOPS[reg-(MAX_STO+8)] = (int32_t)STACK[STK_X];
            }
            else
            {
                LOOPS[reg-(MAX_STO+8)] = (int32_t)STACKL[STK_X];
            }
        }
        else if (reg >= MAX_STO) // Stack Contents
        {
            if (progMode == PROG_FLOAT)
            {
                STACK[STK_X+reg-MAX_STO] = STACK[STK_X];
            }
            else
            {
                STACKL[STK_X+reg-MAX_STO] = STACKL[STK_X];
            }
        }
        else  // Must be one of the registers R00-R99
        {
            if (progMode == PROG_FLOAT)
            {
                STO[reg] = STACK[STK_X];
            }
            else
            {
                STOL[reg] = STACKL[STK_X];
            }
        }
        blinkXDisplay(FALSE);
    }
    else
    {
        RPN_error("Indirect Register Out Of Range(R0 to R99, XYZT, ABCD, H'I'J')");
    }
}

void RecallIndirect(int32_t reg)
{
    if ((reg >= 0) && (reg < MAX_STO+14))
    {
        if (reg >= MAX_STO+8) // Loop Counters
        {
            if (progMode == PROG_FLOAT)
            {
                StackPush(LOOPS[reg-(MAX_STO+8)]);
            }
            else
            {
                StackPushL(LOOPS[reg-(MAX_STO+8)]);
            }
        }
        else if (reg >= MAX_STO)  // Stack Contents
        {
            if (progMode == PROG_FLOAT)
            {
                StackPush(STACK[STK_X+reg-MAX_STO]);
            }
            else
            {
                StackPushL(STACKL[STK_X+reg-MAX_STO]);
            }
        }
        else  // Must be one of the registers R00-R99
        {
            if (progMode == PROG_FLOAT)
            {
                StackPush(STO[reg]);
            }
            else
            {
                StackPushL(STOL[reg]);
            }
        }
    }
    else
    {
        RPN_error("Indirect Register Out Of Range(R0 to R99, XYZT, ABCD, H'I'J')");
    }    
}

void Macro_Sto2i(void)
{
    StoreIndirect(indirectRegister);
}

void Macro_Sto2x(void)
{
    int32_t x = PopStackInteger();
    StoreIndirect(x);
}

void Macro_Rcl2i(void)
{
    RecallIndirect(indirectRegister);
}

void Macro_Rcl2x(void)
{
    int32_t x = PopStackInteger();
    RecallIndirect(x);
}
