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

unsigned long loop = 0;
HWND debugTraceWindow = NULL;

double debugValue = 0.0;
int dinputDebugInProgress = 0;

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

void Macro_InpC(void);
void Macro_StoC(void);

void Macro_InpD(void);

void Macro_StoInd(void);
void Macro_RclInd(void);
void Macro_GotoInd(void);
void Macro_GosubInd(void);

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
extern void Macro_BST(void);
extern void Macro_Trace(void);
extern void Macro_StopTrace(void);
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


struct funcStruct Program1_funcs[MAX_FUNCS] = 
{
    {FN1,   0,  UNI_LBLA,       USES_FL,    ALLOWREC,   ' ',    "Lbl A",        YES_L,  X_NEW,   Macro_LblA,     T_LBLA,     H_LABELS},
    {FN2,   0,  UNI_GOTOA,      USES_FL,    ALLOWREC,   ' ',    "Goto A",       YES_L,  X_NEW,   Macro_GotoA,    T_GOTOA,    H_GOTOS},
    {FN3,   0,  UNI_GSBA,       USES_FL,    ALLOWREC,   ' ',    "Gsb A",        YES_L,  X_NEW,   Macro_GsbA,     T_GSBA,     H_GOSUBS},
    {FN4,   0,  UNI_LOOPA,      USES_FL,    ALLOWREC,   ' ',    "Loop A",       YES_L,  X_NEW,   Macro_LoopA,    T_LOOPA,    H_LOOPA},
                                        
    {FN5,   0,  UNI_LBLB,       USES_FL,    ALLOWREC,   ' ',    "Lbl B",        YES_L,  X_NEW,   Macro_LblB,     T_LBLB,     H_LABELS},
    {FN6,   0,  UNI_GOTOB,      USES_FL,    ALLOWREC,   ' ',    "Goto B",       YES_L,  X_NEW,   Macro_GotoB,    T_GOTOB,    H_GOTOS},
    {FN7,   0,  UNI_GSBB,       USES_FL,    ALLOWREC,   ' ',    "Gsb B",        YES_L,  X_NEW,   Macro_GsbB,     T_GSBB,     H_GOSUBS},
    {FN8,   0,  UNI_LOOPB,      USES_FL,    ALLOWREC,   ' ',    "Loop B",       YES_L,  X_NEW,   Macro_LoopB,    T_LOOPB,    H_LOOPB},
                                    
    {FN9,   0,  UNI_LBLC,       USES_FL,    ALLOWREC,   ' ',    "Lbl C",        YES_L,  X_NEW,   Macro_LblC,     T_LBLC,     H_LABELS},
    {FN10,  0,  UNI_GOTOC,      USES_FL,    ALLOWREC,   ' ',    "Goto C",       YES_L,  X_NEW,   Macro_GotoC,    T_GOTOC,    H_GOTOS},
    {FN11,  0,  UNI_GSBC,       USES_FL,    ALLOWREC,   ' ',    "Gsb C",        YES_L,  X_NEW,   Macro_GsbC,     T_GSBC,     H_GOSUBS},
    {FN12,  0,  UNI_LOOPC,      USES_FL,    ALLOWREC,   ' ',    "Loop C",       YES_L,  X_NEW,   Macro_LoopC,    T_LOOPC,    H_LOOPC},
                                        
    {FN13,  0,  UNI_LBLD,       USES_FL,    ALLOWREC,   ' ',    "Lbl D",        YES_L,  X_NEW,   Macro_LblD,     T_LBLD,     H_LABELS},
    {FN14,  0,  UNI_GOTOD,      USES_FL,    ALLOWREC,   ' ',    "Goto D",       YES_L,  X_NEW,   Macro_GotoD,    T_GOTOD,    H_GOTOS},
    {FN15,  0,  UNI_GSBD,       USES_FL,    ALLOWREC,   ' ',    "Gsb D",        YES_L,  X_NEW,   Macro_GsbD,     T_GSBD,     H_GOSUBS},
    {FN16,  0,  UNI_LOOPD,      USES_FL,    ALLOWREC,   ' ',    "Loop D",       YES_L,  X_NEW,   Macro_LoopD,    T_LOOPD,    H_LOOPD},
                                        
