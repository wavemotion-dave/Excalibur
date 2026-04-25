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

int userTimer = 0;
unsigned long userTicks = 0;

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
extern void SCI_const(void);
extern void SCI_hyp(void);
extern void BUSI_SdevX(void);
extern void SCI_PdevX(void);
extern void BUSI_SdevY(void);
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
extern void Sci_Ceil(void);
extern void Sci_GCD(void);
extern void Sci_LCM(void);
extern void Sci_MinR(void);
extern void Sci_MaxR(void);
extern void Sci_RoundYX(void);
extern void Sci_prime(void);
extern void Sci_elements(void);
extern void Sci_resist(void);
extern void Sci_metricPre(void);
extern void Sci_LogBase2(void);
extern void Sci_TimerStart(void);
extern void Sci_TimerStop(void);
extern void Sci_TimerClear(void);
extern void Sci_TimerPush(void);

struct funcStruct Sci_I_funcs[MAX_FUNCS] = {
    {FN1,       0,  UNI_SIN,    USES_F,     ALLOWREC,   ' ',    "SIN",      YES_L,  X_NEW,  SCI_sin,        T_SIN,        H_SIN},
    {FN2,       0,  UNI_COS,    USES_F,     ALLOWREC,   ' ',    "COS",      YES_L,  X_NEW,  SCI_cos,        T_COS,        H_COS},
    {FN3,       0,  UNI_TAN,    USES_F,     ALLOWREC,   ' ',    "TAN",      YES_L,  X_NEW,  SCI_tan,        T_TAN,        H_TAN},
    {FN4,       0,  UNI_HYP,    USES_F,     ALLOWREC,   ' ',    "HYP",      NO_L,   X_NULL, SCI_hyp,        T_HYP,        H_HYP},
    {FN5,       0,  UNI_ASIN,   USES_F,     ALLOWREC,   ' ',    "ASIN",     YES_L,  X_NEW,  SCI_asin,       T_ASIN,       H_ASIN},
    {FN6,       0,  UNI_ACOS,   USES_F,     ALLOWREC,   ' ',    "ACOS",     YES_L,  X_NEW,  SCI_acos,       T_ACOS,       H_ACOS},
    {FN7,       0,  UNI_ATAN,   USES_F,     ALLOWREC,   ' ',    "ATAN",     YES_L,  X_NEW,  SCI_atan,       T_ATAN,       H_ATAN},
    {FN8,       0,  UNI_ABS,    USES_F,     ALLOWREC,   ' ',    "ABS",      YES_L,  X_NEW,  SCI_abs,        T_ABS,        H_ABS},
    {FN9,       0,  UNI_XX,     USES_F,     ALLOWREC,   ' ',    " X² ",     YES_L,  X_NEW,  SCI_square,     T_XX,         H_XX},
    {FN10,      0,  UNI_SQRT,   USES_F,     ALLOWREC,   ' ',    "SQRT",     YES_L,  X_NEW,  SCI_sqrt,       T_SQRT,       H_SQRT},
    {FN11,      0,  UNI_INV,    USES_F,     ALLOWREC,   ' ',    "1/X",      YES_L,  X_NEW,  SCI_inverse,    T_INV,        H_INV},
    {FN12,      0,  UNI_POW10,  USES_F,     ALLOWREC,   ' ',    "10^X",     YES_L,  X_NEW,  SCI_10x,        T_POW10,      H_POW10},
    {FN13,      0,  UNI_EXP,    USES_F,     ALLOWREC,   ' ',    "e^X",      YES_L,  X_NEW,  SCI_exp,        T_EXP,        H_EXP},
    {FN14,      0,  UNI_LN,     USES_F,     ALLOWREC,   ' ',    " LN ",     YES_L,  X_NEW,  SCI_ln,         T_LN,         H_LN},
    {FN15,      0,  UNI_LOG,    USES_F,     ALLOWREC,   ' ',    "LOG",      YES_L,  X_NEW,  SCI_log,        T_LOG,        H_LOG},
    {FN16,      0,  UNI_FACT,   USES_F,     ALLOWREC,   ' ',    " X! ",     YES_L,  X_NEW,  SCI_xfact,      T_FACT,       H_FACT},
    {FN17,      0,  UNI_POW,    USES_F,     ALLOWREC,   ' ',    "Y^X",      YES_L,  X_NEW,  SCI_y_to_x,     T_POW,        H_POW},
    {FN18,      0,  UNI_INT,    USES_F,     ALLOWREC,   ' ',    "INT",      YES_L,  X_NEW,  SCI_int,        T_INT,        H_INT},
    {FN19,      0,  UNI_FRACT,  USES_F,     ALLOWREC,   ' ',    "FRAC",     YES_L,  X_NEW,  SCI_frac,       T_FRAC,       H_FRAC},
    {FN20,      0,  UNI_RAND,   USES_F,     ALLOWREC,   ' ',    "RAND",     YES_L,  X_NEW,  SCI_rand,       T_RAND,       H_RAND},
    {FN21,      0,  UNI_COMB,   USES_F,     ALLOWREC,   ' ',    "Cn,r",     YES_L,  X_NEW,  SCI_Cnr,        T_COMB,       H_COMB},
    {FN22,      0,  UNI_PERM,   USES_F,     ALLOWREC,   ' ',    "Pn,r",     YES_L,  X_NEW,  SCI_Pnr,        T_PERM,       H_PERM},
    {FN23,      0,  UNI_PI,     USES_F,     ALLOWREC,   ' ',    " PI ",     YES_L,  X_NEW,  SCI_pi,         T_PI,         H_PI},
    {FN24,      0,  UNI_CONST,  USES_F,     NORECORD,   ' ',    "Const",    YES_L,  X_NEW,  SCI_const,      T_CONST,      H_CONST},
    {FN25,      0,  UNI_ROUND,  USES_F,     ALLOWREC,   ' ',    "Round",    YES_L,  X_NEW,  SCI_round,      T_ROUND,      H_ROUND},
    {FN26,      0,  UNI_FLOOR,  USES_F,     ALLOWREC,   ' ',    "Floor",    YES_L,  X_NEW,  SCI_floor,      T_FLOOR,      H_FLOOR},
    {FN27,      0,  UNI_CEIL,   USES_F,     ALLOWREC,   ' ',    "Ceil",     YES_L,  X_NEW,  Sci_Ceil,       T_CEIL,       H_CEIL},
    {FN28,      0,  UNI_RNDXY,  USES_F,     ALLOWREC,   ' ',    "RndYX",    YES_L,  X_NEW,  Sci_RoundYX,    T_ROUNDYX,    H_ROUNDYX},
    {FN29,      0,  UNI_GCD,    USES_F,     ALLOWREC,   ' ',    "GCD",      YES_L,  X_NEW,  Sci_GCD,        T_GCD,        H_GCD},
    {FN30,      0,  UNI_LCM,    USES_F,     ALLOWREC,   ' ',    "LCM",      YES_L,  X_NEW,  Sci_LCM,        T_LCM,        H_LCM},
    {FN31,      0,  UNI_MINR,   USES_F,     ALLOWREC,   ' ',    "MinR",     YES_L,  X_NEW,  Sci_MinR,       T_MINR,       H_MINR},
    {FN32,      0,  UNI_MAXR,   USES_F,     ALLOWREC,   ' ',    "MaxR",     YES_L,  X_NEW,  Sci_MaxR,       T_MAXR,       H_MAXR},
    {FN33,      0,  UNI_PRIME,  USES_F,     NORECORD,   ' ',    "Prime#",   YES_L,  X_NEW,  Sci_prime,      T_PRIME,      H_PRIME},
    {FN34,      0,  UNI_RESIST, USES_F,     ALLOWREC,   ' ',    "Resist",   YES_L,  X_NEW,  Sci_resist,     T_RESIST,     H_RESIST},
    {FN35,      0,  UNI_ELEMENT,USES_F,     NORECORD,   ' ',    "Elmnts",   YES_L,  X_NEW,  Sci_elements,   T_ELEMENT,    H_ELEMENT},
    {FN36,      0,  UNI_LOG2,   USES_F,     ALLOWREC,   ' ',    "Log2",     YES_L,  X_NEW,  Sci_LogBase2,   T_LOG2,       H_LOG2},
    {FN37,      0,  UNI_TSTART, USES_F,     ALLOWREC,   ' ',    "T Start",  YES_L,  X_NEW,  Sci_TimerStart, T_TIMERSTART, H_TIMERSTART},
    {FN38,      0,  UNI_TSTOP,  USES_F,     ALLOWREC,   ' ',    "T Stop",   YES_L,  X_NEW,  Sci_TimerStop,  T_TIMERSTOP,  H_TIMERSTOP},
    {FN39,      0,  UNI_TCLR,   USES_F,     ALLOWREC,   ' ',    "T Clear",  YES_L,  X_NEW,  Sci_TimerClear, T_TIMERCLEAR, H_TIMERCLEAR},
    {FN40,      0,  UNI_TPUSH,  USES_F,     ALLOWREC,   ' ',    "T Push",   YES_L,  X_NEW,  Sci_TimerPush,  T_TIMERPUSH,  H_TIMERPUSH}
};

