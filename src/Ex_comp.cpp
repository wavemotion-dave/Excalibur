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

extern "C" struct funcStruct Complex_funcs[MAX_FUNCS];
extern "C" void COMP_plus(void);
extern "C" void COMP_minus(void);
extern "C" void COMP_div(void);
extern "C" void COMP_mult(void);
extern "C" void COMP_sin(void);
extern "C" void COMP_cos(void);
extern "C" void COMP_tan(void);
extern "C" void COMP_conj(void);
extern "C" void COMP_exp(void);
extern "C" void COMP_ln(void);
extern "C" void COMP_log(void);
extern "C" void COMP_pow(void);
extern "C" void COMP_sqrt(void);
extern "C" void COMP_abs(void);
extern "C" void COMP_CHS(void);

extern "C" void COMP_VectAddion(void);
extern "C" void COMP_VectSubtract(void);
extern "C" void COMP_VectCross(void);
extern "C" void COMP_VectDot(void);
extern "C" void COMP_VectAngleBetween(void);
extern "C" void COMP_VectConvertR(void);
extern "C" void COMP_VectConvertP(void);


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


// TBD - All these are currently done in X,Y,Z not Polar!!!  Should we have an option?!?
extern "C" void COMP_VectAddion(void)
{
  double x1, y1, z1, x2, y2, z2;

  x1 = StackPop();
  y1 = StackPop();
  z1 = StackPop();
  x2 = StackPop();
  y2 = StackPop();
  z2 = StackPop();
  StackPush(z1+z2);    
  StackPush(y1+y2);    
  StackPush(x1+x2);    
}

extern "C" void COMP_VectSubtract(void)
{
  double x1, y1, z1, x2, y2, z2;
    
  x1 = StackPop();
  y1 = StackPop();
  z1 = StackPop();
  x2 = StackPop();
  y2 = StackPop();
  z2 = StackPop();
  StackPush(z2-z1);    
  StackPush(y2-y1);    
  StackPush(x2-x1);    
}

extern "C" void COMP_VectCross(void)
{
  double x, y, z, u, v, w;
    
  x = StackPop();
  y = StackPop();
  z = StackPop();
  u = StackPop();
  v = StackPop();
  w = StackPop();
  StackPush(x*v - y*u);    
  StackPush(z*u - x*w);    
  StackPush(y*w - z*v);    
}

extern "C" void COMP_VectDot(void)
{
  double x, y, z, u, v, w;
    
  x = StackPop();
  y = StackPop();
  z = StackPop();
  u = StackPop();
  v = StackPop();
  w = StackPop();
  StackPush(x*u + y*v + z*w);    
}

extern "C" void COMP_VectAngleBetween(void)
{
  double x, y, z, u, v, w, D, R1, R2, G;
    
  x = StackPop();
  y = StackPop();
  z = StackPop();
  u = StackPop();
  v = StackPop();
  w = StackPop();
  D = x*u + y*v + z*w;    
  R1 = sqrt( (x*x) + (y*y) + (z*z) );
  R2 = sqrt( (u*u) + (v*v) + (w*w) );
  G = D / (R1 * R2);
  StackPush(FromRadians(acos(G)));
}

extern "C" void COMP_VectConvertR(void)  // Convert to rect coordinates
{
  double x1, y1, z1, x2, y2, z2;
  double r1, t1, p1, r2, t2, p2;
  
  r1 = StackPop();
  t1 = StackPop();
  p1 = StackPop();
  r2 = StackPop();
  t2 = StackPop();
  p2 = StackPop();
  
  x1 = r1 * sin(ToRadians(p1)) * cos(ToRadians(t1));
  y1 = r1 * sin(ToRadians(p1)) * sin(ToRadians(t1));
  z1 = r1 * cos(ToRadians(p1));

  x2 = r2 * sin(ToRadians(p2)) * cos(ToRadians(t2));
  y2 = r2 * sin(ToRadians(p2)) * sin(ToRadians(t2));
  z2 = r2 * cos(ToRadians(p2));
  
  StackPush(z2);
  StackPush(y2);
  StackPush(x2);
  StackPush(z1);
  StackPush(y1);
  StackPush(x1);
}

extern "C" void COMP_VectConvertP(void)  // Convert to polar coordinates
{
  double x1, y1, z1, x2, y2, z2;
  double r1, t1, p1, r2, t2, p2;
  
  x1 = StackPop();
  y1 = StackPop();
  z1 = StackPop();
  x2 = StackPop();
  y2 = StackPop();
  z2 = StackPop();

  r1 = sqrt( (x1*x1) + (y1*y1) + (z1*z1) );
  if (x1 == 0.0)
     t1 = 0.0;
  else   
     t1 = FromRadians(atan(y1/x1));
  if (sqrt((x1*x1)+(y1*y1)) == 0.0)
     p1 = 0.0;
  else   
     p1 = FromRadians(atan(z1/sqrt((x1*x1)+(y1*y1))));
  p1 = p1 + 90.0;

  r2 = sqrt( (x2*x2) + (y2*y2) + (z2*z2) );
  if (x2 == 0.0)
     t2 = 0.0;
  else   
     t2 = FromRadians(atan(y2/x2));
  if (sqrt((x2*x2)+(y2*y2)) == 0.0)
     p2 = 0.0;
  else   
     p2 = FromRadians(atan(z2/sqrt((x2*x2)+(y2*y2))));
   
  p2 = p2 + 90.0;
  
  StackPush(p2);
  StackPush(t2);
  StackPush(r2);
  StackPush(p1);
  StackPush(t1);
  StackPush(r1);
}

