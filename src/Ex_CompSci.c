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

/* Programming externs */
extern void PROG_hex(void);
extern void PROG_bin(void);
extern void PROG_oct(void);
extern void PROG_dec(void);
extern void PROG_hexA(void);
extern void PROG_hexB(void);
extern void PROG_hexC(void);
extern void PROG_hexD(void);
extern void PROG_hexE(void);
extern void PROG_hexF(void);
extern void PROG_and(void);
extern void PROG_or(void);
extern void PROG_not(void);
extern void PROG_nand(void);
extern void PROG_nor(void);
extern void PROG_xor(void);
extern void PROG_shl(void);
extern void PROG_shr(void);
extern void PROG_rol(void);
extern void PROG_ror(void);
extern void PROG_ascii(void);
extern void PROG_mod(void);
extern void PROG_binHi(void);
extern void PROG_binLo(void);
extern void PROG_shlX(void);
extern void PROG_shrX(void);
extern void PROG_rlX(void);
extern void PROG_rrX(void);
extern void PROG_ASR(void);
extern void PROG_NumBits(void);
extern void PROG_SetBit(void);
extern void PROG_ClrBit(void);
extern void PROG_MaskL(void);
extern void PROG_MaskR(void);
extern void PROG_WordSize(void);
extern void PROG_2sComp(void);
extern void PROG_MinW(void);
extern void PROG_MaxW(void);
extern void PROG_Mirror(void);
extern void Prog_IEEE(void);

uint8_t binMode = 0;

