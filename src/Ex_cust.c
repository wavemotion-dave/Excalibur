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
extern void Cust_MacroPlayback65(void);
extern void Cust_MacroPlayback66(void);
extern void Cust_MacroPlayback67(void);
extern void Cust_MacroPlayback68(void);
extern void Cust_MacroPlayback69(void);
extern void Cust_MacroPlayback70(void);
extern void Cust_MacroPlayback71(void);
extern void Cust_MacroPlayback72(void);
extern void Cust_MacroPlayback73(void);
extern void Cust_MacroPlayback74(void);
extern void Cust_MacroPlayback75(void);
extern void Cust_MacroPlayback76(void);
extern void Cust_MacroPlayback77(void);
extern void Cust_MacroPlayback78(void);
extern void Cust_MacroPlayback79(void);
extern void Cust_MacroPlayback80(void);
extern void Cust_MacroPlayback81(void);
extern void Cust_MacroPlayback82(void);
extern void Cust_MacroPlayback83(void);
extern void Cust_MacroPlayback84(void);
extern void Cust_MacroPlayback85(void);
extern void Cust_MacroPlayback86(void);
extern void Cust_MacroPlayback87(void);
extern void Cust_MacroPlayback88(void);
extern void Cust_MacroPlayback89(void);
extern void Cust_MacroPlayback90(void);
extern void Cust_MacroPlayback91(void);
extern void Cust_MacroPlayback92(void);
extern void Cust_MacroPlayback93(void);
extern void Cust_MacroPlayback94(void);
extern void Cust_MacroPlayback95(void);
extern void Cust_MacroPlayback96(void);
extern void Cust_MacroPlayback97(void);
extern void Cust_MacroPlayback98(void);
extern void Cust_MacroPlayback99(void);
extern void Cust_MacroPlayback100(void);
extern void Cust_MacroPlayback101(void);
extern void Cust_MacroPlayback102(void);
extern void Cust_MacroPlayback103(void);
extern void Cust_MacroPlayback104(void);
extern void Cust_MacroPlayback105(void);
extern void Cust_MacroPlayback106(void);
extern void Cust_MacroPlayback107(void);
extern void Cust_MacroPlayback108(void);
extern void Cust_MacroPlayback109(void);
extern void Cust_MacroPlayback110(void);
extern void Cust_MacroPlayback111(void);
extern void Cust_MacroPlayback112(void);
extern void Cust_MacroPlayback113(void);
extern void Cust_MacroPlayback114(void);
extern void Cust_MacroPlayback115(void);
extern void Cust_MacroPlayback116(void);
extern void Cust_MacroPlayback117(void);
extern void Cust_MacroPlayback118(void);
extern void Cust_MacroPlayback119(void);
extern void Cust_MacroPlayback120(void);
extern void Cust_MacroPlayback121(void);
extern void Cust_MacroPlayback122(void);
extern void Cust_MacroPlayback123(void);
extern void Cust_MacroPlayback124(void);
extern void Cust_MacroPlayback125(void);
extern void Cust_MacroPlayback126(void);
extern void Cust_MacroPlayback127(void);
extern void Cust_MacroPlayback128(void);
extern void Cust_MacroPlayback129(void);
extern void Cust_MacroPlayback130(void);
extern void Cust_MacroPlayback131(void);
extern void Cust_MacroPlayback132(void);
extern void Cust_MacroPlayback133(void);
extern void Cust_MacroPlayback134(void);
extern void Cust_MacroPlayback135(void);
extern void Cust_MacroPlayback136(void);
extern void Cust_MacroPlayback137(void);
extern void Cust_MacroPlayback138(void);
extern void Cust_MacroPlayback139(void);
extern void Cust_MacroPlayback140(void);
extern void Cust_MacroPlayback141(void);
extern void Cust_MacroPlayback142(void);
extern void Cust_MacroPlayback143(void);
extern void Cust_MacroPlayback144(void);
extern void Cust_MacroPlayback145(void);
extern void Cust_MacroPlayback146(void);
extern void Cust_MacroPlayback147(void);
extern void Cust_MacroPlayback148(void);
extern void Cust_MacroPlayback149(void);
extern void Cust_MacroPlayback150(void);
extern void Cust_MacroPlayback151(void);
extern void Cust_MacroPlayback152(void);
extern void Cust_MacroPlayback153(void);
extern void Cust_MacroPlayback154(void);
extern void Cust_MacroPlayback155(void);
extern void Cust_MacroPlayback156(void);
extern void Cust_MacroPlayback157(void);
extern void Cust_MacroPlayback158(void);
extern void Cust_MacroPlayback159(void);
extern void Cust_MacroPlayback160(void);
extern void Cust_MacroPlayback161(void);
extern void Cust_MacroPlayback162(void);
extern void Cust_MacroPlayback163(void);
extern void Cust_MacroPlayback164(void);
extern void Cust_MacroPlayback165(void);
extern void Cust_MacroPlayback166(void);
extern void Cust_MacroPlayback167(void);
extern void Cust_MacroPlayback168(void);
extern void Cust_MacroPlayback169(void);
extern void Cust_MacroPlayback170(void);
extern void Cust_MacroPlayback171(void);
extern void Cust_MacroPlayback172(void);
extern void Cust_MacroPlayback173(void);
extern void Cust_MacroPlayback174(void);
extern void Cust_MacroPlayback175(void);
extern void Cust_MacroPlayback176(void);
extern void Cust_MacroPlayback177(void);
extern void Cust_MacroPlayback178(void);
extern void Cust_MacroPlayback179(void);
extern void Cust_MacroPlayback180(void);
extern void Cust_MacroPlayback181(void);
extern void Cust_MacroPlayback182(void);
extern void Cust_MacroPlayback183(void);
extern void Cust_MacroPlayback184(void);
extern void Cust_MacroPlayback185(void);
extern void Cust_MacroPlayback186(void);
extern void Cust_MacroPlayback187(void);
extern void Cust_MacroPlayback188(void);
extern void Cust_MacroPlayback189(void);
extern void Cust_MacroPlayback190(void);
extern void Cust_MacroPlayback191(void);
extern void Cust_MacroPlayback192(void);
extern void Cust_MacroPlayback193(void);
extern void Cust_MacroPlayback194(void);
extern void Cust_MacroPlayback195(void);
extern void Cust_MacroPlayback196(void);
extern void Cust_MacroPlayback197(void);
extern void Cust_MacroPlayback198(void);
extern void Cust_MacroPlayback199(void);
extern void Cust_MacroPlayback200(void);