/* ------------- */
/* SCI functions */
/* ------------- */
int hyperbolic = 0;

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



BOOL CALLBACK fnDIALOG_REDEFINECONST(HWND, UINT, WPARAM, LPARAM);
char constName[26];
char constUnits[11];
double constVal;
int cancelRedefineConst = 0;
void RedefineConst(void)
{
    DLGPROC lpfnDIALOG_REDEFINECONST;

    lpfnDIALOG_REDEFINECONST = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_REDEFINECONST, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_REDEFINE_CONST", calcMainWindow, lpfnDIALOG_REDEFINECONST)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_REDEFINECONST);
}

BOOL CALLBACK fnDIALOG_REDEFINECONST(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[50];

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, constName);
        SetDlgItemText(hDlg, IDC_EDIT2, constUnits);
        sprintf(tmp, "%.12g", constVal);
        SetDlgItemText(hDlg, IDC_EDIT3, tmp);

        return TRUE;

    case WM_COMMAND:

        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, constName, 25);
            constName[25] = '\0';
            GetDlgItemText(hDlg, IDC_EDIT2, constUnits, 10);
            constUnits[10] = '\0';
            GetDlgItemText(hDlg, IDC_EDIT3, tmp, 20);
            tmp[20] = '\0';
            constVal = atof(tmp);
            sprintf(tmp, "%.12g", constVal); // Make the Number the correct # significant digits...
            constVal = atof(tmp);
            EndDialog(hDlg, FALSE);
            return TRUE;

            break;
        case(IDCANCEL):       // CANCEL
            cancelRedefineConst = 1;
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