    {FN17,  0,  UNI_LBLE,       USES_FL,    ALLOWREC,   ' ',    "Lbl E",        YES_L,  X_NEW,   Macro_LblE,     T_LBLE,     H_LABELS},
    {FN18,  0,  UNI_GOTOE,      USES_FL,    ALLOWREC,   ' ',    "Goto E",       YES_L,  X_NEW,   Macro_GotoE,    T_GOTOE,    H_GOTOS},
    {FN19,  0,  UNI_GSBE,       USES_FL,    ALLOWREC,   ' ',    "Gsb E",        YES_L,  X_NEW,   Macro_GsbE,     T_GSBE,     H_GOSUBS},
    {FN20,  0,  UNI_LOOPE,      USES_FL,    ALLOWREC,   ' ',    "Loop E",       YES_L,  X_NEW,   Macro_LoopE,    T_LOOPE,    H_LOOPE},
                                        
    {FN21,  0,  UNI_LBLF,       USES_FL,    ALLOWREC,   ' ',    "Lbl F",        YES_L,  X_NEW,   Macro_LblF,     T_LBLF,     H_LABELS},
    {FN22,  0,  UNI_GOTOF,      USES_FL,    ALLOWREC,   ' ',    "Goto F",       YES_L,  X_NEW,   Macro_GotoF,    T_GOTOF,    H_GOTOS},
    {FN23,  0,  UNI_GSBF,       USES_FL,    ALLOWREC,   ' ',    "Gsb F",        YES_L,  X_NEW,   Macro_GsbF,     T_GSBF,     H_GOSUBS},
    {FN24,  0,  UNI_LOOPF,      USES_FL,    ALLOWREC,   ' ',    "Loop F",       YES_L,  X_NEW,   Macro_LoopF,    T_LOOPF,    H_LOOPF},
                                        
    {FN25,  0,  UNI_LBLG,       USES_FL,    ALLOWREC,   ' ',    "Lbl G",        YES_L,  X_NEW,   Macro_LblG,     T_LBLG,     H_LABELS},
    {FN26,  0,  UNI_GOTOG,      USES_FL,    ALLOWREC,   ' ',    "Goto G",       YES_L,  X_NEW,   Macro_GotoG,    T_GOTOG,    H_GOTOS},
    {FN27,  0,  UNI_GSBG,       USES_FL,    ALLOWREC,   ' ',    "Gsb G",        YES_L,  X_NEW,   Macro_GsbG,     T_GSBG,     H_GOSUBS},
    {FN28,  0,  UNI_LOOPG,      USES_FL,    ALLOWREC,   ' ',    "Loop G",       YES_L,  X_NEW,   Macro_LoopG,    T_LOOPG,    H_LOOPG},
                                    
    {FN29,  0,  UNI_LBLH,       USES_FL,    ALLOWREC,   ' ',    "Lbl H",        YES_L,  X_NEW,   Macro_LblH,     T_LBLH,     H_LABELS},
    {FN30,  0,  UNI_GOTOH,      USES_FL,    ALLOWREC,   ' ',    "Goto H",       YES_L,  X_NEW,   Macro_GotoH,    T_GOTOH,    H_GOTOS},
    {FN31,  0,  UNI_GSBH,       USES_FL,    ALLOWREC,   ' ',    "Gsb H",        YES_L,  X_NEW,   Macro_GsbH,     T_GSBH,     H_GOSUBS},
    {FN32,  0,  UNI_LOOPH,      USES_FL,    ALLOWREC,   ' ',    "Loop H",       YES_L,  X_NEW,   Macro_LoopH,    T_LOOPH,    H_LOOPH},
                                    
    {FN33,  0,  UNI_LBLI,       USES_FL,    ALLOWREC,   ' ',    "Lbl I",        YES_L,  X_NEW,   Macro_LblI,     T_LBLI,     H_LABELS},
    {FN34,  0,  UNI_GOTOI,      USES_FL,    ALLOWREC,   ' ',    "Goto I",       YES_L,  X_NEW,   Macro_GotoI,    T_GOTOI,    H_GOTOS},
    {FN35,  0,  UNI_GSBI,       USES_FL,    ALLOWREC,   ' ',    "Gsb I",        YES_L,  X_NEW,   Macro_GsbI,     T_GSBI,     H_GOSUBS},
    {FN36,  0,  UNI_LOOPI,      USES_FL,    ALLOWREC,   ' ',    "Loop I",       YES_L,  X_NEW,   Macro_LoopI,    T_LOOPI,    H_LOOPI},
                                    
