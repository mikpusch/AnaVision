#include "stdafx.h"
//#include "hh.h"
#include "../common/DCompl.h"

#include <math.h>

dcomplex MakeComplex (double r, double i)
{
dcomplex t;
t.r=r;
t.i=i;
return t;
}

dcomplex Cadd( dcomplex a, dcomplex b)
{
dcomplex t;
t.r=a.r+b.r;
t.i=a.i+b.i;
return t;
}

dcomplex Csub( dcomplex a, dcomplex b)
{
dcomplex t;
t.r=a.r-b.r;
t.i=a.i-b.i;
return t;
}

dcomplex Cmul( dcomplex a, dcomplex b)
{
dcomplex t;
t.r=a.r*b.r-a.i*b.i;
t.i=a.i*b.r+a.r*b.i;
return t;
}

dcomplex RCmul( double a, dcomplex b)
{
dcomplex t;
t.r=a*b.r;
t.i=a*b.i;
return t;
}


dcomplex Cconj(dcomplex a)
{
dcomplex t;
t.r=a.r;
t.i= -a.i;
return t;
}

double Cabs(dcomplex a)
{
return sqrt(a.r*a.r+a.i*a.i);
}

dcomplex Cdiv( dcomplex a, dcomplex b)
{
return RCmul( 1.0/(b.r*b.r+b.i*b.i), Cmul(a, Cconj(b)));
}

dcomplex Csqrt( dcomplex a)
{
double arg, r;

if (a.i==0.0) {
 if (a.r>0) {
  return MakeComplex(sqrt(a.r), 0.0);
 }
 else {
  return MakeComplex(0.0, sqrt(-a.r));
 }
}

r=sqrt(Cabs(a));

if (a.r!=0.0) {
  arg = atan(fabs(a.i/a.r));  /* M_PI  = pi */
  if (a.r<0.0) {
    if (a.i<0.0)
     arg += M_PI;
    else
     arg = M_PI - arg;
   }
   else {
    if (a.i<0.0)
      arg = -arg ; /* M_2_PI - arg; */
   }
}
else {
  if (a.i>0.0) arg=M_PI/2.0;
    else
  arg = -0.5*M_PI;
}
arg = arg / 2.0;
return MakeComplex ( r*cos(arg), r*sin(arg));
}

void SolveCubic(double r, double s, double t, dcomplex *roots)
{
 double p, q, r2, D, absu3, arg, absu;
 double dd;
 dcomplex cD, u3, u, v, t1, t2, r3;
 int i;

 r2=r*r;
 p=(3.0*s-r2)/3.0;
 q=2.0*r*r2/27.0 - r*s/3.0 + t;

 D=p*p*p/27.0+q*q/4.0;
 cD = MakeComplex(D, 0.0);

 u3 = Cadd( MakeComplex( -q/2.0, 0.0), Csqrt(cD) );

 if (u3.r == 0.0)
  {
   if (u3.i>0.0)
     u = MakeComplex( 0.0, -1.0* pow ( u3.i, 1.0/3.0 ) );
   else
     u = MakeComplex( 0.0,  pow ( -1.0* u3.i, 1.0/3.0) );
  }
 else
  {
   absu3 = Cabs(u3);
   dd = fabs(u3.i/u3.r);
   arg = atan(dd);  /* M_PI  = pi */

   absu = pow ( absu3, 1.0/3.0 );
   if (u3.r<0.0)
    {
     if (u3.i<0.0)
       arg += M_PI;
     else
       arg = M_PI - arg;
    }
   else
    {
    if (u3.i<0.0)
     arg = M_2_PI - arg;
    }
   arg = arg / 3.0;
   u = MakeComplex ( absu*cos(arg), absu*sin(arg));
  }

 v = MakeComplex( -1.0*p/3.0, 0.0);
 v = Cdiv(v, u );

 r3 = MakeComplex(-r/3.0, 0.0);
 roots[0] = Cadd (u, v);
 t1 = RCmul ( -0.5, roots[0] );
 t2 = RCmul ( 0.5,  Cmul ( Csub ( u, v), MakeComplex (0.0, sqrt (3.0))));
 roots[1] = Cadd ( t1, t2 );
 roots[2] = Csub ( t1, t2 );

 for (i=0; i<3; i++)
  roots[i] = Cadd( roots[i], r3 );

}

/*  ************************* Wird nicht mehr benutzt!
void TestCubic()
{
 double r, s, t;
 dcomplex roots[3];
 dcomplex t1, t2, t3, t4, t5;
 int i;
 do
 {
  printf("\n Gib r, s, t \n");
  scanf("%lf%lf%lf", &r, &s, &t);
  printf("\nr %f s %f t %f\n", r, s , t);
  SolveCubic(r,s,t,roots);
  printf("\n 0: %f + i %f", roots[0].r, roots[0].i);
  printf("\n 1: %f + i %f", roots[1].r, roots[1].i);
  printf("\n 2: %f + i %f", roots[2].r, roots[2].i);
  for (i=0; i<3; i++)
   {
    t1=Complex(r,0.0);
    t2=Complex(s,0.0);
    t3=Complex(t,0.0);
    t4=roots[i];
    t5=t4;
    t3=Cadd(t3, Cmul(t4, t2) );
    t4 = Cmul(t4, t5);
    t3=Cadd(t3, Cmul(t4, t1));
    t3=Cadd(t3, Cmul(t4, t5));
    printf("\n t3.r %f t3.i %f \n", t3.r, t3.i);
   }

 }
  while (1>0);
}
*  ************************* Wird nicht mehr benutzt!
*/

void SolveQuartic ( double a, double b, double c, double d, double e,
		    dcomplex *roots)
{
 double b2, p, q, r;
 dcomplex rts[3], z1, z2, z3, tp1;
 int k;

 b /= a;
 b2 = b*b;
 c /= a;
 d /= a;
 e /= a;
 p = c-3.0*b2/8.0;
 q = b2*b/8.0 - b*c/2.0 +d;
 r = b2*c/16.0 + e - d*b/4.0 -3.0*b2*b2/256.0;

 SolveCubic(2.0*p, p*p-4.0*r, -q*q, rts);

 z1=Csqrt(rts[0]);
 z2=Csqrt(rts[1]);
 z3=Csqrt(rts[2]);

 tp1 = Cmul(z1, Cmul(z2, z3));

 if ( fabs (tp1.r-q) > fabs(tp1.r+q))
  {
   z1=RCmul(-1.0, z1);
  }

 roots[0]=RCmul( 0.5, Csub( Cadd( z1, z2), z3 ));
 roots[1]=RCmul( 0.5, Cadd( Csub( z1, z2), z3 ));
 roots[2]=RCmul( 0.5, Cadd( Csub( z2, z1), z3 ));
 roots[3]=RCmul(-0.5, Cadd( Cadd( z1, z2), z3 ));

 tp1=MakeComplex(b/4.0, 0.0);
 for (k=0; k<4; k++)
  roots[k] = Csub ( roots[k], tp1 );
}


