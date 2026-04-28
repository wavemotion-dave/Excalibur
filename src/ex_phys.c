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

extern void Phys_Constants(void);
extern void Phys_Moles(void);
extern void Phys_Molecules(void);
extern void Phys_GasVol(void);              // in liters
extern void Phys_OhmsE(void);               // Current in Y, Resistance in X
extern void Phys_OhmsI(void);               // Voltage in Y, Resistance in X
extern void Phys_OhmsR(void);               // Voltage in Y, Current in X
extern void Phys_Gravitation(void);         // In newtons
extern void Phys_Energy(void);              // in joules
extern void Phys_CentripetalForce(void);    // In newtons
extern void Phys_CentripetalAccel(void);    // in(m/sec^2)
extern void Phys_AccelerationDist(void);    // in meters
extern void Phys_ProjectileRange(void);     // In meters
extern void Phys_ProjectileHeight(void);    // In meters
extern void Phys_Astro(void);

extern void COMP_plus (void);
extern void COMP_minus (void);
extern void COMP_div (void);
extern void COMP_mult (void);
extern void COMP_sin (void);
extern void COMP_cos (void);
extern void COMP_tan (void);
extern void COMP_conj (void);
extern void COMP_exp (void);
extern void COMP_ln (void);
extern void COMP_log (void);
extern void COMP_pow (void);
extern void COMP_sqrt (void);
extern void COMP_abs (void);
extern void COMP_CHS (void);

struct funcStruct Physics_funcs[MAX_FUNCS] = {
    {FN1,   UNI_CONST2,     USES_F,     NORECORD,   ' ',    "Const",    YES_L,      X_NEW,      Phys_Constants,         T_CONST,        H_CONST},
    {FN2,   UNI_MOLES,      USES_F,     ALLOWREC,   ' ',    "Moles",    YES_L,      X_NEW,      Phys_Moles,             T_MOLES,        H_MOLES},
    {FN3,   UNI_MOLEC,      USES_F,     ALLOWREC,   ' ',    "Molec",    YES_L,      X_NEW,      Phys_Molecules,         T_MOLECULES,    H_MOLECULES},
    {FN4,   UNI_GASVOL,     USES_F,     ALLOWREC,   ' ',    "Gas Vol",  YES_L,      X_NEW,      Phys_GasVol,            T_GASVOL,       H_GASVOL},
    {FN5,   UNI_OHME,       USES_F,     ALLOWREC,   ' ',    "Ohm's E",  YES_L,      X_NEW,      Phys_OhmsE,             T_OHMSE,        H_OHMSE},
    {FN6,   UNI_OHMI,       USES_F,     ALLOWREC,   ' ',    "Ohm's I",  YES_L,      X_NEW,      Phys_OhmsI,             T_OHMSI,        H_OHMSI},
    {FN7,   UNI_OHMR,       USES_F,     ALLOWREC,   ' ',    "Ohm's R",  YES_L,      X_NEW,      Phys_OhmsR,             T_OHMSR,        H_OHMSR},
    {FN8,   UNI_GRAV,       USES_F,     ALLOWREC,   ' ',    "Grav",     YES_L,      X_NEW,      Phys_Gravitation,       T_GRAVITATION,  H_GRAVITATION},
    {FN9,   UNI_ENERGY,     USES_F,     ALLOWREC,   ' ',    "Energy",   YES_L,      X_NEW,      Phys_Energy,            T_ENERGY,       H_ENERGY},
    {FN10,  UNI_CENTF,      USES_F,     ALLOWREC,   ' ',    "Cent F",   YES_L,      X_NEW,      Phys_CentripetalForce,  T_CENTFORCE,    H_CENTFORCE},
    {FN11,  UNI_CENTA,      USES_F,     ALLOWREC,   ' ',    "Cent A",   YES_L,      X_NEW,      Phys_CentripetalAccel,  T_CENTACCEL,    H_CENTACCEL},
    {FN12,  UNI_ACELDIST,   USES_F,     ALLOWREC,   ' ',    "Acc D",    YES_L,      X_NEW,      Phys_AccelerationDist,  T_ACCELDIST,    H_ACCELDIST},
    {FN13,  UNI_PROJRANGE,  USES_F,     ALLOWREC,   ' ',    "Proj R",   YES_L,      X_NEW,      Phys_ProjectileRange,   T_PROJRANGE,    H_PROJRANGE},
    {FN14,  UNI_PROJHEIGHT, USES_F,     ALLOWREC,   ' ',    "Proj H",   YES_L,      X_NEW,      Phys_ProjectileHeight,  T_PROJHEIGHT,   H_PROJHEIGHT},
    {FN15,  UNI_ASTRO,      USES_F,     NORECORD,   ' ',    "Astro",    YES_L,      X_NEW,      Phys_Astro,             T_ASTRO,        H_ASTRO},
    {FN16,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN17,  UNI_CPLUS,      USES_F,     ALLOWREC,   ' ',    "Cpx +",    YES_L,      X_NEW,      COMP_plus,              T_CPLXADD,      H_CPLXADD},
    {FN18,  UNI_CMINUS,     USES_F,     ALLOWREC,   ' ',    "Cpx --",   YES_L,      X_NEW,      COMP_minus,             T_CPLXSUB,      H_CPLXSUB},
    {FN19,  UNI_CDIV,       USES_F,     ALLOWREC,   ' ',    "Cpx ÷",    YES_L,      X_NEW,      COMP_div,               T_CPLXDIV,      H_CPLXDIV},
    {FN20,  UNI_CMUL,       USES_F,     ALLOWREC,   ' ',    "Cpx ×",    YES_L,      X_NEW,      COMP_mult,              T_CPLXMUL,      H_CPLXMUL},
    {FN21,  UNI_CSIN,       USES_F,     ALLOWREC,   ' ',    "cSIN",     YES_L,      X_NEW,      COMP_sin,               T_CPLXSIN,      H_CPLXSIN},
    {FN22,  UNI_CCOS,       USES_F,     ALLOWREC,   ' ',    "cCOS",     YES_L,      X_NEW,      COMP_cos,               T_CPLXCOS,      H_CPLXCOS},
    {FN23,  UNI_CTAN,       USES_F,     ALLOWREC,   ' ',    "cTAN",     YES_L,      X_NEW,      COMP_tan,               T_CPLXTAN,      H_CPLXTAN},
    {FN24,  UNI_CCHS,       USES_F,     ALLOWREC,   ' ',    "cCHS",     YES_L,      X_NEW,      COMP_CHS,               T_CPLXCHS,      H_CPLXCHS},
    {FN25,  UNI_CCONJ,      USES_F,     ALLOWREC,   ' ',    "Conj",     YES_L,      X_NEW,      COMP_conj,              T_CONJ,         H_CONJ},
    {FN26,  UNI_CEXP,       USES_F,     ALLOWREC,   ' ',    "cEXP",     YES_L,      X_NEW,      COMP_exp,               T_CPLXEXP,      H_CPLXEXP},
    {FN27,  UNI_CLN,        USES_F,     ALLOWREC,   ' ',    "cLN",      YES_L,      X_NEW,      COMP_ln,                T_CPLXLN,       H_CPLXLN},
    {FN28,  UNI_CLOG,       USES_F,     ALLOWREC,   ' ',    "cLOG",     YES_L,      X_NEW,      COMP_log,               T_CPLXLOG,      H_CPLXLOG},
    {FN29,  UNI_CPOW,       USES_F,     ALLOWREC,   ' ',    "cPOW",     YES_L,      X_NEW,      COMP_pow,               T_CPLXPOW,      H_CPLXPOW},
    {FN30,  UNI_CSQRT,      USES_F,     ALLOWREC,   ' ',    "cSQRT",    YES_L,      X_NEW,      COMP_sqrt,              T_CPLXSQRT,     H_CPLXSQRT},
    {FN31,  UNI_CABS,       USES_F,     ALLOWREC,   ' ',    "cABS",     YES_L,      X_NEW,      COMP_abs,               T_CPLXABS,      H_CPLXABS},
    {FN32,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN33,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN34,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN35,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN36,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN37,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN38,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN39,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_ATLAS}, //hidden!
    {FN40,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,      X_NEW,      NULL,                   T_NULL,         H_NULL}
};

