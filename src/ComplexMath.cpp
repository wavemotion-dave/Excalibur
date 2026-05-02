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
#include "EXCAL.h"

extern "C" struct funcStruct Scientific2_funcs[MAX_FUNCS];

extern "C" void COMP_plus(void)
{
  double a, b, c, d;

  a = StackPop();
  b = StackPop();
  c = StackPop();
  d = StackPop();

  std::complex<double> c1(a,b);
  std::complex<double> c2(c,d);
  std::complex<double> c3;
  c3 = c1+c2;
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

  std::complex<double> c1(a,b);
  std::complex<double> c2(c,d);
  std::complex<double> c3;
  c3 = c2-c1;
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

  std::complex<double> c1(a,b);
  std::complex<double> c2(c,d);
  std::complex<double> c3;
  c3 = c2/c1;
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

  std::complex<double> c1(a,b);
  std::complex<double> c2(c,d);
  std::complex<double> c3;
  c3 = c1*c2;
  StackPush(c3.imag());
  StackPush(c3.real());
}

extern "C" void COMP_sin(void)
{
  double a, b;

  a = StackPop();
  b = StackPop();

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
  std::complex<double> c2;
  std::complex<double> c3;

  c2 = std::sin(c1);
  c3 = std::cos(c1);
  c3 = c2/c3;
  StackPush(c3.imag());
  StackPush(c3.real());
}

extern "C" void COMP_hsin(void)
{
  double a, b;

  a = StackPop();
  b = StackPop();

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
  std::complex<double> c2;
  std::complex<double> c3;

  c2 = std::sinh(c1);
  c3 = std::cosh(c1);
  c3 = c2/c3;
  StackPush(c3.imag());
  StackPush(c3.real());
}


extern "C" void COMP_conj(void)
{
  double a, b;

  a = StackPop();
  b = StackPop();

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(c,d);
  std::complex<double> c2(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);
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

  std::complex<double> c1(a,b);

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

  std::complex<double> c1 = std::polar(a,b);
  std::complex<double> c3;
  c3 = std::arg(c1);
  StackPush(c3.imag());
  StackPush(c3.real());
}