    {FN37,  0,  UNI_RET,        USES_FL,    ALLOWREC,   ' ',    "Return",       YES_L,  X_NEW,   Macro_Return,   T_RETURN,   H_RETURN},
    {FN38,  0,  UNI_HALT,       USES_FL,    ALLOWREC,   ' ',    "Halt",         YES_L,  X_NEW,   Macro_Halt,     T_HALT,     H_HALT},
    {FN39,  0,  UNI_PAUSE,      USES_FL,    ALLOWREC,   ' ',    "Pause",        YES_L,  X_NEW,   Macro_Pause,    T_PAUSE,    H_PAUSE},
    {FN40,  0,  UNI_DSZ,        USES_FL,    ALLOWREC,   ' ',    "DSZ",          YES_L,  X_NULL,  Macro_DSZ,      T_DSZ,      H_DSZ}
};

struct funcStruct Program2_funcs[MAX_FUNCS] = 
{
    {FN1,   0,  UNI_LBLJ,       USES_FL,    ALLOWREC,   ' ',    "Lbl J",    YES_L,      X_NEW,   Macro_LblJ,     T_LBLJ,     H_LABELS},
    {FN2,   0,  UNI_GOTOJ,      USES_FL,    ALLOWREC,   ' ',    "Goto J",   YES_L,      X_NEW,   Macro_GotoJ,    T_GOTOJ,    H_GOTOS},
    {FN3,   0,  UNI_GSBJ,       USES_FL,    ALLOWREC,   ' ',    "Gsb J",    YES_L,      X_NEW,   Macro_GsbJ,     T_GSBJ,     H_GOSUBS},
    {FN4,   0,  UNI_LOOPJ,      USES_FL,    ALLOWREC,   ' ',    "Loop J",   YES_L,      X_NEW,   Macro_LoopJ,    T_LOOPJ,    H_LOOPJ},
                                        
    {FN5,   0,  UNI_LBLK,       USES_FL,    ALLOWREC,   ' ',    "Lbl K",    YES_L,      X_NEW,   Macro_LblK,     T_LBLK,     H_LABELS},
    {FN6,   0,  UNI_GOTOK,      USES_FL,    ALLOWREC,   ' ',    "Goto K",   YES_L,      X_NEW,   Macro_GotoK,    T_GOTOK,    H_GOTOS},
    {FN7,   0,  UNI_GSBK,       USES_FL,    ALLOWREC,   ' ',    "Gsb K",    YES_L,      X_NEW,   Macro_GsbK,     T_GSBK,     H_GOSUBS},
    {FN8,   0,  UNI_LOOPK,      USES_FL,    ALLOWREC,   ' ',    "Loop K",   YES_L,      X_NEW,   Macro_LoopK,    T_LOOPK,    H_LOOPK},
                                        
    {FN9,   0,  UNI_INPA,       USES_FL,    ALLOWREC,   ' ',    "Inp R1",   YES_L,      X_NEW,   Macro_InpA,     T_INPA,     H_INPA},
    {FN10,  0,  UNI_INPB,       USES_FL,    ALLOWREC,   ' ',    "Inp R2",   YES_L,      X_NEW,   Macro_InpB,     T_INPB,     H_INPB},
    {FN11,  0,  UNI_INPC,       USES_FL,    ALLOWREC,   ' ',    "Inp R3",   YES_L,      X_NEW,   Macro_InpC,     T_INPC,     H_INPC},
    {FN12,  0,  UNI_INPD,       USES_FL,    ALLOWREC,   ' ',    "Inp R4",   YES_L,      X_NEW,   Macro_InpD,     T_INPD,     H_INPD},
                                        
