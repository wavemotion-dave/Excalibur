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

extern void CONV_inverse (void);
extern void CONV_lb_kg (void);
extern void CONV_Oz_g (void);
extern void CONV_flOz_ml (void);
extern void CONV_gal_l (void);
extern void CONV_gal_cm3 (void);
extern void CONV_pin_l (void);
extern void CONV_Igal_l (void);
extern void CONV_F_C (void);
extern void CONV_F_K (void);
extern void CONV_In_Cm (void);
extern void CONV_In_mm (void);
extern void CONV_Ft_Mi (void);
extern void CONV_Ft_Km (void);
extern void CONV_Ft_Fa (void);
extern void CONV_Mi_Km (void);
extern void CONV_J_Cal (void);
extern void CONV_H_Ms (void);
extern void CONV_Deg_Rad (void);
extern void CONV_R_P (void);
extern void CONV_Ft_m (void);
extern void CONV_in2_cm2 (void);
extern void CONV_ft2_m2 (void);
extern void CONV_in3_cm3 (void);
extern void CONV_ft3_m3 (void);
extern void CONV_at_psi (void);
extern void CONV_acre_ft2 (void);
extern void CONV_ah_col (void);
extern void CONV_ah_far (void);
extern void CONV_kmh_mps (void);
extern void CONV_knot_mph (void);
extern void CONV_hp_kw (void);
extern void CONV_Mil_mm (void);
extern void CONV_ShortTons_lbs (void);
extern void CONV_LongTons_lbs (void);
extern void CONV_MetricTons_lbs (void);

uint8_t convInverse = 0;

struct funcStruct Conversion_funcs[MAX_FUNCS] = {
    {FN1,   0,  UNI_CINV,    USES_F,    ALLOWREC,   ' ',    "INV",      NO_L,   X_NULL,     CONV_inverse,       T_CONV_INV,     H_CONV_INV},
    {FN2,   0,  UNI_LBKG,    USES_F,    ALLOWREC,   ' ',    "lb»kg",    YES_L,  X_NEW,      CONV_lb_kg,         T_LB_KG,        H_LB_KG},
    {FN3,   0,  UNI_OZGR,    USES_F,    ALLOWREC,   ' ',    "Oz»g",     YES_L,  X_NEW,      CONV_Oz_g,          T_OZ_GR,        H_OZ_GR},
    {FN4,   0,  UNI_OZML,    USES_F,    ALLOWREC,   ' ',    "flOz»ml",  YES_L,  X_NEW,      CONV_flOz_ml,       T_FO_ML,        H_FO_ML},
                                                
    {FN5,   0,  UNI_GALL,    USES_F,    ALLOWREC,   ' ',    "gal»l",    YES_L,  X_NEW,      CONV_gal_l,         T_GA_LI,        H_GA_LI},
    {FN6,   0,  UNI_GALCM3,  USES_F,    ALLOWREC,   ' ',    "gal»cm³",  YES_L,  X_NEW,      CONV_gal_cm3,       T_GA_CM3,       H_GA_CM3},
    {FN7,   0,  UNI_PINL,    USES_F,    ALLOWREC,   ' ',    "pnt»l",    YES_L,  X_NEW,      CONV_pin_l,         T_PINT_LI,      H_PINT_LI},
    {FN8,   0,  UNI_IGALL,   USES_F,    ALLOWREC,   ' ',    "Igal»l",   YES_L,  X_NEW,      CONV_Igal_l,        T_IGA_LI,       H_IGA_LI},
                                                
    {FN9,   0,  UNI_FACE,    USES_F,    ALLOWREC,   ' ',    "F»C",      YES_L,  X_NEW,      CONV_F_C,           T_FA_CE,        H_FA_CE},
    {FN10,  0,  UNI_FAKE,    USES_F,    ALLOWREC,   ' ',    "F»K",      YES_L,  X_NEW,      CONV_F_K,           T_FA_KE,        H_FA_KE},
    {FN11,  0,  UNI_INCM,    USES_F,    ALLOWREC,   ' ',    "in»cm",    YES_L,  X_NEW,      CONV_In_Cm,         T_IN_CM,        H_IN_CM},
    {FN12,  0,  UNI_INMM,    USES_F,    ALLOWREC,   ' ',    "in»mm",    YES_L,  X_NEW,      CONV_In_mm,         T_IN_MM,        H_IN_MM},
                                                        
