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

// Programming externs 
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

uint8_t binOffset = 0; // We need to show more binary digits in windows of 16 bits

struct funcStruct CompSci_funcs[MAX_FUNCS] = {
    {FN1,   UNI_DEC,     USES_L,     ALLOWREC,   ' ',    "DEC",      YES_L,  X_NEW,      PROG_dec,       T_DEC,      H_DEC},
    {FN2,   UNI_HEX,     USES_L,     ALLOWREC,   ' ',    "HEX",      YES_L,  X_NEW,      PROG_hex,       T_HEX,      H_HEX},
    {FN3,   UNI_BIN,     USES_L,     ALLOWREC,   ' ',    "BIN",      YES_L,  X_NEW,      PROG_bin,       T_BIN,      H_BIN},
    {FN4,   UNI_OCT,     USES_L,     ALLOWREC,   ' ',    "OCT",      YES_L,  X_NEW,      PROG_oct,       T_OCT,      H_OCT},
    {FN5,   UNI_HEXA,    USES_L,     ALLOWREC,   'a',    "A",        NO_L,   X_NULL,     PROG_hexA,      T_HEXA,     H_HEXDIG},
    {FN6,   UNI_HEXB,    USES_L,     ALLOWREC,   'b',    "B",        NO_L,   X_NULL,     PROG_hexB,      T_HEXB,     H_HEXDIG},
    {FN7,   UNI_HEXC,    USES_L,     ALLOWREC,   'c',    "C",        NO_L,   X_NULL,     PROG_hexC,      T_HEXC,     H_HEXDIG},
    {FN8,   UNI_HEXD,    USES_L,     ALLOWREC,   'd',    "D",        NO_L,   X_NULL,     PROG_hexD,      T_HEXD,     H_HEXDIG},
    {FN9,   UNI_HEXE,    USES_L,     ALLOWREC,   'e',    "E",        NO_L,   X_NULL,     PROG_hexE,      T_HEXE,     H_HEXDIG},
    {FN10,  UNI_HEXF,    USES_L,     ALLOWREC,   'f',    "F",        NO_L,   X_NULL,     PROG_hexF,      T_HEXF,     H_HEXDIG},
    {FN11,  UNI_BINLO,   USES_L,     ALLOWREC,   ' ',    "Bin «",    YES_L,  X_NEW,      PROG_binHi,     T_BINH,     H_BINH},
    {FN12,  UNI_BINHI,   USES_L,     ALLOWREC,   ' ',    "Bin »",    YES_L,  X_NEW,      PROG_binLo,     T_BINL,     H_BINL},
    {FN13,  UNI_AND,     USES_L,     ALLOWREC,   ' ',    "AND",      YES_L,  X_NEW,      PROG_and,       T_AND,      H_AND},
    {FN14,  UNI_OR,      USES_L,     ALLOWREC,   ' ',    "OR",       YES_L,  X_NEW,      PROG_or,        T_OR,       H_OR},
    {FN15,  UNI_NOT,     USES_L,     ALLOWREC,   ' ',    "NOT",      YES_L,  X_NEW,      PROG_not,       T_NOT,      H_NOT},
    {FN16,  UNI_XOR,     USES_L,     ALLOWREC,   ' ',    "XOR",      YES_L,  X_NEW,      PROG_xor,       T_XOR,      H_XOR},
    {FN17,  UNI_NAND,    USES_L,     ALLOWREC,   ' ',    "NAND",     YES_L,  X_NEW,      PROG_nand,      T_NAND,     H_NAND},
    {FN18,  UNI_NOR,     USES_L,     ALLOWREC,   ' ',    "NOR",      YES_L,  X_NEW,      PROG_nor,       T_NOR,      H_NOR},
    {FN19,  UNI_2SCOMP,  USES_L,     ALLOWREC,   ' ',    "2's ",     YES_L,  X_NEW,      PROG_2sComp,    T_2COMP,    H_2COMP},
    {FN20,  UNI_ASR,     USES_L,     ALLOWREC,   ' ',    "ASR",      YES_L,  X_NEW,      PROG_ASR,       T_ASR,      H_ASR},
    {FN21,  UNI_SHL,     USES_L,     ALLOWREC,   ' ',    "SL",       YES_L,  X_NEW,      PROG_shl,       T_SL,       H_SL},
    {FN22,  UNI_SHR,     USES_L,     ALLOWREC,   ' ',    "SR",       YES_L,  X_NEW,      PROG_shr,       T_SR,       H_SR},
    {FN23,  UNI_ROL,     USES_L,     ALLOWREC,   ' ',    "RL",       YES_L,  X_NEW,      PROG_rol,       T_RL,       H_RL},
    {FN24,  UNI_ROR,     USES_L,     ALLOWREC,   ' ',    "RR",       YES_L,  X_NEW,      PROG_ror,       T_RR,       H_RR},
    {FN25,  UNI_SHLX,    USES_L,     ALLOWREC,   ' ',    "SL,x",     YES_L,  X_NEW,      PROG_shlX,      T_SLM,      H_SLM},
    {FN26,  UNI_SHRX,    USES_L,     ALLOWREC,   ' ',    "SR,x",     YES_L,  X_NEW,      PROG_shrX,      T_SRM,      H_SRM},
    {FN27,  UNI_RLX,     USES_L,     ALLOWREC,   ' ',    "RL,x",     YES_L,  X_NEW,      PROG_rlX,       T_RLX,      H_RLX},
    {FN28,  UNI_RRX,     USES_L,     ALLOWREC,   ' ',    "RR,x",     YES_L,  X_NEW,      PROG_rrX,       T_RRX,      H_RRX},
    {FN29,  UNI_MOD,     USES_L,     ALLOWREC,   ' ',    "MOD",      YES_L,  X_NEW,      PROG_mod,       T_MOD,      H_MOD},
    {FN30,  UNI_NUMB,    USES_L,     ALLOWREC,   ' ',    "#B",       YES_L,  X_NEW,      PROG_NumBits,   T_NUMBITS,  H_NUMBITS},
    {FN31,  UNI_SETB,    USES_L,     ALLOWREC,   ' ',    "SB",       YES_L,  X_NEW,      PROG_SetBit,    T_SETBIT,   H_SETBIT},
    {FN32,  UNI_CLRB,    USES_L,     ALLOWREC,   ' ',    "CB",       YES_L,  X_NEW,      PROG_ClrBit,    T_CLRBIT,   H_CLRBIT},
    {FN33,  UNI_MASKL,   USES_L,     ALLOWREC,   ' ',    "MaskL",    YES_L,  X_NEW,      PROG_MaskL,     T_MASKL,    H_MASKL},
    {FN34,  UNI_MASKR,   USES_L,     ALLOWREC,   ' ',    "MaskR",    YES_L,  X_NEW,      PROG_MaskR,     T_MASKR,    H_MASKR},
    {FN35,  UNI_MINW,    USES_L,     ALLOWREC,   ' ',    "MINW",     YES_L,  X_NEW,      PROG_MinW,      T_MINW,     H_MINW},
    {FN36,  UNI_MAXW,    USES_L,     ALLOWREC,   ' ',    "MAXW",     YES_L,  X_NEW,      PROG_MaxW,      T_MAXW,     H_MAXW},
    {FN37,  UNI_ASCII,   USES_L,     ALLOWREC,   ' ',    "ASCII",    NO_L,   X_NEW,      PROG_ascii,     T_ASCII,    H_ASCII},
    {FN38,  UNI_WSIZE,   USES_L,     ALLOWREC,   ' ',    "WSIZE",    YES_L,  X_NEW,      PROG_WordSize,  T_WORDSIZE, H_WORDSIZE},
    {FN39,  UNI_MIRROR,  USES_L,     ALLOWREC,   ' ',    "Mirror",   YES_L,  X_NEW,      PROG_Mirror,    T_MIRROR,   H_MIRROR},
    {FN40,  UNI_IEEE,    USES_L,     ALLOWREC,   ' ',    "IEEE",     YES_L,  X_NEW,      Prog_IEEE,      T_IEEE,     H_IEEE}
};