    {FN13,  0,  UNI_SFX,        USES_FL,    ALLOWREC,   ' ',    "SFx",      YES_L,      X_NEW,   Macro_SFx,      T_SFX,      H_SFX},
    {FN14,  0,  UNI_CFX,        USES_FL,    ALLOWREC,   ' ',    "CFx",      YES_L,      X_NEW,   Macro_CFx,      T_CFX,      H_CFX},
    {FN15,  0,  UNI_TFX,        USES_FL,    ALLOWREC,   ' ',    "TFx?",     YES_L,      X_NEW,   Macro_TFx,      T_TFX,      H_TFX},
    {FN16,  0,  UNI_CLRFL,      USES_FL,    ALLOWREC,   ' ',    "ClrF",     YES_L,      X_NEW,   Macro_ClearF,   T_CLEARF,   H_CLEARF},
                                        
    {FN17,  0,  UNI_XLEY,       USES_FL,    ALLOWREC,   ' ',    "X<=Y?",    YES_L,      X_NEW,   Macro_XLessY,   T_MACXLY,   H_MACXLY},
    {FN18,  0,  UNI_XGY,        USES_FL,    ALLOWREC,   ' ',    "X>Y?",     YES_L,      X_NEW,   Macro_XGreaterY,T_MACXGY,   H_MACXGY},
    {FN19,  0,  UNI_XEY,        USES_FL,    ALLOWREC,   ' ',    "X=Y?",     YES_L,      X_NEW,   Macro_XEY,      T_XEY,      H_XEY},
    {FN20,  0,  UNI_XNEY,       USES_FL,    ALLOWREC,   ' ',    "X!=Y?",    YES_L,      X_NEW,   Macro_XNEY,     T_XNEY,     H_XNEY},
                                        
    {FN21,  0,  UNI_XLEZ,       USES_FL,    ALLOWREC,   ' ',    "X<=0?",    YES_L,      X_NEW,   Macro_XLEZero,  T_XLEZERO,  H_XLEZERO},
    {FN22,  0,  UNI_XGZ,        USES_FL,    ALLOWREC,   ' ',    "X>0?",     YES_L,      X_NEW,   Macro_XGTZero,  T_XGTZERO,  H_XGTZERO},
    {FN23,  0,  UNI_XEZ,        USES_FL,    ALLOWREC,   ' ',    "X=0?",     YES_L,      X_NEW,   Macro_XEZero,   T_XEZERO,   H_XEZERO},
    {FN24,  0,  UNI_XNEZ,       USES_FL,    ALLOWREC,   ' ',    "X!=0?",    YES_L,      X_NEW,   Macro_XNEZero,  T_XNEZERO,  H_XNEZERO},
                                        
    {FN25,  0,  UNI_STOIND,     USES_FL,    ALLOWREC,   ' ',    "Sto i",    YES_L,      X_NEW,   Macro_StoInd,   T_STOIND,   H_STOIND},
    {FN26,  0,  UNI_RCLIND,     USES_FL,    ALLOWREC,   ' ',    "Rcl i",    YES_L,      X_NEW,   Macro_RclInd,   T_RCLIND,   H_RCLIND},
    {FN27,  0,  UNI_STOXTH,     USES_FL,    ALLOWREC,   ' ',    "StoXth",   YES_L,      X_NEW,   Macro_StoreN,   T_STOREN,   H_STOREN},
    {FN28,  0,  UNI_RCLXTH,     USES_FL,    ALLOWREC,   ' ',    "RclXth",   YES_L,      X_NEW,   Macro_RecallN,  T_RECALLN,  H_RECALLN},
                                    
    {FN29,  0,  UNI_STO2I,      USES_FL,    ALLOWREC,   ' ',    "Sto(i)",   YES_L,      X_NEW,   Macro_Sto2i,    T_STO2I,    H_STO2I},
    {FN30,  0,  UNI_RCL2I,      USES_FL,    ALLOWREC,   ' ',    "Rcl(i)",   YES_L,      X_NEW,   Macro_Rcl2i,    T_RCL2I,    H_RCL2I},
    {FN31,  0,  UNI_GOTOIND,    USES_FL,    ALLOWREC,   ' ',    "Goto(i)",  YES_L,      X_NEW,   Macro_GotoInd,  T_GOTOIND,  H_GOTOIND},
    {FN32,  0,  UNI_GOSUBIND,   USES_FL,    ALLOWREC,   ' ',    "Gsb(i)",   YES_L,      X_NEW,   Macro_GosubInd, T_GSBIND,   H_GSBIND},
                                
