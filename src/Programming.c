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
#include "Excal.h"

#pragma comment(lib, "winmm.lib")   // Needed to playback WAV beep

HWND debugTraceWindow = NULL;

double  debugValue = 0.0;
uint8_t dinputDebugInProgress = 0;
uint8_t RegisterToInput = 0;

extern void Macro_RecallN(void);
extern void Macro_StoreN(void);
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
extern void Macro_LblK(void);
extern void Macro_GotoK(void);
extern void Macro_GsbK(void);
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
extern void Macro_Return(void);
extern void Macro_SFx(void);
extern void Macro_CFx(void);
extern void Macro_TFx(void);
extern void Macro_ClearF(void);
extern void Macro_Pause(void);
extern void Macro_InpA(void);
extern void Macro_InpB(void);
extern void Macro_InpC(void);
extern void Macro_InpD(void);
extern void Macro_Trace(void);
extern void Macro_DEL(void);
extern void Macro_FWD(void);
extern void Macro_REV(void);
extern void Macro_EDIT(void);
extern void Macro_Debug(void);
extern void Macro_Step(void);
extern void Macro_LoopA(void);
extern void Macro_LoopB(void);
extern void Macro_LoopC(void);
extern void Macro_LoopD(void);
extern void Macro_LoopE(void);
extern void Macro_LoopF(void);
extern void Macro_LoopG(void);
extern void Macro_LoopH(void);
extern void Macro_LoopI(void);
extern void Macro_LoopJ(void);
extern void Macro_LoopK(void);
extern void Macro_DSZ(void);
extern void Macro_Sto2i(void);
extern void Macro_Rcl2i(void);
extern void Macro_Beep(void);

struct funcStruct Program1_funcs[MAX_FUNCS] =
{
    {FN1,   UNI_LBLA,       USES_FL,    ALLOWREC,   ' ',    "Lbl A",        YES_L,  X_NEW,   Macro_LblA,     T_LBLA,     H_LABELS},
    {FN2,   UNI_GOTOA,      USES_FL,    ALLOWREC,   ' ',    "Goto A",       YES_L,  X_NEW,   Macro_GotoA,    T_GOTOA,    H_GOTOS},
    {FN3,   UNI_GSBA,       USES_FL,    ALLOWREC,   ' ',    "Gsb A",        YES_L,  X_NEW,   Macro_GsbA,     T_GSBA,     H_GOSUBS},
    {FN4,   UNI_LOOPA,      USES_FL,    ALLOWREC,   ' ',    "Loop A",       YES_L,  X_NEW,   Macro_LoopA,    T_LOOPA,    H_LOOPA},

    {FN5,   UNI_LBLB,       USES_FL,    ALLOWREC,   ' ',    "Lbl B",        YES_L,  X_NEW,   Macro_LblB,     T_LBLB,     H_LABELS},
    {FN6,   UNI_GOTOB,      USES_FL,    ALLOWREC,   ' ',    "Goto B",       YES_L,  X_NEW,   Macro_GotoB,    T_GOTOB,    H_GOTOS},
    {FN7,   UNI_GSBB,       USES_FL,    ALLOWREC,   ' ',    "Gsb B",        YES_L,  X_NEW,   Macro_GsbB,     T_GSBB,     H_GOSUBS},
    {FN8,   UNI_LOOPB,      USES_FL,    ALLOWREC,   ' ',    "Loop B",       YES_L,  X_NEW,   Macro_LoopB,    T_LOOPB,    H_LOOPB},

    {FN9,   UNI_LBLC,       USES_FL,    ALLOWREC,   ' ',    "Lbl C",        YES_L,  X_NEW,   Macro_LblC,     T_LBLC,     H_LABELS},
    {FN10,  UNI_GOTOC,      USES_FL,    ALLOWREC,   ' ',    "Goto C",       YES_L,  X_NEW,   Macro_GotoC,    T_GOTOC,    H_GOTOS},
    {FN11,  UNI_GSBC,       USES_FL,    ALLOWREC,   ' ',    "Gsb C",        YES_L,  X_NEW,   Macro_GsbC,     T_GSBC,     H_GOSUBS},
    {FN12,  UNI_LOOPC,      USES_FL,    ALLOWREC,   ' ',    "Loop C",       YES_L,  X_NEW,   Macro_LoopC,    T_LOOPC,    H_LOOPC},

