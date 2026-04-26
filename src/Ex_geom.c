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

/* Geometry */
extern void GEOM_circC(void);
extern void GEOM_circA(void);
extern void GEOM_elipA(void);
extern void GEOM_triA(void);
extern void GEOM_rectC(void);
extern void GEOM_rectA(void);
extern void GEOM_boxA(void);
extern void GEOM_boxV(void);
extern void GEOM_SphrA(void);
extern void GEOM_SphrV(void);
extern void GEOM_CylA(void);
extern void GEOM_CylV(void);
extern void GEOM_ConeA(void);
extern void GEOM_ConeV(void);
extern void GEOM_quad(void);
extern void GEOM_pyth(void);
extern void GEOM_hero(void);
extern void GEOM_inscCircle(void);
extern void GEOM_PyramidA(void);
extern void GEOM_PyramidV(void);

struct funcStruct Geometry_funcs[MAX_FUNCS] = {
    {FN1,   UNI_CIRCC,      USES_F,     ALLOWREC,   ' ',    "Circ C",   YES_L,  X_NEW,      GEOM_circC,     T_CIRCC,    H_CIRCC},
    {FN2,   UNI_CIRCA,      USES_F,     ALLOWREC,   ' ',    "Circ A",   YES_L,  X_NEW,      GEOM_circA,     T_CIRCA,    H_CIRCA},
    {FN3,   UNI_ELIPA,      USES_F,     ALLOWREC,   ' ',    "Elip A",   YES_L,  X_NEW,      GEOM_elipA,     T_ELIPA,    H_ELIPA},
    {FN4,   UNI_TRIA,       USES_F,     ALLOWREC,   ' ',    "Tri A",    YES_L,  X_NEW,      GEOM_triA,      T_TRIA,     H_TRIA},
    {FN5,   UNI_RECTC,      USES_F,     ALLOWREC,   ' ',    "Rect C",   YES_L,  X_NEW,      GEOM_rectC,     T_RECTC,    H_RECTC},
    {FN6,   UNI_RECTA,      USES_F,     ALLOWREC,   ' ',    "Rect A",   YES_L,  X_NEW,      GEOM_rectA,     T_RECTA,    H_RECTA},
    {FN7,   UNI_BOXA,       USES_F,     ALLOWREC,   ' ',    "Box A",    YES_L,  X_NEW,      GEOM_boxA,      T_BOXA,     H_BOXA},
    {FN8,   UNI_BOXV,       USES_F,     ALLOWREC,   ' ',    "Box V",    YES_L,  X_NEW,      GEOM_boxV,      T_BOXV,     H_BOXV},
    {FN9,   UNI_SPHA,       USES_F,     ALLOWREC,   ' ',    "Sphr A",   YES_L,  X_NEW,      GEOM_SphrA,     T_SPHA,     H_SPHA},
    {FN10,  UNI_SPHV,       USES_F,     ALLOWREC,   ' ',    "Sphr V",   YES_L,  X_NEW,      GEOM_SphrV,     T_SPHV,     H_SPHV},
    {FN11,  UNI_CYLA,       USES_F,     ALLOWREC,   ' ',    "Cyl A",    YES_L,  X_NEW,      GEOM_CylA,      T_CYLA,     H_CYLA},
    {FN12,  UNI_CYLV,       USES_F,     ALLOWREC,   ' ',    "Cyl V",    YES_L,  X_NEW,      GEOM_CylV,      T_CYLV,     H_CYLV},
    {FN13,  UNI_CONEA,      USES_F,     ALLOWREC,   ' ',    "Cone A",   YES_L,  X_NEW,      GEOM_ConeA,     T_CONA,     H_CONA},
    {FN14,  UNI_CONEB,      USES_F,     ALLOWREC,   ' ',    "Cone V",   YES_L,  X_NEW,      GEOM_ConeV,     T_CONEV,    H_CONEV},
    {FN15,  UNI_PYRA,       USES_F,     ALLOWREC,   ' ',    "Pyrm A",   YES_L,  X_NEW,      GEOM_PyramidA,  T_PYRA,     H_PYRA},
    {FN16,  UNI_PYRV,       USES_F,     ALLOWREC,   ' ',    "Pyrm V",   YES_L,  X_NEW,      GEOM_PyramidV,  T_PYRV,     H_PYRV},
    {FN17,  UNI_QUAD,       USES_F,     ALLOWREC,   ' ',    "Quad",     YES_L,  X_NEW,      GEOM_quad,      T_QUAD,     H_QUAD},
    {FN18,  UNI_PYTH,       USES_F,     ALLOWREC,   ' ',    "Pyth",     YES_L,  X_NEW,      GEOM_pyth,      T_PYTHAG,   H_PYTHAG},
    {FN19,  UNI_HERO,       USES_F,     ALLOWREC,   ' ',    "Hero's",   YES_L,  X_NEW,      GEOM_hero,      T_HERO,     H_HERO},
    {FN20,  UNI_INSCR,      USES_F,     ALLOWREC,   ' ',    "Inscrb",   YES_L,  X_NEW,      GEOM_inscCircle,T_ICIRC,    H_ICIRC},
    {FN21,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN22,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN23,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN24,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN25,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN26,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN27,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN28,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN29,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN30,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN31,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN32,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN33,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN34,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN35,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN36,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN37,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN38,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN39,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL},
    {FN40,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,           T_NULL,     H_NULL}
};