    {FN33,  0,  UNI_MEDIT,      USES_FL,    NORECORD,   ' ',    "EDIT",     YES_L,      X_NEW,   Macro_EDIT,     T_MEDIT,    H_MEDIT},
    {FN34,  0,  UNI_REV,        USES_FL,    NORECORD,   ' ',    "REV",      YES_L,      X_NEW,   Macro_REV,      T_REV,      H_REV},
    {FN35,  0,  UNI_DEL,        USES_FL,    NORECORD,   ' ',    "DEL",      YES_L,      X_NEW,   Macro_DEL,      T_DEL,      H_DEL},
    {FN36,  0,  UNI_FWD,        USES_FL,    NORECORD,   ' ',    "FWD",      YES_L,      X_NEW,   Macro_FWD,      T_FWD,      H_FWD},
                                    
    {FN37,  0,  UNI_TRACE,      USES_FL,    NORECORD,   ' ',    "Trace",    YES_L,      X_NEW,   Macro_Trace,    T_TRACE,    H_TRACE},
    {FN38,  0,  UNI_STRACE,     USES_FL,    NORECORD,   ' ',    "StopTr",   YES_L,      X_NEW,   Macro_StopTrace,T_STRACE,   H_STRACE},
    {FN39,  0,  UNI_DEBUG,      USES_FL,    NORECORD,   ' ',    "Debug",    YES_L,      X_NEW,   Macro_Debug,    T_DEBUG,    H_DEBUG},
    {FN40,  0,  UNI_STEP,       USES_FL,    NORECORD,   ' ',    "Step",     YES_L,      X_NULL,  Macro_Step,     T_STEP,     H_STEP}
};


void Macro_LblA(void)
{
}

void Macro_LblB(void)
{
}

void Macro_LblC(void)
{
}

void Macro_LblD(void)
{
}

void Macro_LblE(void)
{
}

void Macro_LblF(void)
{
}

void Macro_LblG(void)
{
}


void Macro_GotoA(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLA)       // Unique Index for label A
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoB(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLB)       // Unique Index for label B
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}


void Macro_GotoC(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLC)       // Unique Index for label C
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoD(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLD)       // Unique Index for label D
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoE(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLE)       // Unique Index for label E
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoF(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLF)       // Unique Index for label F
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoG(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLG)       // Unique Index for label G
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}



void Macro_XLessY(void)
{
    if (macroPlayback == TRUE)
    {
        if (X <= Y)
        {
            // Do nothing - execute next line!
        }
        else
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
        else
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
        else
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
        else
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
        else
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
        else
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
        else
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
        else
        {
            currentMacroPlaybackIdx++;
        }
    }
}


void Macro_GsbA(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoA();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbB(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoB();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbC(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoC();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbD(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoD();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbE(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoE();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbF(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoF();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbG(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoG();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
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
        else
        {
            currentMacroPlaybackIdx++;
        }
    }
}

void Macro_ClearF(void)
{
    macroFlags = 0x00000000;
}


void Macro_Pause(void)
{
    if (macroPlayback == TRUE)
    {
        ShowStack();
        Sleep(progDelayValue);
    }
}


BOOL CALLBACK inputARegisterProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[75];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        MakeSciFormat(STO[0], tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 74);
            STO[0] = atof(tmp);
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
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUTA", calcMainWindow, inputARegisterProc);
}


BOOL CALLBACK inputBRegisterProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[75];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        MakeSciFormat(STO[1], tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 74);
            STO[1] = atof(tmp);
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }
    }
    return FALSE;
}

void Macro_InpB(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUTB", calcMainWindow, inputBRegisterProc);
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
        traceMacroPlayback = TRUE;
        RPN_Playback();
        traceMacroPlayback = FALSE;
    }
}

void Macro_StopTrace(void)
{
    traceMacroPlayback = FALSE;
    endRunningMacro();
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
    if (i >= 0 && i < 26)
    {
        StackPush(STO[i]);
    }
    else
    {
        RPN_error("Register Out Of Range(0=A to 25=Z)");
    }
}