    {FN13,  UNI_LBLD,       USES_FL,    ALLOWREC,   ' ',    "Lbl D",        YES_L,  X_NEW,   Macro_LblD,     T_LBLD,     H_LABELS},
    {FN14,  UNI_GOTOD,      USES_FL,    ALLOWREC,   ' ',    "Goto D",       YES_L,  X_NEW,   Macro_GotoD,    T_GOTOD,    H_GOTOS},
    {FN15,  UNI_GSBD,       USES_FL,    ALLOWREC,   ' ',    "Gsb D",        YES_L,  X_NEW,   Macro_GsbD,     T_GSBD,     H_GOSUBS},
    {FN16,  UNI_LOOPD,      USES_FL,    ALLOWREC,   ' ',    "Loop D",       YES_L,  X_NEW,   Macro_LoopD,    T_LOOPD,    H_LOOPD},

    {FN17,  UNI_LBLE,       USES_FL,    ALLOWREC,   ' ',    "Lbl E",        YES_L,  X_NEW,   Macro_LblE,     T_LBLE,     H_LABELS},
    {FN18,  UNI_GOTOE,      USES_FL,    ALLOWREC,   ' ',    "Goto E",       YES_L,  X_NEW,   Macro_GotoE,    T_GOTOE,    H_GOTOS},
    {FN19,  UNI_GSBE,       USES_FL,    ALLOWREC,   ' ',    "Gsb E",        YES_L,  X_NEW,   Macro_GsbE,     T_GSBE,     H_GOSUBS},
    {FN20,  UNI_LOOPE,      USES_FL,    ALLOWREC,   ' ',    "Loop E",       YES_L,  X_NEW,   Macro_LoopE,    T_LOOPE,    H_LOOPE},

    {FN21,  UNI_LBLF,       USES_FL,    ALLOWREC,   ' ',    "Lbl F",        YES_L,  X_NEW,   Macro_LblF,     T_LBLF,     H_LABELS},
    {FN22,  UNI_GOTOF,      USES_FL,    ALLOWREC,   ' ',    "Goto F",       YES_L,  X_NEW,   Macro_GotoF,    T_GOTOF,    H_GOTOS},
    {FN23,  UNI_GSBF,       USES_FL,    ALLOWREC,   ' ',    "Gsb F",        YES_L,  X_NEW,   Macro_GsbF,     T_GSBF,     H_GOSUBS},
    {FN24,  UNI_LOOPF,      USES_FL,    ALLOWREC,   ' ',    "Loop F",       YES_L,  X_NEW,   Macro_LoopF,    T_LOOPF,    H_LOOPF},

    {FN25,  UNI_LBLG,       USES_FL,    ALLOWREC,   ' ',    "Lbl G",        YES_L,  X_NEW,   Macro_LblG,     T_LBLG,     H_LABELS},
    {FN26,  UNI_GOTOG,      USES_FL,    ALLOWREC,   ' ',    "Goto G",       YES_L,  X_NEW,   Macro_GotoG,    T_GOTOG,    H_GOTOS},
    {FN27,  UNI_GSBG,       USES_FL,    ALLOWREC,   ' ',    "Gsb G",        YES_L,  X_NEW,   Macro_GsbG,     T_GSBG,     H_GOSUBS},
    {FN28,  UNI_LOOPG,      USES_FL,    ALLOWREC,   ' ',    "Loop G",       YES_L,  X_NEW,   Macro_LoopG,    T_LOOPG,    H_LOOPG},

    {FN29,  UNI_LBLH,       USES_FL,    ALLOWREC,   ' ',    "Lbl H",        YES_L,  X_NEW,   Macro_LblH,     T_LBLH,     H_LABELS},
    {FN30,  UNI_GOTOH,      USES_FL,    ALLOWREC,   ' ',    "Goto H",       YES_L,  X_NEW,   Macro_GotoH,    T_GOTOH,    H_GOTOS},
    {FN31,  UNI_GSBH,       USES_FL,    ALLOWREC,   ' ',    "Gsb H",        YES_L,  X_NEW,   Macro_GsbH,     T_GSBH,     H_GOSUBS},
    {FN32,  UNI_LOOPH,      USES_FL,    ALLOWREC,   ' ',    "Loop H",       YES_L,  X_NEW,   Macro_LoopH,    T_LOOPH,    H_LOOPH},

