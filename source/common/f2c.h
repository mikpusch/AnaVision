/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

	- From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#ifndef F2C_INCLUDE
#define F2C_INCLUDE


typedef long int integer;
typedef unsigned long int uinteger;
typedef char *address;
typedef short int shortint;
//typedef float real;
typedef double doublereal;
typedef struct { float r, i; } f2c_complex;
typedef struct { doublereal r, i; } doublef2c_complex;
typedef long int logical;
typedef short int shortlogical;
typedef char logical1;
typedef char integer1;
#ifdef INTEGER_STAR_8	/* Adjust for integer*8. */
typedef long long longint;		/* system-dependent */
typedef unsigned long long ulongint;	/* system-dependent */
#define qbit_clear(a,b)	((a) & ~((ulongint)1 << (b)))
#define qbit_set(a,b)	((a) |  ((ulongint)1 << (b)))
#endif

#define TRUE_ (1)
#define FALSE_ (0)

/* Extern is for use with -E */
#ifndef Extern
#define Extern extern
#endif

/* I/O stuff */

#ifdef f2c_i2
/* for -i2 */
typedef short flag;
typedef short ftnlen;
typedef short ftnint;
#else
typedef long int flag;
typedef long int ftnlen;
typedef long int ftnint;
#endif

/*external read, write*/
typedef struct
{	flag cierr;
	ftnint ciunit;
	flag ciend;
	char *cifmt;
	ftnint cirec;
} cilist;

/*internal read, write*/
typedef struct
{	flag icierr;
	char *iciunit;
	flag iciend;
	char *icifmt;
	ftnint icirlen;
	ftnint icirnum;
} icilist;

/*open*/
typedef struct
{	flag oerr;
	ftnint ounit;
	char *ofnm;
	ftnlen ofnmlen;
	char *osta;
	char *oacc;
	char *ofm;
	ftnint orl;
	char *oblnk;
} olist;

/*close*/
typedef struct
{	flag cerr;
	ftnint cunit;
	char *csta;
} cllist;

/*rewind, backspace, endfile*/
typedef struct
{	flag aerr;
	ftnint aunit;
} alist;

/* inquire */
typedef struct
{	flag inerr;
	ftnint inunit;
	char *infile;
	ftnlen infilen;
	ftnint	*inex;	/*parameters in standard's order*/
	ftnint	*inopen;
	ftnint	*innum;
	ftnint	*innamed;
	char	*inname;
	ftnlen	innamlen;
	char	*inacc;
	ftnlen	inacclen;
	char	*inseq;
	ftnlen	inseqlen;
	char 	*indir;
	ftnlen	indirlen;
	char	*infmt;
	ftnlen	infmtlen;
	char	*inform;
	ftnint	informlen;
	char	*inunf;
	ftnlen	inunflen;
	ftnint	*inrecl;
	ftnint	*innrec;
	char	*inblank;
	ftnlen	inblanklen;
} inlist;

#define VOID void

union Multitype {	/* for multiple entry points */
	integer1 g;
	shortint h;
	integer i;
	/* longint j; */
	float r;
	doublereal d;
	f2c_complex c;
	doublef2c_complex z;
	};

typedef union Multitype Multitype;

/*typedef long int Long;*/	/* No longer used; formerly in Namelist */

struct Vardesc {	/* for Namelist */
	char *name;
	char *addr;
	ftnlen *dims;
	int  type;
	};
typedef struct Vardesc Vardesc;

struct Namelist {
	char *name;
	Vardesc **vars;
	int nvars;
	};
typedef struct Namelist Namelist;

//#define abs(x) ((x) >= 0 ? (x) : -(x))
#define f2c_abs(x) ((x) >= 0 ? (x) : -(x))
//#define dabs(x) (doublereal)abs(x)
#define dabs(x) (doublereal)f2c_abs(x)
//#define min(a,b) ((a) <= (b) ? (a) : (b))
//#define max(a,b) ((a) >= (b) ? (a) : (b))

#define dmin(a,b) (doublereal)min(a,b)
#define dmax(a,b) (doublereal)max(a,b)
#define bit_test(a,b)	((a) >> (b) & 1)
#define bit_clear(a,b)	((a) & ~((uinteger)1 << (b)))
#define bit_set(a,b)	((a) |  ((uinteger)1 << (b)))

