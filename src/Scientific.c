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
#include <float.h>
#include "EXCAL.h"

uint8_t  userTimer = 0;
uint32_t userTicks = 0;

extern void SCI_sin(void);
extern void SCI_cos(void);
extern void SCI_tan(void);
extern void SCI_square(void);
extern void SCI_asin(void);
extern void SCI_acos(void);
extern void SCI_atan(void);
extern void SCI_sqrt(void);
extern void SCI_inverse(void);
extern void SCI_exp(void);
extern void SCI_ln(void);
extern void SCI_log(void);
extern void SCI_xfact(void);
extern void SCI_y_to_x(void);
extern void SCI_pi(void);
extern void SCI_sinh(void);
extern void SCI_cosh(void);
extern void SCI_tanh(void);
extern void SCI_asinh(void);
extern void SCI_acosh(void);
extern void SCI_atanh(void);
extern void SCI_10x(void);
extern void SCI_rand(void);
extern void SCI_int(void);
extern void SCI_frac(void);
extern void SCI_hyp(void);
extern void SCI_PdevX(void);
extern void SCI_PdevY(void);
extern void SCI_sumPlus(void);
extern void SCI_sumMinus(void);
extern void SCI_sumCl(void);
extern void SCI_aveX(void);
extern void SCI_aveY(void);
extern void SCI_sumX(void);
extern void SCI_sumY(void);
extern void SCI_sumXsq(void);
extern void SCI_sumYsq(void);
extern void SCI_sumXY(void);
extern void SCI_n(void);
extern void SCI_Cnr(void);
extern void SCI_Pnr(void);
extern void SCI_abs(void);
extern void SCI_round(void);
extern void SCI_floor(void);
extern void SCI_Ceil(void);
extern void SCI_GCD(void);
extern void SCI_LCM(void);
extern void SCI_MinR(void);
extern void SCI_MaxR(void);
extern void SCI_RoundYX(void);
extern void SCI_prime(void);
extern void SCI_elements(void);
extern void SCI_resist(void);
extern void SCI_metricPre(void);
extern void SCI_LogBase2(void);
extern void SCI_TimerStart(void);
extern void SCI_TimerStop(void);
extern void SCI_TimerClear(void);
extern void SCI_TimerPush(void);
extern void SCI_Astro(void);

struct funcStruct Scientific_funcs[MAX_FUNCS] = {
    {FN1,   UNI_SIN,    USES_F,     ALLOWREC,   ' ',    "SIN",      YES_L,  X_NEW,  SCI_sin,        T_SIN,        H_SIN},
    {FN2,   UNI_COS,    USES_F,     ALLOWREC,   ' ',    "COS",      YES_L,  X_NEW,  SCI_cos,        T_COS,        H_COS},
    {FN3,   UNI_TAN,    USES_F,     ALLOWREC,   ' ',    "TAN",      YES_L,  X_NEW,  SCI_tan,        T_TAN,        H_TAN},
    {FN4,   UNI_HYP,    USES_F,     ALLOWREC,   ' ',    "HYP",      NO_L,   X_NULL, SCI_hyp,        T_HYP,        H_HYP},
    {FN5,   UNI_ASIN,   USES_F,     ALLOWREC,   ' ',    "ASIN",     YES_L,  X_NEW,  SCI_asin,       T_ASIN,       H_ASIN},
    {FN6,   UNI_ACOS,   USES_F,     ALLOWREC,   ' ',    "ACOS",     YES_L,  X_NEW,  SCI_acos,       T_ACOS,       H_ACOS},
    {FN7,   UNI_ATAN,   USES_F,     ALLOWREC,   ' ',    "ATAN",     YES_L,  X_NEW,  SCI_atan,       T_ATAN,       H_ATAN},
    {FN8,   UNI_ABS,    USES_F,     ALLOWREC,   ' ',    "ABS",      YES_L,  X_NEW,  SCI_abs,        T_ABS,        H_ABS},
    {FN9,   UNI_XX,     USES_F,     ALLOWREC,   ' ',    " X² ",     YES_L,  X_NEW,  SCI_square,     T_XX,         H_XX},
    {FN10,  UNI_SQRT,   USES_F,     ALLOWREC,   ' ',    "SQRT",     YES_L,  X_NEW,  SCI_sqrt,       T_SQRT,       H_SQRT},
    {FN11,  UNI_INV,    USES_F,     ALLOWREC,   ' ',    "1/X",      YES_L,  X_NEW,  SCI_inverse,    T_INV,        H_INV},
    {FN12,  UNI_POW10,  USES_F,     ALLOWREC,   ' ',    "10^X",     YES_L,  X_NEW,  SCI_10x,        T_POW10,      H_POW10},
    {FN13,  UNI_EXP,    USES_F,     ALLOWREC,   ' ',    "e^X",      YES_L,  X_NEW,  SCI_exp,        T_EXP,        H_EXP},
    {FN14,  UNI_LN,     USES_F,     ALLOWREC,   ' ',    " LN ",     YES_L,  X_NEW,  SCI_ln,         T_LN,         H_LN},
    {FN15,  UNI_LOG,    USES_F,     ALLOWREC,   ' ',    "LOG",      YES_L,  X_NEW,  SCI_log,        T_LOG,        H_LOG},
    {FN16,  UNI_FACT,   USES_F,     ALLOWREC,   ' ',    " X! ",     YES_L,  X_NEW,  SCI_xfact,      T_FACT,       H_FACT},
    {FN17,  UNI_POW,    USES_F,     ALLOWREC,   ' ',    "Y^X",      YES_L,  X_NEW,  SCI_y_to_x,     T_POW,        H_POW},
    {FN18,  UNI_INT,    USES_F,     ALLOWREC,   ' ',    "INT",      YES_L,  X_NEW,  SCI_int,        T_INT,        H_INT},
    {FN19,  UNI_FRACT,  USES_F,     ALLOWREC,   ' ',    "FRAC",     YES_L,  X_NEW,  SCI_frac,       T_FRAC,       H_FRAC},
    {FN20,  UNI_RAND,   USES_F,     ALLOWREC,   ' ',    "RAND",     YES_L,  X_NEW,  SCI_rand,       T_RAND,       H_RAND},
    {FN21,  UNI_COMB,   USES_F,     ALLOWREC,   ' ',    "Cn,r",     YES_L,  X_NEW,  SCI_Cnr,        T_COMB,       H_COMB},
    {FN22,  UNI_PERM,   USES_F,     ALLOWREC,   ' ',    "Pn,r",     YES_L,  X_NEW,  SCI_Pnr,        T_PERM,       H_PERM},
    {FN23,  UNI_PI,     USES_F,     ALLOWREC,   ' ',    " PI ",     YES_L,  X_NEW,  SCI_pi,         T_PI,         H_PI},
    {FN24,  UNI_LOG2,   USES_F,     ALLOWREC,   ' ',    "Log2",     YES_L,  X_NEW,  SCI_LogBase2,   T_LOG2,       H_LOG2},
    {FN25,  UNI_ROUND,  USES_F,     ALLOWREC,   ' ',    "Round",    YES_L,  X_NEW,  SCI_round,      T_ROUND,      H_ROUND},
    {FN26,  UNI_FLOOR,  USES_F,     ALLOWREC,   ' ',    "Floor",    YES_L,  X_NEW,  SCI_floor,      T_FLOOR,      H_FLOOR},
    {FN27,  UNI_CEIL,   USES_F,     ALLOWREC,   ' ',    "Ceil",     YES_L,  X_NEW,  SCI_Ceil,       T_CEIL,       H_CEIL},
    {FN28,  UNI_RNDXY,  USES_F,     ALLOWREC,   ' ',    "RndYX",    YES_L,  X_NEW,  SCI_RoundYX,    T_ROUNDYX,    H_ROUNDYX},
    {FN29,  UNI_GCD,    USES_F,     ALLOWREC,   ' ',    "GCD",      YES_L,  X_NEW,  SCI_GCD,        T_GCD,        H_GCD},
    {FN30,  UNI_LCM,    USES_F,     ALLOWREC,   ' ',    "LCM",      YES_L,  X_NEW,  SCI_LCM,        T_LCM,        H_LCM},
    {FN31,  UNI_MINR,   USES_F,     ALLOWREC,   ' ',    "MinR",     YES_L,  X_NEW,  SCI_MinR,       T_MINR,       H_MINR},
    {FN32,  UNI_MAXR,   USES_F,     ALLOWREC,   ' ',    "MaxR",     YES_L,  X_NEW,  SCI_MaxR,       T_MAXR,       H_MAXR},
    {FN33,  UNI_PRIME,  USES_F,     NORECORD,   ' ',    "Prime#",   YES_L,  X_NEW,  SCI_prime,      T_PRIME,      H_PRIME},
    {FN34,  UNI_RESIST, USES_F,     ALLOWREC,   ' ',    "Resist",   YES_L,  X_NEW,  SCI_resist,     T_RESIST,     H_RESIST},
    {FN35,  UNI_ELEMENT,USES_F,     NORECORD,   ' ',    "Elmnts",   YES_L,  X_NEW,  SCI_elements,   T_ELEMENT,    H_ELEMENT},
    {FN36,  UNI_ASTRO,  USES_F,     NORECORD,   ' ',    "Astro",    YES_L,  X_NEW,  SCI_Astro,      T_ASTRO,      H_ASTRO},
    {FN37,  UNI_TSTART, USES_F,     ALLOWREC,   ' ',    "T Start",  YES_L,  X_NEW,  SCI_TimerStart, T_TIMERSTART, H_TIMERSTART},
    {FN38,  UNI_TSTOP,  USES_F,     ALLOWREC,   ' ',    "T Stop",   YES_L,  X_NEW,  SCI_TimerStop,  T_TIMERSTOP,  H_TIMERSTOP},
    {FN39,  UNI_TCLR,   USES_F,     ALLOWREC,   ' ',    "T Clear",  YES_L,  X_NEW,  SCI_TimerClear, T_TIMERCLEAR, H_TIMERCLEAR},
    {FN40,  UNI_TPUSH,  USES_F,     ALLOWREC,   ' ',    "T Push",   YES_L,  X_NEW,  SCI_TimerPush,  T_TIMERPUSH,  H_TIMERPUSH}
};