void Macro_StoreN(void)
{
    int i;

    i = (int) StackPop();
    if (i >= 0 && i < 26)
    {
        STO[i] = X;
    }
    else
    {
        RPN_error("Register Out Of Range(0=A to 25=Z)");
    }
}

void Macro_LblH(void)
{
}

void Macro_LblI(void)
{
}

void Macro_LblJ(void)
{
}

void Macro_LblK(void)
{
}

void Macro_GotoH(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLH)       // Unique Index for label H
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoI(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLI)       // Unique Index for label I
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoJ(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLJ)       // Unique Index for label J
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}

void Macro_GotoK(void)
{
    int j;

    if (macroPlayback == TRUE)
    {
        for (j = 0; j < playBackIdx; j++)
        {
            if (playBackMap[playBack[j]].uniqueIndex == UNI_LBLK)       // Unique Index for label K
            {
                currentMacroPlaybackIdx = j;
                break;
            }
        }
    }
}


void Macro_GsbH(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoH();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbI(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoI();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbJ(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoJ();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

void Macro_GsbK(void)
{
    if (macroPlayback == TRUE)
    {
        if (MacroStackIdx < MAX_MACRO_STACK - 1)
        {
            MacroStack[MacroStackIdx++] = currentMacroPlaybackIdx;
            Macro_GotoK();
        }
        else
        {
            RPN_error("Max Program Function Stack Reached - No more Gosubs!");
            currentMacroPlaybackIdx = playBackIdx; // end program.
        }
    }
}

BOOL CALLBACK inputCRegisterProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[75];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        MakeSciFormat(STO[0], tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 74);
            STO[2] = atof(tmp);
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }
    }
    return FALSE;
}

void Macro_InpC(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUTC", calcMainWindow, inputCRegisterProc);
}


BOOL CALLBACK inputDRegisterProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[75];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        MakeSciFormat(STO[0], tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 74);
            STO[3] = atof(tmp);
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        default:
            return FALSE;
        }
    }
    return FALSE;
}

void Macro_InpD(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_INPUTD", calcMainWindow, inputDRegisterProc);
}



void Macro_StoC(void)          // A quick way to store the A register.
{
    if (progMode == PROG_NORMAL)
        STO[2] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[2] = (PROG_SIGNEDLONG) XL;
        else
            STO[2] = XL;
    }
}

void Macro_StoD(void)          // A quick way to store the B register.
{
    if (progMode == PROG_NORMAL)
        STO[3] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[3] = (PROG_SIGNEDLONG) XL;
        else
            STO[3] = XL;
    }
}

void Macro_RclC(void)         // A quick way to recall the C register.
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_NORMAL)
            X = STO[2];
        else
            XL = maskStackStuff((PROG_LONG) STO[2]);
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(STO[2]);
        else
            StackPushL((PROG_LONG) STO[2]);
    }
}

void Macro_RclD(void)         // A quick way to recall the D register.
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_NORMAL)
            X = STO[3];
        else
            XL = maskStackStuff((PROG_LONG) STO[3]);
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(STO[3]);
        else
            StackPushL((PROG_LONG) STO[3]);
    }
}


void Macro_StoInd(void)
{
    if (progMode == PROG_NORMAL)
        indirectRegister = (unsigned long) X;
    else
    {
        if (wordMode == PROG_SIGNED)
            indirectRegister = (PROG_SIGNEDLONG) XL;
        else
            indirectRegister = XL;
    }
}

void Macro_RclInd(void)
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_NORMAL)
            X = indirectRegister;
        else
            XL = maskStackStuff((PROG_LONG) indirectRegister);
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(indirectRegister);
        else
            StackPushL((PROG_LONG) indirectRegister);
    }
}

void Macro_GotoInd(void)
{
    int uniqueLabel = 0;
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
                case 10: uniqueLabel=UNI_LBLJ; break;
            }
            for (j = 0; j < playBackIdx; j++)
            {
                if (playBackMap[playBack[j]].uniqueIndex == uniqueLabel)       // Unique Index for label J
                {
                    currentMacroPlaybackIdx = j;
                    break;
                }
            }
        }
        else
        {
            RPN_error("Run Time Error: Value of indirect register must point to A=0 through K=10");
            endRunningMacro();
        }
    }
}