    {FN13,  0,  UNI_FTMI,    USES_F,    ALLOWREC,   ' ',    "ft»mi",    YES_L,  X_NEW,      CONV_Ft_Mi,         T_FT_MI,        H_FT_MI},
    {FN14,  0,  UNI_FTKM,    USES_F,    ALLOWREC,   ' ',    "ft»km",    YES_L,  X_NEW,      CONV_Ft_Km,         T_FT_KM,        H_FT_KM},
    {FN15,  0,  UNI_FTFA,    USES_F,    ALLOWREC,   ' ',    "ft»fa",    YES_L,  X_NEW,      CONV_Ft_Fa,         T_FT_FA,        H_FT_FA},
    {FN16,  0,  UNI_FTM,     USES_F,    ALLOWREC,   ' ',    "ft»m",     YES_L,  X_NEW,      CONV_Ft_m,          T_FT_ME,        H_FT_ME},
                                                
    {FN17,  0,  UNI_MIKM,    USES_F,    ALLOWREC,   ' ',    "mi»km",    YES_L,  X_NEW,      CONV_Mi_Km,         T_MI_KM,        H_MI_KM},
    {FN18,  0,  UNI_IN2CM2,  USES_F,    ALLOWREC,   ' ',    "in²»cm²",  YES_L,  X_NEW,      CONV_in2_cm2,       T_SI_SC,        H_SI_SC},
    {FN19,  0,  UNI_FT2M2,   USES_F,    ALLOWREC,   ' ',    "Ft²»m²",   YES_L,  X_NEW,      CONV_ft2_m2,        T_SF_SM,        H_SF_SM},
    {FN20,  0,  UNI_IN3CM3,  USES_F,    ALLOWREC,   ' ',    "in³»cm³",  YES_L,  X_NEW,      CONV_in3_cm3,       T_CI_CC,        H_CI_CC},
                                                                                                        
    {FN21,  0,  UNI_FT3M3,   USES_F,    ALLOWREC,   ' ',    "Ft³»m³",   YES_L,  X_NEW,      CONV_ft3_m3,        T_CF_CM,        H_CF_CM},
    {FN22,  0,  UNI_ACRFT2,  USES_F,    ALLOWREC,   ' ',    "Acr»ft²",  YES_L,  X_NEW,      CONV_acre_ft2,      T_ACR_FT2,      H_ACR_FT2},
    {FN23,  0,  UNI_ATPSI,   USES_F,    ALLOWREC,   ' ',    "At»PSI",   YES_L,  X_NEW,      CONV_at_psi,        T_AT_PSI,       H_AT_PSI},
    {FN24,  0,  UNI_HHMS,    USES_F,    ALLOWREC,   ' ',    "h»h.ms",   YES_L,  X_NEW,      CONV_H_Ms,          T_HMS,          H_HMS},
                                                                                                
    {FN25,  0,  UNI_DRAD,    USES_F,    ALLOWREC,   ' ',    "D»Rad",    YES_L,  X_NEW,      CONV_Deg_Rad,       T_DE_RA,        H_DE_RA},
    {FN26,  0,  UNI_RP,      USES_F,    ALLOWREC,   ' ',    "R»P",      YES_L,  X_NEW,      CONV_R_P,           T_RE_PO,        H_RE_PO},
    {FN27,  0,  UNI_KMHMPS,  USES_F,    ALLOWREC,   ' ',    "kph»mps",  YES_L,  X_NEW,      CONV_kmh_mps,       T_KMH_MPS,      H_KMH_MPS},
    {FN28,  0,  UNI_KNOTMPH, USES_F,    ALLOWREC,   ' ',    "kt»mph",   YES_L,  X_NEW,      CONV_knot_mph,      T_KNOT_MPH,     H_KNOT_MPH},
                                                                                                
    {FN29,  0,  UNI_AHCOL,   USES_F,    ALLOWREC,   ' ',    "AH»Col",   YES_L,  X_NEW,      CONV_ah_col,        T_AH_COL,       H_AH_COL},
    {FN30,  0,  UNI_AHFAR,   USES_F,    ALLOWREC,   ' ',    "AH»Far",   YES_L,  X_NEW,      CONV_ah_far,        T_AH_FAR,       H_AH_FAR},
    {FN31,  0,  UNI_HPKW,    USES_F,    ALLOWREC,   ' ',    "HP»kW",    YES_L,  X_NEW,      CONV_hp_kw,         T_HP_KW,        H_HP_KW},
    {FN32,  0,  UNI_JCAL,    USES_F,    ALLOWREC,   ' ',    "Cal»J",    YES_L,  X_NEW,      CONV_J_Cal,         T_CA_JO,        H_CA_JO},
                                                                                            