void PutHexSpaces(char *str)
{
    int i;
    int group;
    int count;
    int out_len;
    char tmp2[80];
    int len;

    if (hexSpacing == HEX_SPACE_NONE)
        return;

    if (hexSpacing == HEX_SPACE_4)
        group = 4;
    else
        group = 2;

    len = (int) strlen(str);
    out_len = 0;
    count = 0;

    for (i = len - 1; i >= 0; i--)
    {
        if (count == group)
        {
            tmp2[out_len++] = ' ';
            count = 0;
        }
        tmp2[out_len++] = str[i];
        count++;
    }

    if ((out_len > 0) && (tmp2[out_len - 1] == ' '))
        out_len--;

    for (i = 0; i < out_len; i++)
        str[i] = tmp2[out_len - i - 1];

    str[out_len] = CNULL;
}

// ---------------------------------------------------------------------------------------------------
// This routine takes a long value and converts it to a string based on the current programming mode.
// It also adds spaces for hex mode and commas for decimal mode as needed.  The resulting string is 
// returned in tmpL which should be at least 60 characters long to be safe.  This routine is used 
// by the computer science mode display and also by the stack display when in Comp-Sci mode.
// ---------------------------------------------------------------------------------------------------
void MakeCompSciStr(PROG_LONG val, char *tmpL)
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
                sprintf(tmpL, "%02I64X", val);
            else if (wordSize == 16)
                sprintf(tmpL, "%04I64X", val);
            else if (wordSize == 32)
                sprintf(tmpL, "%08I64X", val);
            else
                sprintf(tmpL, "%016I64X", val);
        }
        else
        {
            sprintf(tmpL, "%I64X", val);
        }
        PutHexSpaces(tmpL);
    }
    else if (progMode == PROG_BIN)
    {
        val = val >> binOffset; // Shift the 16-bits we wish to show

        _i64toa(val & 0xFFFF, temp4, progMode);
        if (padZeros == 1)
        {
            temp[17] = CNULL;
            if (wordSize == 8)
                sprintf(temp, "%08s", temp4);
            else
                sprintf(temp, "%016s", temp4);
        }
        else
        {
            sprintf(temp, "%16s", temp4);
        }
        if ((val & 0xFFFF0000) != 0)
        {
            for (i = 0; i < (int) strlen(temp); i++)
                if (temp[i] == ' ')
                    temp[i] = '0';
        }
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
        {
            j--;
        }
        temp2[j] = CNULL;
        for (i = 0; i < j; i++)
        {
            temp3[i] = temp2[j - i - 1];
        }
        temp3[j - 2] = CNULL;

        if ((val & 0xFFFF0000) != 0 || (padZeros == 1 && binOffset == 0))
            sprintf(tmpL, "«%s", temp3);    // Left Arrow!
        else
        {
            sprintf(tmpL, "%s", temp3);
        }
    }
    else if (progMode == PROG_DEC)
    {
        if (wordMode == COMPSCI_SIGNED)
            sprintf(tmpL, "%I64d", val);
        else
            sprintf(tmpL, "%I64u", val);
        if (strlen(tmpL) < 18) // Above this we can't fit commas
        {
            PutCommas(tmpL);
        }
    }
    else
    {
        _i64toa(val, tmpL, progMode);     // Lonely OCTal
    }
}