struct funcStruct CompSci_funcs[MAX_FUNCS] = {
    {FN1,   0, UNI_DEC,     USES_L,     ALLOWREC,   ' ',    "DEC",      YES_L,  X_NEW,      PROG_dec,       T_DEC,      H_DEC},
    {FN2,   0, UNI_HEX,     USES_L,     ALLOWREC,   ' ',    "HEX",      YES_L,  X_NEW,      PROG_hex,       T_HEX,      H_HEX},
    {FN3,   0, UNI_BIN,     USES_L,     ALLOWREC,   ' ',    "BIN",      YES_L,  X_NEW,      PROG_bin,       T_BIN,      H_BIN},
    {FN4,   0, UNI_OCT,     USES_L,     ALLOWREC,   ' ',    "OCT",      YES_L,  X_NEW,      PROG_oct,       T_OCT,      H_OCT},
    {FN5,   0, UNI_HEXA,    USES_L,     ALLOWREC,   'a',    "A",        NO_L,   X_NULL,     PROG_hexA,      T_HEXA,     H_HEXDIG},
    {FN6,   0, UNI_HEXB,    USES_L,     ALLOWREC,   'b',    "B",        NO_L,   X_NULL,     PROG_hexB,      T_HEXB,     H_HEXDIG},
    {FN7,   0, UNI_HEXC,    USES_L,     ALLOWREC,   'c',    "C",        NO_L,   X_NULL,     PROG_hexC,      T_HEXC,     H_HEXDIG},
    {FN8,   0, UNI_HEXD,    USES_L,     ALLOWREC,   'd',    "D",        NO_L,   X_NULL,     PROG_hexD,      T_HEXD,     H_HEXDIG},
    {FN9,   0, UNI_HEXE,    USES_L,     ALLOWREC,   'e',    "E",        NO_L,   X_NULL,     PROG_hexE,      T_HEXE,     H_HEXDIG},
    {FN10,  0, UNI_HEXF,    USES_L,     ALLOWREC,   'f',    "F",        NO_L,   X_NULL,     PROG_hexF,      T_HEXF,     H_HEXDIG},
    {FN11,  0, UNI_BINLO,   USES_L,     ALLOWREC,   ' ',    "Bin HI",   YES_L,  X_NEW,      PROG_binHi,     T_BINH,     H_BINH},
    {FN12,  0, UNI_BINHI,   USES_L,     ALLOWREC,   ' ',    "Bin LO",   YES_L,  X_NEW,      PROG_binLo,     T_BINL,     H_BINL},
    {FN13,  0, UNI_AND,     USES_L,     ALLOWREC,   ' ',    "AND",      YES_L,  X_NEW,      PROG_and,       T_AND,      H_AND},
    {FN14,  0, UNI_OR,      USES_L,     ALLOWREC,   ' ',    "OR",       YES_L,  X_NEW,      PROG_or,        T_OR,       H_OR},
    {FN15,  0, UNI_NOT,     USES_L,     ALLOWREC,   ' ',    "NOT",      YES_L,  X_NEW,      PROG_not,       T_NOT,      H_NOT},
    {FN16,  0, UNI_XOR,     USES_L,     ALLOWREC,   ' ',    "XOR",      YES_L,  X_NEW,      PROG_xor,       T_XOR,      H_XOR},
    {FN17,  0, UNI_NAND,    USES_L,     ALLOWREC,   ' ',    "NAND",     YES_L,  X_NEW,      PROG_nand,      T_NAND,     H_NAND},
    {FN18,  0, UNI_NOR,     USES_L,     ALLOWREC,   ' ',    "NOR",      YES_L,  X_NEW,      PROG_nor,       T_NOR,      H_NOR},
    {FN19,  0, UNI_2SCOMP,  USES_L,     ALLOWREC,   ' ',    "2's ",     YES_L,  X_NEW,      PROG_2sComp,    T_2COMP,    H_2COMP},
    {FN20,  0, UNI_ASR,     USES_L,     ALLOWREC,   ' ',    "ASR",      YES_L,  X_NEW,      PROG_ASR,       T_ASR,      H_ASR},
    {FN21,  0, UNI_SHL,     USES_L,     ALLOWREC,   ' ',    "SHL",      YES_L,  X_NEW,      PROG_shl,       T_SL,       H_SL},
    {FN22,  0, UNI_SHR,     USES_L,     ALLOWREC,   ' ',    "SHR",      YES_L,  X_NEW,      PROG_shr,       T_SR,       H_SR},
    {FN23,  0, UNI_ROL,     USES_L,     ALLOWREC,   ' ',    "ROL",      YES_L,  X_NEW,      PROG_rol,       T_RL,       H_RL},
    {FN24,  0, UNI_ROR,     USES_L,     ALLOWREC,   ' ',    "ROR",      YES_L,  X_NEW,      PROG_ror,       T_RR,       H_RR},
    {FN25,  0, UNI_SHLX,    USES_L,     ALLOWREC,   ' ',    "SHL,x",    YES_L,  X_NEW,      PROG_shlX,      T_SLM,      H_SLM},
    {FN26,  0, UNI_SHRX,    USES_L,     ALLOWREC,   ' ',    "SHR,x",    YES_L,  X_NEW,      PROG_shrX,      T_SRM,      H_SRM},
    {FN27,  0, UNI_RLX,     USES_L,     ALLOWREC,   ' ',    "RL,x",     YES_L,  X_NEW,      PROG_rlX,       T_RLX,      H_RLX},
    {FN28,  0, UNI_RRX,     USES_L,     ALLOWREC,   ' ',    "RR,x",     YES_L,  X_NEW,      PROG_rrX,       T_RRX,      H_RRX},
    {FN29,  0, UNI_MOD,     USES_L,     ALLOWREC,   ' ',    "MOD",      YES_L,  X_NEW,      PROG_mod,       T_MOD,      H_MOD},
    {FN30,  0, UNI_NUMB,    USES_L,     ALLOWREC,   ' ',    "#B",       YES_L,  X_NEW,      PROG_NumBits,   T_NUMBITS,  H_NUMBITS},
    {FN31,  0, UNI_SETB,    USES_L,     ALLOWREC,   ' ',    "SB",       YES_L,  X_NEW,      PROG_SetBit,    T_SETBIT,   H_SETBIT},
    {FN32,  0, UNI_CLRB,    USES_L,     ALLOWREC,   ' ',    "CB",       YES_L,  X_NEW,      PROG_ClrBit,    T_CLRBIT,   H_CLRBIT},
    {FN33,  0, UNI_MASKL,   USES_L,     ALLOWREC,   ' ',    "MaskL",    YES_L,  X_NEW,      PROG_MaskL,     T_MASKL,    H_MASKL},
    {FN34,  0, UNI_MASKR,   USES_L,     ALLOWREC,   ' ',    "MaskR",    YES_L,  X_NEW,      PROG_MaskR,     T_MASKR,    H_MASKR},
    {FN35,  0, UNI_MINW,    USES_L,     ALLOWREC,   ' ',    "MINW",     YES_L,  X_NEW,      PROG_MinW,      T_MINW,     H_MINW},
    {FN36,  0, UNI_MAXW,    USES_L,     ALLOWREC,   ' ',    "MAXW",     YES_L,  X_NEW,      PROG_MaxW,      T_MAXW,     H_MAXW},
    {FN37,  0, UNI_ASCII,   USES_L,     ALLOWREC,   ' ',    "ASCII",    NO_L,   X_NEW,      PROG_ascii,     T_ASCII,    H_ASCII},
    {FN38,  0, UNI_WSIZE,   USES_L,     ALLOWREC,   ' ',    "WSIZE",    YES_L,  X_NEW,      PROG_WordSize,  T_WORDSIZE, H_WORDSIZE},
    {FN39,  0, UNI_MIRROR,  USES_L,     ALLOWREC,   ' ',    "Mirror",   YES_L,  X_NEW,      PROG_Mirror,    T_MIRROR,   H_MIRROR},
    {FN40,  0, UNI_IEEE,    USES_F,     ALLOWREC,   ' ',    "IEEE",     YES_L,  X_NEW,      Prog_IEEE,      T_IEEE,     H_IEEE}
};



