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
#include <time.h>
#include "EXCAL.h"

/* TVM Registers */
#define FIN_REG_n      0
#define FIN_REG_i      1
#define FIN_REG_PV     2
#define FIN_REG_PMT    3
#define FIN_REG_FV     4

/* Markup Registers */
#define FIN_REG_MUC    5
#define FIN_REG_MUP    6
#define FIN_REG_COST   7
#define FIN_REG_PRICE  8

double taxConstant = 1.05;

uint8_t payMode = 0;                // Pay mode END
uint8_t dateMode = 1;               // MM.DDYYYY
uint8_t depreciationType = 0;       // Straight Line(1==SOYD, 2=DB)
uint8_t finStore = 0;               // Financial Store flag
uint8_t finRecall = 0;              // Financial Recall flag

double fin_reg[FIN_REG_MAX];
char finRegDesc[FIN_REG_MAX][18] = {
    "Num Payments",
    "Interest Rate",
    "Present Value",
    "Payment Amt",
    "Future Value",
    "Markup%C",
    "Markup%P",
    "Cost(markup)",
    "Price(markup)",
};

double cashFlow[MAX_CF];
uint8_t CFn;

char finTmpStr[50];

extern void BUSI_tax(void);
extern void BUSI_percent(void);
extern void BUSI_percentChg(void);
extern void BUSI_percentTot(void);
extern void BUSI_store(void);
extern void BUSI_clearReg(void);
extern void BUSI_12div(void);
extern void BUSI_12mult(void);
extern void BUSI_fv(void);
extern void BUSI_pv(void);
extern void BUSI_amort(void);
extern void BUSI_100div(void);
extern void BUSI_100mult(void);
extern void BUSI_pmt(void);
extern void BUSI_n(void);
extern void BUSI_i(void);
extern void BUSI_recall(void);
extern void BUSI_cashFlow0(void);
extern void BUSI_cashFlowj(void);
extern void BUSI_cashFlowNj(void);
extern void BUSI_NPV(void);
extern void BUSI_IRR(void);
extern void BUSI_date(void);
extern void BUSI_days(void);
extern void BUSI_today(void);
extern void BUSI_muc(void);
extern void BUSI_mup(void);
extern void BUSI_cost(void);
extern void BUSI_price(void);
extern void BUSI_payMode(void);
extern void BUSI_bond(void);
extern void BUSI_ytm(void);
extern void BUSI_currency(void);
extern void BUSI_SPPV(void);
extern void BUSI_SPFV(void);
extern void BUSI_USPV(void);
extern void BUSI_USFV(void);
extern void BUSI_INFL(void);
extern void BUSI_EFF(void);
extern void BUSI_Depr(void);