/* ------------- */
/* SCI functions */
/* ------------- */
uint8_t hyperbolic = 0;

void SCI_hyp(void)
{
    hyperbolic ^= 1;
    if (hyperbolic == 0)
        UpdateSpareBar(" ");
    else
        UpdateSpareBar("HYP");
}

void SCI_sin(void)
{
    if (hyperbolic)
        SCI_sinh();
    else
        StackPush(sin(ToRadians(StackPop())));
}

void SCI_cos(void)
{
    if (hyperbolic)
        SCI_cosh();
    else
        StackPush(cos(ToRadians(StackPop())));
}

void SCI_tan(void)
{
    if (hyperbolic)
        SCI_tanh();
    else
        StackPush(tan(ToRadians(StackPop())));
}

void SCI_square(void)
{
    double temp;

    temp = StackPop();
    StackPush(temp * temp);
}

void SCI_asin(void)
{
    if (hyperbolic)
        SCI_asinh();
    else
    {
        if (X < -1.0 || X > 1.0)
            RPN_error("ASIN:  X is out of range");
        else
            StackPush(FromRadians(asin(StackPop())));
    }
}
void SCI_acos(void)
{
    if (hyperbolic)
        SCI_acosh();
    else
    {
        if (X < -1.0 || X > 1.0)
            RPN_error("ACOS:  X is out of range");
        else
            StackPush(FromRadians(acos(StackPop())));
    }
}
void SCI_atan(void)
{
    if (hyperbolic)
        SCI_atanh();
    else
        StackPush(FromRadians(atan(StackPop())));
}

void SCI_sinh(void)
{
    StackPush(sinh(StackPop()));
}

void SCI_cosh(void)
{
    StackPush(cosh(StackPop()));
}

void SCI_tanh(void)
{
    StackPush(tanh(StackPop()));
}

void SCI_asinh(void)
{
    double val;

    val = StackPop();
    StackPush(log(val + sqrt(val * val + 1)));
}

void SCI_acosh(void)
{
    double val;

    if (X < 1.0)
        RPN_error("ACOSH: X Must Be Greater Than or Equal To 1.0");
    else
    {
        val = StackPop();
        StackPush(log(val + sqrt(val * val - 1)));
    }
}

void SCI_atanh(void)
{
    double val;

    if (X * X >= 1.0)
        RPN_error("ATANH: X*X Must Be Less Than 1.0");
    else
    {
        val = StackPop();
        StackPush(0.5 * log((1 + val) / (1 - val)));
    }
}

void SCI_10x(void)
{
    StackPush(pow(10, StackPop()));
}


void SCI_sqrt(void)
{

    if (X < 0.0)
    {
        RPN_error("Square Root of Negative Number");
    }
    else
    {
        StackPush(sqrt(StackPop()));
    }
}
void SCI_inverse(void)
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
void SCI_exp(void)
{
    StackPush(exp(StackPop()));
}

void SCI_ln(void)
{
    if (X <= 0.0)
        RPN_error("LN:  X must be positive");
    else
        StackPush(log(StackPop()));
}

void SCI_log(void)
{
    if (X <= 0.0)
        RPN_error("LOG:  X must be positive");
    else
        StackPush(log10(StackPop()));
}