PROG_LONG ConvertCompSciStrTo64(char *str)
{
    char *tmpPtr;
    if (str[0] == '0' && toupper(str[1]) == 'X') // If user entered 0x we treat as HEX
        return(strtoi64(str, &tmpPtr, PROG_HEX));
    else
        return(strtoi64(str, &tmpPtr, progMode)); // Otherwise use the selected Comp-Sci mode
}

PROG_LONG MaskStack(PROG_LONG lng)
{
    PROG_LONG mask;

    lng = lng & wordSizeMask;

    mask = 0x00000001L << (wordSize - 1);

    if ((progMode == PROG_DEC) && (lng & mask))
    {
        if (wordMode == COMPSCI_SIGNED)
            lng |= ~wordSizeMask;
    }
    return(lng);
}

void StackPushL(PROG_LONG temp)
{
    stackPushes++;

    if (extendedStack)
    {
        STACKL[STK_D] = MaskStack(STACKL[STK_C]);
        STACKL[STK_C] = MaskStack(STACKL[STK_B]);
        STACKL[STK_B] = MaskStack(STACKL[STK_A]);
        STACKL[STK_A] = MaskStack(STACKL[STK_T]);
    }

    STACKL[STK_T] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_X]);
    STACKL[STK_X] = MaskStack(temp);

    LongsToFloats(); // Keep the floating stack up-to-date
}

PROG_LONG StackPopL(void)
{
    PROG_LONG temp;
    
    stackPops++;

    temp = MaskStack(STACKL[STK_X]);
    STACKL[STK_X] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_T]);
    
    if (extendedStack)
    {
        STACKL[STK_T] = MaskStack(STACKL[STK_A]);
        STACKL[STK_A] = MaskStack(STACKL[STK_B]);
        STACKL[STK_B] = MaskStack(STACKL[STK_C]);
        STACKL[STK_C] = MaskStack(STACKL[STK_D]);
        if (popFillZero)
        {
            STACKL[STK_D] = MaskStack(0L);
        }
    }
    else
    {
        if (popFillZero)
        {
            STACKL[STK_T] = MaskStack(0L);
        }
    }

    LongsToFloats(); // Keep the floating stack up-to-date

    return(temp);
}