struct funcStruct Financial_funcs[MAX_FUNCS] = {
    {FN1,   UNI_TAX,    USES_F,     ALLOWREC,   ' ',    "TAX",      YES_L,  X_NEW,      BUSI_tax,           T_TAX,      H_TAX},
    {FN2,   UNI_PERC,   USES_F,     ALLOWREC,   ' ',    " % ",      YES_L,  X_NEW,      BUSI_percent,       T_PERC,     H_PERC},
    {FN3,   UNI_PERCC,  USES_F,     ALLOWREC,   ' ',    "%CHG",     YES_L,  X_NEW,      BUSI_percentChg,    T_PERCC,    H_PERCC},
    {FN4,   UNI_PERCT,  USES_F,     ALLOWREC,   ' ',    "%TOT",     YES_L,  X_NEW,      BUSI_percentTot,    T_PERCT,    H_PERCT},
    {FN5,   UNI_CLRF,   USES_F,     ALLOWREC,   ' ',    "CLRF",     YES_L,  X_NEW,      BUSI_clearReg,      T_CLRF,     H_CLRF},
    {FN6,   UNI_STOFN,  USES_F,     ALLOWREC,   ' ',    "STOF",     NO_L,   X_NULL,     BUSI_store,         T_STOFN,    H_STOFN},
    {FN7,   UNI_RCLFN,  USES_F,     ALLOWREC,   ' ',    "RCLF",     NO_L,   X_NULL,     BUSI_recall,        T_RCLFN,    H_RCLFN},
    {FN8,   UNI_FINN,   USES_F,     ALLOWREC,   ' ',    "n",        YES_L,  X_NEW,      BUSI_n,             T_FINN,     H_FINN},
    {FN9,   UNI_INTR,   USES_F,     ALLOWREC,   ' ',    "i%",       YES_L,  X_NEW,      BUSI_i,             T_INTR,     H_INTR},
    {FN10,  UNI_FINPV,  USES_F,     ALLOWREC,   ' ',    "PV",       YES_L,  X_NEW,      BUSI_pv,            T_FINPV,    H_FINPV},
    {FN11,  UNI_FINPMT, USES_F,     ALLOWREC,   ' ',    "PMT",      YES_L,  X_NEW,      BUSI_pmt,           T_FINPMT,   H_FINPMT},
    {FN12,  UNI_FINFV,  USES_F,     ALLOWREC,   ' ',    "FV",       YES_L,  X_NEW,      BUSI_fv,            T_FINFV,    H_FINFV},
    {FN13,  UNI_AMORT,  USES_F,     ALLOWREC,   ' ',    "Amort",    YES_L,  X_NEW,      BUSI_amort,         T_AMORT,    H_AMORT},
    {FN14,  UNI_CF0,    USES_F,     ALLOWREC,   ' ',    "CF0",      YES_L,  X_NEW,      BUSI_cashFlow0,     T_CF0,      H_CF0},
    {FN15,  UNI_CFJ,    USES_F,     ALLOWREC,   ' ',    "CFj",      YES_L,  X_NEW,      BUSI_cashFlowj,     T_CFJ,      H_CFJ},
    {FN16,  UNI_CFNJ,   USES_F,     ALLOWREC,   ' ',    "CFNj",     YES_L,  X_NEW,      BUSI_cashFlowNj,    T_CFNJ,     H_CFNJ},
    {FN17,  UNI_NPV,    USES_F,     ALLOWREC,   ' ',    "NPV",      YES_L,  X_NEW,      BUSI_NPV,           T_NPV,      H_NPV},
    {FN18,  UNI_IIR,    USES_F,     ALLOWREC,   ' ',    "IRR",      YES_L,  X_NEW,      BUSI_IRR,           T_IRR,      H_IRR},
    {FN19,  UNI_BOND,   USES_F,     ALLOWREC,   ' ',    "BOND",     YES_L,  X_NEW,      BUSI_bond,          T_BOND,     H_BOND},
    {FN20,  UNI_YTM,    USES_F,     ALLOWREC,   ' ',    "YTM",      YES_L,  X_NEW,      BUSI_ytm,           T_YTM,      H_YTM},
    {FN21,  UNI_DATE,   USES_F,     ALLOWREC,   ' ',    "DATE",     YES_L,  X_NEW,      BUSI_date,          T_DATE,     H_DATE},
    {FN22,  UNI_DAYS,   USES_F,     ALLOWREC,   ' ',    "DAYS",     YES_L,  X_NEW,      BUSI_days,          T_DAYS,     H_DAYS},
    {FN23,  UNI_TODAY,  USES_F,     ALLOWREC,   ' ',    "Today",    YES_L,  X_NEW,      BUSI_today,         T_TODAY,    H_TODAY},
    {FN24,  UNI_DEPR,   USES_F,     ALLOWREC,   ' ',    "DEPR",     YES_L,  X_NEW,      BUSI_Depr,          T_DEPR,     H_DEPR},
    {FN25,  UNI_DIV12,  USES_F,     ALLOWREC,   ' ',    "12÷",      YES_L,  X_NEW,      BUSI_12div,         T_DIV12,    H_DIV12},
    {FN26,  UNI_MUL12,  USES_F,     ALLOWREC,   ' ',    "12×",      YES_L,  X_NEW,      BUSI_12mult,        T_MUL12,    H_MUL12},
    {FN27,  UNI_DIV100, USES_F,     ALLOWREC,   ' ',    "100÷",     YES_L,  X_NEW,      BUSI_100div,        T_DIV100,   H_DIV100},
    {FN28,  UNI_MUL100, USES_F,     ALLOWREC,   ' ',    "100×",     YES_L,  X_NEW,      BUSI_100mult,       T_MUL100,   H_MUL100},
    {FN29,  UNI_MUC,    USES_F,     ALLOWREC,   ' ',    "MU%C",     YES_L,  X_NEW,      BUSI_muc,           T_MUC,      H_MUC},
    {FN30,  UNI_MUP,    USES_F,     ALLOWREC,   ' ',    "MU%P",     YES_L,  X_NEW,      BUSI_mup,           T_MUP,      H_MUP},
    {FN31,  UNI_COST,   USES_F,     ALLOWREC,   ' ',    "COST",     YES_L,  X_NEW,      BUSI_cost,          T_COST,     H_COST},
    {FN32,  UNI_PRICE,  USES_F,     ALLOWREC,   ' ',    "PRICE",    YES_L,  X_NEW,      BUSI_price,         T_PRICE,    H_PRICE},
    {FN33,  UNI_SPPV,   USES_F,     ALLOWREC,   ' ',    "SPPV",     YES_L,  X_NEW,      BUSI_SPPV,          T_SPPV,     H_SPPV},
    {FN34,  UNI_SPFV,   USES_F,     ALLOWREC,   ' ',    "SPFV",     YES_L,  X_NEW,      BUSI_SPFV,          T_SPFV,     H_SPFV},
    {FN35,  UNI_USPV,   USES_F,     ALLOWREC,   ' ',    "USPV",     YES_L,  X_NEW,      BUSI_USPV,          T_USPV,     H_USPV},
    {FN36,  UNI_USFV,   USES_F,     ALLOWREC,   ' ',    "USFV",     YES_L,  X_NEW,      BUSI_USFV,          T_USFV,     H_USFV},
    {FN37,  UNI_FMODE,  USES_F,     ALLOWREC,   ' ',    "fModes",   YES_L,  X_NEW,      BUSI_payMode,       T_FMODE,    H_FMODE},
    {FN38,  UNI_CURR,   USES_F,     ALLOWREC,   ' ',    "CURR",     YES_L,  X_NEW,      BUSI_currency,      T_CURRENCY, H_CURRENCY},
    {FN39,  UNI_EFF,    USES_F,     ALLOWREC,   ' ',    "EFF%",     YES_L,  X_NEW,      BUSI_EFF,           T_EFF,      H_EFF},
    {FN40,  UNI_INFL,   USES_F,     ALLOWREC,   ' ',    "INFL",     YES_L,  X_NEW,      BUSI_INFL,          T_INFL,     H_INFL}
};

void BUSI_tax(void)
{
    StackPush(StackPop() * taxConstant);
}

void BUSI_percent(void)
{
    StackPush((StackPop() / 100.0F) * X);
}

void BUSI_percentChg(void)
{
    if (Y == 0.0)
        RPN_error("Percent Change:  Y register cannot equal 0");
    else
        StackPush(100.0F * ((StackPop() - X) / X));
}

void BUSI_percentTot(void)
{
    if (Y == 0.0)
        RPN_error("Percent Total:  Y register cannot equal 0");
    else
        StackPush(100.0F * (StackPop() / X));
}

void BUSI_clearReg(void)
{
    int i;
    for (i = 0; i < FIN_REG_MAX; i++)
        fin_reg[i] = 0.0;
    CFn = 0;
}

void BUSI_store(void)
{
    finRecall = 0;
    finStore ^= 1;
    if (finStore == 0)
        UpdateSpareBar(" ");
    else
        UpdateSpareBar("STOF");
}

void BUSI_recall(void)
{
    finStore = 0;
    finRecall ^= 1;
    if (finRecall == 0)
        UpdateSpareBar(" ");
    else
        UpdateSpareBar("RCLF");
}

void BUSI_12div(void)
{
    StackPush(StackPop() / 12.0);
}

void BUSI_12mult(void)
{
    StackPush(StackPop() * 12.0);
}

void BUSI_fv(void)
{
    double fintemp1, fintemp2;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_FV] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_FV]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_i] == 0.0)
    {
        RPN_error("FV: Interest cannot be zero.\nUse the STOF key to store values into the financial registers.");
    }
    else if (fin_reg[FIN_REG_n] <= 0.0)
    {
        RPN_error
            ("FV: Number of Periods(n) must be greater than 0\nUse the STOF key to store values into the financial registers.");
    }
    else
    {
        fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]);
        fintemp2 =
            fin_reg[FIN_REG_PMT] * (1.0 +
                                    fin_reg[FIN_REG_i] / 100.0 * payMode) *
            ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]) - 1.0) / (fin_reg[FIN_REG_i] / 100.0));
        StackPush((fintemp1 + fintemp2) / -1.0);
        fin_reg[FIN_REG_FV] = X;
    }
}