void Macro_GosubInd(void)
{
    int uniqueLabel = 0;
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
                    case 10: uniqueLabel=UNI_LBLJ; break;
                }
                for (j = 0; j < playBackIdx; j++)
                {
                    if (playBackMap[playBack[j]].uniqueIndex == uniqueLabel)       // Unique Index for label J
                    {
                        currentMacroPlaybackIdx = j;
                        break;
                    }
                }
            }
            else
            {
                RPN_error("Run Time Error: Value of indirect register must point to A=0 through K=10");
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
    char tmp[75];
    switch(wMessage)
    {
    case WM_INITDIALOG:
        MakeSciFormat(debugValue, tmp);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp);
        SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 74);
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


BOOL CALLBACK debugWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT item;

     switch(message)
     {
     case WM_INITDIALOG:
          return TRUE;

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
            Macro_Debug(); // This will toggle the window off...
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
    char tmp[50];
    int i;

    SendMessage(GetDlgItem(debugTraceWindow, TRACE_REGS1), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
    SendMessage(GetDlgItem(debugTraceWindow, TRACE_REGS2), WM_SETFONT, (WPARAM) hFixedFont, FALSE);

    SendDlgItemMessage(debugTraceWindow, TRACE_REGS1, LB_RESETCONTENT, 0, 0);
    SendDlgItemMessage(debugTraceWindow, TRACE_REGS2, LB_RESETCONTENT, 0, 0);

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

    for (i = 0; i < MAX_STO; i++)
    {
        sprintf(tmp, " R%02d: %-18.11g", i, STO[i]);
        makeInternational(tmp);
        SendDlgItemMessage(debugTraceWindow, TRACE_REGS2, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
    }
}

void UpdateDebugProgram(int resetProgramList)
{
    char tmp[60];
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

void Macro_StoE(void)
{
    if (progMode == PROG_NORMAL)
        STO[4] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[4] = (PROG_SIGNEDLONG) XL;
        else
            STO[4] = XL;
    }
}

void Macro_RclE(void)
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_NORMAL)
            X = STO[4];
        else
            XL = maskStackStuff((PROG_LONG) STO[4]);
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(STO[4]);
        else
            StackPushL((PROG_LONG) STO[4]);
    }
}

void Macro_StoF(void)
{
    if (progMode == PROG_NORMAL)
        STO[5] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[5] = (PROG_SIGNEDLONG) XL;
        else
            STO[5] = XL;
    }
}

void Macro_RclF(void)
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_NORMAL)
            X = STO[5];
        else
            XL = maskStackStuff((PROG_LONG) STO[5]);
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(STO[5]);
        else
            StackPushL((PROG_LONG) STO[5]);
    }
}


void Macro_StoG(void)
{
    if (progMode == PROG_NORMAL)
        STO[6] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[6] = (PROG_SIGNEDLONG) XL;
        else
            STO[6] = XL;
    }
}

void Macro_RclG(void)
{
    if (Xedit == X_ENTER)
    {
        if (progMode == PROG_NORMAL)
            X = STO[6];
        else
            XL = maskStackStuff((PROG_LONG) STO[6]);
    }
    else
    {
        if (progMode == PROG_NORMAL)
            StackPush(STO[6]);
        else
            StackPushL((PROG_LONG) STO[6]);
    }
}

void Macro_StoH(void)
{
    if (progMode == PROG_NORMAL)
        STO[7] = X;
    else
    {
        if (wordMode == PROG_SIGNED)
            STO[7] = (PROG_SIGNEDLONG) XL;
        else
            STO[7] = XL;
    }
}

void Macro_Sto2i(void)
{
    if (indirectRegister >= 0 && indirectRegister < 26)
    {
        STO[indirectRegister] = X;
    }
    else
    {
        RPN_error("Register Out Of Range(R0 to R25)");
    }
}

void Macro_Rcl2i(void)
{
    if (indirectRegister >= 0 && indirectRegister < 26)
    {
        StackPush(STO[indirectRegister]);
    }
    else
    {
        RPN_error("Register Out Of Range(R0 to R25)");
    }
}