// ----------------------- 
// Programming mode stuff  
// ----------------------- 
void PROG_hex(void)
{
    progMode = PROG_HEX;
    lastProgMode = PROG_HEX;
    STACKL[STK_X] = MaskStack(STACKL[STK_X]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_T] = MaskStack(STACKL[STK_T]);
}

void PROG_bin(void)
{
    progMode = PROG_BIN;
    lastProgMode = PROG_BIN;
    binOffset = 0;
    STACKL[STK_X] = MaskStack(STACKL[STK_X]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_T] = MaskStack(STACKL[STK_T]);
}

void PROG_oct(void)
{
    progMode = PROG_OCT;
    lastProgMode = PROG_OCT;
    STACKL[STK_X] = MaskStack(STACKL[STK_X]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_T] = MaskStack(STACKL[STK_T]);
}

void PROG_dec(void)
{
    progMode = PROG_DEC;
    lastProgMode = PROG_DEC;
    STACKL[STK_X] = MaskStack(STACKL[STK_X]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_T] = MaskStack(STACKL[STK_T]);
}

void process_hex_digit(char *hexDigit)
{
    if (progMode == PROG_HEX)
    {
        if (Xedit == X_NEW)
        {
            StackPushL(0L);
            strcpy(Xstr, hexDigit);
        }
        else if (Xedit == X_ENTER)
        {
            sprintf(Xstr, hexDigit);
        }
        else
        {
            if (allowDigitBasedOnMaxStringSize(Xstr, hexDigit[0]))
            {
                strcat(Xstr, hexDigit);
            }
        }
        STACKL[STK_X] = ConvertCompSciStrTo64(Xstr);
        Xedit = X_EDIT;
    }
}

void PROG_hexA(void) { process_hex_digit("A"); }
void PROG_hexB(void) { process_hex_digit("B"); }
void PROG_hexC(void) { process_hex_digit("C"); }
void PROG_hexD(void) { process_hex_digit("D"); }
void PROG_hexE(void) { process_hex_digit("E"); }
void PROG_hexF(void) { process_hex_digit("F"); }


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
    PROG_LONG xl = StackPopL();
    progModeOverflow = 0;
    if (xl & (1 << (wordSize - 1))) progModeCarry = 1; else progModeCarry = 0;
    StackPushL(xl << 1);     // this always shifts in a zero
}

void PROG_shr(void)
{
    PROG_LONG mask;
    PROG_LONG val;
    PROG_LONG xl = StackPopL();

    progModeOverflow = 0;
    if (xl & 1) progModeCarry = 1; else progModeCarry = 0;
    val = (xl >> 1);
    mask = 1 << (wordSize - 1);
    val &= (PROG_LONG) ~ mask;  // Always shift in a zero...
    StackPushL(val);
}

void PROG_rol(void)
{
    PROG_LONG r;
    PROG_LONG rotatedBit;

    r = StackPopL();                    // Get the value to be rotated from our stack.

    progModeOverflow = 0;
    if (rotateThroughCarry)
    {
        rotatedBit = (r & (1 << (wordSize - 1))) >> (wordSize - 1);   // Get the bit that will be rotated around to the LSBit.
        r = (r << 1) | progModeCarry;                                 // Shift left and OR in the carry bit
        progModeCarry = (uint8_t)rotatedBit;                          // The bit that was rotated off the word goes into Carry
    }
    else
    {
        rotatedBit = (r & (1 << (wordSize - 1))) >> (wordSize - 1);   // Get the bit that will be rotated around to the LSBit.
        r = (r << 1) | rotatedBit;                                    // Shift left and OR in the bit that was rotated around to the LSBit.
        progModeCarry = (uint8_t)rotatedBit;                          // The bit that was rotated around goes into Carry
    }

    StackPushL(r);
}

void PROG_ror(void)
{
    PROG_LONG r;
    PROG_LONG rotatedBit;

    r = StackPopL();                    // Get the value to be rotated from our stack.

    progModeOverflow = 0;
    if (rotateThroughCarry)
    {
        rotatedBit = (r & 1);                           // Get the bit that will be rotated around to the MSBit.
        r = (r >> 1) | progModeCarry << (wordSize - 1); // Shift right and OR in the carry bit 
        progModeCarry = (uint8_t)rotatedBit;            // The bit that was rotated off the word goes into Carry
    }
    else
    {
        rotatedBit = (r & 1);                           // Get the bit that will be rotated around to the MSBit.
        r = (r >> 1) | rotatedBit << (wordSize - 1);    // Shift right and OR in the bit that was rotated around to the MSBit.
        progModeCarry = (uint8_t)rotatedBit;            // The bit that was rotated around goes into Carry
    }

    StackPushL(r);
}