void BUSI_pv(void)
{
    double fintemp1, fintemp2;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_PV] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_PV]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_i] == 0.0)
    {
        RPN_error("PV: Interest cannot be zero.\nUse the STOF key to store values into the financial registers.");
    }
    else if (fin_reg[FIN_REG_n] <= 0.0)
    {
        RPN_error
            ("PV: Number of Periods(n) must be greater than 0.\nUse the STOF key to store values into the financial registers.");
    }
    else
    {
        fintemp1 = pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]);
        fintemp2 =
            -1.0 * fin_reg[FIN_REG_PMT] * (1.0 +
                                           fin_reg[FIN_REG_i] / 100.0 *
                                           payMode) *
            ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]) -
              1.0) / (fin_reg[FIN_REG_i] / 100.0)) - fin_reg[FIN_REG_FV];
        StackPush(fintemp2 / fintemp1);
        fin_reg[FIN_REG_PV] = X;
    }
}


BOOL CALLBACK fnDIALOG_AmortProc(HWND, UINT, WPARAM, LPARAM);

void BUSI_amort(void)
{
    DLGPROC lpfnDIALOG_AmortProc;

    lpfnDIALOG_AmortProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_AmortProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_AMORT", calcMainWindow, lpfnDIALOG_AmortProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_AmortProc);
}

BOOL CALLBACK fnDIALOG_AmortProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    double pmt;
    double interest, principal, bulk;
    double finTemp1, finTemp2, finTemp3;
    HGLOBAL tptr;
    char far *cptr;
    int idx;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);

        if (fin_reg[FIN_REG_i] <= 0.0)
        {
            RPN_error
                ("AMORT:  Interest must be positive.\nUse the STOF key to store values into the financial registers.");
            EndDialog(hDlg, FALSE);
            return TRUE;
        }
        else if (fin_reg[FIN_REG_n] <= 0.0)
        {
            RPN_error
                ("AMORT: Number of Periods(n) must be greater than 0.\nUse the STOF key to store values into the financial registers.");
            EndDialog(hDlg, FALSE);
            return TRUE;
        }

        finTemp1 =
            (-1.0 * fin_reg[FIN_REG_PV] *
             pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n])) - fin_reg[FIN_REG_FV];
        finTemp2 =
            (1.0 +
             (fin_reg[FIN_REG_i] / 100.0) * payMode) *
            ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]) - 1.0) / (fin_reg[FIN_REG_i] / 100.0));
        pmt = finTemp1 / finTemp2;
        pmt = -1.0 * pmt;

        principal = fin_reg[FIN_REG_PV];
        sprintf(finTmpStr, "%3d     %-9.2f %-9.2f  %-9.2f %-9.2f", 0, 0.0, 0.0, 0.0, principal);
        makeInternational(finTmpStr);
        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
        finTemp1 = 0.0;
        finTemp2 = 0.0;
        finTemp3 = 0.0;
        for (i = 0; i < (int) ceil(fin_reg[FIN_REG_n]); i++)
        {
            if (payMode == 0)   // In END mode we calculate normally
            {
                interest = principal * (fin_reg[FIN_REG_i] / 100.0);
                bulk = pmt - interest;
                principal -= bulk;
            }
            else                // In begining mode we need to subtract out payment first!
            {
                interest = (principal - pmt) * (fin_reg[FIN_REG_i] / 100.0);
                bulk = pmt - interest;
                principal -= bulk;
            }
            if (principal < 0.005)
                principal = 0.0;
            if (interest < 0.005)
                interest = 0.0;

            sprintf(finTmpStr, "%3d     %-9.2f %-9.2f  %-9.2f %-9.2f", i + 1, pmt, bulk, interest, principal);
            makeInternational(finTmpStr);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
            finTemp1 += pmt;
            finTemp2 += bulk;
            finTemp3 += interest;
        }
        sprintf(finTmpStr, "Tot:    %-9.2f %-9.2f  %-9.2f", finTemp1, finTemp2, finTemp3);
        makeInternational(finTmpStr);
        SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
        return TRUE;
    case WM_COMMAND:
        switch(wParam)
        {
        case(102):
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(103):
            tptr = GlobalAlloc(GHND, (DWORD) MAX_IMPORT_CLIPBOARD_SIZE);
            cptr = GlobalLock(tptr);
            lstrcpy(cptr, "");
            idx = 0;
            while (SendDlgItemMessage(hDlg, 101, LB_GETTEXT, idx, (LONG) ((LPSTR) finTmpStr)) != LB_ERR)
            {
                lstrcat(cptr, (LPSTR) finTmpStr);
                lstrcat(cptr, (LPSTR) "\r\n");
                idx++;
            }
            OpenClipboard(calcMainWindow);
            EmptyClipboard();
            GlobalUnlock(tptr);
            SetClipboardData(CF_TEXT, tptr);
            CloseClipboard();
            sprintf(finTmpStr, "The amortization schedule has been saved to the clipboard.");
            MessageBox(hDlg, finTmpStr, "Excalibur For Windows", MB_ICONINFORMATION | MB_OK);
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


void BUSI_100div(void)
{
    StackPush(StackPop() / 100.0);
}

void BUSI_100mult(void)
{
    StackPush(StackPop() * 100.0);
}

void BUSI_pmt(void)
{
    double fintemp1, fintemp2;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_PMT] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_PMT]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_i] == 0.0)
    {
        RPN_error("PMT: Interest cannot be zero.\nUse the STOF key to store values into the financial registers.");
    }
    else if (fin_reg[FIN_REG_n] <= 0.0)
    {
        RPN_error
            ("PMT: Number of Periods(n) must be greater than 0.\nUse the STOF key to store values into the financial registers.");
    }
    else
    {
        fintemp1 =
            (-1.0 * fin_reg[FIN_REG_PV] *
             pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n])) - fin_reg[FIN_REG_FV];
        fintemp2 =
            (1.0 +
             (fin_reg[FIN_REG_i] / 100.0) * payMode) *
            ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]) - 1.0) / (fin_reg[FIN_REG_i] / 100.0));
        StackPush(fintemp1 / fintemp2);
        fin_reg[FIN_REG_PMT] = X;
    }
}