void MakeRadixStr(PROG_LONG val, char *tmpL)
{
    char temp[60];
    char temp2[60];
    char temp3[60];
    char temp4[60];
    int i, j;

    if (progMode == PROG_HEX)
    {
        if (padZeros == 1)
        {
            if (wordSize == 8)
                sprintf(tmpL, "%02lX", val);
            else if (wordSize == 16)
                sprintf(tmpL, "%04lX", val);
            else
                sprintf(tmpL, "%08lX", val);
        }
        else
            sprintf(tmpL, "%lX", val);
    }
    else if (progMode == PROG_BIN)
    {
        if (binMode == 1)       /* Show upper */
            val = val >> 16;

        ltoa(val & (PROG_LONG) 0x0000FFFF, temp4, progMode);
        if (padZeros == 1)
        {
            if (wordSize == 8)
                sprintf(temp, "%08s", temp4);
            else
                sprintf(temp, "%016s", temp4);
        }
        else
            sprintf(temp, "%16s", temp4);
        if ((val & 0xFFFF0000) != 0)
            for (i = 0; i < (int) strlen(temp); i++)
                if (temp[i] == ' ')
                    temp[i] = '0';

        j = 0;
        for (i = strlen(temp); i >= 0; i--)
        {
            temp2[j] = temp[i];
            if ((j % 5) == 0)
            {
                j++;
                temp2[j] = ' ';
            }
            j++;
        }
        if (temp2[j - 1] == ' ')
            j--;
        temp2[j] = CNULL;
        for (i = 0; i < j; i++)
        {
            temp3[i] = temp2[j - i - 1];
        }
        temp3[j - 2] = CNULL;

        if ((val & 0xFFFF0000) != 0 || (padZeros == 1 && binMode == 0))
            sprintf(tmpL, "«%s", temp3);    // Left Arrow!
        else
        {
            sprintf(tmpL, "%s", temp3);
        }
    }
    else if (progMode == PROG_DEC)
    {
        if (wordMode == PROG_SIGNED)
            ltoa(val, tmpL, progMode);
        else
            ultoa(val, tmpL, progMode);
        PutCommas(tmpL);
    }
    else
        ltoa(val, tmpL, progMode);     // Lonely OCTal
}