struct funcStruct MacroFuncs[MAX_MACROS] = {
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback1, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback2, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback3, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback4, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback5, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback6, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback7, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback8, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback9, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback10, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback11, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback12, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback13, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback14, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback15, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback16, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback17, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback18, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback19, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback20, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback21, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback22, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback23, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback24, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback25, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback26, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback27, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback28, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback29, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback30, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback31, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback32, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback33, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback34, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback35, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback36, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback37, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback38, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback39, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback40, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback41, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback42, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback43, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback44, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback45, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback46, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback47, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback48, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback49, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback50, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback51, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback52, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback53, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback54, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback55, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback56, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback57, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback58, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback59, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback60, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback61, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback62, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback63, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback64, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback65, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback66, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback67, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback68, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback69, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback70, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback71, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback72, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback73, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback74, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback75, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback76, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback77, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback78, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback79, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback80, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback81, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback82, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback83, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback84, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback85, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback86, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback87, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback88, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback89, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback90, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback91, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback92, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback93, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback94, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback95, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback96, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback97, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback98, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback99, T_NULL, H_NULL},

    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback100, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback101, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback102, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback103, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback104, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback105, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback106, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback107, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback108, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback109, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback110, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback111, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback112, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback113, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback114, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback115, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback116, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback117, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback118, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback119, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback120, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback121, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback122, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback123, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback124, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback125, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback126, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback127, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback128, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback129, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback130, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback131, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback132, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback133, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback134, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback135, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback136, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback137, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback138, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback139, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback140, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback141, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback142, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback143, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback144, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback145, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback146, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback147, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback148, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback149, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback150, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback151, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback152, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback153, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback154, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback155, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback156, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback157, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback158, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback159, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback160, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback161, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback162, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback163, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback164, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback165, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback166, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback167, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback168, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback169, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback170, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback171, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback172, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback173, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback174, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback175, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback176, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback177, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback178, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback179, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback180, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback181, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback182, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback183, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback184, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback185, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback186, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback187, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback188, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback189, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback190, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback191, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback192, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback193, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback194, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback195, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback196, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback197, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback198, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback199, T_NULL, H_NULL},
    {FN1,  0, UNI_UNUSED, USES_F, ALLOWREC, ' ', "   ", NO_L, X_NULL,  Cust_MacroPlayback200, T_NULL, H_NULL}
};