void Phys_Constants(void)
{
    extern void SCI_const(void);
    SCI_const();
}

void Phys_Moles(void)
{
    double x, y;
    x = StackPop();
    y = StackPop();
    if (x == 0.0)
        StackPush(0.0);
    else
        StackPush(y / x);
}

void Phys_Molecules(void)
{
    StackPush(StackPop() * 6.022045E+23);
}

void Phys_GasVol(void)
{
    StackPush(StackPop() * 22.414);
}

void Phys_OhmsE(void)          // Current in Y, Resistance in X
{
    StackPush(StackPop() * StackPop());
}

void Phys_OhmsI(void)          // Voltage in Y, Resistance in X
{
    double x, y;

    x = StackPop();
    y = StackPop();
    if (x != 0.0)
        StackPush(y / x);
    else
        StackPush(0.0);
}

void Phys_OhmsR(void)          // Voltage in Y, Current in X
{
    double x, y;

    x = StackPop();
    y = StackPop();
    if (x != 0.0)
        StackPush(y / x);
    else
        StackPush(0.0);
}

void Phys_Gravitation(void)
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

void Phys_Energy(void)
{
    StackPush(StackPop() * (299792458.0 * 299792458.0));
}

void Phys_CentripetalForce(void)       // In newtons
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

void Phys_CentripetalAccel(void)        // in(m/sec^2)
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

void Phys_AccelerationDist(void)   // in meters
{
    double a;                   //acceleration in m/s^2
    double t;                   //time in seconds
    double d;

    t = StackPop();
    a = StackPop();
    d = 0.5 * a * (t * t);
    StackPush(d);
}


void Phys_ProjectileRange(void)        // in meters
{
    double V;
    double angle;

    angle = StackPop();
    V = StackPop();
    StackPush(((V * V) / 9.80665) * sin(ToRadians(2.0 * angle)));
}

void Phys_ProjectileHeight(void)       // in meters
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


void Phys_Astro(void)
{
    DialogBox(hExcaliburInstance, (LPCSTR) "DIALOG_ASTRO", calcMainWindow, AstroDlgProc);
}