void PROG_mod(void)
{
    PROG_LONG l1, l2;
    if (STACKL[STK_X] == 0L)
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
            ltoa(i, tmp2, 2);  // Convert to binary
            sprintf(tmp3, "%8s", tmp2);
            for (j = 0; j < (int) strlen(tmp3); j++)
                if (tmp3[j] == ' ')
                    tmp3[j] = '0';

            ltoa(i, tmp2, 8);  // Convert to binary
            sprintf(tmp4, "%3s", tmp2);
            for (j = 0; j < (int) strlen(tmp4); j++)
                if (tmp4[j] == ' ')
                    tmp4[j] = '0';
            sprintf(tmp, "%3s  %3d   %02X   %8s  %3s", ASCII_Str(i), i, i, tmp3, tmp4);
            SendDlgItemMessage(hDlg, 101, LB_ADDSTRING, 0, (LONG) ((LPSTR) tmp));
        }
        i = (unsigned char) (STACKL[STK_X] & 0x000000FF);
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
    if (binOffset == 0)  binOffset = 16;
    else if (wordSize > 32)
    {
        if (binOffset == 16) binOffset = 32;
        else if (binOffset == 32) binOffset = 48;
    }
    STACKL[STK_X] = MaskStack(STACKL[STK_X]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_T] = MaskStack(STACKL[STK_T]);
}

void PROG_binLo(void)
{
    progMode = PROG_BIN;
         if (binOffset == 16) binOffset = 0;
    else if (binOffset == 32) binOffset = 16;
    else if (binOffset == 48) binOffset = 32;
    STACKL[STK_X] = MaskStack(STACKL[STK_X]);
    STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
    STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
    STACKL[STK_T] = MaskStack(STACKL[STK_T]);
}

void PROG_shlX(void)
{
    // Limit to 16-bit... no sense otherwise
    int16_t num = (int16_t)StackPopL();

    while (num > 0)
    {
        PROG_shl();
        num--;
    }
}


void PROG_shrX(void)
{
    // Limit to 16-bit... no sense otherwise
    int16_t num = (int16_t)StackPopL();

    while (num > 0)
    {
        PROG_shr();
        num--;
    }
}


void PROG_rlX(void)
{
    // Limit to 16-bit... no sense otherwise
    int16_t num = (int16_t)StackPopL();
    
    while (num > 0)
    {
        PROG_rol();
        num--;
    }
}

void PROG_rrX(void)
{
    // Limit to 16-bit... no sense otherwise
    int16_t num = (int16_t)StackPopL();
    
    while (num > 0)
    {
        PROG_ror();
        num--;
    }
}