void SCI_xfact(void)
{
    int fact;
    double temp;

    if (X >= 0.0 && X <= 170.0)
    {
        temp = 1.0;
        for (fact = (int) X; fact > 0; fact--)
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

void SCI_y_to_x(void)
{
    double xtemp, ytemp;

    xtemp = StackPop();
    ytemp = StackPop();
    if (ytemp == 0.0 && xtemp < 0.0)
        RPN_error("Power:  Y=0 and X < 0");
    else
        StackPush(pow(ytemp, xtemp));
}

void SCI_rand(void)
{
    unsigned int randVal;

    randVal = (rand() << 15) + rand();
    randVal = randVal % 10000000;

    if (Xedit == X_ENTER)
        X = ((float) randVal / 10000000.0);
    else
        StackPush((float) randVal / 10000000.0);

}

void SCI_pi(void)
{
    if (Xedit == X_ENTER)
        X = M_PI;
    else
        StackPush(M_PI);
}

void SCI_frac(void)
{
    double temp;
    double temp2;
    temp = modf(MakeAccurate(StackPop()), &temp2);
    StackPush(temp);
}

void SCI_int(void)
{
    double temp;
    modf(MakeAccurate(StackPop()), &temp);
    StackPush(temp);
}


double Factorial(double val)
{
    double temp;
    int fact;

    if (val >= 0.0 && val <= 100.0)
    {
        temp = 1.0;
        for (fact = (int) val; fact > 0; fact--)
        {
            temp = (float) temp *(float) fact;
        }
        return(temp);
    }
    else
    {
        return(-1.0);
    }
}

void SCI_Cnr(void)
{
    unsigned int i, n, r;
    double cnr = 1.0;

    r = (unsigned int) StackPop();
    n = (unsigned int) StackPop();

    if (n >= r) // We must have at least 1 combination!
    {
        if (r*2 > n)
        {
            r = n-r;
        }
        for (i=1; i <= r; n--, i++)
        {
            cnr = (cnr * n) / i;
        }
        StackPush((double)cnr);
    }
    else
    {
        StackPush(0.0);
    }
}

void SCI_Pnr(void)
{
    double N, R, c;
    int i, j, k;

    R = StackPop();
    N = StackPop();

    i = (int) R;
    j = (int) N;

    if (j >= i)
    {
        c = 1.0;
        for (k = j; k > j - i; k--)
        {
            c = c * (double) k;
        }
        StackPush(c);
    }
    else
        StackPush(0.0);
}


void SCI_abs(void)
{
    StackPush(fabs(StackPop()));
}


void SCI_round(void)
{
    double temp;
    temp = floor(MakeAccurate(StackPop()) + 0.5);
    StackPush(temp);
}

void SCI_floor(void)
{
    double temp;
    temp = floor(MakeAccurate(StackPop()));
    StackPush(temp);
}

void SCI_Ceil(void)
{
    double temp;
    temp = ceil(MakeAccurate(StackPop()));
    StackPush(temp);
}

void SCI_GCD(void)
{
    long l1, l2, t1, t2;
    long try_value;

    l1 = (long) X;
    l2 = (long) Y;

    if (X < 0.0 || Y < 0.0)
    {
        RPN_error("GCD:  X,Y values should be greater than or equal to zero.");
        return;
    }

    /* Get smallest non-zero of the 4 */
    try_value = max(l1, l2);
    if (l1 != 0L)
        try_value = min(try_value, l1);
    if (l2 != 0L)
        try_value = min(try_value, l2);

    t1 = t2 = 0L;
    while (try_value > 1)
    {
        if (l1 != 0L)
            t1 = l1 % try_value;
        if (l2 != 0L)
            t2 = l2 % try_value;
        if (t1 == 0L && t2 == 0L)
            break;
        try_value--;
    }
    StackPush((double) try_value);
}

void SCI_LCM(void)
{
    long l1, l2, t1, t2;
    long try_value;
    long most_tries;
    long orig_value;

    if (X < 0.0 || Y < 0.0)
    {
        RPN_error("LCM:  X,Y values should be greater than or equal to zero.");
        return;
    }

    l1 = (long) X;
    l2 = (long) Y;

    /* Get biggest of the 4 */
    try_value = max(l1, l2);

    if (try_value != 0)
        most_tries = (MAXLONG / try_value) - 2;
    else
        return;                 // tbd error ?!?

    orig_value = try_value;
    t1 = t2 = 0L;
    while (--most_tries)
    {
        if (l1 != 0L)
            t1 = try_value % l1;
        if (l2 != 0L)
            t2 = try_value % l2;
        if (t1 == 0L && t2 == 0L)
            break;
        try_value += orig_value;
    }

    if (most_tries == 0)
        StackPush((double) -1.0);
    else
        StackPush((double) try_value);
}

void SCI_MinR(void)
{
    StackPush(DBL_MIN);
}

void SCI_MaxR(void)
{
    StackPush(DBL_MAX);
}


void SCI_RoundYX(void)
{
    long mult;
    double temp;

    mult = (long) pow(10, StackPop());

    if (mult < 10)
        mult = 1;

    temp = StackPop();
    temp = temp * (double) mult;
    temp = floor(MakeAccurate(temp) + 0.5);
    temp = MakeAccurate(temp / (double) mult);
    StackPush(temp);
}

extern BOOL CALLBACK fnDIALOG_PrimesProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void SCI_prime(void)
{
    DLGPROC lpfnDIALOG_PrimesProc;

    lpfnDIALOG_PrimesProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_PrimesProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_PRIMES", calcMainWindow, lpfnDIALOG_PrimesProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_PrimesProc);
}

BOOL CALLBACK fnDIALOG_PrimesProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i, j, prime, iprime;
    char tmp[32];
    LRESULT item;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) "     2 "));
        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) "     3 "));
        for (i = 3; i < 100000; i += 2)
        {
            prime = 1;
            iprime = i;
            for (j = 3; j <= iprime; j += 2)
                if ((i % j) == 0)
                {
                    prime = 0;
                    break;
                }
                else
                {
                    iprime = i / j;
                }
            if (prime == 1)
            {
                sprintf(tmp, "  %4d ", i);
                SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
            }
        }
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(101):            // double click?!?
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;

        case(102):
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                SendDlgItemMessage(hDlg, 101, LB_GETTEXT, item, (LPARAM) ((LPSTR) tmp));
                PushConstant(atof(tmp));
                Xedit = X_NEW;
                EndDialog(hDlg, FALSE);
            }
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


extern BOOL CALLBACK fnDIALOG_ElementsProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void SCI_elements(void)
{
    DLGPROC lpfnDIALOG_ElementsProc;

    lpfnDIALOG_ElementsProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_ElementsProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_ELEMENTS", calcMainWindow, lpfnDIALOG_ElementsProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_ElementsProc);
}