void BUSI_n(void)
{
    double n, fintemp1, fintemp2, fintemp3;
    int newSign, oldSign;
    int found;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_n] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_n]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_i] == 0.0)
    {
        RPN_error("n: Interest cannot be zero.\nUse the STOF key to store values into the financial registers.");
        return;
    }
    found = FALSE;
    n = 0.1;
    fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), n);
    fintemp2 =
        fin_reg[FIN_REG_PMT] * (1.0 +
                                fin_reg[FIN_REG_i] / 100.0 * payMode) *
        ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), n) - 1.0) / (fin_reg[FIN_REG_i] / 100.0));
    fintemp3 = fintemp1 + fintemp2 + fin_reg[FIN_REG_FV];
    if (fintemp3 < 0.0)
        oldSign = -1;
    else
        oldSign = 1;

    for (n = 0.1; n < 1000.0; n += 0.1)
    {
        fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), n);
        fintemp2 =
            fin_reg[FIN_REG_PMT] * (1.0 +
                                    fin_reg[FIN_REG_i] / 100.0 * payMode) *
            ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), n) - 1.0) / (fin_reg[FIN_REG_i] / 100.0));
        fintemp3 = fintemp1 + fintemp2 + fin_reg[FIN_REG_FV];
        if (fintemp3 < 0.0)
            newSign = -1;
        else
            newSign = 1;

        if (newSign != oldSign)
        {
            found = TRUE;
            StackPush(n);
            fin_reg[FIN_REG_n] = X;
            break;
        }
    }
    if (!found)
        RPN_error("A solution could not be found...");
}

void BUSI_i(void)
{
    double i, fintemp1, fintemp2, fintemp3;
    int newSign, oldSign;
    double closei;
    int found;
    char tmpBuff[15];

    if (finStore == 1)
    {
        fin_reg[FIN_REG_i] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_i]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_n] < 1.0)
    {
        RPN_error
            ("i%: Number of periods(n) must be greater than or equal to 1.0\nUse the STOF key to store values into the financial registers.");
        return;
    }
    found = FALSE;
    i = 0.00001;
    fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + i, fin_reg[FIN_REG_n]);
    fintemp2 = fin_reg[FIN_REG_PMT] * (1.0 + i / 100.0 * payMode) * ((pow(1.0 + i, fin_reg[FIN_REG_n]) - 1.0) / i);
    fintemp3 = fintemp1 + fintemp2 + fin_reg[FIN_REG_FV];
    if (fintemp3 < 0.0)
        oldSign = -1;
    else
        oldSign = 1;

    for (i = 0.00001; i < 1.0; i += 0.00001)
    {
        fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + i, fin_reg[FIN_REG_n]);
        fintemp2 = fin_reg[FIN_REG_PMT] * (1.0 + i / 100.0 * payMode) * ((pow(1.0 + i, fin_reg[FIN_REG_n]) - 1.0) / i);
        fintemp3 = fintemp1 + fintemp2 + fin_reg[FIN_REG_FV];
        if (fintemp3 < 0.0)
            newSign = -1;
        else
            newSign = 1;

        if (newSign != oldSign)
        {
            closei = i - 0.00001;
            i = closei;
            fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + i, fin_reg[FIN_REG_n]);
            fintemp2 =
                fin_reg[FIN_REG_PMT] * (1.0 + i / 100.0 * payMode) * ((pow(1.0 + i, fin_reg[FIN_REG_n]) - 1.0) / i);
            fintemp3 = fintemp1 + fintemp2 + fin_reg[FIN_REG_FV];
            if (fintemp3 < 0.0)
                oldSign = -1;
            else
                oldSign = 1;

            for (i = closei; i <= closei + 0.000015; i += 0.0000001)
            {
                fintemp1 = fin_reg[FIN_REG_PV] * pow(1.0 + i, fin_reg[FIN_REG_n]);
                fintemp2 =
                    fin_reg[FIN_REG_PMT] * (1.0 +
                                            fin_reg[FIN_REG_i] / 100.0 *
                                            payMode) * ((pow(1.0 + i, fin_reg[FIN_REG_n]) - 1.0) / i);
                fintemp3 = fintemp1 + fintemp2 + fin_reg[FIN_REG_FV];
                if (fintemp3 < 0.0)
                    newSign = -1;
                else
                    newSign = 1;
                if (newSign != oldSign)
                {
                    found = TRUE;
                    sprintf(tmpBuff, "%.4f", i * 100.0F);
                    StackPush(atof(tmpBuff));
                    fin_reg[FIN_REG_i] = X;
                    break;
                }
            }
            break;
        }
    }
    if (!found)
        RPN_error("A solution could not be found...");
}

void BUSI_cashFlow0(void)
{
    CFn = 0;
    fin_reg[FIN_REG_n] = 0.0;
    cashFlow[CFn] = X;
    blinkXDisplay();
}

void BUSI_cashFlowj(void)
{
    if (CFn >= MAX_CF - 1)
    {
        RPN_error("CFj - Maximum cash flow's reached!");
    }
    else
    {
        CFn++;
        fin_reg[FIN_REG_n] = CFn;
        cashFlow[CFn] = X;
        blinkXDisplay();
    }
}

void BUSI_cashFlowNj(void)
{
    int loop, finalLoop;

    blinkXDisplay();

    finalLoop = (int) X - 1;
    if (finalLoop < 0)
        finalLoop = 0;
    if (CFn < 1)
    {
        RPN_error("CFNj - An initial and at least one additional cash flow entry is required.");
        return;
    }

    for (loop = 0; loop < finalLoop; loop++)
    {
        if (CFn >= MAX_CF - 1)
        {
            RPN_error("CFNj - Maximum cash flow's reached!");
            break;
        }
        else
        {
            CFn++;
            fin_reg[FIN_REG_n] = CFn;
            cashFlow[CFn] = cashFlow[CFn - 1];
        }
    }
}

