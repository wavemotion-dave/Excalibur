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
#include "EXCAL.h"

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
extern void COMP_VectAddion (void);
extern void COMP_VectSubtract (void);
extern void COMP_VectCross (void);
extern void COMP_VectDot (void);
extern void COMP_VectAngleBetween (void);
extern void COMP_VectConvertR (void);
extern void COMP_VectConvertP (void);

/* I had to put this here to get the link to work.  All complex functions are */
/* defined and stored in EX_COMP.CPP file. */
struct funcStruct Complex_funcs[MAX_FUNCS] = {
    {FN1,   0,  UNI_CPLUS,      USES_F,     ALLOWREC,   ' ',    "Cpx +",    YES_L,  X_NEW,      COMP_plus,              T_CPLXADD,      H_CPLXADD},
    {FN2,   0,  UNI_CMINUS,     USES_F,     ALLOWREC,   ' ',    "Cpx --",   YES_L,  X_NEW,      COMP_minus,             T_CPLXSUB,      H_CPLXSUB},
    {FN3,   0,  UNI_CDIV,       USES_F,     ALLOWREC,   ' ',    "Cpx ÷",    YES_L,  X_NEW,      COMP_div,               T_CPLXDIV,      H_CPLXDIV},
    {FN4,   0,  UNI_CMUL,       USES_F,     ALLOWREC,   ' ',    "Cpx ×",    YES_L,  X_NEW,      COMP_mult,              T_CPLXMUL,      H_CPLXMUL},
    {FN5,   0,  UNI_CSIN,       USES_F,     ALLOWREC,   ' ',    "cSIN",     YES_L,  X_NEW,      COMP_sin,               T_CPLXSIN,      H_CPLXSIN},
    {FN6,   0,  UNI_CCOS,       USES_F,     ALLOWREC,   ' ',    "cCOS",     YES_L,  X_NEW,      COMP_cos,               T_CPLXCOS,      H_CPLXCOS},
    {FN7,   0,  UNI_CTAN,       USES_F,     ALLOWREC,   ' ',    "cTAN",     YES_L,  X_NEW,      COMP_tan,               T_CPLXTAN,      H_CPLXTAN},
    {FN8,   0,  UNI_CCHS,       USES_F,     ALLOWREC,   ' ',    "cCHS",     YES_L,  X_NEW,      COMP_CHS,               T_CPLXCHS,      H_CPLXCHS},
    {FN9,   0,  UNI_CCONJ,      USES_F,     ALLOWREC,   ' ',    "Conj",     YES_L,  X_NEW,      COMP_conj,              T_CONJ,         H_CONJ},
    {FN10,  0,  UNI_CEXP,       USES_F,     ALLOWREC,   ' ',    "cEXP",     YES_L,  X_NEW,      COMP_exp,               T_CPLXEXP,      H_CPLXEXP},
    {FN11,  0,  UNI_CLN,        USES_F,     ALLOWREC,   ' ',    "cLN",      YES_L,  X_NEW,      COMP_ln,                T_CPLXLN,       H_CPLXLN},
    {FN12,  0,  UNI_CLOG,       USES_F,     ALLOWREC,   ' ',    "cLOG",     YES_L,  X_NEW,      COMP_log,               T_CPLXLOG,      H_CPLXLOG},
    {FN13,  0,  UNI_CPOW,       USES_F,     ALLOWREC,   ' ',    "cPOW",     YES_L,  X_NEW,      COMP_pow,               T_CPLXPOW,      H_CPLXPOW},
    {FN14,  0,  UNI_CSQRT,      USES_F,     ALLOWREC,   ' ',    "cSQRT",    YES_L,  X_NEW,      COMP_sqrt,              T_CPLXSQRT,     H_CPLXSQRT},
    {FN15,  0,  UNI_CABS,       USES_F,     ALLOWREC,   ' ',    "cABS",     YES_L,  X_NEW,      COMP_abs,               T_CPLXABS,      H_CPLXABS},
    {FN16,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN17,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN18,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN19,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN20,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN21,  0,  UNI_VPLUS,      USES_F,     ALLOWREC,   ' ',    "Vect +",   YES_L,  X_NEW,      COMP_VectAddion,        T_VECTADD,      H_VECTADD},
    {FN22,  0,  UNI_VMINUS,     USES_F,     ALLOWREC,   ' ',    "Vect -",   YES_L,  X_NEW,      COMP_VectSubtract,      T_VECTSUB,      H_VECTSUB},
    {FN23,  0,  UNI_VCROSS,     USES_F,     ALLOWREC,   ' ',    "Vect x",   YES_L,  X_NEW,      COMP_VectCross,         T_VECTCROSS,    H_VECTCROSS},
    {FN24,  0,  UNI_VDOT,       USES_F,     ALLOWREC,   ' ',    "Vect o",   YES_L,  X_NEW,      COMP_VectDot,           T_VECTDOT,      H_VECTDOT},
    {FN25,  0,  UNI_VANGLE,     USES_F,     ALLOWREC,   ' ',    "Vect A",   YES_L,  X_NEW,      COMP_VectAngleBetween,  T_VECTANGLE,    H_VECTANGLE},
    {FN26,  0,  UNI_VCONVR,     USES_F,     ALLOWREC,   ' ',    "Vect»R",   YES_L,  X_NEW,      COMP_VectConvertR,      T_VECTCONVERTR, H_VECTCONVERTR},
    {FN27,  0,  UNI_VCONVP,     USES_F,     ALLOWREC,   ' ',    "Vect»P",   YES_L,  X_NEW,      COMP_VectConvertP,      T_VECTCONVERTP, H_VECTCONVERTP},                            
    {FN28,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN29,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN30,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN31,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN32,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN33,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN34,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN35,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN36,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN37,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN38,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN39,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL},
    {FN40,  0,  UNI_UNUSED,     USES_F,     ALLOWREC,   ' ',    "   ",      YES_L,  X_NEW,      NULL,                   T_NULL,         H_NULL}
};