    {FN33,  0,  UNI_MILMM,   USES_F,    ALLOWREC,   ' ',    "Mil»mm",   YES_L,  X_NEW,      CONV_Mil_mm,        T_MIL_MM,       H_MIL_MM},
    {FN34,  0,  UNI_STONLB,  USES_F,    ALLOWREC,   ' ',    "sTon»lb",  YES_L,  X_NEW,      CONV_ShortTons_lbs, T_STON_LB,      H_STON_LB},
    {FN35,  0,  UNI_LTONLB,  USES_F,    ALLOWREC,   ' ',    "lTon»lb",  YES_L,  X_NEW,      CONV_LongTons_lbs,  T_LTON_LB,      H_LTON_LB},
    {FN36,  0,  UNI_MTONLB,  USES_F,    ALLOWREC,   ' ',    "mTon»lb",  YES_L,  X_NEW,      CONV_MetricTons_lbs,T_MTON_LB,      H_MTON_LB},
                                        
    {FN37,  0,  UNI_UNUSED,  USES_F,    ALLOWREC,   ' ',    "    ",     YES_L,  X_NEW,      NULL,               T_NULL,         H_NULL},
    {FN38,  0,  UNI_UNUSED,  USES_F,    ALLOWREC,   ' ',    "    ",     YES_L,  X_NEW,      NULL,               T_NULL,         H_NULL},
    {FN39,  0,  UNI_UNUSED,  USES_F,    ALLOWREC,   ' ',    "    ",     YES_L,  X_NEW,      NULL,               T_NULL,         H_NULL},
    {FN40,  0,  UNI_UNUSED,  USES_F,    ALLOWREC,   ' ',    "    ",     YES_L,  X_NEW,      NULL,               T_NULL,         H_NULL}
};


double inverse (double x)
{
    return (1.0 / x);
}

/* --------------------- */
/* Conversion Functioins */
/* --------------------- */
void CONV_lb_kg (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (2.204622622));
    else
        StackPush (StackPop () * 2.204622622);
}


void CONV_F_C (void)
{
    if (convInverse == 0)
        StackPush ((StackPop () - 32.0) * inverse (1.8));
    else
        StackPush ((StackPop () * 1.8) + 32.0);
}


void CONV_In_Cm (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 2.54);
    else
        StackPush (StackPop () / 2.54);
}


void CONV_Oz_g (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 28.3495);
    else
        StackPush (StackPop () * inverse (28.3495));
}

void CONV_gal_l (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 3.785332);
    else
        StackPush (StackPop () * inverse (3.785332));
}

void CONV_Ft_Mi (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (5280.0));
    else
        StackPush (StackPop () * 5280.0);
}


void CONV_J_Cal (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 4.18605);
    else
        StackPush (StackPop () * inverse (4.18605));
}


void CONV_H_Ms (void)
{
    double temp;
    double temp2;
    double temp3;
    double mint;
    double sec;
    double op1;

    if (convInverse == 0)
    {
        op1 = StackPop ();
        temp2 = modf (op1, &temp);
        mint = temp2 * 60.0F;
        mint = MakeAccurate (mint);
        temp2 = modf (mint, &sec);
        sec = temp2 * 60.0F;
        sec = MakeAccurate (sec);
        mint = floor (mint) / 100.0F;
        sec = sec / 10000.0F;
        temp = temp + mint + sec;
        StackPush (temp);
    }
    else
    {
        op1 = StackPop ();
        temp2 = modf ((double) op1, (double *) &temp);
        temp2 = MakeAccurate (temp2);
        temp2 = temp2 * 100.0F;
        temp2 = MakeAccurate (temp2);
        mint = (double) floor (temp2);
        sec = modf (temp2, &temp3);
        sec = sec * 100.0F;
        sec = (sec / 60.0F);
        mint = mint + sec;
        mint = mint / 60.0F;
        temp = temp + mint;
        StackPush (temp);
    }
}