void BUSI_NPV(void)
{
    int j;
    double npv;

    npv = 0.0;
    for (j = 0; j <= CFn; j++)
    {
        npv += cashFlow[j] / pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), (double) j);
    }
    StackPush(npv);
    fin_reg[FIN_REG_PV] = npv;
}

void BUSI_IRR(void)
{
    int j;
    double npv;
    double i;
    int irr_found = FALSE;

    for (i = 0.00001; i < 1.0; i += 0.00001)
    {
        npv = 0.0;
        for (j = 0; j <= CFn; j++)
        {
            npv += cashFlow[j] / pow(1.0 + i, (double) j);
        }
        if (npv <= 0.0F)
        {
            StackPush(i * 100.0);
            fin_reg[FIN_REG_i] = i * 100.0;
            irr_found = TRUE;
            break;
        }
    }
    if (!irr_found)
        RPN_error("A solution could not be found...");
}


int leapYear(int year)
{
    if ((year % 4 == 0 && (year % 100 != 0)) || (year % 100 == 0 && year % 400 == 0))
        return TRUE;
    else
        return FALSE;
}

struct dateStruct
{
    int day;
    int month;
    int year;
};
int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

unsigned long dateSerial(struct dateStruct dt)
{
    long i;
    unsigned long serial;

    serial = 0;
    for (i = 1900; i < (dt.year); i++)
    {
        serial += 365;
        if (leapYear(i))
            serial++;
    }

    if (dt.month > 2)
        if (leapYear(dt.year))
            serial++;

    for (i = 0; i < (dt.month - 1); i++)
    {
        serial += daysInMonth[i];
    }
    for (i = 0; i < (dt.day - 1); i++)
    {
        serial++;
    }
    return(serial);
}

unsigned long dateSerial360(struct dateStruct dt)
{
    long i;
    unsigned long serial;

    serial = 0;
    for (i = 1900; i < (dt.year); i++)
    {
        serial += 360;          /* TBD, check leap years */
    }

    for (i = 0; i < (dt.month - 1); i++)
    {
        serial += 30;
    }
    for (i = 0; i < (dt.day - 1); i++)
    {
        serial++;
    }
    return(serial);
}

void ConvertFloatToDate(double flt, struct dateStruct *dt)
{
    char *ptr;
    char strval[4];
    unsigned long diff;

    dt->month = (long) flt;
    sprintf(finTmpStr, "%8.6f", flt);
    ptr = strchr(finTmpStr, '.');
    ptr++;
    strval[0] = *ptr;
    ptr++;
    strval[1] = *ptr;
    ptr++;
    strval[2] = CNULL;
    dt->day = atoi(strval);
    dt->year = atoi(ptr);
    if (dateMode == 0)          // For DD.MMYYYY, switch them
    {
        diff = dt->day;
        dt->day = dt->month;
        dt->month = diff;
    }
    if (dt->month > 12)         // Handle bogus values
        dt->month = 12;
}

void BUSI_date(void)
{
    struct dateStruct d2;
    long numDays;
    unsigned long diff;

    numDays = (long) StackPop();
    ConvertFloatToDate(StackPop(), &d2);

    while (numDays != 0)
    {
        if (numDays > 0)
        {
            d2.day++;
            if ((d2.month == 2) && (d2.day == 29) && (leapYear(d2.year)))
            {
                numDays--;
                continue;
            }
            if ((d2.day) > daysInMonth[d2.month - 1])
            {
                d2.day = 1;
                d2.month++;
                if (d2.month > 12)
                {
                    d2.month = 1;
                    d2.year++;
                }
            }
            numDays--;
        }
        else
        {
            d2.day--;
            if ((d2.day) < 1)
            {
                d2.month--;
                if ((d2.month) < 1)
                {
                    d2.month = 12;
                    d2.year--;
                }
                d2.day = daysInMonth[d2.month - 1];
                if ((d2.month == 2) && (leapYear(d2.year)))
                    d2.day++;
            }
            numDays++;
        }
    }
    if (dateMode == 1)
        sprintf(finTmpStr, "%d.%02d%04d", d2.month, d2.day, d2.year);
    else
        sprintf(finTmpStr, "%d.%02d%04d", d2.day, d2.month, d2.year);
    diff = dateSerial(d2);
    diff = (diff % 7) + 1;
    StackPush((double) diff);
    StackPush(atof(finTmpStr));
}

void BUSI_days(void)
{
    long diff;
    struct dateStruct d1;
    struct dateStruct d2;

    ConvertFloatToDate(StackPop(), &d1);
    ConvertFloatToDate(StackPop(), &d2);

    diff = dateSerial360(d1) - dateSerial360(d2);
    StackPush((double) diff);

    diff = dateSerial(d1) - dateSerial(d2);
    StackPush((double) diff);
}

void BUSI_today(void)
{
    char datestr[12];
    int day, month, year;

    _strdate(datestr);
    month = atoi(&datestr[0]);
    day = atoi(&datestr[3]);
    year = atoi(&datestr[6]);
    if (year >= 00 && year < 90)
        year += 2000;
    else
        year += 1900;
    if (dateMode == 1)
        sprintf(finTmpStr, "%d.%02d%04d", month, day, year);
    else
        sprintf(finTmpStr, "%d.%02d%04d", day, month, year);
    if (Xedit == X_ENTER)
        X = (double) atof(finTmpStr);
    else
        StackPush((double) atof(finTmpStr));
}


void BUSI_muc(void)
{
    double muc;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_MUC] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_MUC]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_COST] <= 0.00)
        RPN_error
            ("Markup Cost:  Cost must be greater than 0.00\nUse the STOF key to store values into the financial registers.");
    else
    {
        muc = (fin_reg[FIN_REG_PRICE] - fin_reg[FIN_REG_COST]) / fin_reg[FIN_REG_COST];
        muc = muc * 100.0;
        StackPush(muc);
        fin_reg[FIN_REG_MUC] = X;
    }
}

void BUSI_mup(void)
{
    double muc;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_MUP] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_MUP]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_PRICE] <= 0.0)
        RPN_error
            ("Markup Percent:  Price must be greater than 0.00\nUse the STOF key to store values into the financial registers.");
    else
    {
        muc = (fin_reg[FIN_REG_PRICE] - fin_reg[FIN_REG_COST]) / fin_reg[FIN_REG_PRICE];
        muc = muc * 100.0;
        StackPush(muc);
        fin_reg[FIN_REG_MUP] = X;
    }
}

