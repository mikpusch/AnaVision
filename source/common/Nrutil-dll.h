#ifndef _NR_UTILS_H_
#define _NR_UTILS_H_

// float sqrarg;
#define SQR(a) ((float sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

// double dsqrarg;
#define DSQR(a) ((double dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg)

// double dmaxarg1,dmaxarg2;
#define DMAX(a,b) (double dmaxarg1=(a),dmaxarg2=(b),(dmaxarg1) > (dmaxarg2) ?\
        (dmaxarg1) : (dmaxarg2))

// double dminarg1,dminarg2;
#define DMIN(a,b) (double dminarg1=(a),dminarg2=(b),(dminarg1) < (dminarg2) ?\
        (dminarg1) : (dminarg2))

// float maxarg1,maxarg2;
#define FMAX(a,b) (float maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))

// float minarg1,minarg2;
#define FMIN(a,b) (float minarg1=(a),minarg2=(b),(minarg1) < (minarg2) ?\
        (minarg1) : (minarg2))

// long lmaxarg1,lmaxarg2;
#define LMAX(a,b) (long lmaxarg1=(a),lmaxarg2=(b),(lmaxarg1) > (lmaxarg2) ?\
        (lmaxarg1) : (lmaxarg2))

// long lminarg1,lminarg2;
#define LMIN(a,b) (long lminarg1=(a),lminarg2=(b),(lminarg1) < (lminarg2) ?\
        (lminarg1) : (lminarg2))

// int imaxarg1,imaxarg2;

// int iminarg1,iminarg2;
#define IMIN(a,b) (int iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))

#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

void nrerror(char * error_text);
__declspec(dllexport) float *vector(long nl, long nh);
__declspec(dllexport) int *ivector(long nl, long nh);
__declspec(dllexport) unsigned char *cvector(long nl, long nh);
__declspec(dllexport)  unsigned long *lvector(long nl, long nh);
__declspec(dllexport)  double *dvector(long nl, long nh);
__declspec(dllexport)  float **matrix(long nrl, long nrh, long ncl, long nch);
__declspec(dllexport)  double **dmatrix(long nrl, long nrh, long ncl, long nch);
__declspec(dllexport)  int **imatrix(long nrl, long nrh, long ncl, long nch);
__declspec(dllexport)  float **submatrix(float **a, long oldrl, long oldrh, long oldcl, long oldch,
	long newrl, long newcl);
__declspec(dllexport)  float **convert_matrix(float *a, long nrl, long nrh, long ncl, long nch);
__declspec(dllexport) float ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
__declspec(dllexport) double ***d3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);
__declspec(dllexport) void free_vector(float *v, long nl, long nh);
__declspec(dllexport) void free_ivector(int *v, long nl, long nh);
__declspec(dllexport) void free_cvector(unsigned char *v, long nl, long nh);
 __declspec(dllexport) void free_lvector(unsigned long *v, long nl, long nh);
 __declspec(dllexport) void free_dvector(double *v, long nl, long nh);
 __declspec(dllexport) void free_matrix(float **m, long nrl, long nrh, long ncl, long nch);
 __declspec(dllexport) void free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch);
 __declspec(dllexport) void free_imatrix(int **m, long nrl, long nrh, long ncl, long nch);
 __declspec(dllexport) void free_submatrix(float **b, long nrl, long nrh, long ncl, long nch);
 __declspec(dllexport) void free_convert_matrix(float **b, long nrl, long nrh, long ncl, long nch);
 __declspec(dllexport) void free_f3tensor(float ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh);
 __declspec(dllexport) void free_d3tensor(double ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh);


void balanc(double **a, int n);
void elmhes(double **a, int n);
void hqr(double **a, int n, double wr[], double wi[]);

void spline(double x[], double y[], int n, double yp1, double ypn, double y2[]);
void splint(double xa[], double ya[], double y2a[], int n, double x, double *y);


#endif /* _NR_UTILS_H_ */