void CONV_Mi_Km (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 1.6093472);
    else
        StackPush (StackPop () * inverse (1.6093472));
}


void CONV_Ft_Fa (void)
{
    if (convInverse == 0)
        StackPush (StackPop () / 6.0);
    else
        StackPush (StackPop () * 6.0);
}


void CONV_in2_cm2 (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 6.452);
    else
        StackPush (StackPop () * inverse (6.452));
}


void CONV_in3_cm3 (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 16.39);
    else
        StackPush (StackPop () * inverse (16.39));
}


void CONV_Deg_Rad (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * (M_PI / 180.0));
    else
        StackPush (StackPop () * (180.0 / M_PI));
}


void CONV_R_P (void)
{
    double a, b;
    double r, theta;

    if (convInverse == 0)
    {
        a = StackPop ();
        b = StackPop ();
        r = sqrt ((a * a) + (b * b));
        if (a != 0.0)
            theta = atan (b / a);
        else
            theta = 0.0;
        StackPush (FromRadians (theta));
        StackPush (r);
    }
    else
    {
        r = StackPop ();
        theta = StackPop ();
        theta = ToRadians (theta);
        a = r * cos (theta);
        b = r * sin (theta);
        StackPush (b);
        StackPush (a);
    }
}


void CONV_flOz_ml (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 29.6F);
    else
        StackPush (StackPop () / 29.6F);
}


void CONV_Ft_m (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 0.3048);
    else
        StackPush (StackPop () * inverse (0.3048));
}


void CONV_ft2_m2 (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (10.76391042));
    else
        StackPush (StackPop () * 10.76391042);
}


void CONV_ft3_m3 (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (35.31466672));
    else
        StackPush (StackPop () * 35.31466672);
}


void CONV_at_psi (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * (1.0 / 6.804e-2));
    else
        StackPush (StackPop () * 6.804e-2);
}


void CONV_acre_ft2 (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 43560);
    else
        StackPush (StackPop () / 43560);
}


void CONV_inverse (void)
{
    convInverse ^= 1;
    if (convInverse == 0)
        UpdateSpareBar (" ");
    else
        UpdateSpareBar ("INV");
}


void CONV_gal_cm3 (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 3785.0);
    else
        StackPush (StackPop () * inverse (3785.0));
}

void CONV_pin_l (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (2.113));
    else
        StackPush (StackPop () * 2.113);
}

void CONV_Igal_l (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 4.5458051988);
    else
        StackPush (StackPop () * inverse (4.5458051988));
}

void CONV_F_K (void)
{
    double tmp;

    if (convInverse == 0)
    {
        tmp = (StackPop () - 32.0) * inverse (1.8);
        StackPush (tmp + 273.15);
    }
    else
    {
        tmp = (((StackPop () - 273.15) * 1.8) + 32.0);
        StackPush (tmp);
    }
}

void CONV_In_mm (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 25.4);
    else
        StackPush (StackPop () / 25.4);
}

void CONV_Ft_Km (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 0.0003048);
    else
        StackPush (StackPop () * inverse (0.0003048));
}

void CONV_ah_col (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 3600.0);
    else
        StackPush (StackPop () * inverse (3600.0));
}

void CONV_ah_far (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 0.037307);
    else
        StackPush (StackPop () * inverse (0.037307));
}


void CONV_kmh_mps (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (3.6));
    else
        StackPush (StackPop () * 3.6);
}

void CONV_knot_mph (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * inverse (0.8690));
    else
        StackPush (StackPop () * 0.8690);
}

void CONV_hp_kw (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 0.7457);
    else
        StackPush (StackPop () * inverse (0.7457));
}


void CONV_Mil_mm (void)
{
    if (convInverse == 0)
        StackPush ((StackPop () / 1000.0) * 25.4);
    else
        StackPush ((StackPop () / 25.4) * 1000.0);
}

void CONV_ShortTons_lbs (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 2000.0);
    else
        StackPush (StackPop () / 2000.0);
}

void CONV_LongTons_lbs (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 2240.0);
    else
        StackPush (StackPop () / 2240.0);
}

void CONV_MetricTons_lbs (void)
{
    if (convInverse == 0)
        StackPush (StackPop () * 2205.0);
    else
        StackPush (StackPop () / 2205.0);
}