void BUSI_cost(void)
{
    double cost;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_COST] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_COST]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_MUC] != 0.0)
        cost = fin_reg[FIN_REG_PRICE] / ((fin_reg[FIN_REG_MUC] / 100.0) + 1.0);
    else
        cost = fin_reg[FIN_REG_PRICE] * (1.0 - (fin_reg[FIN_REG_MUP] / 100.0));
    StackPush(cost);
    fin_reg[FIN_REG_COST] = X;
}

void BUSI_price(void)
{
    double price;

    if (finStore == 1)
    {
        fin_reg[FIN_REG_PRICE] = X;
        Xedit = X_NEW;
        blinkXDisplay();
        return;
    }
    if (finRecall == 1)
    {
        StackPush(fin_reg[FIN_REG_PRICE]);
        Xedit = X_NEW;
        return;
    }

    if (fin_reg[FIN_REG_MUC] != 0.0)
        price = fin_reg[FIN_REG_COST] * ((fin_reg[FIN_REG_MUC] / 100.0) + 1.0);
    else
        price = fin_reg[FIN_REG_COST] / (1.0 - (fin_reg[FIN_REG_MUP] / 100.0));
    StackPush(price);
    fin_reg[FIN_REG_PRICE] = X;
}

extern BOOL CALLBACK fnDIALOG_FinancialProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void BUSI_payMode(void)
{
    DLGPROC lpfnDIALOG_FinancialProc;
    lpfnDIALOG_FinancialProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_FinancialProc, hExcaliburInstance);
    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_FINANCIAL", calcMainWindow, lpfnDIALOG_FinancialProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_FinancialProc);
}