    {FN33,  UNI_LBLI,       USES_FL,    ALLOWREC,   ' ',    "Lbl I",        YES_L,  X_NEW,   Macro_LblI,     T_LBLI,     H_LABELS},
    {FN34,  UNI_GOTOI,      USES_FL,    ALLOWREC,   ' ',    "Goto I",       YES_L,  X_NEW,   Macro_GotoI,    T_GOTOI,    H_GOTOS},
    {FN35,  UNI_GSBI,       USES_FL,    ALLOWREC,   ' ',    "Gsb I",        YES_L,  X_NEW,   Macro_GsbI,     T_GSBI,     H_GOSUBS},
    {FN36,  UNI_LOOPI,      USES_FL,    ALLOWREC,   ' ',    "Loop I",       YES_L,  X_NEW,   Macro_LoopI,    T_LOOPI,    H_LOOPI},

    {FN37,  UNI_RET,        USES_FL,    ALLOWREC,   ' ',    "Return",       YES_L,  X_NEW,   Macro_Return,   T_RETURN,   H_RETURN},
    {FN38,  UNI_HALT,       USES_FL,    ALLOWREC,   ' ',    "Halt",         YES_L,  X_NEW,   Macro_Halt,     T_HALT,     H_HALT},
    {FN39,  UNI_PAUSE,      USES_FL,    ALLOWREC,   ' ',    "Pause",        YES_L,  X_NEW,   Macro_Pause,    T_PAUSE,    H_PAUSE},
    {FN40,  UNI_DSZ,        USES_FL,    ALLOWREC,   ' ',    "DSZ",          YES_L,  X_NULL,  Macro_DSZ,      T_DSZ,      H_DSZ}
};

struct funcStruct Program2_funcs[MAX_FUNCS] =
{
    {FN1,   UNI_LBLJ,       USES_FL,    ALLOWREC,   ' ',    "Lbl J",    YES_L,      X_NEW,   Macro_LblJ,     T_LBLJ,     H_LABELS},
    {FN2,   UNI_GOTOJ,      USES_FL,    ALLOWREC,   ' ',    "Goto J",   YES_L,      X_NEW,   Macro_GotoJ,    T_GOTOJ,    H_GOTOS},
    {FN3,   UNI_GSBJ,       USES_FL,    ALLOWREC,   ' ',    "Gsb J",    YES_L,      X_NEW,   Macro_GsbJ,     T_GSBJ,     H_GOSUBS},
    {FN4,   UNI_LOOPJ,      USES_FL,    ALLOWREC,   ' ',    "Loop J",   YES_L,      X_NEW,   Macro_LoopJ,    T_LOOPJ,    H_LOOPJ},

    {FN5,   UNI_LBLK,       USES_FL,    ALLOWREC,   ' ',    "Lbl K",    YES_L,      X_NEW,   Macro_LblK,     T_LBLK,     H_LABELS},
    {FN6,   UNI_GOTOK,      USES_FL,    ALLOWREC,   ' ',    "Goto K",   YES_L,      X_NEW,   Macro_GotoK,    T_GOTOK,    H_GOTOS},
    {FN7,   UNI_GSBK,       USES_FL,    ALLOWREC,   ' ',    "Gsb K",    YES_L,      X_NEW,   Macro_GsbK,     T_GSBK,     H_GOSUBS},
    {FN8,   UNI_LOOPK,      USES_FL,    ALLOWREC,   ' ',    "Loop K",   YES_L,      X_NEW,   Macro_LoopK,    T_LOOPK,    H_LOOPK},

