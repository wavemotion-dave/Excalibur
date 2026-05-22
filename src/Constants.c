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
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <mmsystem.h>
#include <ctype.h>
#include <float.h>
#include <signal.h>
#include <shlobj.h>
#include "Excal.h"

void PushConstant(double value)
{
    char tmpStr[32];
    int i, startPos;

    if (Xedit == X_ENTER)
        X = (double) value;
    else
        StackPush((double) value);

    //@TODO convert this to a simple UNI_xxx value
    if (recModeON == 1) // If recording, generate key sequence to playback struct
    {
        sprintf(tmpStr, "%.13g", value);
        startPos = 0;
        if (tmpStr[0] == '-')
        {
            startPos=1;
        }
        for (i=startPos; i<(int)strlen(tmpStr); i++)
        {
            switch(tmpStr[i])
            {
                case('.'): SaveProgramStep(UNI_DIGDP); break;
                case('0'): SaveProgramStep(UNI_DIG0);  break;
                case('1'): SaveProgramStep(UNI_DIG1);  break;
                case('2'): SaveProgramStep(UNI_DIG2);  break;
                case('3'): SaveProgramStep(UNI_DIG3);  break;
                case('4'): SaveProgramStep(UNI_DIG4);  break;
                case('5'): SaveProgramStep(UNI_DIG5);  break;
                case('6'): SaveProgramStep(UNI_DIG6);  break;
                case('7'): SaveProgramStep(UNI_DIG7);  break;
                case('8'): SaveProgramStep(UNI_DIG8);  break;
                case('9'): SaveProgramStep(UNI_DIG9);  break;
                case('e'): //no break...
                case('E'):
                    if (startPos == 1)
                    {
                        startPos = 2;
                        SaveProgramStep(UNI_CHS);
                    }
                    SaveProgramStep(UNI_E);
                    break;
                case('-'): SaveProgramStep(UNI_CHS); break;
            }
        }
        if (startPos == 1)
        {
            startPos = 2;
            SaveProgramStep(UNI_CHS);
        }
        SaveProgramStep(UNI_ENDCONST);
    }
}

BOOL CALLBACK fnDIALOG_REDEFINECONST(HWND, UINT, WPARAM, LPARAM);
char constName[26];
char constUnits[11];
double constVal;
int cancelRedefineConst = 0;
void RedefineConst(void)
{
    DLGPROC lpfnDIALOG_REDEFINECONST;

    lpfnDIALOG_REDEFINECONST = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_REDEFINECONST, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_REDEFINE_CONST", calcMainWindow, lpfnDIALOG_REDEFINECONST)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_REDEFINECONST);
}

BOOL CALLBACK fnDIALOG_REDEFINECONST(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    char tmp[64];

    switch(wMessage)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT1, constName);
        SetDlgItemText(hDlg, IDC_EDIT2, constUnits);
        sprintf(tmp, "%.13g", constVal);
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
            sprintf(tmp, "%.13g", constVal); // Make the Number the correct # significant digits...
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
struct constTableStruct constants[MAX_CONST_BANKS][MAX_CONSTS] = 
{
    
