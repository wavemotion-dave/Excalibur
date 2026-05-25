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
#define CPPLUS 1

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <mmsystem.h>
#include <ctype.h>
#include <complex>
#include "Excal.h"

extern "C" struct funcStruct Scientific2_funcs[MAX_FUNCS];

extern "C" void COMP_plus(void)
{
    double a, b, c, d;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    d = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c2(c, d);
    std::complex<double> c3;
    c3 = c1 + c2;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_minus(void)
{
    double a, b, c, d;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    d = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c2(c, d);
    std::complex<double> c3;
    c3 = c2 - c1;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_div(void)
{
    double a, b, c, d;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    d = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c2(c, d);
    std::complex<double> c3;
    c3 = c2 / c1;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_mult(void)
{
    double a, b, c, d;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    d = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c2(c, d);
    std::complex<double> c3;
    c3 = c1 * c2;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_sin(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::sin(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_cos(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::cos(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_tan(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c2;
    std::complex<double> c3;

    c2 = std::sin(c1);
    c3 = std::cos(c1);
    c3 = c2 / c3;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_hsin(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::sinh(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_hcos(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::cosh(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_htan(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c2;
    std::complex<double> c3;

    c2 = std::sinh(c1);
    c3 = std::cosh(c1);
    c3 = c2 / c3;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_conj(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::conj(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_exp(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::exp(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_ln(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::log(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_log(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::log10(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_pow(void)
{
    double a, b, c, d;

    a = StackPop();
    b = StackPop();
    c = StackPop();
    d = StackPop();

    std::complex<double> c1(c, d);
    std::complex<double> c2(a, b);
    std::complex<double> c3;
    c3 = std::pow(c1, c2);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_sqrt(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::sqrt(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_abs(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::abs(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_CHS(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;

    c3 = -c1;
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_norm(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::norm(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_arg(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);
    std::complex<double> c3;
    c3 = std::arg(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}

extern "C" void COMP_ToPolar(void)
{
    double a, b, r, theta;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1(a, b);

    r = std::abs(c1);     // Magnitude
    theta = std::arg(c1); // Phase angle in radians
    StackPush(theta);
    StackPush(r);
}

extern "C" void COMP_FromPolar(void)
{
    double a, b;

    a = StackPop();
    b = StackPop();

    std::complex<double> c1 = std::polar(a, b);
    std::complex<double> c3;
    c3 = std::arg(c1);
    StackPush(c3.imag());
    StackPush(c3.real());
}


/* Visual C++ 5.0 compatible Complex structure */
struct Complex {
    double re;
    double im;
};

/* Basic Complex Arithmetic */
struct Complex add(struct Complex a, struct Complex b) {
    struct Complex res;
    res.re = a.re + b.re;
    res.im = a.im + b.im;
    return res;
}

struct Complex multiply(struct Complex a, struct Complex b) {
    struct Complex res;
    res.re = a.re * b.re - a.im * b.im;
    res.im = a.re * b.im + a.im * b.re;
    return res;
}

struct Complex divide(struct Complex a, struct Complex b) {
    struct Complex res;
    double den = b.re * b.re + b.im * b.im;
    res.re = (a.re * b.re + a.im * b.im) / den;
    res.im = (a.im * b.re - a.re * b.im) / den;
    return res;
}

/* Complex Power: a^b = exp(b * log(a)) */
struct Complex c_pow(struct Complex a, struct Complex b) {
    struct Complex res;
    double r = sqrt(a.re * a.re + a.im * a.im);
    double theta = atan2(a.im, a.re);
    
    /* log(a) */
    double log_r = log(r);
    double log_i = theta;
    
    /* b * log(a) */
    double real_part = b.re * log_r - b.im * log_i;
    double imag_part = b.re * log_i + b.im * log_r;
    
    /* exp(...) */
    double exp_real = exp(real_part);
    res.re = exp_real * cos(imag_part);
    res.im = exp_real * sin(imag_part);
    return res;
}

/* Lanczos Approximation for Complex Gamma */
struct Complex gamma_complex(struct Complex z) {
    static const double p[] = {
        0.99999999999980993,
        676.5203681218851,
        -1259.1392167224028,
        771.32342877765313,
        -176.61502916214059,
        12.507343278686905,
        -0.13857109526572012,
        9.9843695780195716e-6,
        1.5056327351493116e-7
    };

    struct Complex x, t, result;
    int i;

    /* Reflection formula not included for brevity; 
       works best for Re(z) > 0.5 */
    
    x.re = p[0]; x.im = 0;
    for (i = 1; i < 9; i++) {
        struct Complex num, den, term;
        num.re = p[i]; num.im = 0;
        den.re = z.re + (i - 1);
        den.im = z.im;
        term = divide(num, den);
        x = add(x, term);
    }

    t.re = z.re + 7.0 - 0.5;
    t.im = z.im;

    /* result = sqrt(2*PI) * t^(z-0.5) * exp(-t) * x */
    {
        struct Complex z_minus_half = { z.re - 0.5, z.im };
        struct Complex term1 = c_pow(t, z_minus_half);
        struct Complex term2;
        double exp_neg_t = exp(-t.re);
        
        term2.re = exp_neg_t * cos(-t.im);
        term2.im = exp_neg_t * sin(-t.im);

        result = multiply(term1, term2);
        result = multiply(result, x);
        
        /* sqrt(2*PI) */
        result.re *= 2.5066282746310005;
        result.im *= 2.5066282746310005;
    }

    return result;
}

extern "C" void COMP_Gamma(void)
{
    struct Complex c1;
    struct Complex c3;

	c1.re = StackPop();
	c1.im = StackPop();

    c3 = gamma_complex(c1);

    StackPush(c3.im);
    StackPush(c3.re);
}