PROG_LONG MakeProgStr(char *str)
{
    char *tmpPtr;
    return(strtoul(str, &tmpPtr, progMode));
}

PROG_LONG maskStackStuff(PROG_LONG lng)
{
    PROG_LONG mask;

    lng = lng & wordSizeMask;

    mask = 0x00000001L << (wordSize - 1);

    if ((progMode == PROG_DEC) && (lng & mask))
    {
        if (wordMode == PROG_SIGNED)
            lng |= ~wordSizeMask;
    }
    return(lng);
}

void StackPushL(PROG_LONG temp)
{
    stackPushes++;

    if (extendedStack)
    {
        DL = maskStackStuff(CL);
        CL = maskStackStuff(BL);
        BL = maskStackStuff(AL);
        AL = maskStackStuff(TL);
    }

    TL = maskStackStuff(ZL);
    ZL = maskStackStuff(YL);
    YL = maskStackStuff(XL);
    XL = maskStackStuff(temp);

    LongsToFloats();
}

PROG_LONG StackPopL(void)
{
    PROG_LONG temp;
    stackPops++;
    temp = maskStackStuff(XL);
    XL = maskStackStuff(YL);
    YL = maskStackStuff(ZL);
    ZL = maskStackStuff(TL);
    if (extendedStack)
    {
        TL = maskStackStuff(AL);
        AL = maskStackStuff(BL);
        BL = maskStackStuff(CL);
        CL = maskStackStuff(DL);
        if (popFillZero != 0)
            DL = maskStackStuff(0L);
    }
    else
    {
        if (popFillZero != 0)
            TL = maskStackStuff(0L);
    }

    LongsToFloats();

    return(temp);
}



/* ----------------------- */
/* Programming mode stuff  */
/* ----------------------- */
void PROG_hex(void)
{
    progMode = PROG_HEX;
    XL = maskStackStuff(XL);
    YL = maskStackStuff(YL);
    ZL = maskStackStuff(ZL);
    TL = maskStackStuff(TL);
}

void PROG_bin(void)
{
    progMode = PROG_BIN;
    binMode = 0;
    XL = maskStackStuff(XL);
    YL = maskStackStuff(YL);
    ZL = maskStackStuff(ZL);
    TL = maskStackStuff(TL);
}

void PROG_oct(void)
{
    progMode = PROG_OCT;
    XL = maskStackStuff(XL);
    YL = maskStackStuff(YL);
    ZL = maskStackStuff(ZL);
    TL = maskStackStuff(TL);
}

void PROG_dec(void)
{
    progMode = PROG_DEC;
    XL = maskStackStuff(XL);
    YL = maskStackStuff(YL);
    ZL = maskStackStuff(ZL);
    TL = maskStackStuff(TL);
}

