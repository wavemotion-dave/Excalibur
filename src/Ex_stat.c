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

extern void STAT_SdevX(void);
extern void STAT_PdevX(void);
extern void STAT_SdevY(void);
extern void STAT_PdevY(void);
extern void STAT_sumPlus(void);
extern void STAT_sumMinus(void);
extern void STAT_sumCl(void);
extern void STAT_aveX(void);
extern void STAT_aveY(void);
extern void STAT_sumX(void);
extern void STAT_sumY(void);
extern void STAT_sumXsq(void);
extern void STAT_sumYsq(void);
extern void STAT_sumXY(void);
extern void STAT_n(void);
extern void STAT_r(void);
extern void STAT_b(void);
extern void STAT_m(void);
extern void STAT_xGuess(void);
extern void STAT_yGuess(void);

struct funcStruct Stat_funcs[MAX_FUNCS] = {
    {FN1,  0, UNI_SUMPLUS,  USES_F, ALLOWREC, ' ',   "SUM+",  YES_L,   X_ENTER, STAT_sumPlus,  T_SUMP,    H_SUMP},
    {FN2,  0, UNI_SUMMINUS, USES_F, ALLOWREC, ' ',   "SUM-",  YES_L,   X_ENTER, STAT_sumMinus, T_SUMM,    H_SUMM},
    {FN3,  0, UNI_SUMCLR,   USES_F, ALLOWREC, ' ',   "SCLR",  YES_L,   X_NULL,  STAT_sumCl,    T_SUMC,    H_SUMC},
    {FN4,  0, UNI_STATN,    USES_F, ALLOWREC, ' ',   " N ",   YES_L,   X_NEW,   STAT_n,        T_SUMN,    H_SUMN},
    {FN5,  0, UNI_STATSDX,  USES_F, ALLOWREC, ' ',   "SDX",   YES_L,   X_NEW,   STAT_SdevX,    T_SUMSDX,  H_SUMSDX},
    {FN6,  0, UNI_STATPDX,  USES_F, ALLOWREC, ' ',   "PDX",   YES_L,   X_NEW,   STAT_PdevX,    T_SUMPDX,  H_SUMPDX},
    {FN7,  0, UNI_STATSDY,  USES_F, ALLOWREC, ' ',   "SDY",   YES_L,   X_NEW,   STAT_SdevY,    T_SUMSDX,  H_SUMSDY},
    {FN8,  0, UNI_STATPDY,  USES_F, ALLOWREC, ' ',   "PDY",   YES_L,   X_NEW,   STAT_PdevY,    T_SUMPDX,  H_SUMPDY},
    {FN9,  0, UNI_STATAVEX, USES_F, ALLOWREC, ' ',   "AVG X", YES_L,   X_NEW,   STAT_aveX,     T_MEANX,   H_MEANX},
    {FN10, 0, UNI_STATAVEY, USES_F, ALLOWREC, ' ',   "AVG Y", YES_L,   X_NEW,   STAT_aveY,     T_MEANY,   H_MEANY},
    {FN11, 0, UNI_SUMX,     USES_F, ALLOWREC, ' ',   "SUM X", YES_L,   X_NEW,   STAT_sumX,     T_SUMX,    H_SUMX},
    {FN12, 0, UNI_SUMY,     USES_F, ALLOWREC, ' ',   "SUM Y", YES_L,   X_NEW,   STAT_sumY,     T_SUMY,    H_SUMY},
    {FN13, 0, UNI_SUMXX,    USES_F, ALLOWREC, ' ',   "SUMXX", YES_L,   X_NEW,   STAT_sumXsq,   T_SUMXX,   H_SUMXX},
    {FN14, 0, UNI_SUMYY,    USES_F, ALLOWREC, ' ',   "SUMYY", YES_L,   X_NEW,   STAT_sumYsq,   T_SUMYY,   H_SUMYY},
    {FN15, 0, UNI_SUMXY,    USES_F, ALLOWREC, ' ',   "CORR",  YES_L,   X_NEW,   STAT_sumXY,    T_SUMXY,   H_SUMXY},
    {FN16, 0, UNI_CORR,     USES_F, ALLOWREC, ' ',   "r",     YES_L,   X_NEW,   STAT_r,        T_CORR,    H_CORR},
    {FN17, 0, UNI_YINT,     USES_F, ALLOWREC, ' ',   "b",     YES_L,   X_NEW,   STAT_b,        T_YINT,    H_YINT},
    {FN18, 0, UNI_SLOPE,    USES_F, ALLOWREC, ' ',   "m",     YES_L,   X_NEW,   STAT_m,        T_SLOPE,   H_SLOPE},
    {FN19, 0, UNI_YGUESS,   USES_F, ALLOWREC, ' ',   "y",     YES_L,   X_NEW,   STAT_yGuess,   T_ESTY,    H_ESTY},
    {FN20, 0, UNI_XGUESS,   USES_F, ALLOWREC, ' ',   "x",     YES_L,   X_NEW,   STAT_xGuess,   T_ESTX,    H_ESTX},
    {FN21, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN22, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN23, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN24, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN25, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN26, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN27, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN28, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN29, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN30, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN31, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN32, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN33, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN34, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN35, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN36, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN37, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN38, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN39, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL},
    {FN40, 0, UNI_UNUSED,   USES_F, ALLOWREC, ' ',   " ",     YES_L,   X_NEW,   NULL,          T_NULL,    H_NULL}
};

