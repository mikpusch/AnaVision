
#ifndef VCF2C_INCLUDE
#define VCF2C_INCLUDE

#include "../common/f2c.h"


double __cdecl c_abs( complex * c);

void r_cnjg(complex * a , complex * b);

double __cdecl r_imag(complex * c );

void __cdecl c_div(complex * a,complex * b, complex * c);

int __cdecl s_cat(char * a,char * * b,long * c,long * d,long e);

double __cdecl pow_ri(float * a,long * b);

double __cdecl r_sign(float * a,float * b);

void __cdecl c_sqrt( complex * a, complex * b);

void __cdecl pow_ci( complex * a, complex * b,long * c);

long __cdecl pow_ii(long * a,long * b);

long __cdecl i_nint(float * a);

void __cdecl c_exp( complex * a, complex * b);

double __cdecl d_sign(double * a,double * b);

double __cdecl pow_dd(double * a,double * b);

double __cdecl d_imag( doublecomplex * a);

extern double __cdecl pow_di(double * a,long * b);

double __cdecl d_lg10(double *a);

long __cdecl i_dnnt(double * a);

double __cdecl z_abs( doublecomplex * a);

long __cdecl s_cmp(char * a,char * b,long c,long d);

int __cdecl s_copy(char * a,char * b,long c,long d);

long __cdecl i_len(char * a,long b);

int __cdecl s_stop(char * a,long b);

long __cdecl e_wsfe(void );

long __cdecl do_fio(long * a,char * b,long c);

long __cdecl s_wsfe( cilist * c);

void __cdecl d_cnjg( doublecomplex * a, doublecomplex * b);

void __cdecl z_div( doublecomplex * a, doublecomplex * b, doublecomplex * v);

void __cdecl z_sqrt( doublecomplex * a, doublecomplex * b);

void __cdecl pow_zi( doublecomplex * a, doublecomplex * b,long * c);

void __cdecl z_exp( doublecomplex * a, doublecomplex * b);

double __cdecl r_lg10(float * a);

#endif