struct constTableStruct constantsUsed[MAX_CONSTS];
struct constTableStruct constants[MAX_CONST_BANKS][MAX_CONSTS] = {
    {
     {TRUE, "Pi", "",                           M_PI},
     {TRUE, "Speed Of Light", "m/s",            299792458.0},
     {TRUE, "Acceleration Gravity", "m/s²",     9.80665},
     {TRUE, "Plank's Constant", "Js",           6.626E-34},
     {TRUE, "Avogadro's Number", "Mol",         6.022045E+23},
     {TRUE, "Electron Charge", "Col",           1.6021892E-19},
     {TRUE, "Atomic Mass Unit", "Kg",           1.6606E-27},
     {TRUE, "Electron Mass", "Kg",              9.109534E-31},
     {TRUE, "Proton Mass", "Kg",                1.6722E-27},
     {TRUE, "Electron-Proton Ratio", "",        1836.1},
     {TRUE, "Ideal Gas Volume @ STP", "l/mol",  22.4136},
     {TRUE, "Bohr Radius", "m",                 5.292E-11},
     {TRUE, "Electron Volt", "J",               1.602E-19},
     {TRUE, "Boltzman Constant", "JK-1",        1.380622e-23},
     {TRUE, "Faraday's Constant", "Col",        9.65E4},
     {TRUE, "Unit Atomic Energy", "MeV",        931.34},
     {TRUE, "Universal Answer", "",             42.0},
     {FALSE, "None", "",                        0.00}  // end of list...
     },
    {
     {FALSE, "None", "",                        0.00}  // end of list...
     },
    {
     {FALSE, "None", "",                        0.00}  // end of list...
     },
    {
     {FALSE, "None", "",                        0.00}  // end of list...
     },
    {
     {FALSE, "None", "",                        0.00}  // end of list...
     },
};