/* ------------- */
/* STAT functions */
/* ------------- */
void STAT_SdevX(void)
{
    double temp;

    if (SUM[SUM_N] == 0.0 || (SUM[SUM_N] - 1.0) == 0.0)
    {
        RPN_error("STAT:  Statistical Divide By Zero");
    }
    else
    {
        temp = ((SUM[SUM_XSQ] - ((SUM[SUM_X] * SUM[SUM_X]) / SUM[SUM_N])) / (SUM[SUM_N] - 1.0));
        temp = sqrt(fabs(temp));
        StackPush(temp);
    }
}

void STAT_PdevX(void)
{
    double temp;

    if (SUM[SUM_N] == 0.0 || (SUM[SUM_N] - 1.0) == 0.0)
    {
        RPN_error("STAT:  Statistical Divide By Zero");
    }
    else
    {
        temp = ((SUM[SUM_XSQ] - ((SUM[SUM_X] * SUM[SUM_X]) / SUM[SUM_N])) / (SUM[SUM_N] - 1.0));
        temp = sqrt(fabs(temp));
        temp = temp * (double) sqrt((double) (SUM[SUM_N] - 1.0) / (double) SUM[SUM_N]);
        StackPush(temp);
    }
}

void STAT_SdevY(void)
{
    double temp;

    if (SUM[SUM_N] == 0.0 || (SUM[SUM_N] - 1.0) == 0.0)
    {
        RPN_error("STAT:  Statistical Divide By Zero");
    }
    else
    {
        temp = ((SUM[SUM_YSQ] - ((SUM[SUM_Y] * SUM[SUM_Y]) / SUM[SUM_N])) / (SUM[SUM_N] - 1.0));
        temp = sqrt(fabs(temp));
        StackPush(temp);
    }
}

void STAT_PdevY(void)
{
    double temp;

    if (SUM[SUM_N] == 0.0 || (SUM[SUM_N] - 1.0) == 0.0)
    {
        RPN_error("STAT:  Statistical Divide By Zero");
    }
    else
    {
        temp = ((SUM[SUM_YSQ] - ((SUM[SUM_Y] * SUM[SUM_Y]) / SUM[SUM_N])) / (SUM[SUM_N] - 1.0));
        temp = sqrt(fabs(temp));
        temp = temp * (double) sqrt((double) (SUM[SUM_N] - 1.0) / (double) SUM[SUM_N]);
        StackPush(temp);
    }
}

void STAT_sumPlus(void)
{
    SUM[SUM_N] += 1.0F;
    SUM[SUM_X] += X;
    SUM[SUM_Y] += Y;
    SUM[SUM_XSQ] += X * X;
    SUM[SUM_YSQ] += Y * Y;
    SUM[SUM_XY] += X * Y;
    StackPop();
    StackPush(SUM[SUM_N]);
}