struct elementTableStruct
{
    char name[20];
    int number;
    double weight;
};
struct elementTableStruct elementTable[] = {
    {"Actinium(Ac)",       89,      227.0278},
    {"Aluminum(Al)",       13,      26.98154},
    {"Americium(Am)",      95,      243.0},
    {"Antimony(Sb)",       51,      121.75},
    {"Argon(Ar)",          18,      39.948},
    {"Arsenic(As)",        33,      74.9216},
    {"Astatine(At)",       85,      210.0},
    {"Barium(Ba)",         56,      137.33},
    {"Berkelium(Bk)",      97,      247.0},
    {"Beryllium(Be)",      4,       9.01218},
    {"Bismuth(Bi)",        83,      208.9804},
    {"Boron(B) ",          5,       10.81},
    {"Bromine(Br)",        35,      79.904},
    {"Cadmium(Cd)",        48,      112.41},
    {"Calcium(Ca)",        20,      40.08},
    {"Californium(Cf)",    98,      251.0},
    {"Carbon(C) ",         6,       12.011},
    {"Cerium(Ce)",         58,      140.12},
    {"Cesium(Cs)",         55,      132.9054},
    {"Chlorine(Cl)",       17,      35.453},
    {"Chromium(Cr)",       24,      51.996},
    {"Cobalt(Co)",         27,      58.9332},
    {"Copper(Cu)",         29,      63.546},
    {"Curium(Cm)",         96,      247.0},
    {"Dysprosium(Dy)",     66,      162.50},
    {"Einsteinium(Es)",    99,      254.0},
    {"Erbium(Er)",         68,      167.26},
    {"Europium(Eu)",       63,      151.96},
    {"Fermium(Fm)",        100,      257.0},
    {"Flourine(F) ",       9,       18.998403},
    {"Francium(Fr)",       87,      223},
    {"Gadolinium(Gd)",     64,      157.25},
    {"Gallium(Ga)",        31,      69.72},
    {"Germanium(Ge)",      32,      72.59},
    {"Gold(Au)",           79,      196.9665},
    {"Hafnium(Hf)",        72,      178.49},
    {"Helium(He)",         2,       4.00260},
    {"Holmium(Ho)",        67,      164.9304},
    {"Hydrogen(H) ",       1,       1.0079},
    {"Indium(In)",         49,      114.82},
    {"Iodine(I) ",         53,      126.9045},
    {"Iridium(Ir)",        77,      192.22},
    {"Iron(Fe)",           26,      55.847},
    {"Krypton(Kr)",        36,      83.80},
    {"Lanthanum(La)",      57,      138.905},
    {"Lawrencium(Lr)",     103,     0.00},
    {"Lead(Pb)",           82,      207.2},
    {"Lithium(Li)",        3,       6.941},
    {"Lutetium(Lu)",       71,      174.967},
    {"Magnesium(Mg)",      12,      24.305},
    {"Manganese(Mn)",      25,      54.9380},
    {"Mendelevium(Md)",    101,     256.0},
    {"Mercury(Hg)",        80,      200.59},
    {"Molybdenum(Mo)",     42,      95.94},
    {"Neodymium(Nd)",      60,      144.24},
    {"Neon(Ne)",           10,      20.17},
    {"Neptunium(Np)",      93,      237.048},
    {"Nickel(Ni)",         28,      58.71},
    {"Niobium(Nb)",        41,      92.9064},
    {"Nitrogen(N) ",       7,       14.0067},
    {"Nobelium(No)",       102,     254},
    {"Osmium(Os)",         76,      190.2},
    {"Oxygen(O) ",         8,       15.9994},
    {"Palladium(Pd)",      46,      106.42},
    {"Phosphorus(P) ",     15,      30.97376},
    {"Platinum(Pt)",       78,      195.09},
    {"Plutonium(Pu)",      94,      244.0},
    {"Polonium(Po)",       84,      209.0},
    {"Potassium(K) ",      19,      39.102},
    {"Praseodymium(Pr)",   59,      140.907},
    {"Promethium(Pm)",     61,      145.0},
    {"Protactinium(Pa)",   91,      231.035},
    {"Radium(Ra)",         88,      226.0254},
    {"Radon(Rn)",          86,      222.0},
    {"Rhenium(Re)",        75,      186.2},
    {"Rhodium(Rh)",        45,      102.9055},
    {"Rubidium(Rb)",       37,      85.467},
    {"Ruthenium(Ru)",      44,      101.07},
    {"Samarium(Sm)",       62,      150.4},
    {"Scandium(Sc)",       21,      44.9559},
    {"Selenium(Se)",       34,      78.96},
    {"Silicon(Si)",        14,      28.086},
    {"Silver(Ag)",         47,      107.868},
    {"Sodium(Na)",         11,      22.9898},
    {"Strontium(Sr)",      38,      87.62},
    {"Sulfur(S) ",         16,      32.06},
    {"Tantalum(Ta)",       73,      180.947},
    {"Technetium(Tc)",     43,      98.9062},
    {"Tellurium(Te)",      52,      127.60},
    {"Terbium(Tb)",        65,      158.925},
    {"Thallium(Tl)",       81,      204.37},
    {"Thorium(Th)",        90,      232.038},
    {"Thulium(Tm)",        69,      169.934},
    {"Tin(Sn)",            50,      118.69},
    {"Titanium(Ti)",       22,      47.90},
    {"Tungsten(W) ",       74,      183.85},
    {"Uranium(U) ",        92,      238.0289},
    {"Vanadium(V) ",       23,      50.9415},
    {"Xenon(Xe)",          54,      131.29},
    {"Ytterbium(Yb)",      70,      173.04},
    {"Yttrium(Y) ",        39,      88.9059},
    {"Zinc(Zn)",           30,      65.38},
    {"Zirconium(Zr)",      40,      91.22},
    {"None ",              -1,      0.00}
};

BOOL CALLBACK fnDIALOG_ElementsProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    char tmp[40];
    LRESULT item;
    static int lbTabStops[2] = { 60, 100 };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), LB_SETTABSTOPS, 2, (DWORD) lbTabStops);
        i = 0;
        for (;;)
        {
            sprintf(tmp, "%s\t%d\t%-8.4f", elementTable[i].name, elementTable[i].number, elementTable[i].weight);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
            i++;
            if (elementTable[i].number == -1)
                break;
        }
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(101):            // double click?!?
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;
        case(102):            /* Insert atomic number */
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                PushConstant((double) elementTable[item].number);
                EndDialog(hDlg, FALSE);
            }
            return TRUE;
        case(103):            /* Insert atomic mass */
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                PushConstant((double) elementTable[item].weight);
                EndDialog(hDlg, FALSE);
            }
            return TRUE;
        case(104):            /* Cancel */
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

extern BOOL CALLBACK fnDIALOG_ResistorProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void SCI_resist(void)
{

    DLGPROC lpfnDIALOG_ResistorProc;

    lpfnDIALOG_ResistorProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_ResistorProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_RESISTOR", calcMainWindow, lpfnDIALOG_ResistorProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_ResistorProc);
}