void PROG_ASR(void)
{
    PROG_LONG val;
    PROG_LONG mask;

    mask = 0x00000001L << (wordSize - 1);

    val = StackPopL();

    progModeOverflow = 0;
    if (val & 1) progModeCarry = 1; else progModeCarry = 0;

    if (wordMode == COMPSCI_SIGNED)
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
    for (shiftVal = 0; shiftVal < (PROG_LONG) wordSize; shiftVal++)
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
    RPN_negate_x(); // 2's complement is same as CHS
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
            SendMessage(GetDlgItem(hDlg, 107), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else
            SendMessage(GetDlgItem(hDlg, 108), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (wordSize == 8)
            SendMessage(GetDlgItem(hDlg, 101), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (wordSize == 16)
            SendMessage(GetDlgItem(hDlg, 102), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (wordSize == 32)
            SendMessage(GetDlgItem(hDlg, 103), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else // wordSize is 64
            SendMessage(GetDlgItem(hDlg, 104), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (wordMode == COMPSCI_SIGNED)
            SendMessage(GetDlgItem(hDlg, 105), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else // COMPSCI_UNSIGNED
            SendMessage(GetDlgItem(hDlg, 106), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (hexSpacing == HEX_SPACE_NONE)
            SendMessage(GetDlgItem(hDlg, 111), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else if (hexSpacing == HEX_SPACE_2)
            SendMessage(GetDlgItem(hDlg, 112), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        else // HEX_SPACE_4
            SendMessage(GetDlgItem(hDlg, 113), BM_SETCHECK, (WORD) 1, (DWORD) 0L);

        if (rotateThroughCarry)
        {
            SendMessage(GetDlgItem(hDlg, 114), BM_SETCHECK, (WORD) 1, (DWORD) 0L);
        }

        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case(IDOK):           // OK
            bs = SendMessage(GetDlgItem(hDlg, 107), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                padZeros = 0;
            else
                padZeros = 1;

            bs = SendMessage(GetDlgItem(hDlg, 105), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
                wordMode = COMPSCI_SIGNED;
            else
                wordMode = COMPSCI_UNSIGNED;

            bs = SendMessage(GetDlgItem(hDlg, 101), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 8;
                wordSizeMask = 0x000000FFL;
            }

            bs = SendMessage(GetDlgItem(hDlg, 102), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 16;
                wordSizeMask = 0x0000FFFFL;
            }

            bs = SendMessage(GetDlgItem(hDlg, 103), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 32;
                wordSizeMask = 0xFFFFFFFFL;
            }

            bs = SendMessage(GetDlgItem(hDlg, 104), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                wordSize = 64;
                wordSizeMask = (uint64_t) 0xFFFFFFFFFFFFFFFF;
            }

            bs = SendMessage(GetDlgItem(hDlg, 111), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                hexSpacing = HEX_SPACE_NONE;
            }

            bs = SendMessage(GetDlgItem(hDlg, 112), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                hexSpacing = HEX_SPACE_2;
            }

            bs = SendMessage(GetDlgItem(hDlg, 113), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                hexSpacing = HEX_SPACE_4;
            }

            bs = SendMessage(GetDlgItem(hDlg, 114), BM_GETCHECK, (WORD) 0, (DWORD) 0L);
            if (bs != 0L)
            {
                rotateThroughCarry = 1;
            }
            else 
            {
                rotateThroughCarry = 0;
            }

            EndDialog(hDlg, FALSE);
            binOffset = 0;
            ShowStatus();

            Xedit = X_NEW;
            STACKL[STK_T] = MaskStack(STACKL[STK_T]);
            STACKL[STK_Z] = MaskStack(STACKL[STK_Z]);
            STACKL[STK_Y] = MaskStack(STACKL[STK_Y]);
            STACKL[STK_X] = MaskStack(STACKL[STK_X]);
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
    for (shiftVal = 0; shiftVal < (PROG_LONG) wordSize; shiftVal++)
    {
        mask1 = 1L << shiftVal;
        mask2 = 1L << (wordSize - shiftVal - 1);
        if ((mask1 & xTmp) != 0)
            newVal = newVal | mask2;
    }
    StackPushL((PROG_LONG) newVal);
}

PROG_LONG smallestProgVal(void)
{
    if (wordMode == COMPSCI_SIGNED)
    {
        if (wordSize == 8)
            return((PROG_LONG) 0x80);
        else if (wordSize == 16)
            return((PROG_LONG) 0x8000);
        else if (wordSize == 32)
            return((PROG_LONG) 0x80000000);
        else 
            return((PROG_LONG) 0x8000000000000000);
    }
    else
    {
        return((PROG_LONG) 0x00000000);
    }
}

PROG_LONG biggestProgVal(void)
{
    if (wordMode == COMPSCI_SIGNED)
    {
        if (wordSize == 8)
            return((PROG_LONG) 0x7F);
        else if (wordSize == 16)
            return((PROG_LONG) 0x7FFF);
        else  if (wordSize == 32)
            return((PROG_LONG) 0x7FFFFFFF);
        else
            return((PROG_LONG) 0x7FFFFFFFFFFFFFFF);
    }
    else
    {
        if (wordSize == 8)
            return((PROG_LONG) 0xFF);
        else if (wordSize == 16)
            return((PROG_LONG) 0xFFFF);
        else if (wordSize == 32)
            return((PROG_LONG) 0xFFFFFFFF);
        else
            return((PROG_LONG) 0xFFFFFFFFFFFFFFFF);
    }
}

void PROG_MinW(void)
{
    StackPushL(smallestProgVal());
}

void PROG_MaxW(void)
{
    StackPushL(biggestProgVal());
}

// --------------------------------------------------------------------------
// Converts a 64-bit unsigned integer to a null-terminated binary string.
// @param n: The 64-bit integer to convert.
// @param out_str: A pointer to a char array of at least 65 bytes.
// --------------------------------------------------------------------------
void int64_to_binary_str(uint64_t n, char *out_str, uint8_t bits)
{
    int i;
    int idx=0;
    for (i = (bits-1); i >= 0; i--) 
    {
        if (bits == 32)
        {
            if (i==(bits-1)) {out_str[idx++] = '[';}
            if (i==(bits-2)) {out_str[idx++] = ']'; out_str[idx++] = ' '; out_str[idx++] = '[';}
            if (i==(bits-10)) {out_str[idx++] = ']'; out_str[idx++] = ' '; out_str[idx++] = '[';}
        }
        else // 64 bits
        {
            if (i==(bits-1)) {out_str[idx++] = '[';}
            if (i==(bits-2)) {out_str[idx++] = ']'; out_str[idx++] = ' '; out_str[idx++] = '[';}
            if (i==(bits-13)) {out_str[idx++] = ']'; out_str[idx++] = ' '; out_str[idx++] = '[';}
        }
        
        // Shift n right by 'i' positions and mask the last bit
        // Then add '0' to convert the bit (0 or 1) to its ASCII character ('0' or '1')
        out_str[idx++] = ((uint8_t)(n >> i) & 1) + '0';
    }
    out_str[idx++] = ']';
    out_str[idx] = '\0'; // null-terminate
}

BOOL CALLBACK DlgProcIEEE(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[128];
    char tmp2[80];
    char tmp3[32];
    float  val_float = 0.0;
    double val_double = 0.0;
    static uint32_t ieee_val32 = 0;
    static uint64_t ieee_val64 = 0;
    HDC hdc;
    SIZE lpSize;
    RECT rect;
    
    switch(wMessage)
    {
    case WM_INITDIALOG:
        SendMessage(GetDlgItem(hDlg, 101), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        SendMessage(GetDlgItem(hDlg, 102), WM_SETFONT, (WPARAM) hFixedFont, FALSE);
        
        ieee_val32 = 0;
        ieee_val64 = 0;
        hdc = GetDC(hDlg);
        sprintf(tmp2, "%f", lastFloat);
        SetDlgItemText(hDlg, IDC_EDIT1, tmp2);
        SelectObject(hdc, hFixedFont);
        (void)GetTextExtentPoint32(hdc, "[0] [00000000000] [00000000000000000000000000000000000000000000000000000]", 73, &lpSize);
        ReleaseDC(hDlg, hdc);
        if (GetWindowRect(hDlg, &rect))
        {
            int height = rect.bottom - rect.top;
            MoveWindow(hDlg, main_x+10, main_y+50, lpSize.cx + 40, height, TRUE);
        }        
        SetDlgItemText(hDlg, 101, "32-bit:  Enter Number Above - Click Convert");
        SetDlgItemText(hDlg, 102, "64-bit:  Enter Number Above - Click Convert");
        return TRUE;
        
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case(105):           // Convert 
            GetDlgItemText(hDlg, IDC_EDIT1, tmp, 25);
            
            val_float = (float)atof(tmp);
            val_double = (double)atof(tmp);
            
            memcpy(&ieee_val32, &val_float, 4);
            int64_to_binary_str(ieee_val32, tmp2, 32);
            sprintf(tmp, "32-bit:  0x%08X\n%s", (uint32_t)ieee_val32, tmp2);
            SetDlgItemText(hDlg, 101, tmp);
            
            memcpy(&ieee_val64, &val_double, 8);
            int64_to_binary_str(ieee_val64, tmp2, 64);
            sprintf(tmp3, "0x%016I64X", (uint64_t)ieee_val64);
            sprintf(tmp, "64-bit:  %s\n%s", tmp3, tmp2);
            SetDlgItemText(hDlg, 102, tmp);            
            return TRUE;

        case(106):           // Push 4-byte
            StackPushL(ieee_val32);
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(107):           // Push 8-byte
            StackPushL(ieee_val64);
            EndDialog(hDlg, FALSE);
            return TRUE;

        case(IDOK):           // OK - Close 
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

PROG_LONG CompSciAdd(PROG_LONG xl, PROG_LONG yl)
{
    if (wordMode == COMPSCI_SIGNED)
    {
        int64_t a = (int64_t)xl;
        int64_t b = (int64_t)yl;
        int64_t r = a+b;
        if (((a ^ r) & (b ^ r)) & (1 << (wordSize-1)))
        {
            progModeOverflow = 1;
        }
        else
        {
            progModeOverflow = 0;
        }

        if (r < a) progModeCarry = 1; else progModeCarry = 0;
    }
    else // Unsigned (Carry same as Overflow)
    {
        uint64_t a = (uint64_t)xl;
        uint64_t b = (uint64_t)yl;
        uint64_t r = (a+b) & wordSizeMask;
        if (r < a)
        {
            progModeOverflow = 1;
            progModeCarry = 1;
        }
        else
        {
            progModeOverflow = 0;
            progModeCarry = 0;
        }
    }

    return xl+yl;
}

PROG_LONG CompSciSub(PROG_LONG xl, PROG_LONG yl)
{
    if (wordMode == COMPSCI_SIGNED)
    {
        int64_t a = (int64_t)xl;
        int64_t b = (int64_t)yl;
        int64_t r = a+b;
        if (((a ^ b) & (1 << (wordSize-1))) && ((r ^ a) & (1 << (wordSize-1))))
        {
            progModeOverflow = 1;
        }
        else
        {
            progModeOverflow = 0;
        }

        if (r < a) progModeCarry = 1; else progModeCarry = 0;
    }
    else // Unsigned - carry same as overflow
    {
        uint64_t a = (uint64_t)xl;
        uint64_t b = (uint64_t)yl;
        if (a > b)
        {
            progModeOverflow = 1;
            progModeCarry = 1;
        }
        else
        {
            progModeOverflow = 0;
            progModeCarry = 0;
        }
    }

    return yl-xl;
}

uint8_t will_unsigned_multiply_overflow(uint64_t a, uint64_t b) {
    // 0 multiplied by anything is 0, so it never overflows
    if (a == 0 || b == 0) {
        return 0;
    }

    // UINT64_MAX is (2^64 - 1)
    return (b > (UINT64_MAX / a)) ? 1:0;
}

uint8_t will_signed_multiply_overflow(int64_t a, int64_t b) {
    // Base cases for 0 and 1
    if (a == 0 || b == 0) return 0;
    if (a == 1 || b == 1) return 0;

    // Handle the dangerous -1 edge case because INT64_MIN / -1 overflows
    if (a == -1) return (b == INT64_MIN) ? 1:0;
    if (b == -1) return (a == INT64_MIN) ? 1:0;

    if (a > 0) {
        if (b > 0) {
            // Positive * Positive
            return (b > (INT64_MAX / a)) ? 1:0;
        } else {
            // Positive * Negative
            return (b < (INT64_MIN / a)) ? 1:0;
        }
    } else {
        if (b > 0) {
            // Negative * Positive
            return (a < (INT64_MIN / b)) ? 1:0;
        } else {
            // Negative * Negative (result must be <= INT64_MAX)
            // Since both are negative, 'INT64_MAX / a' is negative.
            // If b is smaller (more negative) than that, it overflows.
            return (b < (INT64_MAX / a)) ? 1:0;
        }
    }
}

PROG_LONG CompSciMul(PROG_LONG xl, PROG_LONG yl)
{
    progModeOverflow = 0;

    if (wordMode == COMPSCI_SIGNED)
    {
        int64_t a = (int64_t)xl;
        int64_t b = (int64_t)yl;
        int64_t r = a * b;
        if (wordSize == 64) progModeOverflow = will_signed_multiply_overflow(a,b);
        if (wordSize == 32) if ((r > (int32_t)0x7FFFFFFF) || (r < (int32_t)0x80000000)) progModeOverflow = 1;
        if (wordSize == 16) if ((r > (int16_t)0x7FFF) || (r < (int16_t)0x8000)) progModeOverflow = 1;
        if (wordSize ==  8) if ((r > (int8_t)0x7F) || (r < (int8_t)0x80)) progModeOverflow = 1;
    }
    else
    {
        uint64_t a = (uint64_t)xl;
        uint64_t b = (uint64_t)yl;
        uint64_t r = a * b;
        if (wordSize == 64) progModeOverflow = will_unsigned_multiply_overflow(a,b);
        if (wordSize == 32) if (r > (uint64_t)0xFFFFFFFF) progModeOverflow = 1;
        if (wordSize == 16) if (r > (uint64_t)0xFFFF) progModeOverflow = 1;
        if (wordSize ==  8) if (r > (uint64_t)0xFF) progModeOverflow = 1;
    }
    
    return xl * yl;
}

PROG_LONG CompSciDiv(PROG_LONG xl, PROG_LONG yl)
{
    PROG_SIGNEDLONG sxl, syl;

    progModeOverflow = 0;
    if (wordMode == COMPSCI_SIGNED)
    {
        sxl = (PROG_SIGNEDLONG)xl;
        syl = (PROG_SIGNEDLONG)yl;
        progModeCarry = (syl % sxl) ? 1:0; // If it doesn't divide evenly... Carry
        return (syl / sxl);
    }
    else
    {
        progModeCarry = (yl % xl) ? 1:0; // If it doesn't divide evenly... Carry
        return (yl / xl);
    }
    return 0L; // Never reached
}