struct funcStruct *customPullFuncs = (struct funcStruct *) &Sci_I_funcs;
int savedIndexFunc = 0;
struct customSaveStruct customSave[MAX_FUNCS];

extern BOOL CALLBACK CustomDefDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void cust_define(void)
{
    DLGPROC lpCustomDefDlgProc;

    lpCustomDefDlgProc = (DLGPROC) MakeProcInstance((FARPROC) CustomDefDlgProc, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_CUSTOMDEF", calcMainWindow, lpCustomDefDlgProc)) == -1)
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
    extern HINSTANCE hInst;
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
        customPullFuncs = (struct funcStruct *) &Sci_I_funcs;
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
            LoadString(hInst, customPullFuncs[index].keyTitle, tmp, sizeof(tmp) - 1);
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
                customSave[newIdx].func_idx = macroItem;
                Custom_funcs[newIdx].keyTitle = 9000 + macroItem;
            }
            else
            {
                memcpy(&Custom_funcs[newIdx], &customPullFuncs[index], sizeof(struct funcStruct));
                customSave[newIdx].custom_save_idx = savedIndexFunc;
                customSave[newIdx].func_idx = index;
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
                customPullFuncs = (struct funcStruct *) &Sci_I_funcs;
                processCustomFuncs(hDlg);
                break;
            case(191):        // Buisness funcs
                savedIndexFunc = CUSTOM_SAVE_BUS;
                customPullFuncs = (struct funcStruct *) &Business_funcs;
                processCustomFuncs(hDlg);
                break;
            case(192):        // Programming funcs
                savedIndexFunc = CUSTOM_SAVE_PRO;
                customPullFuncs = (struct funcStruct *) &Programming_funcs;
                processCustomFuncs(hDlg);
                break;
            case(193):        // Stats funcs
                savedIndexFunc = CUSTOM_SAVE_STA;
                customPullFuncs = (struct funcStruct *) &Stat_funcs;
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
                customPullFuncs = (struct funcStruct *) &Prog1_funcs;
                processCustomFuncs(hDlg);
                break;
            case(201):        // Program Bank II funcs
                savedIndexFunc = CUSTOM_SAVE_MACBANK2;
                customPullFuncs = (struct funcStruct *) &Prog2_funcs;
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

void Cust_MacroPlayback65(void)
{
    CustMacroPlayback(64);
}

void Cust_MacroPlayback66(void)
{
    CustMacroPlayback(65);
}

void Cust_MacroPlayback67(void)
{
    CustMacroPlayback(66);
}

void Cust_MacroPlayback68(void)
{
    CustMacroPlayback(67);
}

void Cust_MacroPlayback69(void)
{
    CustMacroPlayback(68);
}

void Cust_MacroPlayback70(void)
{
    CustMacroPlayback(69);
}

void Cust_MacroPlayback71(void)
{
    CustMacroPlayback(70);
}

void Cust_MacroPlayback72(void)
{
    CustMacroPlayback(71);
}

void Cust_MacroPlayback73(void)
{
    CustMacroPlayback(72);
}

void Cust_MacroPlayback74(void)
{
    CustMacroPlayback(73);
}

void Cust_MacroPlayback75(void)
{
    CustMacroPlayback(74);
}

void Cust_MacroPlayback76(void)
{
    CustMacroPlayback(75);
}

void Cust_MacroPlayback77(void)
{
    CustMacroPlayback(76);
}

void Cust_MacroPlayback78(void)
{
    CustMacroPlayback(77);
}

void Cust_MacroPlayback79(void)
{
    CustMacroPlayback(78);
}

void Cust_MacroPlayback80(void)
{
    CustMacroPlayback(79);
}

void Cust_MacroPlayback81(void)
{
    CustMacroPlayback(80);
}

void Cust_MacroPlayback82(void)
{
    CustMacroPlayback(81);
}

void Cust_MacroPlayback83(void)
{
    CustMacroPlayback(82);
}

void Cust_MacroPlayback84(void)
{
    CustMacroPlayback(83);
}

void Cust_MacroPlayback85(void)
{
    CustMacroPlayback(84);
}

void Cust_MacroPlayback86(void)
{
    CustMacroPlayback(85);
}

void Cust_MacroPlayback87(void)
{
    CustMacroPlayback(86);
}

void Cust_MacroPlayback88(void)
{
    CustMacroPlayback(87);
}

void Cust_MacroPlayback89(void)
{
    CustMacroPlayback(88);
}

void Cust_MacroPlayback90(void)
{
    CustMacroPlayback(89);
}

void Cust_MacroPlayback91(void)
{
    CustMacroPlayback(90);
}

void Cust_MacroPlayback92(void)
{
    CustMacroPlayback(91);
}

void Cust_MacroPlayback93(void)
{
    CustMacroPlayback(92);
}

void Cust_MacroPlayback94(void)
{
    CustMacroPlayback(93);
}

void Cust_MacroPlayback95(void)
{
    CustMacroPlayback(94);
}

void Cust_MacroPlayback96(void)
{
    CustMacroPlayback(95);
}

void Cust_MacroPlayback97(void)
{
    CustMacroPlayback(96);
}

void Cust_MacroPlayback98(void)
{
    CustMacroPlayback(97);
}

void Cust_MacroPlayback99(void)
{
    CustMacroPlayback(98);
}

void Cust_MacroPlayback100(void)
{
    CustMacroPlayback(99);
}

extern void Cust_MacroPlayback101(void)
{
    CustMacroPlayback(100);
}

extern void Cust_MacroPlayback102(void)
{
    CustMacroPlayback(101);
}

extern void Cust_MacroPlayback103(void)
{
    CustMacroPlayback(102);
}

extern void Cust_MacroPlayback104(void)
{
    CustMacroPlayback(103);
}

extern void Cust_MacroPlayback105(void)
{
    CustMacroPlayback(104);
}

extern void Cust_MacroPlayback106(void)
{
    CustMacroPlayback(105);
}

extern void Cust_MacroPlayback107(void)
{
    CustMacroPlayback(106);
}

extern void Cust_MacroPlayback108(void)
{
    CustMacroPlayback(107);
}

extern void Cust_MacroPlayback109(void)
{
    CustMacroPlayback(108);
}

extern void Cust_MacroPlayback110(void)
{
    CustMacroPlayback(109);
}

extern void Cust_MacroPlayback111(void)
{
    CustMacroPlayback(110);
}

extern void Cust_MacroPlayback112(void)
{
    CustMacroPlayback(111);
}

extern void Cust_MacroPlayback113(void)
{
    CustMacroPlayback(112);
}

extern void Cust_MacroPlayback114(void)
{
    CustMacroPlayback(113);
}

extern void Cust_MacroPlayback115(void)
{
    CustMacroPlayback(114);
}

extern void Cust_MacroPlayback116(void)
{
    CustMacroPlayback(115);
}

extern void Cust_MacroPlayback117(void)
{
    CustMacroPlayback(116);
}

extern void Cust_MacroPlayback118(void)
{
    CustMacroPlayback(117);
}

extern void Cust_MacroPlayback119(void)
{
    CustMacroPlayback(118);
}

extern void Cust_MacroPlayback120(void)
{
    CustMacroPlayback(119);
}

extern void Cust_MacroPlayback121(void)
{
    CustMacroPlayback(120);
}

extern void Cust_MacroPlayback122(void)
{
    CustMacroPlayback(121);
}

extern void Cust_MacroPlayback123(void)
{
    CustMacroPlayback(122);
}

extern void Cust_MacroPlayback124(void)
{
    CustMacroPlayback(123);
}

extern void Cust_MacroPlayback125(void)
{
    CustMacroPlayback(124);
}

extern void Cust_MacroPlayback126(void)
{
    CustMacroPlayback(125);
}

extern void Cust_MacroPlayback127(void)
{
    CustMacroPlayback(126);
}

extern void Cust_MacroPlayback128(void)
{
    CustMacroPlayback(127);
}

extern void Cust_MacroPlayback129(void)
{
    CustMacroPlayback(128);
}

extern void Cust_MacroPlayback130(void)
{
    CustMacroPlayback(129);
}

extern void Cust_MacroPlayback131(void)
{
    CustMacroPlayback(130);
}

extern void Cust_MacroPlayback132(void)
{
    CustMacroPlayback(131);
}

extern void Cust_MacroPlayback133(void)
{
    CustMacroPlayback(132);
}

extern void Cust_MacroPlayback134(void)
{
    CustMacroPlayback(133);
}

extern void Cust_MacroPlayback135(void)
{
    CustMacroPlayback(134);
}

extern void Cust_MacroPlayback136(void)
{
    CustMacroPlayback(135);
}

extern void Cust_MacroPlayback137(void)
{
    CustMacroPlayback(136);
}

extern void Cust_MacroPlayback138(void)
{
    CustMacroPlayback(137);
}

extern void Cust_MacroPlayback139(void)
{
    CustMacroPlayback(138);
}

extern void Cust_MacroPlayback140(void)
{
    CustMacroPlayback(139);
}

extern void Cust_MacroPlayback141(void)
{
    CustMacroPlayback(140);
}

extern void Cust_MacroPlayback142(void)
{
    CustMacroPlayback(141);
}

extern void Cust_MacroPlayback143(void)
{
    CustMacroPlayback(142);
}

extern void Cust_MacroPlayback144(void)
{
    CustMacroPlayback(143);
}

extern void Cust_MacroPlayback145(void)
{
    CustMacroPlayback(144);
}

extern void Cust_MacroPlayback146(void)
{
    CustMacroPlayback(145);
}

extern void Cust_MacroPlayback147(void)
{
    CustMacroPlayback(146);
}

extern void Cust_MacroPlayback148(void)
{
    CustMacroPlayback(147);
}

extern void Cust_MacroPlayback149(void)
{
    CustMacroPlayback(148);
}

extern void Cust_MacroPlayback150(void)
{
    CustMacroPlayback(149);
}

extern void Cust_MacroPlayback151(void)
{
    CustMacroPlayback(150);
}

extern void Cust_MacroPlayback152(void)
{
    CustMacroPlayback(151);
}

extern void Cust_MacroPlayback153(void)
{
    CustMacroPlayback(152);
}

extern void Cust_MacroPlayback154(void)
{
    CustMacroPlayback(153);
}

extern void Cust_MacroPlayback155(void)
{
    CustMacroPlayback(154);
}

extern void Cust_MacroPlayback156(void)
{
    CustMacroPlayback(155);
}

extern void Cust_MacroPlayback157(void)
{
    CustMacroPlayback(156);
}

extern void Cust_MacroPlayback158(void)
{
    CustMacroPlayback(157);
}

extern void Cust_MacroPlayback159(void)
{
    CustMacroPlayback(158);
}

extern void Cust_MacroPlayback160(void)
{
    CustMacroPlayback(159);
}

extern void Cust_MacroPlayback161(void)
{
    CustMacroPlayback(160);
}

extern void Cust_MacroPlayback162(void)
{
    CustMacroPlayback(161);
}

extern void Cust_MacroPlayback163(void)
{
    CustMacroPlayback(162);
}

extern void Cust_MacroPlayback164(void)
{
    CustMacroPlayback(163);
}

extern void Cust_MacroPlayback165(void)
{
    CustMacroPlayback(164);
}

extern void Cust_MacroPlayback166(void)
{
    CustMacroPlayback(165);
}

extern void Cust_MacroPlayback167(void)
{
    CustMacroPlayback(166);
}

extern void Cust_MacroPlayback168(void)
{
    CustMacroPlayback(167);
}

extern void Cust_MacroPlayback169(void)
{
    CustMacroPlayback(168);
}

extern void Cust_MacroPlayback170(void)
{
    CustMacroPlayback(169);
}

extern void Cust_MacroPlayback171(void)
{
    CustMacroPlayback(170);
}

extern void Cust_MacroPlayback172(void)
{
    CustMacroPlayback(171);
}

extern void Cust_MacroPlayback173(void)
{
    CustMacroPlayback(172);
}

extern void Cust_MacroPlayback174(void)
{
    CustMacroPlayback(173);
}

extern void Cust_MacroPlayback175(void)
{
    CustMacroPlayback(174);
}

extern void Cust_MacroPlayback176(void)
{
    CustMacroPlayback(175);
}

extern void Cust_MacroPlayback177(void)
{
    CustMacroPlayback(176);
}

extern void Cust_MacroPlayback178(void)
{
    CustMacroPlayback(177);
}

extern void Cust_MacroPlayback179(void)
{
    CustMacroPlayback(178);
}

extern void Cust_MacroPlayback180(void)
{
    CustMacroPlayback(179);
}

extern void Cust_MacroPlayback181(void)
{
    CustMacroPlayback(180);
}

extern void Cust_MacroPlayback182(void)
{
    CustMacroPlayback(181);
}

extern void Cust_MacroPlayback183(void)
{
    CustMacroPlayback(182);
}

extern void Cust_MacroPlayback184(void)
{
    CustMacroPlayback(183);
}

extern void Cust_MacroPlayback185(void)
{
    CustMacroPlayback(184);
}

extern void Cust_MacroPlayback186(void)
{
    CustMacroPlayback(185);
}

extern void Cust_MacroPlayback187(void)
{
    CustMacroPlayback(186);
}

extern void Cust_MacroPlayback188(void)
{
    CustMacroPlayback(187);
}

extern void Cust_MacroPlayback189(void)
{
    CustMacroPlayback(188);
}

extern void Cust_MacroPlayback190(void)
{
    CustMacroPlayback(189);
}

extern void Cust_MacroPlayback191(void)
{
    CustMacroPlayback(190);
}

extern void Cust_MacroPlayback192(void)
{
    CustMacroPlayback(191);
}

extern void Cust_MacroPlayback193(void)
{
    CustMacroPlayback(192);
}

extern void Cust_MacroPlayback194(void)
{
    CustMacroPlayback(193);
}

extern void Cust_MacroPlayback195(void)
{
    CustMacroPlayback(194);
}

extern void Cust_MacroPlayback196(void)
{
    CustMacroPlayback(195);
}

extern void Cust_MacroPlayback197(void)
{
    CustMacroPlayback(196);
}

extern void Cust_MacroPlayback198(void)
{
    CustMacroPlayback(197);
}

extern void Cust_MacroPlayback199(void)
{
    CustMacroPlayback(198);
}

extern void Cust_MacroPlayback200(void)
{
    CustMacroPlayback(199);
}