BOOL CALLBACK fnDIALOG_ResistorProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    static int res_val_1st_band  = 0;
    static int res_val_2nd_band  = 0;
    static int res_val_3rd_band  = 0;
    static int res_val_mult_band = 0;
    static double res_tolerance  = 0.05;
    static double res_val        = 0.0;
    char tmp[50];
    int i;

    char color_list[10][4] = {
        "BLK",
        "BRN",
        "RED",
        "ORG",
        "YEL",
        "GRN",
        "BLU",
        "PUR",
        "GRY",
        "WHT"
    };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        for (i = 101; i <= 183; i++)
            SendMessage(GetDlgItem(hDlg, i), WM_SETFONT, (WPARAM) hMainFont, FALSE);
        sprintf(tmp, "%0.0f ohms", 0.0);
        SendDlgItemMessage(hDlg, 150, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        sprintf(tmp, "%0.0f ohms", 0.0);
        SendDlgItemMessage(hDlg, 152, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        sprintf(tmp, "%0.0f ohms", 0.0);
        SendDlgItemMessage(hDlg, 153, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        sprintf(tmp, "%0.0f%%", 0.0);
        SendDlgItemMessage(hDlg, 151, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        SendDlgItemMessage(hDlg, 170, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[0]));
        SendDlgItemMessage(hDlg, 171, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[0]));
        SendDlgItemMessage(hDlg, 172, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[0]));
        SendDlgItemMessage(hDlg, 173, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[0]));
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case 180:
            StackPush(res_val);
            EndDialog(hDlg, FALSE);
            return TRUE;
        case 181:
            StackPush(res_val * (1.0 - res_tolerance));
            EndDialog(hDlg, FALSE);
            return TRUE;
        case 182:
            StackPush(res_val * (1.0 + res_tolerance));
            EndDialog(hDlg, FALSE);
            return TRUE;
        case 183:
            EndDialog(hDlg, FALSE);
            return TRUE;

        case 101:
        case 105:
        case 109:
        case 113:
        case 117:
        case 121:
        case 125:
        case 129:
        case 133:
        case 137:
            res_val_1st_band = (wParam - 101) / 4;
            break;
        case 102:
        case 106:
        case 110:
        case 114:
        case 118:
        case 122:
        case 126:
        case 130:
        case 134:
        case 138:
            res_val_2nd_band = (wParam - 102) / 4;
            break;
        case 103:
        case 107:
        case 111:
        case 115:
        case 119:
        case 123:
        case 127:
        case 131:
        case 135:
        case 139:
            res_val_3rd_band = (wParam - 103) / 4;
            break;
        case 104:
        case 108:
        case 112:
        case 116:
        case 120:
        case 124:
        case 128:
        case 132:
        case 136:
        case 140:
            res_val_mult_band = (wParam - 104) / 4;
            break;
        case 141:
            res_tolerance = 0.10;
            break;
        case 142:
            res_tolerance = 0.05;
            break;
        case 143:
            res_tolerance = 0.01;
            break;

        default:
            return FALSE;
        }
        res_val = (res_val_3rd_band + res_val_2nd_band * 10 + res_val_1st_band * 100) * pow(10.0, res_val_mult_band);
        sprintf(tmp, "%g ohms", res_val);
        SendDlgItemMessage(hDlg, 150, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        sprintf(tmp, "%g ohms", res_val * (1.0 - res_tolerance));
        SendDlgItemMessage(hDlg, 152, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        sprintf(tmp, "%g ohms", res_val * (1.0 + res_tolerance));
        SendDlgItemMessage(hDlg, 153, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));
        sprintf(tmp, "%g%%", res_tolerance * 100);
        SendDlgItemMessage(hDlg, 151, WM_SETTEXT, 0, (LONG) ((LPSTR) tmp));

        SendDlgItemMessage(hDlg, 170, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[res_val_1st_band]));
        SendDlgItemMessage(hDlg, 171, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[res_val_2nd_band]));
        SendDlgItemMessage(hDlg, STATUS_BAR, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[res_val_3rd_band]));
        SendDlgItemMessage(hDlg, 173, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[res_val_mult_band]));

        return TRUE;

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

extern BOOL CALLBACK MetricPrefixDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void SCI_metricPre(void)
{
    DLGPROC lpMetricPrefixDlgProc;

    lpMetricPrefixDlgProc = (DLGPROC) MakeProcInstance((FARPROC) MetricPrefixDlgProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_METRICPRE", calcMainWindow, lpMetricPrefixDlgProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpMetricPrefixDlgProc);
}

struct MetricPrefixStruct
{
    char name[60];
    double value;
};
struct MetricPrefixStruct MetricPrefixTable[] = {
    {"Peta    1.0e15    1,000,000,000,000,000(quadrillion)  ",  1.0e15},
    {"Tera    1.0e12    1,000,000,000,000(trillion)     ",      1.0e12},
    {"Giga    1.0e9     1,000,000,000(billion)      ",          1.0e9},
    {"Mega    1.0e6     1,000,000(million)      ",              1.0e6},
    {"Kilo    1.0e3     1,000(thousand)     ",                  1.0e3},
    {"Hecto   1.0e2     100(hundred)      ",                    1.0e2},
    {"Deka    1.0e1     10(ten)          ",                     1.0e1},
    {"Deci    1.0e-1    0.1(tenth)        ",                    1.0e-1},
    {"Centi   1.0e-2    0.01(hundredth)    ",                   1.0e-2},
    {"Milli   1.0e-3    0.001(thousandth)   ",                  1.0e-3},
    {"Micro   1.0e-6    0.000001(millionth)    ",               1.0e-6},
    {"Nano    1.0e-9    0.000000001(billionth)    ",            1.0e-9},
    {"Pico    1.0e-12   0.000000000001(trillionth)   ",         1.0e-12},
    {"Femto   1.0e-15   0.000000000000001(quadrillionth)",      1.0e-15},
    {"None                                                  ",  0.00}
};

BOOL CALLBACK MetricPrefixDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    LRESULT item;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        i = 0;
        for (;;)
        {

            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) MetricPrefixTable[i].name));
            i++;
            if (MetricPrefixTable[i].value == 0.0)
                break;
        }
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(101):            // double click?!?
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;
        case(102):            /* Insert value */
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                StackPush((double) MetricPrefixTable[item].value);
                EndDialog(hDlg, FALSE);
            }
            return TRUE;
        case(103):            /* Cancel */
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


void SCI_LogBase2(void)
{
    double temp;

    if (X <= 0.0)
    {
        RPN_error("LOG:  X must be positive");
    }
    else
    {
        temp = StackPop();
        temp = log10(temp) / log10(2.0);
        StackPush(temp);
    }
}

void SCI_TimerStart(void)
{
    userTimer = 1;
}

void SCI_TimerStop(void)
{
    userTimer = 2;
}

void SCI_TimerClear(void)
{
    userTicks = 0;
    userTimer = 0;
    UpdateTimeBar();
}

void SCI_TimerPush(void)
{
    unsigned long ut;
    char tmp[21];

    ut = userTicks / 100;
    sprintf(tmp, "%d.%d", (ut/10), (ut%10));
    StackPush(atof(tmp));
}



extern void SCI_Moles(void);
extern void SCI_Molecules(void);
extern void SCI_GasVol(void);              // in liters
extern void SCI_OhmsE(void);               // Current in Y, Resistance in X
extern void SCI_OhmsI(void);               // Voltage in Y, Resistance in X
extern void SCI_OhmsR(void);               // Voltage in Y, Current in X
extern void SCI_Gravitation(void);         // In newtons
extern void SCI_Energy(void);              // in joules
extern void SCI_CentripetalForce(void);    // In newtons
extern void SCI_CentripetalAccel(void);    // in(m/sec^2)
extern void SCI_AccelerationDist(void);    // in meters
extern void SCI_ProjectileRange(void);     // In meters
extern void SCI_ProjectileHeight(void);    // In meters
extern void SCI_Astro(void);