    {FN9,   UNI_INPA,       USES_FL,    ALLOWREC,   ' ',    "Inp R0",   YES_L,      X_NEW,   Macro_InpA,     T_INPA,     H_INPA},
    {FN10,  UNI_INPB,       USES_FL,    ALLOWREC,   ' ',    "Inp R1",   YES_L,      X_NEW,   Macro_InpB,     T_INPB,     H_INPB},
    {FN11,  UNI_INPC,       USES_FL,    ALLOWREC,   ' ',    "Inp R2",   YES_L,      X_NEW,   Macro_InpC,     T_INPC,     H_INPC},
    {FN12,  UNI_INPD,       USES_FL,    ALLOWREC,   ' ',    "Inp R3",   YES_L,      X_NEW,   Macro_InpD,     T_INPD,     H_INPD},

    {FN13,  UNI_SFX,        USES_FL,    ALLOWREC,   ' ',    "SFx",      YES_L,      X_NEW,   Macro_SFx,      T_SFX,      H_SFX},
    {FN14,  UNI_CFX,        USES_FL,    ALLOWREC,   ' ',    "CFx",      YES_L,      X_NEW,   Macro_CFx,      T_CFX,      H_CFX},
    {FN15,  UNI_TFX,        USES_FL,    ALLOWREC,   ' ',    "TFx?",     YES_L,      X_NEW,   Macro_TFx,      T_TFX,      H_TFX},
    {FN16,  UNI_CLRFL,      USES_FL,    ALLOWREC,   ' ',    "ClrF",     YES_L,      X_NEW,   Macro_ClearF,   T_CLEARF,   H_CLEARF},

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
    {FN27,  UNI_STOXTH,     USES_FL,    ALLOWREC,   ' ',    "StoXth",   YES_L,      X_NEW,   Macro_StoreN,   T_STOREN,   H_STOREN},
    {FN28,  UNI_RCLXTH,     USES_FL,    ALLOWREC,   ' ',    "RclXth",   YES_L,      X_NEW,   Macro_RecallN,  T_RECALLN,  H_RECALLN},

    {FN29,  UNI_STO2I,      USES_FL,    ALLOWREC,   ' ',    "Sto(i)",   YES_L,      X_NEW,   Macro_Sto2i,    T_STO2I,    H_STO2I},
    {FN30,  UNI_RCL2I,      USES_FL,    ALLOWREC,   ' ',    "Rcl(i)",   YES_L,      X_NEW,   Macro_Rcl2i,    T_RCL2I,    H_RCL2I},
    {FN31,  UNI_GOTOIND,    USES_FL,    ALLOWREC,   ' ',    "Goto(i)",  YES_L,      X_NEW,   Macro_GotoInd,  T_GOTOIND,  H_GOTOIND},
    {FN32,  UNI_GOSUBIND,   USES_FL,    ALLOWREC,   ' ',    "Gsb(i)",   YES_L,      X_NEW,   Macro_GosubInd, T_GSBIND,   H_GSBIND},

    {FN33,  UNI_MEDIT,      USES_FL,    NORECORD,   ' ',    "EDIT",     YES_L,      X_NEW,   Macro_EDIT,     T_MEDIT,    H_MEDIT},
    {FN34,  UNI_REV,        USES_FL,    NORECORD,   ' ',    "REV",      YES_L,      X_NEW,   Macro_REV,      T_REV,      H_REV},
    {FN35,  UNI_DEL,        USES_FL,    NORECORD,   ' ',    "DEL",      YES_L,      X_NEW,   Macro_DEL,      T_DEL,      H_DEL},
    {FN36,  UNI_FWD,        USES_FL,    NORECORD,   ' ',    "FWD",      YES_L,      X_NEW,   Macro_FWD,      T_FWD,      H_FWD},