BOOL CALLBACK fnDIALOG_FinancialProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    DWORD bs;                   // Button Status

    switch(wMessage)
    {
    case WM_INITDIALOG:
        if (payMode == 1)
            SendMessage(GetDlgItem(hDlg, 101), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 102), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (dateMode == 1)
            SendMessage(GetDlgItem(hDlg, 105), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 106), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (depreciationType == 0)
            SendMessage(GetDlgItem(hDlg, 107), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (depreciationType == 1)
            SendMessage(GetDlgItem(hDlg, 108), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (depreciationType == 2)
            SendMessage(GetDlgItem(hDlg, 109), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(103):            // OK
            bs = SendMessage(GetDlgItem(hDlg, 101), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                payMode = 1;
            else
                payMode = 0;

            bs = SendMessage(GetDlgItem(hDlg, 105), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                dateMode = 1;
            else
                dateMode = 0;

            bs = SendMessage(GetDlgItem(hDlg, 107), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                depreciationType = 0;

            bs = SendMessage(GetDlgItem(hDlg, 108), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                depreciationType = 1;

            bs = SendMessage(GetDlgItem(hDlg, 109), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                depreciationType = 2;


            EndDialog(hDlg, FALSE);
            return TRUE;
        case(104):            // CANCEL
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


double bondPrice(double fBeginDate, double fEndDate, double fYIELD, double fCPN, double *fACCRU)
{
    int DSM, DCS, E, DSC, N;
    double CPN, YIELD, PRICE, RDV, ACCRU;
    double ft1, ft2, ft3, floatN;
    int k;
    struct dateStruct beginDate, endDate;

    RDV = 100.0;

    YIELD = fYIELD;
    CPN = fCPN;

    ConvertFloatToDate(fEndDate, &endDate);
    ConvertFloatToDate(fBeginDate, &beginDate);
    DSM = (dateSerial360(endDate) - dateSerial360(beginDate));

    E = 180;
    DCS = E - (DSM % E);
    if (DCS == E)
        DCS = 0;
    DSC = E - DCS;

    N = DSM / E;
    if ((DSM % E) != 0)         // Bump N up for fractional parts
        N++;
    floatN = (double) N;

    if (DSM <= E)
    {
        MessageBox(calcMainWindow, "here!", "There!", MB_OK);
        ft1 = (100.0 * (RDV + ((double) CPN / 2.0))) / (100.0 + (((double) DSM / (double) E) * ((double) YIELD / 2.0)));
        ft2 = 0.0;
        ft3 = (double) ((double) CPN / 2.0) * (double) ((double) DCS / (double) E);
    }
    else
    {
        ft1 = (RDV / pow(1.0 + (YIELD / 200.0), (double) (floatN - 1.0) + (double) ((double) DSC / (double) E)));
        ft2 = 0.0;
        for (k = 1; k <= N; k++)
        {
            ft2 +=
                ((double) ((double) CPN / 2.0) /
                 pow(1.0 + (YIELD / 200.0), (double) (k - 1) + (double) ((double) DSC / (double) E)));
        }
        ft3 = (double) ((double) CPN / 2.0) * (double) ((double) DCS / (double) E);
    }

    PRICE = (ft1 + ft2) - ft3;
    ACCRU = (double) ((double) DCS / (double) E) * (double) ((double) CPN / 2.0);

    fin_reg[FIN_REG_PV] = PRICE;
    *fACCRU = ACCRU;
    return(PRICE);
}

void BUSI_bond(void)
{
    double fPRICE, fACCRU;
    double fYIELD, fCPN;
    double fBeginDate, fEndDate;
    struct dateStruct beginDate, endDate;

    fYIELD = fin_reg[FIN_REG_i];
    fCPN = fin_reg[FIN_REG_PMT];

    fEndDate = StackPop();
    fBeginDate = StackPop();
    ConvertFloatToDate(fEndDate, &endDate);
    ConvertFloatToDate(fBeginDate, &beginDate);
    if ((long) (dateSerial360(endDate) - (long) dateSerial360(beginDate)) <= 0)
    {
        RPN_error("Bond: Maturity date must be in X, Settlement date in Y");
        return;
    }
    
    fPRICE = bondPrice(fBeginDate, fEndDate, fYIELD, fCPN, &fACCRU);

    StackPush(fACCRU);
    StackPush(fPRICE);
}

void BUSI_ytm(void)
{
    double fPRICE, fACCRU;
    double fCPN;
    double fBeginDate, fEndDate;
    double finTmpStr, i;
    int oldSign = 0;
    int newSign = 0;
    int found = FALSE;
    struct dateStruct beginDate, endDate;

    fPRICE = fin_reg[FIN_REG_PV];
    fCPN = fin_reg[FIN_REG_PMT];
    fEndDate = StackPop();
    fBeginDate = StackPop();

    ConvertFloatToDate(fEndDate, &endDate);
    ConvertFloatToDate(fBeginDate, &beginDate);
    if ((long) (dateSerial360(endDate) - (long) dateSerial360(beginDate)) <= 0)
    {
        RPN_error("YTM: Maturity date must be in X, Settlement date in Y");
        return;
    }

    finTmpStr = bondPrice(fBeginDate, fEndDate, 0.0001 * 100.0, fCPN, &fACCRU);
    if ((finTmpStr - fPRICE) < 0.0)
        oldSign = -1;
    else
        oldSign = 1;

    for (i = 0.0001; i < 1.00; i += 0.0001)
    {
        finTmpStr = bondPrice(fBeginDate, fEndDate, i * 100.0, fCPN, &fACCRU);

        if ((finTmpStr - fPRICE) < 0.0)
            newSign = -1;
        else
            newSign = 1;

        if (newSign != oldSign)
        {
            i -= 0.0001;
            fin_reg[FIN_REG_i] = i * 100.0;
            StackPush(i * 100.0);
            found = TRUE;
            break;
        }
    }
    if (!found)
        RPN_error("YTM: A solution could not be found...");
}


void BUSI_SPPV(void)
{
    double sppv;

    sppv = pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), -1.0 * fin_reg[FIN_REG_n]);
    StackPush(sppv);
}

void BUSI_SPFV(void)
{
    double spfv;

    spfv = pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]);
    StackPush(spfv);
}

void BUSI_USPV(void)
{
    double uspv;

    uspv = (1.0 - (pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), -1.0 * fin_reg[FIN_REG_n]))) / (fin_reg[FIN_REG_i] / 100.0);

    StackPush(uspv);
}

void BUSI_USFV(void)
{
    double usfv;

    usfv = ((pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]) - 1.0)) / (fin_reg[FIN_REG_i] / 100.0);

    StackPush(usfv);
}

void BUSI_INFL(void)
{
    double infl;

    infl = StackPop() / pow(1.0 + (fin_reg[FIN_REG_i] / 100.0), fin_reg[FIN_REG_n]);

    StackPush(infl);
}

void BUSI_EFF(void)
{
    double eff;

    eff = (pow(1.0 + (fin_reg[FIN_REG_i] / (100.0 * fin_reg[FIN_REG_n])), fin_reg[FIN_REG_n]) - 1.0) * 100.0;

    StackPush(eff);
}


//=====================================================
// CURRENCY CONVERSION FUNCTIONS... This maps how much
// each other currency is in relation to the US dollar.
//=====================================================
struct CurrencyStruct CurrencyConv[MAX_CURRENCY_CONV] = {
  {"US Dollar",             1.00000},
  {"Australian Dollar",     0.72},
  {"Brazilian Real",        0.2007},
  {"British Pound",         1.3534},
  {"Canadian Dollar",       0.7329},
  {"Chinese Yuan",          0.1466},
  {"Danish Krone",          0.160318},
  {"Euro",                  1.17206},
  {"Hong Kong Dollar",      0.1276},
  {"Indian Rupee",          0.0107},
  {"Japanese Yen",          0.0062},
  {"Malaysian Ringgit",     0.25299},
  {"Mexican Peso",          0.05763},
  {"New Zealand Dollar",    0.5862},
  {"Norwegian Kroner",      0.153322},
  {"Singapore Dollar",      0.589657},
  {"South African Rand",    0.150489},
  {"South Korean Won",      0.000679},
  {"Sri Lanka Rupee",       0.00317},
  {"Swedish Krona",         0.1091},
  {"Swiss Franc",           0.774234},
  {"Taiwan Dollar",         0.0318},
  {"Thai Baht",             0.0311},
  {"Venezuelan Bolivar",    0.002081},
  {"User Defined 1",        0.0000},
  {"User Defined 2",        0.0000},
  {"User Defined 3",        0.0000},
  {"User Defined 4",        0.0000},
  {"User Defined 5",        0.0000},
  {"Unused",                -1.0},
};

struct CurrencyStruct CurrencyConvTmp;
int32_t currency1index = 0;
int32_t currency2index = 0;

BOOL CALLBACK fnDIALOG_REDEFINECURRENCY(HWND, UINT, WPARAM, LPARAM);
int cancelRedefineCurrency = 0;
void RedefineCurrency(void)
{
    DLGPROC lpfnDIALOG_REDEFINECURRENCY;

    lpfnDIALOG_REDEFINECURRENCY = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_REDEFINECURRENCY, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_REDEFINE_CURRENCY", calcMainWindow, lpfnDIALOG_REDEFINECURRENCY)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_REDEFINECURRENCY);
}

BOOL CALLBACK fnDIALOG_REDEFINECURRENCY(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, CurrencyConvTmp.Country);
        sprintf(finTmpStr, "%.12g", CurrencyConvTmp.conv);
        SetDlgItemText(hDlg, IDC_EDIT3, finTmpStr);

        return TRUE;

    case WM_COMMAND:

        switch(wParam)
        {
        case(IDOK):           // OK
            GetDlgItemText(hDlg, IDC_EDIT1, CurrencyConvTmp.Country, 19);
            CurrencyConvTmp.Country[20] = '\0';
            GetDlgItemText(hDlg, IDC_EDIT3, finTmpStr, 20);
            finTmpStr[20] = '\0';
            CurrencyConvTmp.conv = atof(finTmpStr);
            EndDialog(hDlg, FALSE);
            return TRUE;
            break;

        case(IDCANCEL):       // CANCEL
            cancelRedefineCurrency = 1;
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

// ==================


extern BOOL CALLBACK fnDIALOG_currency(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void BUSI_currency(void)
{
    DLGPROC lpfnfnDIALOG_currency;

    lpfnfnDIALOG_currency = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_currency, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_CURRENCY", calcMainWindow, lpfnfnDIALOG_currency)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnfnDIALOG_currency);
}

BOOL CALLBACK fnDIALOG_currency(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i;
    LRESULT item1, item2;
    double conv1, conv2;
    static int lbTabStops[1] = { 100 };

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, ID_CURRENCY_BOX1), LB_SETTABSTOPS, 1, (DWORD) lbTabStops);
        SendMessage(GetDlgItem(hDlg, ID_CURRENCY_BOX2), LB_SETTABSTOPS, 1, (DWORD) lbTabStops);
        i = 0;
        for (;;)
        {
            sprintf(finTmpStr, "%s\t%-10.9g", CurrencyConv[i].Country, CurrencyConv[i].conv);
            makeInternational(finTmpStr);    // To swap commas and DPs if needed
            SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
            SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
            i++;
            if (CurrencyConv[i].conv == -1.0)   // end of list
                break;
        }
        SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_SETCURSEL, currency1index, 0);
        SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_SETCURSEL, currency2index, 0);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(ID_CURRENCY_A_TO_B):
            item1 = SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_GETCURSEL, 0, 0L);
            item2 = SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_GETCURSEL, 0, 0L);
            if ((item1 == (LRESULT) LB_ERR) || (item2 == (LRESULT) LB_ERR))
            {
                MessageBox(hDlg, "A Country must be selected from both list boxes", "Excalibur User Error", MB_OK);
            }
            else
            {
                conv1 = CurrencyConv[item1].conv;
                conv2 = CurrencyConv[item2].conv;
                if (conv1 == 0.0 || conv2 == 0.0)
                {
                    MessageBox(hDlg,
                                "Can't Convert.  One of the conversion constants is zero.",
                                "Excalibur User Error", MB_OK);
                }
                else
                {
                    StackPush((StackPop() / conv2) * conv1);
                }
                currency1index = item1;
                currency2index = item2;
                EndDialog(hDlg, FALSE);
            }
            return TRUE;
        case(ID_CURRENCY_B_TO_A):
            item1 = SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_GETCURSEL, 0, 0L);
            item2 = SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_GETCURSEL, 0, 0L);
            if ((item1 == (LRESULT) LB_ERR) || (item2 == (LRESULT) LB_ERR))
            {
                MessageBox(hDlg, "A Country must be selected from both list boxes", "Excalibur User Error", MB_OK);
            }
            else
            {
                conv2 = CurrencyConv[item1].conv;
                conv1 = CurrencyConv[item2].conv;
                if (conv1 == 0.0 || conv2 == 0.0)
                {
                    MessageBox(hDlg,
                                "Can't Convert.  One of the conversion constants is zero.",
                                "Excalibur User Error", MB_OK);
                }
                else
                {
                    StackPush((StackPop() / conv2) * conv1);
                }
                currency1index = item1;
                currency2index = item2;
                EndDialog(hDlg, FALSE);
            }
            return TRUE;

        case(ID_CURRENCY_REDEFINE):
            item1 = SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_GETCURSEL, 0, 0L);
            if (item1 == (LRESULT) LB_ERR)
            {
                MessageBox(hDlg, "No Item Selected In The List Box", "Excalibur User Error", MB_OK);
            }
            else
            {
                memcpy(&CurrencyConvTmp, &CurrencyConv[item1], sizeof(CurrencyConvTmp));
                cancelRedefineCurrency = 0;
                RedefineCurrency();
                if (cancelRedefineCurrency == 0)
                {
                    memcpy(&CurrencyConv[item1], &CurrencyConvTmp, sizeof(CurrencyConvTmp));
                    // Must add them to the list again...
                    i = 0;
                    for (;;)
                    {
                        SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_DELETESTRING, 0, 0);
                        SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_DELETESTRING, 0, 0);

                        sprintf(finTmpStr, "%s\t%-10.9g", CurrencyConv[i].Country, CurrencyConv[i].conv);
                        makeInternational(finTmpStr);        // To swap commas and DPs if needed

                        SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
                        SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_ADDSTRING, 0, (LONG) ((LPSTR) finTmpStr));
                        i++;
                        if (CurrencyConv[i].conv == -1.0)       // end of list
                            break;
                    }
                    SendDlgItemMessage(hDlg, ID_CURRENCY_BOX1, LB_SETCURSEL, currency1index, 0);
                    SendDlgItemMessage(hDlg, ID_CURRENCY_BOX2, LB_SETCURSEL, currency2index, 0);
                }
            }
            return TRUE;

        case(ID_CURRENCY_CANCEL):     //cancel pressed
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