void STAT_sumMinus(void)
{
    SUM[SUM_N] -= 1.0F;
    SUM[SUM_X] -= X;
    SUM[SUM_Y] -= Y;
    SUM[SUM_XSQ] -= X * X;
    SUM[SUM_YSQ] -= Y * Y;
    SUM[SUM_XY] -= X * Y;
    StackPop();
    StackPush(SUM[SUM_N]);
}

void STAT_sumCl(void)
{
    SUM[SUM_N] = 0.0F;
    SUM[SUM_X] = 0.0F;
    SUM[SUM_Y] = 0.0F;
    SUM[SUM_XSQ] = 0.0F;
    SUM[SUM_YSQ] = 0.0F;
    SUM[SUM_XY] = 0.0F;
}

void STAT_aveX(void)
{
    if (SUM[SUM_N] == 0.0)
    {
        RPN_error("STAT:  Statistical Divide By Zero");
    }
    else
    {
        StackPush(SUM[SUM_X] / SUM[SUM_N]);
    }
}

void STAT_aveY(void)
{
    if (SUM[SUM_N] == 0.0)
    {
        RPN_error("STAT:  Statistical Divide By Zero");
    }
    else
    {
        StackPush(SUM[SUM_Y] / SUM[SUM_N]);
    }
}

void STAT_sumX(void)
{
    StackPush(SUM[SUM_X]);
}

void STAT_sumY(void)
{
    StackPush(SUM[SUM_Y]);
}

void STAT_sumXsq(void)
{
    StackPush(SUM[SUM_XSQ]);
}

void STAT_sumYsq(void)
{
    StackPush(SUM[SUM_YSQ]);
}

void STAT_sumXY(void)
{
    StackPush(SUM[SUM_XY]);
}

void STAT_n(void)
{
    StackPush(SUM[SUM_N]);
}

void STAT_r(void)
{
    double temp1, temp2, temp3;

    temp1 = (SUM[SUM_N] * SUM[SUM_XY]) - (SUM[SUM_X] * SUM[SUM_Y]);
    temp3 = (SUM[SUM_N] * SUM[SUM_YSQ] - (SUM[SUM_Y] * SUM[SUM_Y]));
    if (temp3 < 0.0)
    {
        RPN_error("STAT: Square Root Error");
    }
    else
    {
        temp2 = sqrt((SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X])) * sqrt(temp3);
        if (temp2 == 0.0)
            RPN_error("STAT: Square Root Error");
        else
            StackPush(temp1 / temp2);
    }
}

void STAT_b(void)
{
    double temp1, temp2;
    temp1 = (SUM[SUM_Y] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_XY]);
    temp2 = (SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X]);
    StackPush(temp1 / temp2);
}

void STAT_m(void)
{
    double temp1, temp2;

    temp1 = (SUM[SUM_N] * SUM[SUM_XY]) - (SUM[SUM_X] * SUM[SUM_Y]);
    temp2 = (SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X]);
    StackPush(temp1 / temp2);

}

void STAT_yGuess(void)
{
    double temp1, temp2, m, b;

    temp1 = (SUM[SUM_Y] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_XY]);
    temp2 = (SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X]);
    b = temp1 / temp2;

    temp1 = (SUM[SUM_N] * SUM[SUM_XY]) - (SUM[SUM_X] * SUM[SUM_Y]);
    temp2 = (SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X]);
    m = temp1 / temp2;
    StackPush(StackPop() * m + b);
}

void STAT_xGuess(void)
{
    double temp1, temp2, m, b;

    temp1 = (SUM[SUM_Y] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_XY]);
    temp2 = (SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X]);
    b = temp1 / temp2;

    temp1 = (SUM[SUM_N] * SUM[SUM_XY]) - (SUM[SUM_X] * SUM[SUM_Y]);
    temp2 = (SUM[SUM_N] * SUM[SUM_XSQ]) - (SUM[SUM_X] * SUM[SUM_X]);
    m = temp1 / temp2;
    StackPush((StackPop() - b) / m);
}