void PROG_hexA(void)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, "A");
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, "A");
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, 'A'))
            {
                strcat(Xstr, "A");
            }
        }
        XL = MakeProgStr(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_hexB(void)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, "B");
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, "B");
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, 'B'))
            {
                strcat(Xstr, "B");
            }
        }
        XL = MakeProgStr(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_hexC(void)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, "C");
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, "C");
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, 'C'))
            {
                strcat(Xstr, "C");
            }
        }

        XL = MakeProgStr(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_hexD(void)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, "D");
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, "D");
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, 'D'))
            {
                strcat(Xstr, "D");
            }
        }

        XL = MakeProgStr(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_hexE(void)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, "E");
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, "E");
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, 'E'))
            {
                strcat(Xstr, "E");
            }
        }
        XL = MakeProgStr(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_hexF(void)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, "F");
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, "F");
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, 'F'))
            {
                strcat(Xstr, "F");
            }
        }
        XL = MakeProgStr(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_and(void)
{
    StackPushL(StackPopL() & StackPopL());
}


void PROG_or(void)
{
    StackPushL(StackPopL() | StackPopL());
}

void PROG_not(void)
{
    StackPushL(~StackPopL());
}

void PROG_nand(void)
{
    StackPushL(~(StackPopL() & StackPopL()));
}

void PROG_nor(void)
{
    StackPushL(~(StackPopL() | StackPopL()));
}

void PROG_xor(void)
{
    StackPushL(StackPopL() ^ StackPopL());
}

void PROG_shl(void)
{
    StackPushL(StackPopL() << 1);     // this always shifts in a zero
}

void PROG_shr(void)
{
    PROG_LONG mask;
    PROG_LONG val;

    val = (StackPopL() >> 1);
    mask = 0x00000001L << (wordSize - 1);
    val &= (PROG_LONG) ~ mask;  // Always shift in a zero...
    StackPushL(val);
}

void PROG_rol(void)
{
    PROG_LONG r;
    PROG_LONG mask;

    mask = 0x00000001L << (wordSize);

    r = (PROG_LONG) _lrotl(StackPopL(), 1);
    if (r & mask)
        r |= 0x00000001L;       // Shift in appropriate bit
    StackPushL(r);
}

void PROG_ror(void)
{
    PROG_LONG r;
    PROG_LONG mask;

    r = (PROG_LONG) _lrotr(StackPopL(), 1);
    mask = 0x00000001L << (wordSize - 1);

    if (r & 0x80000000L)
    {
        r |= mask;
    }
    else
    {
        r &= (PROG_LONG) ~ mask;        // If we didn't rotate a 1 around, always zero the MSBit.
    }
    StackPushL(r);
}

void PROG_mod(void)
{
    PROG_LONG l1, l2;
    if (XL == 0L)
    {
        RPN_error("MOD:  Divide By Zero");
    }
    else
    {
        l1 = StackPopL();
        l2 = StackPopL();
        StackPushL((PROG_LONG) l2 % l1);
    }
}

extern BOOL CALLBACK fnDIALOG_ASCIIProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);

void PROG_ascii(void)
{

    DLGPROC lpfnDIALOG_ASCIIProc;

    lpfnDIALOG_ASCIIProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_ASCIIProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_ASCII", calcMainWindow, lpfnDIALOG_ASCIIProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_ASCIIProc);
}

static char asciiStr[5];
char *ASCII_Str(int idx)
{
    switch (idx)
    {
        case 0:  return "NUL";
        case 1:  return "SOH";
        case 2:  return "STX";
        case 3:  return "ETX";
        case 4:  return "EOT";
        case 5:  return "ENQ";
        case 6:  return "ACK";
        case 7:  return "BEL";
        case 8:  return "BS ";
        case 9:  return "HT ";
        case 10: return "LF ";
        case 11: return "VT ";
        case 12: return "FF ";
        case 13: return "CR ";
        case 14: return "SO ";
        case 15: return "SI ";
        case 16: return "DLE";
        case 17: return "DC1";
        case 18: return "DC2";
        case 19: return "DC3";
        case 20: return "DC4";
        case 21: return "NAK";
        case 22: return "SYN";
        case 23: return "ETB";
        case 24: return "CAN";
        case 25: return "EM ";
        case 26: return "SUB";
        case 27: return "ESC";
        case 28: return "FS ";
        case 29: return "GS ";
        case 30: return "RS ";
        case 31: return "US ";
        case 32: return "SPC";
        
        default:
            sprintf(asciiStr, "%c  ", (char)idx);
            if ((idx != 127) && (idx != 129) && (idx != 141) && (idx != 143) && (idx != 144) && (idx != 157)) return asciiStr;
            else return "   ";
    }
}