void BUSI_Depr(void)
{
    double depr, remain;
    int i, j;
    double rbv, n;
    double fInt, fFrac;

    n = fin_reg[FIN_REG_n];
    j = (int) StackPop();      // Whole numbers only!
    if (j < 0 || j > (int) n)
    {
        RPN_error("Depreciation: X value must be positive whole number between 1 and {n}");
        return;
    }
    else if (j == 0)
    {
        StackPush(fin_reg[FIN_REG_PV] - fin_reg[FIN_REG_FV]);
        StackPush(0.0);
        return;
    }

    if (depreciationType == 0)  // Straight-Line - tbd partial N years?!?
    {
        depr = (fin_reg[FIN_REG_PV] - fin_reg[FIN_REG_FV]) / (double) n;
        remain = (fin_reg[FIN_REG_PV] - fin_reg[FIN_REG_FV]) - (double) ((double) j * depr);
    }
    if (depreciationType == 1)  // SOYD
    {
        fFrac = modf(MakeAccurate(fin_reg[FIN_REG_n]), &fInt);
        remain = (fin_reg[FIN_REG_PV] - fin_reg[FIN_REG_FV]);
        for (i = 1; i <= j; i++)
        {
            depr =
                ((n - (double) i +
                  1.0) / (((fInt + 1.0) * (fInt + (2.0 * fFrac))) / 2.0)) * (fin_reg[FIN_REG_PV] - fin_reg[FIN_REG_FV]);
            remain -= depr;
        }
    }
    if (depreciationType == 2)  // DB
    {
        for (i = 0; i < j; i++)
        {
            if (i == 0)
                rbv = fin_reg[FIN_REG_PV];
            else
                rbv -= rbv * (fin_reg[FIN_REG_i] / (100.0 * n));

            depr = rbv * (fin_reg[FIN_REG_i] / (100.0 * n));
            remain = (rbv - depr) - fin_reg[FIN_REG_FV];
        }
    }

    if (depr < 0.001)
        depr = 0.0;
    if (remain < 0.001)
        remain = 0.0;
    StackPush(remain);
    StackPush(depr);
}