char constantBankNames[MAX_CONST_BANKS][15] = {
    {"Constants 1"},
    {"Constants 2"},
    {"Constants 3"},
    {"Constants 4"},
    {"Constants 5"}
};

BOOL CALLBACK constBankNamesProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, constantBankNames[0]);
        SetDlgItemText(hDlg, IDC_EDIT2, constantBankNames[1]);
        SetDlgItemText(hDlg, IDC_EDIT3, constantBankNames[2]);
        SetDlgItemText(hDlg, IDC_EDIT4, constantBankNames[3]);
        SetDlgItemText(hDlg, IDC_EDIT5, constantBankNames[4]);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(IDOK):           /* OK - Close */
            GetDlgItemText(hDlg, IDC_EDIT1, constantBankNames[0], 15);
            constantBankNames[0][14] = CNULL;
            GetDlgItemText(hDlg, IDC_EDIT2, constantBankNames[1], 15);
            constantBankNames[1][14] = CNULL;
            GetDlgItemText(hDlg, IDC_EDIT3, constantBankNames[2], 15);
            constantBankNames[2][14] = CNULL;
            GetDlgItemText(hDlg, IDC_EDIT4, constantBankNames[3], 15);
            constantBankNames[3][14] = CNULL;
            GetDlgItemText(hDlg, IDC_EDIT5, constantBankNames[4], 15);
            constantBankNames[4][14] = CNULL;
            EndDialog(hDlg, FALSE);
            return TRUE;
        case(IDCANCEL):       /* Cancel */
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

extern BOOL CALLBACK fnDIALOG_8DlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void SCI_const(void)
{
    DLGPROC lpfnfnDIALOG_8DlgProc;

    lpfnfnDIALOG_8DlgProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_8DlgProc, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_CONSTANTS", calcMainWindow, lpfnfnDIALOG_8DlgProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnfnDIALOG_8DlgProc);
}