/* procedure parameter types for -A and -C++ */

#define F2C_proc_par_types 1
#ifdef __cplusplus
typedef int /* Unknown procedure type */ (*U_fp)(...);
typedef shortint (*J_fp)(...);
typedef integer (*I_fp)(...);
typedef float (*R_fp)(...);
typedef doublereal (*D_fp)(...), (*E_fp)(...);
typedef /* f2c_complex */ VOID (*C_fp)(...);
typedef /* Double f2c_complex */ VOID (*Z_fp)(...);
typedef logical (*L_fp)(...);
typedef shortlogical (*K_fp)(...);
typedef /* Character */ VOID (*H_fp)(...);
typedef /* Subroutine */ int (*S_fp)(...);
#else
typedef int /* Unknown procedure type */ (*U_fp)();
typedef shortint (*J_fp)();
typedef integer (*I_fp)();
typedef float (*R_fp)();
typedef doublereal (*D_fp)(), (*E_fp)();
typedef /* f2c_complex */ VOID (*C_fp)();
typedef /* Double f2c_complex */ VOID (*Z_fp)();
typedef logical (*L_fp)();
typedef shortlogical (*K_fp)();
typedef /* Character */ VOID (*H_fp)();
typedef /* Subroutine */ int (*S_fp)();
#endif
/* E_fp is for real functions when -R is not specified */
typedef VOID C_f;	/* f2c_complex function */
typedef VOID H_f;	/* character function */
typedef VOID Z_f;	/* double f2c_complex function */
typedef doublereal E_f;	/* real function with -R not specified */


#ifdef __cplusplus
extern "C"
{
#endif

	// the following routines are in the vcf2c.lib library

double __cdecl c_abs( f2c_complex * c);

void r_cnjg(f2c_complex * a , f2c_complex * b);

double __cdecl r_imag(f2c_complex * c );

void __cdecl c_div(f2c_complex * a,f2c_complex * b, f2c_complex * c);

int __cdecl s_cat(char * a,char * * b,long * c,long * d,long e);

double __cdecl pow_ri(float * a,long * b);

double __cdecl r_sign(float * a,float * b);

void __cdecl c_sqrt( f2c_complex * a, f2c_complex * b);

void __cdecl pow_ci( f2c_complex * a, f2c_complex * b,long * c);

long __cdecl pow_ii(long * a,long * b);

long __cdecl i_nint(float * a);

void __cdecl c_exp( f2c_complex * a, f2c_complex * b);

double __cdecl d_sign(double * a,double * b);

double __cdecl pow_dd(double * a,double * b);

double __cdecl d_imag( doublef2c_complex * a);

extern double __cdecl pow_di(double * a,long * b);

double __cdecl d_lg10(double *a);

long __cdecl i_dnnt(double * a);

double __cdecl z_abs( doublef2c_complex * a);

long __cdecl s_cmp(char * a,char * b,long c,long d);

int __cdecl s_copy(char * a,char * b,long c,long d);

long __cdecl i_len(char * a,long b);

int __cdecl s_stop(char * a,long b);

long __cdecl e_wsfe(void );

long __cdecl do_fio(long * a,char * b,long c);

long __cdecl s_wsfe( cilist * c);

void __cdecl d_cnjg( doublef2c_complex * a, doublef2c_complex * b);

void __cdecl z_div( doublef2c_complex * a, doublef2c_complex * b, doublef2c_complex * v);

void __cdecl z_sqrt( doublef2c_complex * a, doublef2c_complex * b);

void __cdecl pow_zi( doublef2c_complex * a, doublef2c_complex * b,long * c);

void __cdecl z_exp( doublef2c_complex * a, doublef2c_complex * b);

double __cdecl r_lg10(float * a);

#ifdef __cplusplus
}
#endif


/* undef any lower-case symbols that your C compiler predefines, e.g.: */

#ifndef Skip_f2c_Undefs
#undef cray
#undef gcos
#undef mc68010
#undef mc68020
#undef mips
#undef pdp11
#undef sgi
#undef sparc
#undef sun
#undef sun2
#undef sun3
#undef sun4
#undef u370
#undef u3b
#undef u3b2
#undef u3b5
#undef unix
#undef vax
#endif



#endif