    {FN37,  UNI_TRACE,      USES_FL,    NORECORD,   ' ',    "Trace",    YES_L,      X_NEW,   Macro_Trace,    T_TRACE,    H_TRACE},
    {FN38,  UNI_STEP,       USES_FL,    NORECORD,   ' ',    "Step",     YES_L,      X_NULL,  Macro_Step,     T_STEP,     H_STEP},
    {FN39,  UNI_BEEP,       USES_FL,    ALLOWREC,   ' ',    "Beep",     YES_L,      X_NEW,   Macro_Beep,     T_BEEP,     H_BEEP},
    {FN40,  UNI_DEBUG,      USES_FL,    NORECORD,   ' ',    "Debug",    YES_L,      X_NEW,   Macro_Debug,    T_DEBUG,    H_DEBUG}
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
void Macro_LblK(void) {}    // Function does nothing but anchors a label

void rpn_goto(uint16_t uniqueIdx)
{
    int j;

    if (macroPlayback == TRUE)
    {
        // Find the first instance of the label - that's where we jump...
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == uniqueIdx)
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoA(void)  {rpn_goto(UNI_LBLA);}
void Macro_GotoB(void)  {rpn_goto(UNI_LBLB);}
void Macro_GotoC(void)  {rpn_goto(UNI_LBLC);}
void Macro_GotoD(void)  {rpn_goto(UNI_LBLD);}
void Macro_GotoE(void)  {rpn_goto(UNI_LBLE);}
void Macro_GotoF(void)  {rpn_goto(UNI_LBLF);}
void Macro_GotoG(void)  {rpn_goto(UNI_LBLG);}
void Macro_GotoH(void)  {rpn_goto(UNI_LBLH);}
void Macro_GotoI(void)  {rpn_goto(UNI_LBLI);}
void Macro_GotoJ(void)  {rpn_goto(UNI_LBLJ);}
void Macro_GotoK(void)  {rpn_goto(UNI_LBLK);}

void RPN_gosub(uint16_t uniqueIdx)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            rpn_goto(uniqueIdx);
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
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
void Macro_GsbK(void)   {RPN_gosub(UNI_LBLK);}


void Macro_XLessY(void)
{
    if (macroPlayback == TRUE)
    {
        if (X <= Y)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_XGreaterY(void)
{
    if (macroPlayback == TRUE)
    {
        if (X > Y)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_XLEZero(void)
{
    if (macroPlayback == TRUE)
    {
        if (X <= 0.0)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_XGTZero(void)
{
    if (macroPlayback == TRUE)
    {
        if (X > 0.0)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_XEY(void)
{
    if (macroPlayback == TRUE)
    {
        if (X == Y)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}


void Macro_XNEY(void)
{
    if (macroPlayback == TRUE)
    {
        if (X != Y)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_XEZero(void)
{
    if (macroPlayback == TRUE)
    {
        if (X == 0.0)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_XNEZero(void)
{
    if (macroPlayback == TRUE)
    {
        if (X != 0.0)
        {
            // Do nothing - execute next line!
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_Return(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx > 0)
            currentMacroPlaybackIdx = MacroStack[--MacroStackIdx];
        else
            currentMacroPlaybackIdx = playBackIdx;      // Jump to end of program...
    }
}


void Macro_SFx(void)
{
    int iX;
    unsigned int mask;
    iX = (int) StackPop();
    mask = (0x00000001 << iX);
    macroFlags = macroFlags | mask;
}

void Macro_CFx(void)
{
    int iX;
    unsigned int mask;
    iX = (int) StackPop();
    mask = (0x00000001 << iX);
    macroFlags = macroFlags & ~mask;
}

void Macro_TFx(void)
{
    int iX;
    unsigned int mask;
    if (macroPlayback == TRUE)
    {
        iX = (int) StackPop();
        mask = (0x00000001 << iX);
        if (macroFlags & mask)
        {                       // Do nothing if mask set...
        }
        else // Skip next line
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_ClearF(void)
{
    macroFlags = 0x00000000;
    blinkXDisplay(0);
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
    // Play the resource asynchronously
    PlaySound(MAKEINTRESOURCE(IDR_BEEP), GetModuleHandle(NULL), SND_RESOURCE | SND_SYNC);
    sleep_and_peek(250);
}

BOOL CALLBACK inputRegisterProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[64];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        FormatNumberForStack(STO[RegisterToInput], tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 64);
            STO[RegisterToInput] = atof(tmp);
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }
    }
    return FALSE;
}

void Macro_InpA(void)
{
    RegisterToInput = 0;
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_InpB(void)
{
    RegisterToInput = 1;
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_InpC(void)
{
    RegisterToInput = 2;
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_InpD(void)
{
    RegisterToInput = 3;
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT_REG", calcMainWindow, inputRegisterProc);
}

void Macro_DEL(void)
{
    int k;

    if (recModeON == 1 && currentMacroPlaybackIdx > 0)
    {
        for (k = currentMacroPlaybackIdx - 1; k < playBackIdx; k++)
            playBack[k] = playBack[k + 1];      // Shift everything down one line!
        if (currentMacroPlaybackIdx == playBackIdx)
        {
            playBackIdx--;
            currentMacroPlaybackIdx--;
        }
        else
        {
            playBackIdx--;
        }
        MacroStackIdx = 0;
    }
}

void Macro_FWD(void)
{
    if (recModeON == 1 && currentMacroPlaybackIdx < playBackIdx)
    {
        currentMacroPlaybackIdx++;
        MacroStackIdx = 0;
    }
}

void Macro_REV(void)
{
    if (recModeON == 1 && currentMacroPlaybackIdx > 0)
    {
        currentMacroPlaybackIdx--;
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

void Macro_Step(void)
{
    if (recModeON == 0)         // Single Step...
    {
        RPN_SingleStep();
        if (currentMacroPlaybackIdx < playBackIdx)
        {
            currentMacroPlaybackIdx++;
        }
        else
        {
            currentMacroPlaybackIdx=0;
        }
    }
}


void Macro_EDIT(void)
{
    if (recModeON == 0)
    {
        recModeON = 1;
        currentMacroPlaybackIdx = 0;
        MacroStackIdx = 0;
        Xedit = X_NEW;
    }
    else
        recModeON = 0;
    ShowStatus();
}

void endRunningMacro(void)
{
    currentMacroPlaybackIdx = playBackIdx;
    MacroStackIdx = 0;
    macroPlayback = FALSE;
}

void Macro_RecallN(void)
{
    int i;

    i = (int) StackPop();
    if (i >= 0 && i < MAX_STO)
    {
        StackPush(STO[i]);
    }
    else
    {
        RPN_error("Register Out Of Range (R0 to R99)");
    }
}

void Macro_StoreN(void)
{
    int i;

    i = (int) StackPop();
    if (i >= 0 && i < MAX_STO)
    {
        STO[i] = X;
    }
    else
    {
        RPN_error("Register Out Of Range (R0 to R99)");
    }
}

void Macro_StoInd(void)
{
    if (progMode == PROG_FLOAT)
        indirectRegister = (unsigned long) X;
    else
    {
        if (wordMode == COMPSCI_SIGNED)
            indirectRegister = (long) XL;
        else
            indirectRegister = (unsigned long) XL;
    }
}

void Macro_RclInd(void)
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_FLOAT)
            X = indirectRegister;
        else
            XL = maskStackStuff((PROG_LONG) indirectRegister);
    }
    else
    {
        if (progMode == PROG_FLOAT)
            StackPush(indirectRegister);
        else
            StackPushL((PROG_LONG) indirectRegister);
    }
}

void Macro_GotoInd(void)
{
    uint16_t uniqueLabel = 0;
    int j;

    if (macroPlayback == TRUE)
    {
        if (indirectRegister <= 10)
        {
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
                case 10: uniqueLabel=UNI_LBLK; break;
            }
            for (j = 0; j < playBackIdx; j++)
            {
                if (playBackMap[playBack[j]].uniqueIndex == uniqueLabel)  // Unique Index for label
                {
                    currentMacroPlaybackIdx = j;
                    break;
                }
            }
        }
        else
        {
            RPN_error("Run Time Error: Value of indirect Label must point to LabelA=0 through LabelK=10");
            endRunningMacro();
        }
    }
}

void Macro_GosubInd(void)
{
    uint16_t uniqueLabel = 0;
    int j;

    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            if (indirectRegister <= 10)
            {
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
                    case 10: uniqueLabel=UNI_LBLK; break;
                }
                for (j = 0; j < playBackIdx; j++)
                {
                    if (playBackMap[playBack[j]].uniqueIndex == uniqueLabel)  // Unique Index for label
                    {
                        currentMacroPlaybackIdx = j;
                        break;
                    }
                }
            }
            else
            {
                RPN_error("Run Time Error: Value of indirect Label must point to LabelA=0 through LabelK=10");
                endRunningMacro();
            }
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
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
    char tmp[64];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        FormatNumberForStack(debugValue, tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 63);
            tmp[63] = CNULL;
            debugValue = atof(tmp);
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
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUT", calcMainWindow, inputDebugValue);
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
          last_debug_register_checksum = 0xFFFFBEEF;
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
                            case 0:
                                debugValue = T;
                                T = getNewDebugVal();
                                TL = (PROG_LONG) T;
                                break;
                            case 1:
                                debugValue = Z;
                                Z = getNewDebugVal();
                                ZL = (PROG_LONG) Z;
                                break;
                            case 2:
                                debugValue = Y;
                                Y = getNewDebugVal();
                                YL = (PROG_LONG) Y;
                                break;
                            case 3:
                                debugValue = X;
                                X = getNewDebugVal();
                                XL = (PROG_LONG) X;
                                break;
                            case 5:
                                debugValue = (double) indirectRegister;
                                indirectRegister = (int)getNewDebugVal();
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
                        debugValue = STO[item];
                        STO[item] = getNewDebugVal();
                        ShowStack();
                        return TRUE;
                    }
                    break;
                case TRACE_PROGRAM:
                    item = SendDlgItemMessage(hwnd, TRACE_PROGRAM, LB_GETCURSEL, 0, 0L);
                    if (recModeON)
                    {
                        currentMacroPlaybackIdx = (short int)item;
                        if (currentMacroPlaybackIdx > playBackIdx)
                            currentMacroPlaybackIdx = playBackIdx;
                    }
                    else
                    {
                        if (item > 0)
                            currentMacroPlaybackIdx = (short int) item-1;
                        else
                            currentMacroPlaybackIdx = (short int) 0;
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


int CreateDebugWindow(HWND hwnd, HINSTANCE hInstance)
{
    static char szAppName3[] = "DIALOG_PROGTRACE";
    debugTraceWindow = CreateDialog(hInstance, szAppName3, 0, ( DLGPROC )debugWndProc);
    ShowWindow(debugTraceWindow, SW_HIDE);
    UpdateWindow(debugTraceWindow);
    SetFocus(calcMainWindow);

    return 0;
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


void UpdateDebugRegs(void)
{
    char tmp[64];
    int i;

    SendMessage(GetDlgItem(debugTraceWindow, TRACE_REGS1), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
    SendMessage(GetDlgItem(debugTraceWindow, TRACE_REGS2), WM_SETFONT, (WPARAM) hFixedFont, FALSE);

    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_RESETCONTENT, 0, 0);

    sprintf(tmp, " Extended Stack");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " D: %-18.11g", D);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " C: %-18.11g", C);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " B: %-18.11g", B);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " A: %-18.11g", A);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
    sprintf(tmp, " Main Stack");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " T: %-18.11g", T);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Z: %-18.11g", Z);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Y: %-18.11g", Y);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " X: %-18.11g", X);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " i: %d", indirectRegister);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Flags: 0x%08X", macroFlags);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " ");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Financial Regs");
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Num: %-18.11g", fin_reg[0]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " Int: %-18.11g", fin_reg[1]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " PV:  %-18.11g", fin_reg[2]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " FV:  %-18.11g", fin_reg[3]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " MkC: %-18.11g", fin_reg[4]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " MkP: %-18.11g", fin_reg[5]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " CST: %-18.11g", fin_reg[6]);
    makeInternational(tmp);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));

    sprintf(tmp, " PRC: %-18.11g", fin_reg[7]);
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
        uint32_t debug_register_checksum = 0;
        uint8_t *ptr = (uint8_t *)STO;

        for (i = 0; i < sizeof(STO); i++)    
        {
            debug_register_checksum += *ptr++;
        }

        if (debug_register_checksum != last_debug_register_checksum)
        {
            last_debug_register_checksum = debug_register_checksum;
            SendDlgItemMessage(debugTraceWindow, TRACE_REGS2, LB_RESETCONTENT, 0, 0);

            for (i = 0; i < MAX_STO; i++)
            {
                sprintf(tmp, " R%02d: %-18.11g", i, STO[i]);
                makeInternational(tmp);
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
        for (i = 0; i < playBackIdx; i++)
        {
            sprintf(tmp, "%03d - %s", i + 1, playBackMap[playBack[i]].funcText);
            SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        }
        sprintf(tmp, "%03d - <End Of Program>", i + 1);
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
    }
    if (recModeON)
    {
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_SETCURSEL, currentMacroPlaybackIdx, 0);
    }
    else
    {
        SendDlgItemMessage(debugTraceWindow, TRACE_PROGRAM, LB_SETCURSEL, currentMacroPlaybackIdx+1, 0);
    }
}

void Macro_LoopA(void)
{
    if (macroPlayback == TRUE)
    {
        STO[0] = (float) ( ((int)STO[0]) -1 );
        if (STO[0] > 0.0)
        {
            Macro_GotoA();
        }
    }
}

void Macro_LoopB(void)
{
    if (macroPlayback == TRUE)
    {
        STO[1] = (float) ( ((int)STO[1]) -1 );
        if (STO[1] > 0.0)
        {
            Macro_GotoB();
        }
    }
}

void Macro_LoopC(void)
{
    if (macroPlayback == TRUE)
    {
        STO[2] = (float) ( ((int)STO[2]) -1 );
        if (STO[2] > 0.0)
        {
            Macro_GotoC();
        }
    }
}

void Macro_LoopD(void)
{
    if (macroPlayback == TRUE)
    {
        STO[3] = (float) ( ((int)STO[3]) -1 );
        if (STO[3] > 0.0)
        {
            Macro_GotoD();
        }
    }
}

void Macro_LoopE(void)
{
    if (macroPlayback == TRUE)
    {
        STO[4] = (float) ( ((int)STO[4]) -1 );
        if (STO[4] > 0.0)
        {
            Macro_GotoE();
        }
    }
}

void Macro_LoopF(void)
{
    if (macroPlayback == TRUE)
    {
        STO[5] = (float) ( ((int)STO[5]) -1 );
        if (STO[5] > 0.0)
        {
            Macro_GotoF();
        }
    }
}


void Macro_LoopG(void)
{
    if (macroPlayback == TRUE)
    {
        STO[6] = (float) ( ((int)STO[6]) -1 );
        if (STO[6] > 0.0)
        {
            Macro_GotoF();
        }
    }
}


void Macro_LoopH(void)
{
    if (macroPlayback == TRUE)
    {
        STO[7] = (float) ( ((int)STO[7]) -1 );
        if (STO[7] > 0.0)
        {
            Macro_GotoH();
        }
    }
}

void Macro_LoopI(void)
{
    if (macroPlayback == TRUE)
    {
        STO[8] = (float) ( ((int)STO[8]) -1 );
        if (STO[8] > 0.0)
        {
            Macro_GotoI();
        }
    }
}

void Macro_LoopJ(void)
{
    if (macroPlayback == TRUE)
    {
        STO[9] = (float) ( ((int)STO[9]) -1 );
        if (STO[9] > 0.0)
        {
            Macro_GotoJ();
        }
    }
}

void Macro_LoopK(void)
{
    if (macroPlayback == TRUE)
    {
        STO[10] = (float) ( ((int)STO[10]) -1 );
        if (STO[10] > 0.0)
        {
            Macro_GotoK();
        }
    }
}


void Macro_DSZ(void)
{
    if (macroPlayback == TRUE)
    {
        if (indirectRegister > 0)
        {
            indirectRegister--;
            if (indirectRegister == 0)
            {
                currentMacroPlaybackIdx++;
            }
        }
    }
}

void Macro_Sto2i(void)
{
    if (indirectRegister >= 0 && indirectRegister < MAX_STO)
    {
        STO[indirectRegister] = X;
    }
    else
    {
        RPN_error("Register Out Of Range(R0 to R99)");
    }
}

void Macro_Rcl2i(void)
{
    if (indirectRegister >= 0 && indirectRegister < MAX_STO)
    {
        StackPush(STO[indirectRegister]);
    }
    else
    {
        RPN_error("Register Out Of Range(R0 to R99)");
    }
}