extern void COMP_plus(void);
extern void COMP_minus(void);
extern void COMP_div(void);
extern void COMP_mult(void);
extern void COMP_sin(void);
extern void COMP_cos(void);
extern void COMP_tan(void);
extern void COMP_conj(void);
extern void COMP_exp(void);
extern void COMP_ln(void);
extern void COMP_log(void);
extern void COMP_pow(void);
extern void COMP_sqrt(void);
extern void COMP_abs(void);
extern void COMP_CHS(void);
extern void COMP_norm(void);
extern void COMP_arg(void);
extern void COMP_ToPolar(void);
extern void COMP_FromPolar(void);
extern void COMP_hsin(void);
extern void COMP_hcos(void);
extern void COMP_htan(void);

struct funcStruct Scientific2_funcs[MAX_FUNCS] = {
    {FN1,   UNI_MOLES,      USES_F,     ALLOWREC,   ' ',    "Moles",    YES_L,      X_NEW,      SCI_Moles,             T_MOLES,        H_MOLES},
    {FN2,   UNI_MOLEC,      USES_F,     ALLOWREC,   ' ',    "Molec",    YES_L,      X_NEW,      SCI_Molecules,         T_MOLECULES,    H_MOLECULES},
    {FN3,   UNI_GASVOL,     USES_F,     ALLOWREC,   ' ',    "Gas Vol",  YES_L,      X_NEW,      SCI_GasVol,            T_GASVOL,       H_GASVOL},
    {FN4,   UNI_OHME,       USES_F,     ALLOWREC,   ' ',    "Ohm's E",  YES_L,      X_NEW,      SCI_OhmsE,             T_OHMSE,        H_OHMSE},
    {FN5,   UNI_OHMI,       USES_F,     ALLOWREC,   ' ',    "Ohm's I",  YES_L,      X_NEW,      SCI_OhmsI,             T_OHMSI,        H_OHMSI},
    {FN6,   UNI_OHMR,       USES_F,     ALLOWREC,   ' ',    "Ohm's R",  YES_L,      X_NEW,      SCI_OhmsR,             T_OHMSR,        H_OHMSR},
    {FN7,   UNI_GRAV,       USES_F,     ALLOWREC,   ' ',    "Grav",     YES_L,      X_NEW,      SCI_Gravitation,       T_GRAVITATION,  H_GRAVITATION},
    {FN8,   UNI_ENERGY,     USES_F,     ALLOWREC,   ' ',    "Energy",   YES_L,      X_NEW,      SCI_Energy,            T_ENERGY,       H_ENERGY},
    {FN9,   UNI_CENTF,      USES_F,     ALLOWREC,   ' ',    "Cent F",   YES_L,      X_NEW,      SCI_CentripetalForce,  T_CENTFORCE,    H_CENTFORCE},
    {FN10,  UNI_CENTA,      USES_F,     ALLOWREC,   ' ',    "Cent A",   YES_L,      X_NEW,      SCI_CentripetalAccel,  T_CENTACCEL,    H_CENTACCEL},
    {FN11,  UNI_ACELDIST,   USES_F,     ALLOWREC,   ' ',    "Acc D",    YES_L,      X_NEW,      SCI_AccelerationDist,  T_ACCELDIST,    H_ACCELDIST},
    {FN12,  UNI_PROJRANGE,  USES_F,     ALLOWREC,   ' ',    "Proj R",   YES_L,      X_NEW,      SCI_ProjectileRange,   T_PROJRANGE,    H_PROJRANGE},
    {FN13,  UNI_PROJHEIGHT, USES_F,     ALLOWREC,   ' ',    "Proj H",   YES_L,      X_NEW,      SCI_ProjectileHeight,  T_PROJHEIGHT,   H_PROJHEIGHT},
    {FN14,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN15,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_ATLAS}, // hidden
    {FN16,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN17,  UNI_CPLUS,      USES_F,     ALLOWREC,   ' ',    "Cpx +",    YES_L,      X_NEW,      COMP_plus,              T_CPLXADD,      H_CPLXADD},
    {FN18,  UNI_CMINUS,     USES_F,     ALLOWREC,   ' ',    "Cpx --",   YES_L,      X_NEW,      COMP_minus,             T_CPLXSUB,      H_CPLXSUB},
    {FN19,  UNI_CDIV,       USES_F,     ALLOWREC,   ' ',    "Cpx ÷",    YES_L,      X_NEW,      COMP_div,               T_CPLXDIV,      H_CPLXDIV},
    {FN20,  UNI_CMUL,       USES_F,     ALLOWREC,   ' ',    "Cpx ×",    YES_L,      X_NEW,      COMP_mult,              T_CPLXMUL,      H_CPLXMUL},
    {FN21,  UNI_CSIN,       USES_F,     ALLOWREC,   ' ',    "cSIN",     YES_L,      X_NEW,      COMP_sin,               T_CPLXSIN,      H_CPLXSIN},
    {FN22,  UNI_CCOS,       USES_F,     ALLOWREC,   ' ',    "cCOS",     YES_L,      X_NEW,      COMP_cos,               T_CPLXCOS,      H_CPLXCOS},
    {FN23,  UNI_CTAN,       USES_F,     ALLOWREC,   ' ',    "cTAN",     YES_L,      X_NEW,      COMP_tan,               T_CPLXTAN,      H_CPLXTAN},
    {FN24,  UNI_TOPOLAR,    USES_F,     ALLOWREC,   ' ',    "»POLR",    YES_L,      X_NEW,      COMP_ToPolar,           T_TOPOLAR,      H_TOPOLAR},
    {FN25,  UNI_CSINH,      USES_F,     ALLOWREC,   ' ',    "cSINH",    YES_L,      X_NEW,      COMP_hsin,              T_CSINH,        H_CSINH},
    {FN26,  UNI_CCOSH,      USES_F,     ALLOWREC,   ' ',    "cCOSH",    YES_L,      X_NEW,      COMP_hcos,              T_CCOSH,        H_CCOSH},
    {FN27,  UNI_CTANH,      USES_F,     ALLOWREC,   ' ',    "cTANH",    YES_L,      X_NEW,      COMP_htan,              T_CTANH,        H_CTANH},
    {FN28,  UNI_FROMPOLAR,  USES_F,     ALLOWREC,   ' ',    "«POLR",    YES_L,      X_NEW,      COMP_FromPolar,         T_FROMPOLAR,    H_FROMPOLAR},
    {FN29,  UNI_CCHS,       USES_F,     ALLOWREC,   ' ',    "cCHS",     YES_L,      X_NEW,      COMP_CHS,               T_CPLXCHS,      H_CPLXCHS},
    {FN30,  UNI_CCONJ,      USES_F,     ALLOWREC,   ' ',    "cCONJ",    YES_L,      X_NEW,      COMP_conj,              T_CONJ,         H_CONJ},
    {FN31,  UNI_CEXP,       USES_F,     ALLOWREC,   ' ',    "cEXP",     YES_L,      X_NEW,      COMP_exp,               T_CPLXEXP,      H_CPLXEXP},
    {FN32,  UNI_CLN,        USES_F,     ALLOWREC,   ' ',    "cLN",      YES_L,      X_NEW,      COMP_ln,                T_CPLXLN,       H_CPLXLN},
    {FN33,  UNI_CLOG,       USES_F,     ALLOWREC,   ' ',    "cLOG",     YES_L,      X_NEW,      COMP_log,               T_CPLXLOG,      H_CPLXLOG},
    {FN34,  UNI_CPOW,       USES_F,     ALLOWREC,   ' ',    "cPOW",     YES_L,      X_NEW,      COMP_pow,               T_CPLXPOW,      H_CPLXPOW},
    {FN35,  UNI_CSQRT,      USES_F,     ALLOWREC,   ' ',    "cSQRT",    YES_L,      X_NEW,      COMP_sqrt,              T_CPLXSQRT,     H_CPLXSQRT},
    {FN36,  UNI_CABS,       USES_F,     ALLOWREC,   ' ',    "cABS",     YES_L,      X_NEW,      COMP_abs,               T_CPLXABS,      H_CPLXABS},
    {FN37,  UNI_CNORM,      USES_F,     ALLOWREC,   ' ',    "cNORM",    YES_L,      X_NEW,      COMP_norm,              T_CNORM,        H_CNORM},
    {FN38,  UNI_CARG,       USES_F,     ALLOWREC,   ' ',    "cARG",     YES_L,      X_NEW,      COMP_arg,               T_CARG,         H_CARG},
    {FN39,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN40,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
};

void SCI_Moles(void)
{
    double x, y;
    x = StackPop();
    y = StackPop();
    if (x == 0.0)
        StackPush(0.0);
    else
        StackPush(y / x);
}

void SCI_Molecules(void)
{
    StackPush(StackPop() * 6.022045E+23);
}

void SCI_GasVol(void)
{
    StackPush(StackPop() * 22.414);
}

void SCI_OhmsE(void)          // Current in Y, Resistance in X
{
    StackPush(StackPop() * StackPop());
}

void SCI_OhmsI(void)          // Voltage in Y, Resistance in X
{
    double x, y;

    x = StackPop();
    y = StackPop();
    if (x != 0.0)
        StackPush(y / x);
    else
        StackPush(0.0);
}

void SCI_OhmsR(void)          // Voltage in Y, Current in X
{
    double x, y;

    x = StackPop();
    y = StackPop();
    if (x != 0.0)
        StackPush(y / x);
    else
        StackPush(0.0);
}

void SCI_Gravitation(void)
{
    double F, R, m1, m2;

    R = StackPop();
    m1 = StackPop();
    m2 = StackPop();
    if (R != 0.0)
    {
        F = (6.6732e-11) * ((m1 * m2) / R);
        StackPush(F);
    }
    else
        StackPush(0.0);
}

void SCI_Energy(void)
{
    StackPush(StackPop() * (299792458.0 * 299792458.0));
}

void SCI_CentripetalForce(void)       // In newtons
{
    // mass in X, liner speed in Y, radius in Z
    double x, y, z, F;

    x = StackPop();
    y = StackPop();
    z = StackPop();
    if (z != 0.0)
        F = (x * (y * y)) / z;
    else
        F = 0.0;
    StackPush(F);
}

void SCI_CentripetalAccel(void)        // in(m/sec^2)
{
    // mass in speed in X, radius in Y
    double x, y, A;

    x = StackPop();
    y = StackPop();
    if (y != 0.0)
        A = (x * x) / y;
    else
        A = 0.0;
    StackPush(A);
}

void SCI_AccelerationDist(void)   // in meters
{
    double a;                   //acceleration in m/s^2
    double t;                   //time in seconds
    double d;

    t = StackPop();
    a = StackPop();
    d = 0.5 * a * (t * t);
    StackPush(d);
}


void SCI_ProjectileRange(void)        // in meters
{
    double V;
    double angle;

    angle = StackPop();
    V = StackPop();
    StackPush(((V * V) / 9.80665) * sin(ToRadians(2.0 * angle)));
}

void SCI_ProjectileHeight(void)       // in meters
{
    double V;
    double angle;

    angle = StackPop();
    V = StackPop();
    StackPush((V * V * sin(ToRadians(angle)) * sin(ToRadians(angle))) / (2.0 * 9.80665));
}

struct AstroPrefixStruct
{
    char name[60];
    double value;
};
struct AstroPrefixStruct astroTableMercury[] = {
    {"Semimajor Axis(AU)", 0.3871},
    {"Semimajor Axis(km)", 5.79e7},
    {"Sidereal  Period(tropical years)", 0.2508},
    {"Sidereal  Period(days)", 87.97},
    {"Synodic   Period(days)", 115.88},
    {"Mean Orbital Speed(km/s)", 47.9},
    {"Orbital Eccentricity", 0.206},
    {"Inclination to Ecliptic", 7.00},
    {"Equatorial Diameter(km)", 4878.0},
    {"Mass(kg)", 3.3e23},
    {"Mean Density(g/cm^3)", 5.42},
    {"Rotation Period(days)", 58.646},
    {"Surface Gravity(1=Earth)", 0.38},
    {"Albedo", 0.106},
    {"Escape Velocity(km/s)", 4.3},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableVenus[] = {
    {"Semimajor Axis(AU)", 0.7233},
    {"Semimajor Axis(km)", 108.2e6},
    {"Sidereal  Period(tropical years)", 0.6152},
    {"Sidereal  Period(days)", 224.70},
    {"Synodic   Period(days)", 583.96},
    {"Mean Orbital Speed(km/s)", 35.0},
    {"Orbital Eccentricity", 0.007},
    {"Inclination to Ecliptic", 3.39},
    {"Equatorial Diameter(km)", 12104.0},
    {"Mass(kg)", 4.87e24},
    {"Mean Density(g/cm^3)", 5.24},
    {"Rotation Period(days)", -243.0},
    {"Surface Gravity(1=Earth)", 0.91},
    {"Albedo", 0.65},
    {"Escape Velocity(km/s)", 10.4},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableEarth[] = {
    {"Semimajor Axis(AU)", 1.00},
    {"Semimajor Axis(km)", 149.6e6},
    {"Sidereal  Period(tropical years)", 1.00},
    {"Sidereal  Period(days)", 365.26},
    {"Mean Orbital Speed(km/s)", 29.8},
    {"Orbital Eccentricity", 0.017},
    {"Inclination to Ecliptic", 0.0},
    {"Equatorial Diameter(km)", 12756.0},
    {"Mass(kg)", 5.976e24},
    {"Mean Density(g/cm^3)", 5.50},
    {"Rotation Period(days)", 0.997},
    {"Surface Gravity(1=Earth)", 1.00},
    {"Albedo", 0.39},
    {"Escape Velocity(km/s)", 11.2},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableMars[] = {
    {"Semimajor Axis(AU)", 1.5237},
    {"Semimajor Axis(km)", 227.9e6},
    {"Sidereal  Period(tropical years)", 1.8809},
    {"Sidereal  Period(days)", 686.98},
    {"Synodic   Period(days)", 779.94},
    {"Mean Orbital Speed(km/s)", 24.1},
    {"Orbital Eccentricity", 0.093},
    {"Inclination to Ecliptic", 1.85},
    {"Equatorial Diameter(km)", 6794.0},
    {"Mass(kg)", 6.42e23},
    {"Mean Density(g/cm^3)", 3.94},
    {"Rotation Period(days)", 1.026},
    {"Surface Gravity(1=Earth)", 0.38},
    {"Albedo", 0.15},
    {"Escape Velocity(km/s)", 5.0},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableJupiter[] = {
    {"Semimajor Axis(AU)", 5.2028},
    {"Semimajor Axis(km)", 778e6},
    {"Sidereal  Period(tropical years)", 11.86},
    {"Sidereal  Period(days)", 11.86 * 365.26},
    {"Synodic   Period(days)", 399},
    {"Mean Orbital Speed(km/s)", 13.1},
    {"Orbital Eccentricity", 0.048},
    {"Inclination to Ecliptic", 1.30},
    {"Equatorial Diameter(km)", 142800.0},
    {"Mass(kg)", 1.90e27},
    {"Mean Density(g/cm^3)", 1.3},
    {"Rotation Period(days)", 0.41},
    {"Surface Gravity(1=Earth)", 2.53},
    {"Albedo", 0.52},
    {"Escape Velocity(km/s)", 60.0},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableSaturn[] = {
    {"Semimajor Axis(AU)", 9.529},
    {"Semimajor Axis(km)", 1426e6},
    {"Sidereal  Period(tropical years)", 29.41},
    {"Sidereal  Period(days)", 29.41 * 365.26},
    {"Synodic   Period(days)", 378.0},
    {"Mean Orbital Speed(km/s)", 9.6},
    {"Orbital Eccentricity", 0.056},
    {"Inclination to Ecliptic", 2.49},
    {"Equatorial Diameter(km)", 120000.0},
    {"Mass(kg)", 5.69e26},
    {"Mean Density(g/cm^3)", 0.7},
    {"Rotation Period(days)", 0.43},
    {"Surface Gravity(1=Earth)", 1.07},
    {"Albedo", 0.76},
    {"Escape Velocity(km/s)", 36.0},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableUranus[] = {
    {"Semimajor Axis(AU)", 19.192},
    {"Semimajor Axis(km)", 2871e6},
    {"Sidereal  Period(tropical years)", 84.04},
    {"Sidereal  Period(days)", 84.04 * 365.26},
    {"Synodic   Period(days)", 370.0},
    {"Mean Orbital Speed(km/s)", 6.8},
    {"Orbital Eccentricity", 0.046},
    {"Inclination to Ecliptic", 0.77},
    {"Equatorial Diameter(km)", 51120.0},
    {"Mass(kg)", 8.70e25},
    {"Mean Density(g/cm^3)", 1.3},
    {"Rotation Period(days)", -0.65},
    {"Surface Gravity(1=Earth)", 0.92},
    {"Albedo", 0.51},
    {"Escape Velocity(km/s)", 21.0},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableNeptune[] = {
    {"Semimajor Axis(AU)", 30.061},
    {"Semimajor Axis(km)", 4497e6},
    {"Sidereal  Period(tropical years)", 164.79},
    {"Sidereal  Period(days)", 164.79 * 365.26},
    {"Synodic   Period(days)", 367.0},
    {"Mean Orbital Speed(km/s)", 5.4},
    {"Orbital Eccentricity", 0.010},
    {"Inclination to Ecliptic", 1.77},
    {"Equatorial Diameter(km)", 49528.0},
    {"Mass(kg)", 1.03e26},
    {"Mean Density(g/cm^3)", 1.7},
    {"Rotation Period(days)", 0.67},
    {"Surface Gravity(1=Earth)", 1.12},
    {"Albedo", 0.35},
    {"Escape Velocity(km/s)", 24.0},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTablePluto[] = {
    {"Semimajor Axis(AU)", 39.529},
    {"Semimajor Axis(km)", 5914e6},
    {"Sidereal  Period(tropical years)", 248.6},
    {"Sidereal  Period(days)", 248.6 * 365.26},
    {"Synodic   Period(days)", 367},
    {"Mean Orbital Speed(km/s)", 4.7},
    {"Orbital Eccentricity", 0.248},
    {"Inclination to Ecliptic", 17.15},
    {"Equatorial Diameter(km)", 2290.0},
    {"Mass(kg)", 1.0e22},
    {"Mean Density(g/cm^3)", 2.0},
    {"Rotation Period(days)", 6.387},
    {"Surface Gravity(1=Earth)", 0.06},
    {"Albedo", 0.40},
    {"Escape Velocity(km/s)", 1.0},
    {"***", -1.0F}
};
struct AstroPrefixStruct astroTableMisc[] = {
    {"Moon Distance from Earth(km)", 384400.0},
    {"Moon Sidereal Period(days)", 27.322},
    {"Moon Orbital Eccentricity", 0.055},
    {"Moon Diameter(km)", 3476.0},
    {"Solar Radius(m)", 6.960e8},
    {"Solar Mass(kg)", 1.989e30},
    {"Solar Luminosity(W)", 3.90e26},
    {"Light Year(m)", 9.460e15},
    {"Light Year(AU)", 63240.0},
    {"Parsec(m)", 3.086e16},
    {"Astronomical Unit(m)", 1.496e11},
    {"***", -1.0F}
};

struct AstroPrefixStruct *currentAstroTable;
void setAstroContent(HWND hDlg, struct AstroPrefixStruct astroTable[])
{
    int i;
    char tmp[80];

    currentAstroTable = astroTable;
    SendMessage(GetDlgItem(hDlg, 101), LB_RESETCONTENT, 0, 0);
    i = 0;
    for (;;)
    {
        sprintf(tmp, "%s\t%-14.12g", astroTable[i].name, astroTable[i].value);
        makeInternational(tmp);        // To swap commas and DPs if needed
        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        i++;
        if (astroTable[i].value == -1.0F)
            break;
    }
}

BOOL CALLBACK AstroDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    LRESULT item;
    static int lbTabStops[1] = { 150 };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), LB_SETTABSTOPS, 1, (DWORD) lbTabStops);
        SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        setAstroContent(hDlg, astroTableMercury);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(IDC_RADIO1):
            setAstroContent(hDlg, astroTableMercury);
            break;
        case(IDC_RADIO2):
            setAstroContent(hDlg, astroTableVenus);
            break;
        case(IDC_RADIO3):
            setAstroContent(hDlg, astroTableEarth);
            break;
        case(IDC_RADIO4):
            setAstroContent(hDlg, astroTableMars);
            break;
        case(IDC_RADIO5):
            setAstroContent(hDlg, astroTableJupiter);
            break;
        case(IDC_RADIO6):
            setAstroContent(hDlg, astroTableSaturn);
            break;
        case(IDC_RADIO7):
            setAstroContent(hDlg, astroTableNeptune);
            break;
        case(IDC_RADIO8):
            setAstroContent(hDlg, astroTableUranus);
            break;
        case(IDC_RADIO9):
            setAstroContent(hDlg, astroTablePluto);
            break;
        case(IDC_RADIO10):
            setAstroContent(hDlg, astroTableMisc);
            break;

        case(101):            // double click?!?
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;
        case(102):            /* Insert value */
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                PushConstant((double) currentAstroTable[item].value);
                EndDialog(hDlg, FALSE);
            }
            return TRUE;
        case(103):            /* Cancel */
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


void SCI_Astro(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_ASTRO", calcMainWindow, AstroDlgProc);
}