BOOL CALLBACK fnDIALOG_8DlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i, j;
    char tmp[50];
    LRESULT item;
    static int lbTabStops[2] = { 170, 240 };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, 101, LB_SETTABSTOPS, 2, (DWORD) lbTabStops);

        j = 0;
        for (i = 0; i < MAX_CONSTS; i++)
        {
            if (constants[lastConstBank][i].includeInList == TRUE)      // Only include those in the list the user wants...
            {
                sprintf(tmp, "%-25s\t%-14.12g \t%s",
                         constants[lastConstBank][i].name,
                         constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                makeInternational(tmp);        // To swap commas and DPs if needed
                SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                memcpy(&constantsUsed[j], &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                j++;
            }
        }
        SetDlgItemText(hDlg, IDC_RADIO1, constantBankNames[0]);
        SetDlgItemText(hDlg, IDC_RADIO2, constantBankNames[1]);
        SetDlgItemText(hDlg, IDC_RADIO3, constantBankNames[2]);
        SetDlgItemText(hDlg, IDC_RADIO4, constantBankNames[3]);
        SetDlgItemText(hDlg, IDC_RADIO5, constantBankNames[4]);

        SendDlgItemMessage(hDlg, IDC_RADIO1 + lastConstBank, BM_SETCHECK, 1, 0);       // Set up bank selection!
        SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
        SetFocus(GetDlgItem(hDlg, 101));
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(101):
            if (HIWORD(wParam) != LBN_DBLCLK)
                break;
            /* Else fall through!!!! */
        case(102):            /* Insert constant number */
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                // Find item in list even with FALSE entries!
                j = 0;
                for (i = 0; i < MAX_CONSTS; i++)
                {
                    if (constants[lastConstBank][j].includeInList == TRUE)
                    {
                        if (item == j)
                            break;
                        j++;
                    }
                }
                lastChosenConst = j;
                PushConstant(constantsUsed[item].value);
                EndDialog(hDlg, FALSE);
            }
            return TRUE;
        case(105):            /* Add New Constant */
            for (i = 0; i < MAX_CONSTS; i++)
            {
                if (constants[lastConstBank][i].includeInList == FALSE)
                {
                    constants[lastConstBank][i].includeInList = TRUE;
                    strcpy(constants[lastConstBank][i].name, "New Constant");
                    strcpy(constants[lastConstBank][i].units, "");
                    constants[lastConstBank][i].value = 0.0;
                    item = i;
                    break;
                }
            }
            if (i == MAX_CONSTS)        // no room in list!
            {
                MessageBox(hDlg, "Sorry, no room left in this constants bank...", "Excalibur Error", MB_OK);
                break;
            }

            // NO BREAK!!! Fall through!
        case(104):            /* Redefine Constant */
            if (LOWORD(wParam) == 104)
                item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                // Find item in list even with FALSE entries!
                j = 0;
                for (i = 0; i < MAX_CONSTS; i++)
                {
                    if (constants[lastConstBank][j].includeInList == TRUE)
                    {
                        if (item == j)
                            break;
                        j++;
                    }
                }
                lastChosenConst = j;
                strcpy(constName, constants[lastConstBank][item].name);
                strcpy(constUnits, constants[lastConstBank][item].units);
                constVal = constants[lastConstBank][item].value;
                cancelRedefineConst = 0;
                RedefineConst();
                if (cancelRedefineConst == 0)
                {
                    strcpy(constants[lastConstBank][item].name, constName);
                    strcpy(constants[lastConstBank][item].units, constUnits);
                    constants[lastConstBank][item].value = constVal;
                    // Must add them to the list again...
                    SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
                    j = 0;
                    for (i = 0; i < MAX_CONSTS; i++)
                    {
                        if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                        {
                            sprintf(tmp, "%-25s\t%-14.12g \t%s",
                                     constants[lastConstBank][i].name,
                                     constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                            makeInternational(tmp);    // To swap commas and DPs if needed
                            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                            memcpy(&constantsUsed[j],
                                    &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                            j++;
                        }
                    }
                    SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
                    SetFocus(GetDlgItem(hDlg, 101));
                }
            }
            return TRUE;

        case(106):            // Delete!
            item = SendDlgItemMessage(hDlg, 101, LB_GETCURSEL, 0, 0L);
            if (item == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                if (MessageBox
                    (hDlg, "Are you sure you wish to delete this entry?", "Excalibur For Windows", MB_YESNO) == IDYES)
                {
                    // Find item in list even with FALSE entries!
                    j = 0;
                    for (i = 0; i < MAX_CONSTS; i++)
                    {
                        if (constants[lastConstBank][j].includeInList == TRUE)
                        {
                            if (item == j)
                                break;
                            j++;
                        }
                    }
                    constants[lastConstBank][j].includeInList = FALSE;
                    lastChosenConst = j - 1;
                    if (lastChosenConst < 0)
                        lastChosenConst = 0;

                    // Now move existing entries down...
                    for (i = j; i < MAX_CONSTS - 1; i++)
                    {
                        memcpy(&constants[lastConstBank][i],
                                &constants[lastConstBank][i + 1], sizeof(constants[lastConstBank][i]));
                    }
                    constants[lastConstBank][i].includeInList = FALSE;  // Always set last list entry on delete to FALSE

                    SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
                    j = 0;
                    for (i = 0; i < MAX_CONSTS; i++)
                    {
                        if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                        {
                            sprintf(tmp, "%-25s\t%-14.12g \t%s",
                                     constants[lastConstBank][i].name,
                                     constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                            makeInternational(tmp);    // To swap commas and DPs if needed
                            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                            memcpy(&constantsUsed[j],
                                    &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                            j++;
                        }
                    }
                    SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
                    SetFocus(GetDlgItem(hDlg, 101));
                }
            }
            return TRUE;

        case(IDC_RADIO1):
            lastConstBank = 0;
            SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
            j = 0;
            for (i = 0; i < MAX_CONSTS; i++)
            {
                if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                {
                    sprintf(tmp, "%-25s\t%-14.12g \t%s",
                             constants[lastConstBank][i].name,
                             constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                    makeInternational(tmp);    // To swap commas and DPs if needed
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                    memcpy(&constantsUsed[j], &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                    j++;
                }
            }
            SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
            SetFocus(GetDlgItem(hDlg, 101));
            return TRUE;
        case(IDC_RADIO2):
            lastConstBank = 1;
            SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
            j = 0;
            for (i = 0; i < MAX_CONSTS; i++)
            {
                if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                {
                    sprintf(tmp, "%-25s\t%-14.12g \t%s",
                             constants[lastConstBank][i].name,
                             constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                    makeInternational(tmp);    // To swap commas and DPs if needed
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                    memcpy(&constantsUsed[j], &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                    j++;
                }
            }
            SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
            SetFocus(GetDlgItem(hDlg, 101));
            return TRUE;
        case(IDC_RADIO3):
            lastConstBank = 2;
            SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
            j = 0;
            for (i = 0; i < MAX_CONSTS; i++)
            {
                if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                {
                    sprintf(tmp, "%-25s\t%-14.12g \t%s",
                             constants[lastConstBank][i].name,
                             constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                    makeInternational(tmp);    // To swap commas and DPs if needed
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                    memcpy(&constantsUsed[j], &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                    j++;
                }
            }
            SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
            SetFocus(GetDlgItem(hDlg, 101));
            return TRUE;
        case(IDC_RADIO4):
            lastConstBank = 3;
            SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
            j = 0;
            for (i = 0; i < MAX_CONSTS; i++)
            {
                if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                {
                    sprintf(tmp, "%-25s\t%-14.12g \t%s",
                             constants[lastConstBank][i].name,
                             constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                    makeInternational(tmp);    // To swap commas and DPs if needed
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                    memcpy(&constantsUsed[j], &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                    j++;
                }
            }
            SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
            SetFocus(GetDlgItem(hDlg, 101));
            return TRUE;
        case(IDC_RADIO5):
            lastConstBank = 4;
            SendDlgItemMessage(hDlg, 101, LB_RESETCONTENT, 0, 0);
            j = 0;
            for (i = 0; i < MAX_CONSTS; i++)
            {
                if (constants[lastConstBank][i].includeInList == TRUE)  // Only include those in the list the user wants...
                {
                    sprintf(tmp, "%-25s\t%-14.12g \t%s",
                             constants[lastConstBank][i].name,
                             constants[lastConstBank][i].value, constants[lastConstBank][i].units);
                    makeInternational(tmp);    // To swap commas and DPs if needed
                    SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
                    memcpy(&constantsUsed[j], &constants[lastConstBank][i], sizeof(constants[lastConstBank][i]));
                    j++;
                }
            }
            SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, lastChosenConst, 0);
            SetFocus(GetDlgItem(hDlg, 101));
            return TRUE;
        case(107):            /* Redefine Bank Names */
            DialogBox(hInst, (LPCSTR) "DIALOG_CONSTANT_BANK_NAMES", hDlg, constBankNamesProc);
            SetDlgItemText(hDlg, IDC_RADIO1, constantBankNames[0]);
            SetDlgItemText(hDlg, IDC_RADIO2, constantBankNames[1]);
            SetDlgItemText(hDlg, IDC_RADIO3, constantBankNames[2]);
            SetDlgItemText(hDlg, IDC_RADIO4, constantBankNames[3]);
            SetDlgItemText(hDlg, IDC_RADIO5, constantBankNames[4]);
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

void Sci_Ceil(void)
{
    double temp;
    temp = ceil(MakeAccurate(StackPop()));
    StackPush(temp);
}

void Sci_GCD(void)
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

void Sci_LCM(void)
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

void Sci_MinR(void)
{
    StackPush(DBL_MIN);
}

void Sci_MaxR(void)
{
    StackPush(DBL_MAX);
}


void Sci_RoundYX(void)
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

extern BOOL CALLBACK fnDIALOG_6DlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void Sci_prime(void)
{
    DLGPROC lpfnfnDIALOG_6DlgProc;

    lpfnfnDIALOG_6DlgProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_6DlgProc, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_PRIMES", calcMainWindow, lpfnfnDIALOG_6DlgProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnfnDIALOG_6DlgProc);
}

BOOL CALLBACK fnDIALOG_6DlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i, j, prime, iprime;
    char tmp[30];
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


extern BOOL CALLBACK fnDIALOG_7DlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void Sci_elements(void)
{
    DLGPROC lpfnfnDIALOG_7DlgProc;

    lpfnfnDIALOG_7DlgProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_7DlgProc, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_ELEMENTS", calcMainWindow, lpfnfnDIALOG_7DlgProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnfnDIALOG_7DlgProc);
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

BOOL CALLBACK fnDIALOG_7DlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
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

extern BOOL CALLBACK fnDIALOG_EWJDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void Sci_resist(void)
{

    DLGPROC lpfnfnDIALOG_EWJDlgProc;

    lpfnfnDIALOG_EWJDlgProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_EWJDlgProc, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_RESISTOR", calcMainWindow, lpfnfnDIALOG_EWJDlgProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnfnDIALOG_EWJDlgProc);
}


BOOL CALLBACK fnDIALOG_EWJDlgProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    static double res_val_1st_band = 0.0;
    static double res_val_2nd_band = 0.0;
    static double res_val_3rd_band = 0.0;
    static double res_val_mult_band = 0.0;
    static double res_tolerance = 0.05;
    static double res_val = 0.0;
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
#if 1                           /* tbd-dsb why are the color codes floats!?! */
        SendDlgItemMessage(hDlg, 170, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[(int) res_val_1st_band]));
        SendDlgItemMessage(hDlg, 171, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[(int) res_val_2nd_band]));
        SendDlgItemMessage(hDlg, 172, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[(int) res_val_3rd_band]));
        SendDlgItemMessage(hDlg, 173, WM_SETTEXT, 0, (LONG) ((LPSTR) color_list[(int) res_val_mult_band]));
#endif
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
void Sci_metricPre(void)
{
    DLGPROC lpMetricPrefixDlgProc;

    lpMetricPrefixDlgProc = (DLGPROC) MakeProcInstance((FARPROC) MetricPrefixDlgProc, hInst);

    if ((DialogBox(hInst, (LPCSTR) "DIALOG_METRICPRE", calcMainWindow, lpMetricPrefixDlgProc)) == -1)
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


void Sci_LogBase2(void)
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

void Sci_TimerStart(void)
{
    userTimer = 1;
}

void Sci_TimerStop(void)
{
    userTimer = 2;
}

void Sci_TimerClear(void)
{
    userTicks = 0;
    userTimer = 0;
    UpdateTimeBar();
}

void Sci_TimerPush(void)
{
    unsigned long ut;
    char tmp[21];

    ut = userTicks / 100;
    sprintf(tmp, "%d.%d", (ut/10), (ut%10));
    StackPush(atof(tmp));
}