void GEOM_circC(void)
{
    StackPush(StackPop() * 2.0F * M_PI);
}

void GEOM_circA(void)
{
    double temp;
    temp = StackPop();
    StackPush((temp * temp) * M_PI);
}

void GEOM_elipA(void)
{
    StackPush(StackPop() * StackPop() * M_PI);
}

void GEOM_triA(void)
{
    StackPush(StackPop() * StackPop() * 0.5F);
}

void GEOM_rectC(void)
{
    StackPush((2.0F * StackPop()) + (2.0F * StackPop()));
}

void GEOM_rectA(void)
{
    StackPush(StackPop() * StackPop());
}

void GEOM_boxA(void)
{
    double h, w, l;

    l = StackPop();
    w = StackPop();
    h = StackPop();
    StackPush((2.0F * h * w) + (2.0F * h * l) + (2.0F * l * w));
}

void GEOM_boxV(void)
{
    StackPush(StackPop() * StackPop() * StackPop());
}

void GEOM_SphrA(void)
{
    double r;

    r = StackPop();
    StackPush(4.0F * M_PI * (r * r));
}

void GEOM_SphrV(void)
{
    double r;

    r = StackPop();
    StackPush((4.0F / 3.0F) * M_PI * (r * r * r));
}

void GEOM_CylA(void)
{
    double r, h;

    r = StackPop();
    h = StackPop();
    StackPush((2.0F * M_PI * r * h) + (2.0F * M_PI * r * r));
}

void GEOM_CylV(void)
{
    double r, h;

    r = StackPop();
    h = StackPop();
    StackPush(M_PI * (r * r) * h);
}

void GEOM_ConeA(void)
{
    double r, h;

    r = StackPop();
    h = StackPop();
    StackPush(M_PI * r * sqrt((r * r) + (h * h)));
}

void GEOM_ConeV(void)
{
    double r, h;

    r = StackPop();
    h = StackPop();
    StackPush((M_PI * (r * r) * h) / 3.0F);
}

void GEOM_quad(void)
{
    double a, b, c, temp1;
    double X1, X2;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    temp1 = (b * b) - (4.0 * a * c);
    if (temp1 < 0.0)
    {
        RPN_error("Quadratic Equation: Square-Root of negative number");
    }
    else
    {
        X1 = ((-1.0 * b) + sqrt(temp1)) / (2.0 * a);
        X2 = ((-1.0 * b) - sqrt(temp1)) / (2.0 * a);
        StackPush(X2);
        StackPush(X1);
    }
}

void GEOM_pyth(void)
{
    double a, b, c;

    a = StackPop();
    b = StackPop();
    c = sqrt(a * a + b * b);
    StackPush(c);
}

void GEOM_hero(void)
{
    double a, b, c, s, area;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    s = 0.5 * (a + b + c);
    area = s * (s - a) * (s - b) * (s - c);
    if (area >= 0.0)
        area = sqrt(area);
    else
    {
        RPN_error("Hero's Formula:  Tried to take square root of negative #");
        area = 0.0;
    }
    StackPush(area);
}

void GEOM_PyramidA(void)
{
    double s, P, area;

    s = StackPop();
    P = StackPop();
    area = 0.5 * s * P;
    StackPush(P);
}

void GEOM_PyramidV(void)
{
    double h, B, vol;

    h = StackPop();
    B = StackPop();
    vol = (1.0 / 3.0) * h * B;
    StackPush(vol);
}

void GEOM_inscCircle(void)
{
    double a, b, c, s, r;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    s = (a + b + c) / 2.0;

    r = ((s - a) * (s - b) * (s - c)) / s;
    if (r >= 0.0)
        r = sqrt(r);
    else
    {
        RPN_error("Inscribed Circle: Tried to take square root of negative #");
        r = 0.0;
    }
    StackPush(r);
}