BOOL CALLBACK fnDIALOG_ASCIIProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i, j;
    char tmp[80];
    char tmp2[20];
    char tmp3[20];
    char tmp4[20];
    LRESULT item;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);  // Main window
        SendMessage(GetDlgItem(hDlg, 104), WM_SETFONT, (WPARAM) hFixedFont, FALSE);  // Title
        for (i = 0; i <= 255; i++)
        {
            ltoa(i, tmp2, 2);  // Conver to binary
            sprintf(tmp3, "%8s", tmp2);
            for (j = 0; j < (int) strlen(tmp3); j++)
                if (tmp3[j] == ' ')
                    tmp3[j] = '0';

            ltoa(i, tmp2, 8);  // Conver to binary
            sprintf(tmp4, "%3s", tmp2);
            for (j = 0; j < (int) strlen(tmp4); j++)
                if (tmp4[j] == ' ')
                    tmp4[j] = '0';
            sprintf(tmp, "%3s  %3d   %02X   %8s  %3s", ASCII_Str(i), i, i, tmp3, tmp4);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        }
        i = (unsigned char) (XL & 0x000000FF);
        SendDlgItemMessage(hDlg, 101, LB_SETCURSEL, i, 0);
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
                MessageBox(hDlg, "No Item Selected In The List Box", "EXCALIBUR User Error", MB_OK);
            }
            else
            {
                StackPushL(item);
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

void PROG_binHi(void)
{
    progMode = PROG_BIN;
    binMode = 1;
    XL = maskStackStuff(XL);
    YL = maskStackStuff(YL);
    ZL = maskStackStuff(ZL);
    TL = maskStackStuff(TL);
}

void PROG_binLo(void)
{
    progMode = PROG_BIN;
    binMode = 0;
    XL = maskStackStuff(XL);
    YL = maskStackStuff(YL);
    ZL = maskStackStuff(ZL);
    TL = maskStackStuff(TL);
}

void PROG_shlX(void)
{
    PROG_LONG shiftVal;
    shiftVal = StackPopL();
    StackPushL(StackPopL() << shiftVal);
}


void PROG_shrX(void)
{
    PROG_LONG mask;
    PROG_LONG shiftVal;

    shiftVal = StackPopL();

    shiftVal = ((PROG_LONG) StackPopL() >> shiftVal);
    mask = 0x00000001L << (wordSize - 1);
    shiftVal &= (PROG_LONG) ~ mask;
    StackPushL(shiftVal);
}


void PROG_rlX(void)
{
    PROG_LONG shiftVal;

    shiftVal = StackPopL();
    while (shiftVal != 0)
    {
        PROG_rol();
        shiftVal--;
    }
}

void PROG_rrX(void)
{
    PROG_LONG shiftVal;

    shiftVal = StackPopL();
    while (shiftVal != 0)
    {
        PROG_ror();
        shiftVal--;
    }
}

void PROG_ASR(void)
{
    PROG_LONG val;
    PROG_LONG mask;

    mask = 0x00000001L << (wordSize - 1);

    val = StackPopL();

    if (wordMode == PROG_SIGNED)
    {
        if (val & mask)
        {
            val = val >> 1;
            val |= mask;
        }
        else
        {
            val = val >> 1;
        }
    }
    else
    {
        val = val >> 1;         // Unsigned shift
    }
    StackPushL((PROG_LONG) val);
}

void PROG_NumBits(void)
{
    PROG_LONG Val;
    PROG_LONG shiftVal;
    PROG_LONG mask;
    PROG_LONG cnt;
    PROG_LONG cmp;

    cnt = 0;
    Val = StackPopL();
    for (shiftVal = 0; shiftVal < (PROG_LONG) wordSize; shiftVal++)     // tbd-word size
    {
        mask = 1L << shiftVal;
        cmp = Val & mask;;
        if (cmp != 0L)
        {
            cnt += 1;
        }
    }
    StackPushL((PROG_LONG) cnt);
}

void PROG_SetBit(void)         // This is Zero based
{
    PROG_LONG bitNum;
    PROG_LONG mask;

    bitNum = StackPopL();
    mask = 1L << bitNum;
    StackPushL(StackPopL() | mask);
}

void PROG_ClrBit(void)         // This is Zero based
{
    PROG_LONG bitNum;
    PROG_LONG mask;

    bitNum = StackPopL();
    mask = 1L << bitNum;
    mask = ~mask;
    StackPushL(StackPopL() & mask);
}

void PROG_MaskR(void)
{
    PROG_LONG mask, mask2;
    PROG_LONG maskSize;

    maskSize = StackPopL();
    if (maskSize > (PROG_LONG) wordSize)
        maskSize = wordSize;
    mask = 0L;
    while (maskSize != 0)
    {
        maskSize--;
        mask2 = 1L << maskSize;
        mask |= mask2;
    }
    StackPushL(mask);
}

void PROG_MaskL(void)
{
    PROG_LONG mask, mask2, mask3;
    PROG_LONG maskSize;

    maskSize = StackPopL();
    if (maskSize > (PROG_LONG) wordSize)
        maskSize = wordSize;
    mask = 0L;
    while (maskSize != 0)
    {
        mask3 = 0x00000001L << (wordSize - 1);
        maskSize--;
        mask2 = mask3 >> maskSize;
        mask |= mask2;
    }
    StackPushL(mask);
}


void PROG_2sComp(void)
{
    RPN_negate_x();
}


BOOL CALLBACK fnDIALOG_WordSizeProc(HWND, UINT, WPARAM, LPARAM);
void PROG_WordSize(void)
{
    DLGPROC lpfnDIALOG_WordSizeProc;

    lpfnDIALOG_WordSizeProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_WordSizeProc, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_WORDSIZE", calcMainWindow, lpfnDIALOG_WordSizeProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_WordSizeProc);
}

BOOL CALLBACK fnDIALOG_WordSizeProc(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    DWORD bs;                   // Button Status

    switch(wMessage)
    {
    case WM_INITDIALOG:
        if (padZeros == 0)
            SendMessage(GetDlgItem(hDlg, 110), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 111), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (wordSize == 8)
            SendMessage(GetDlgItem(hDlg, 104), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (wordSize == 16)
            SendMessage(GetDlgItem(hDlg, 105), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 106), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (wordMode == PROG_SIGNED)
            SendMessage(GetDlgItem(hDlg, 107), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 108), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        return TRUE;
    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            bs = SendMessage(GetDlgItem(hDlg, 110), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                padZeros = 0;
            else
                padZeros = 1;

            bs = SendMessage(GetDlgItem(hDlg, 107), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                wordMode = PROG_SIGNED;
            else
                wordMode = PROG_UNSIGNED;

            bs = SendMessage(GetDlgItem(hDlg, 104), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 8;
                wordSizeMask = 0x000000FFL;
            }
            bs = SendMessage(GetDlgItem(hDlg, 105), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 16;
                wordSizeMask = 0x0000FFFFL;
            }

            bs = SendMessage(GetDlgItem(hDlg, 106), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 32;
                wordSizeMask = 0xFFFFFFFFL;
            }
            EndDialog(hDlg, FALSE);
            ShowStatus();

            Xedit = X_NEW;
            TL = maskStackStuff(TL);
            ZL = maskStackStuff(ZL);
            YL = maskStackStuff(YL);
            XL = maskStackStuff(XL);
            ShowStack();

            return TRUE;

        case(IDCANCEL):       // CANCEL
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

void PROG_Mirror(void)
{
    PROG_LONG xTmp;
    PROG_LONG newVal, shiftVal, mask1, mask2;

    newVal = (PROG_LONG) 0L;
    xTmp = StackPopL();
    for (shiftVal = 0; shiftVal < (PROG_LONG) wordSize; shiftVal++)     // tbd-word size
    {
        mask1 = 1L << shiftVal;
        mask2 = 1L << (wordSize - shiftVal - 1);
        if ((mask1 & xTmp) != 0)
            newVal = newVal | mask2;
    }
    StackPushL((PROG_LONG) newVal);
}

void PROG_MinW(void)
{
    if (wordMode == PROG_SIGNED)
    {
        if (wordSize == 8)
            StackPushL((PROG_LONG) 0x80);
        else if (wordSize == 16)
            StackPushL((PROG_LONG) 0x8000);
        else
            StackPushL((PROG_LONG) 0x80000000);
    }
    else
    {
        StackPushL((PROG_LONG) 0x00000000);
    }
}

void PROG_MaxW(void)
{
    if (wordMode == PROG_SIGNED)
    {
        if (wordSize == 8)
            StackPushL((PROG_LONG) 0x7F);
        else if (wordSize == 16)
            StackPushL((PROG_LONG) 0x7FFF);
        else
            StackPushL((PROG_LONG) 0x7FFFFFFF);
    }
    else
    {
        if (wordSize == 8)
            StackPushL((PROG_LONG) 0xFF);
        else if (wordSize == 16)
            StackPushL((PROG_LONG) 0xFFFF);
        else
            StackPushL((PROG_LONG) 0xFFFFFFFF);
    }
}


PROG_LONG smallestProgVal(void)
{
    if (wordMode == PROG_SIGNED)
    {
        if (wordSize == 8)
            return((PROG_LONG) 0x80);
        else if (wordSize == 16)
            return((PROG_LONG) 0x8000);
        else
            return((PROG_LONG) 0x80000000);
    }
    else
    {
        return((PROG_LONG) 0x00000000);
    }
}

PROG_LONG biggestProgVal(void)
{
    if (wordMode == PROG_SIGNED)
    {
        if (wordSize == 8)
            return((PROG_LONG) 0x7F);
        else if (wordSize == 16)
            return((PROG_LONG) 0x7FFF);
        else
            return((PROG_LONG) 0x7FFFFFFF);
    }
    else
    {
        if (wordSize == 8)
            return((PROG_LONG) 0xFF);
        else if (wordSize == 16)
            return((PROG_LONG) 0xFFFF);
        else
            return((PROG_LONG) 0xFFFFFFFF);
    }
}

/**
 * Converts a 64-bit unsigned integer to a null-terminated binary string.
 * @param n: The 64-bit integer to convert.
 * @param out_str: A pointer to a char array of at least 65 bytes.
 */
void int64_to_binary(uint64_t n, char *out_str, uint8_t bits)
{
    int i;
    for (i = (bits-1); i >= 0; i--) {
        // Shift n right by 'i' positions and mask the last bit
        // Then add '0' to convert the bit (0 or 1) to its ASCII character ('0' or '1')
        out_str[(bits-1) - i] = ((uint8_t)(n >> i) & 1) + '0';
    }
    out_str[bits] = '\0'; // null-terminate
}

BOOL CALLBACK DlgProcIEEE(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[128];
    char tmp2[80];
    char tmp3[32];
    float  val_float = 0.0;
    double val_double = 0.0;
    uint32_t val32 = 0;
    uint64_t val64 = 0;

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        SendMessage(GetDlgItem(hDlg, 102), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        SetDlgItemText(hDlg, 101, "32-bit:  Enter Number Above - Click Enter");
        SetDlgItemText(hDlg, 102, "64-bit:  Enter Number Above - Click Enter");
        return TRUE;
        
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(105):           /* Enter */
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 25);
            
            val_float = (float)atof(tmp);
            val_double = (double)atof(tmp);
            
            memcpy(&val32, &val_float, 4);
            int64_to_binary(val32, tmp2, 32);
            sprintf(tmp, "32-bit:  0x%08X\n[%s]", (uint32_t)val32, tmp2);
            SetDlgItemText(hDlg, 101, tmp);
            
            memcpy(&val64, &val_double, 8);
            int64_to_binary(val64, tmp2, 64);
            sprintf(tmp3, "0x%016I64X", (uint64_t)val64);
            sprintf(tmp, "64-bit:  %s\n[%s]", tmp3, tmp2);
            SetDlgItemText(hDlg, 102, tmp);            
            return TRUE;

        case(IDOK):           /* OK - Close */
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

void Prog_IEEE(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_IEEE", calcMainWindow, DlgProcIEEE);
}