    // Bank 1
    {
        {TRUE,  "Acceleration Gravity",         "m/s²",          9.80665},
        {TRUE,  "Atomic Mass Unit",             "kg",            1.6606E-27},
        {TRUE,  "Astronomical Unit",            "m",             1.495978707E11},
        {TRUE,  "Avogadro's Number",            "mol",           6.022045E+23},
        {TRUE,  "Bohr Radius",                  "m",             5.292E-11},
        {TRUE,  "Boltzman Constant",            "JK-1",          1.380622E-23},
        {TRUE,  "Coulomb Constant",             "N m²/C²",       8.9875517923E9},
        {TRUE,  "Earth Mass",                   "kg",            5.97219E24},
        {TRUE,  "Earth Radius",                 "m",             6.371E6},
        {TRUE,  "Electron Charge",              "col",           1.6021892E-19},
        {TRUE,  "Electron Mass",                "kg",            9.109534E-31},
        {TRUE,  "Electron-Proton Ratio",        "",              1836.1},
        {TRUE,  "Electron Volt",                "J",             1.602E-19},
        {TRUE,  "Euler-Mascheroni Constant",    "",              0.5772156649015329},
        {TRUE,  "Euler's Number",               "",              2.718281828459045},
        {TRUE,  "Faraday's Constant",           "col",           9.65E4},
        {TRUE,  "Fine-Structure Constant",      "",              7.2973525693E-3},
        {TRUE,  "Golden Ratio",                 "",              1.618033988749895},
        {TRUE,  "Gravitational Constant",       "m³/kg·s²",      6.67430E-11},
        {TRUE,  "Ideal Gas Volume @ STP",       "l/mol",         22.4136},
        {TRUE,  "Light-Year",                   "m",             9.460730472E15},
        {TRUE,  "ln(2)",                        "",              0.6931471805599453},
        {TRUE,  "Parsec",                       "m",             3.085677581E16},
        {TRUE,  "Pi",                           "",              M_PI},
        {TRUE,  "Plank's Constant",             "Js",            6.626E-34},
        {TRUE,  "Proton Mass",                  "kg",            1.6722E-27},
        {TRUE,  "Reduced Planck Constant",      "J s",           1.054571817E-34},
        {TRUE,  "Rydberg Constant",             "m^-1",          1.0973731568508E7},
        {TRUE,  "Sqrt 2",                       "",              1.4142135623730951},
        {TRUE,  "Sqrt 3",                       "",              1.7320508075688772},
        {TRUE,  "Speed Of Light",               "m/s",           299792458.0},
        {TRUE,  "Standard Atmosphere",          "Pa",            101325.0},
        {TRUE,  "Stefan-Boltzmann Constant",    "W/m² K^4",      5.670374419E-8},
        {TRUE,  "Unit Atomic Energy",           "MeV",           931.34},
        {TRUE,  "Universal Answer",             "",              42.0},
        {TRUE,  "Universal Gas Constant",       "J/mol K",       8.314462618},
        {TRUE,  "Vacuum Permittivity",          "F/m",           8.8541878128E-12},
        {TRUE,  "Vacuum Permeability",          "N/A²",          1.2566370614E-6},
        {FALSE, "None",                         "",              0.00}  // end of list...
    },
     
    // Bank 2 
    {
        {FALSE, "None",                     "",             0.00}  // end of list...
    },                                                
    
    // Bank 3
    {                                                  
        {FALSE, "None",                     "",             0.00}  // end of list...
    },                                                
    
    // Bank 4
    {                                                  
        {FALSE, "None",                     "",             0.00}  // end of list...
    },                                                
    
    // Bank 5
    {                                                  
        {FALSE, "None",                     "",             0.00}  // end of list...
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
        case(IDOK):           // OK - Close 
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
        
        case(IDCANCEL):       // Cancel 
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


extern BOOL CALLBACK fnDIALOG_Constants(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam);
void RPN_const(void)
{
    DLGPROC lpfnDIALOG_ConstantsProc;

    lpfnDIALOG_ConstantsProc = (DLGPROC) MakeProcInstance((FARPROC) fnDIALOG_Constants, hExcaliburInstance);

    if ((DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_CONSTANTS", calcMainWindow, lpfnDIALOG_ConstantsProc)) == -1)
    {
        MessageBox(NULL, "Unable to display dialog", "System Error", MB_SYSTEMMODAL | MB_ICONHAND | MB_OK);
    }
    FreeProcInstance((FARPROC) lpfnDIALOG_ConstantsProc);
}

BOOL CALLBACK fnDIALOG_Constants(HWND hDlg, UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    int i, j;
    char tmp[64];
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
                sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
            // Else fall through!!!! 
        case(102):            // Insert constant number 
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
        case(105):            // Add New Constant 
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
        case(104):            // Redefine Constant 
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
                            sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
                            sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
                    sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
                    sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
                    sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
                    sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
                    sprintf(tmp, "%-25s\t%-14.13g \t%s",
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
        case(107):            // Redefine Bank Names 
            DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_CONSTANT_BANK_NAMES", hDlg, constBankNamesProc);
            SetDlgItemText(hDlg, IDC_RADIO1, constantBankNames[0]);
            SetDlgItemText(hDlg, IDC_RADIO2, constantBankNames[1]);
            SetDlgItemText(hDlg, IDC_RADIO3, constantBankNames[2]);
            SetDlgItemText(hDlg, IDC_RADIO4, constantBankNames[3]);
            SetDlgItemText(hDlg, IDC_RADIO5, constantBankNames[4]);
            return TRUE;

        case(103):            // Cancel 
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
