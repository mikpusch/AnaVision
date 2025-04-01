#include "stdafx.h"
#include <math.h>

//#include <stdio.h>
//#include <stddef.h>
//#include <stdlib.h>


#include "nrutil.h"
#include "utils.h"

#define NR_END 1
#define FREE_ARG char*

/*
int IMAX(int a,int b){
	if (a>b) return a;
	return b;
}
*/

void nrerror(char * error_text)
/* Numerical Recipes standard error handler */
{
	CString c = CString(error_text);
	MessageBox(NULL, c, _T("Error in nrutils"), MB_OK);
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

float *fvector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

//	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	v=new float [nh-nl+1+NR_END];
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	int *v;

//	v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
	v=new int [nh-nl+1+NR_END]; 
	if (!v) nrerror("allocation failure in ivector()");
	return v-nl+NR_END;
}

unsigned char *cvector(long nl, long nh)
/* allocate an unsigned char vector with subscript range v[nl..nh] */
{
	unsigned char *v;

	//v=(unsigned char *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned char)));
	v=new unsigned char [nh-nl+1+NR_END];
	if (!v) nrerror("allocation failure in cvector()");
	return v-nl+NR_END;
}

unsigned long *lvector(long nl, long nh)
/* allocate an unsigned long vector with subscript range v[nl..nh] */
{
	unsigned long *v;

//	v=(unsigned long *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(long)));
	v=new unsigned long [nh-nl+1+NR_END];
	if (!v) nrerror("allocation failure in lvector()");
	return v-nl+NR_END;
}

double *dvector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

//	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	v=new double [nh-nl+1+NR_END];
	if (!v) nrerror("allocation failure in dvector()");
	return v-nl+NR_END;
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
//	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	m = new float *[nrow+NR_END];
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
//	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	m[nrl]=new float [nrow*ncol+NR_END];
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

double **dmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	/* allocate pointers to rows */
//	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	m = new double *[nrow+NR_END];
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
//	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	m[nrl]=new double [nrow*ncol+NR_END];
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}
complex<double> **cmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	complex<double> **m;

	/* allocate pointers to rows */
//	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	m = new complex<double> *[nrow+NR_END];
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
//	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	m[nrl]=new complex<double> [nrow*ncol+NR_END];
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

int **imatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	int **m;

	/* allocate pointers to rows */
	//m=(int **) malloc((size_t)((nrow+NR_END)*sizeof(int*)));
	m = new int *[nrow+NR_END];
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;
/* allocate rows and set pointers to them */
//	m[nrl]=(int *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(int)));
	m[nrl]=new int [nrow*ncol+NR_END];
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

float **submatrix(float **a, long oldrl, long oldrh, long oldcl, long oldch,
	long newrl, long newcl)
/* point a submatrix [newrl..][newcl..] to a[oldrl..oldrh][oldcl..oldch] */
{
	long i,j,nrow=oldrh-oldrl+1,ncol=oldcl-newcl;
	float **m;

	/* allocate array of pointers to rows */
//	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	m=new float *[nrow+NR_END];
	if (!m) nrerror("allocation failure in submatrix()");
	m += NR_END;
	m -= newrl;

	/* set pointers to rows */
	for(i=oldrl,j=newrl;i<=oldrh;i++,j++) m[j]=a[i]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

float **convert_matrix(float *a, long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix m[nrl..nrh][ncl..nch] that points to the matrix
declared in the standard C manner as a[nrow][ncol], where nrow=nrh-nrl+1
and ncol=nch-ncl+1. The routine should be called with the address
&a[0][0] as the first argument. */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
//	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	m=new float *[nrow+NR_END];
	if (!m) nrerror("allocation failure in convert_matrix()");
	m += NR_END;
	m -= nrl;

	/* set pointers to rows */
	m[nrl]=a-ncl;
	for(i=1,j=nrl+1;i<nrow;i++,j++) m[j]=m[j-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}

float ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh)
/* allocate a float 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	float ***t;

	/* allocate pointers to pointers to rows */
	//t=(float ***) malloc((size_t)((nrow+NR_END)*sizeof(float**)));
	t=new float **[nrow+NR_END];
	if (!t) nrerror("allocation failure 1 in f3tensor()");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
//	t[nrl]=(float **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float*)));
	t[nrl]=new float *[nrow*ncol+NR_END];
	if (!t[nrl]) nrerror("allocation failure 2 in f3tensor()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
//	t[nrl][ncl]=(float *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(float)));
	t[nrl][ncl]=new float [nrow*ncol*ndep+NR_END];
	if (!t[nrl][ncl]) nrerror("allocation failure 3 in f3tensor()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* return pointer to array of pointers to rows */
	return t;
}


double ***d3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh)
/* allocate a double 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	double ***t;

	/* allocate pointers to pointers to rows */
	//t=(double ***) malloc((size_t)((nrow+NR_END)*sizeof(double**)));
	t=new double **[nrow+NR_END];
	if (!t) nrerror("allocation failure 1 in f3tensor()");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	//t[nrl]=(double **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double*)));
	t[nrl]=new double *[nrow*ncol+NR_END];
	if (!t[nrl]) nrerror("allocation failure 2 in f3tensor()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
//	t[nrl][ncl]=(double *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(double)));
	t[nrl][ncl]=new double [nrow*ncol*ndep+NR_END];
	if (!t[nrl][ncl]) nrerror("allocation failure 3 in f3tensor()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* return pointer to array of pointers to rows */
	return t;
}


complex <double> ***c3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh)
/* allocate a double 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	complex <double> ***t;

	/* allocate pointers to pointers to rows */
	//t=(double ***) malloc((size_t)((nrow+NR_END)*sizeof(double**)));
	t=new complex <double> **[nrow+NR_END];
	if (!t) nrerror("allocation failure 1 in f3tensor()");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	//t[nrl]=(double **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double*)));
	t[nrl]=new complex <double> *[nrow*ncol+NR_END];
	if (!t[nrl]) nrerror("allocation failure 2 in f3tensor()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
//	t[nrl][ncl]=(double *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(double)));
	t[nrl][ncl]=new complex <double> [nrow*ncol*ndep+NR_END];
	if (!t[nrl][ncl]) nrerror("allocation failure 3 in f3tensor()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* return pointer to array of pointers to rows */
	return t;
}



void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	//free((FREE_ARG) (v+nl-NR_END));
	delete [] (v+nl-NR_END);
}

void free_ivector(int *v, long nl, long nh)
/* free an int vector allocated with ivector() */
{
//	free((FREE_ARG) (v+nl-NR_END));
	delete [] (v+nl-NR_END);
}

void free_cvector(unsigned char *v, long nl, long nh)
/* free an unsigned char vector allocated with cvector() */
{
//	free((FREE_ARG) (v+nl-NR_END));
	delete [] (v+nl-NR_END);
}

void free_lvector(unsigned long *v, long nl, long nh)
/* free an unsigned long vector allocated with lvector() */
{
//	free((FREE_ARG) (v+nl-NR_END));
	delete [] (v+nl-NR_END);

}

void free_dvector(double *v, long nl, long nh)
/* free a double vector allocated with dvector() */
{
//	free((FREE_ARG) (v+nl-NR_END));
	delete [] (v+nl-NR_END);
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
//	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	delete [] (m[nrl]+ncl-NR_END);

//	free((FREE_ARG) (m+nrl-NR_END));
	delete [] (m+nrl-NR_END);
}

void free_dmatrix(double **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by dmatrix() */
{
//	free((FREE_ARG) (m[nrl]+ncl-NR_END));
//	free((FREE_ARG) (m+nrl-NR_END));
	delete [] (m[nrl]+ncl-NR_END);
	delete [] (m+nrl-NR_END);
}
void free_cmatrix(complex<double> **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by dmatrix() */
{
//	free((FREE_ARG) (m[nrl]+ncl-NR_END));
//	free((FREE_ARG) (m+nrl-NR_END));
	delete [] (m[nrl]+ncl-NR_END);
	delete [] (m+nrl-NR_END);
}

void free_imatrix(int **m, long nrl, long nrh, long ncl, long nch)
/* free an int matrix allocated by imatrix() */
{
//	free((FREE_ARG) (m[nrl]+ncl-NR_END));
//	free((FREE_ARG) (m+nrl-NR_END));
	delete [] (m[nrl]+ncl-NR_END);
	delete [] (m+nrl-NR_END);
}

void free_submatrix(float **b, long nrl, long nrh, long ncl, long nch)
/* free a submatrix allocated by submatrix() */
{
//	free((FREE_ARG) (b+nrl-NR_END));
	delete [] (b+nrl-NR_END);
}

void free_convert_matrix(float **b, long nrl, long nrh, long ncl, long nch)
/* free a matrix allocated by convert_matrix() */
{
	free((FREE_ARG) (b+nrl-NR_END));
	delete [] (b+nrl-NR_END);
}

void free_f3tensor(float ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh)
/* free a float f3tensor allocated by f3tensor() */
{
//	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
//	free((FREE_ARG) (t[nrl]+ncl-NR_END));
//	free((FREE_ARG) (t+nrl-NR_END));
	delete [] (t[nrl][ncl]+ndl-NR_END);
	delete [] (t[nrl]+ncl-NR_END);
	delete [] (t+nrl-NR_END);
}


void free_d3tensor(double ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh)
/* free a double f3tensor allocated by f3tensor() */
{
//	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
//	free((FREE_ARG) (t[nrl]+ncl-NR_END));
//	free((FREE_ARG) (t+nrl-NR_END));
	delete [] (t[nrl][ncl]+ndl-NR_END);
	delete [] (t[nrl]+ncl-NR_END);
	delete [] (t+nrl-NR_END);
}

void free_c3tensor(complex <double> ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh)
/* free a double f3tensor allocated by f3tensor() */
{
//	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
//	free((FREE_ARG) (t[nrl]+ncl-NR_END));
//	free((FREE_ARG) (t+nrl-NR_END));
	delete [] (t[nrl][ncl]+ndl-NR_END);
	delete [] (t[nrl]+ncl-NR_END);
	delete [] (t+nrl-NR_END);
}



#define RADIX 2.0

void balanc(double **a, int n)
{
	int last,j,i;
	double s,r,g,f,c,sqrdx;

	sqrdx=RADIX*RADIX;
	last=0;
	while (last == 0) {
		last=1;
		for (i=1;i<=n;i++) {
			r=c=0.0;
			for (j=1;j<=n;j++)
				if (j != i) {
					c += fabs(a[j][i]);
					r += fabs(a[i][j]);
				}
			if (c && r) {
				g=r/RADIX;
				f=1.0;
				s=c+r;
				while (c<g) {
					f *= RADIX;
					c *= sqrdx;
				}
				g=r*RADIX;
				while (c>g) {
					f /= RADIX;
					c /= sqrdx;
				}
				if ((c+r)/f < 0.95*s) {
					last=0;
					g=1.0/f;
					for (j=1;j<=n;j++) a[i][j] *= g;
					for (j=1;j<=n;j++) a[j][i] *= f;
				}
			}
		}
	}
}
#undef RADIX


#define SWAP(g,h) {y=(g);(g)=(h);(h)=y;}

void elmhes(double **a, int n)
{
	int m,j,i;
	double y,x;

	for (m=2;m<n;m++) {
		x=0.0;
		i=m;
		for (j=m;j<=n;j++) {
			if (fabs(a[j][m-1]) > fabs(x)) {
				x=a[j][m-1];
				i=j;
			}
		}
		if (i != m) {
			for (j=m-1;j<=n;j++) SWAP(a[i][j],a[m][j])
			for (j=1;j<=n;j++) SWAP(a[j][i],a[j][m])
		}
		if (x) {
			for (i=m+1;i<=n;i++) {
				if ((y=a[i][m-1]) != 0.0) {
					y /= x;
					a[i][m-1]=y;
					for (j=m;j<=n;j++)
						a[i][j] -= y*a[m][j];
					for (j=1;j<=n;j++)
						a[j][m] += y*a[j][i];
				}
			}
		}
	}
}
#undef SWAP



void hqr(double **a, int n, double wr[], double wi[])
{
	int nn,m,l,k,j,its,i,mmin;
	double z,y,x,w,v,u,t,s,r,q,p,anorm;

	anorm=0.0;
	for (i=1;i<=n;i++)
		for (j=IMAX(i-1,1);j<=n;j++)
			anorm += fabs(a[i][j]);
	nn=n;
	t=0.0;
	while (nn >= 1) {
		its=0;
		do {
			for (l=nn;l>=2;l--) {
				s=fabs(a[l-1][l-1])+fabs(a[l][l]);
				if (s == 0.0) s=anorm;
				if ((double)(fabs(a[l][l-1]) + s) == s) break;
			}
			x=a[nn][nn];
			if (l == nn) {
				wr[nn]=x+t;
				wi[nn--]=0.0;
			} else {
				y=a[nn-1][nn-1];
				w=a[nn][nn-1]*a[nn-1][nn];
				if (l == (nn-1)) {
					p=0.5*(y-x);
					q=p*p+w;
					z=sqrt(fabs(q));
					x += t;
					if (q >= 0.0) {
						z=p+SIGN(z,p);
						wr[nn-1]=wr[nn]=x+z;
						if (z) wr[nn]=x-w/z;
						wi[nn-1]=wi[nn]=0.0;
					} else {
						wr[nn-1]=wr[nn]=x+p;
						wi[nn-1]= -(wi[nn]=z);
					}
					nn -= 2;
				} else {
					if (its == 30) nrerror("Too many iterations in hqr");
					if (its == 10 || its == 20) {
						t += x;
						for (i=1;i<=nn;i++) a[i][i] -= x;
						s=fabs(a[nn][nn-1])+fabs(a[nn-1][nn-2]);
						y=x=0.75*s;
						w = -0.4375*s*s;
					}
					++its;
					for (m=(nn-2);m>=l;m--) {
						z=a[m][m];
						r=x-z;
						s=y-z;
						p=(r*s-w)/a[m+1][m]+a[m][m+1];
						q=a[m+1][m+1]-z-r-s;
						r=a[m+2][m+1];
						s=fabs(p)+fabs(q)+fabs(r);
						p /= s;
						q /= s;
						r /= s;
						if (m == l) break;
						u=fabs(a[m][m-1])*(fabs(q)+fabs(r));
						v=fabs(p)*(fabs(a[m-1][m-1])+fabs(z)+fabs(a[m+1][m+1]));
						if ((double)(u+v) == v) break;
					}
					for (i=m+2;i<=nn;i++) {
						a[i][i-2]=0.0;
						if (i != (m+2)) a[i][i-3]=0.0;
					}
					for (k=m;k<=nn-1;k++) {
						if (k != m) {
							p=a[k][k-1];
							q=a[k+1][k-1];
							r=0.0;
							if (k != (nn-1)) r=a[k+2][k-1];
							if ((x=fabs(p)+fabs(q)+fabs(r)) != 0.0) {
								p /= x;
								q /= x;
								r /= x;
							}
						}
						if ((s=SIGN(sqrt(p*p+q*q+r*r),p)) != 0.0) {
							if (k == m) {
								if (l != m)
								a[k][k-1] = -a[k][k-1];
							} else
								a[k][k-1] = -s*x;
							p += s;
							x=p/s;
							y=q/s;
							z=r/s;
							q /= p;
							r /= p;
							for (j=k;j<=nn;j++) {
								p=a[k][j]+q*a[k+1][j];
								if (k != (nn-1)) {
									p += r*a[k+2][j];
									a[k+2][j] -= p*z;
								}
								a[k+1][j] -= p*y;
								a[k][j] -= p*x;
							}
							mmin = nn<k+3 ? nn : k+3;
							for (i=l;i<=mmin;i++) {
								p=x*a[i][k]+y*a[i][k+1];
								if (k != (nn-1)) {
									p += z*a[i][k+2];
									a[i][k+2] -= p*r;
								}
								a[i][k+1] -= p*q;
								a[i][k] -= p;
							}
						}
					}
				}
			}
		} while (l < nn-1);
	}
}


void spline(double x[], double y[], int n, double yp1, double ypn, double y2[])
{
	int i,k;
	double p,qn,sig,un,*u;

	u=dvector(1,n-1);
	if (yp1 > 0.99e200)
		y2[1]=u[1]=0.0;
	else {
		y2[1] = -0.5;
		u[1]=(3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
	}
	for (i=2;i<=n-1;i++) {
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	if (ypn > 0.99e200)
		qn=un=0.0;
	else {
		qn=0.5;
		un=(3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1]));
	}
	y2[n]=(un-qn*u[n-1])/(qn*y2[n-1]+1.0);
	for (k=n-1;k>=1;k--)
		y2[k]=y2[k]*y2[k+1]+u[k];
	free_dvector(u,1,n-1);
}

void splint(double xa[], double ya[], double y2a[], int n, double x, double *y)
{
//	void nrerror(char error_text[]);
	int klo,khi,k;
	double h,b,a;

	klo=1;
	khi=n;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (xa[k] > x) khi=k;
		else klo=k;
	}
	h=xa[khi]-xa[klo];
//	if (h == 0.0) nrerror("Bad xa input to routine splint");
	a=(xa[khi]-x)/h;
	b=(x-xa[klo])/h;
	*y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
}



void pinvs(int ie1, int ie2, int je1, int jsf, int jc1, int k, double ***c,
	double **s)
{

	int js1,jpiv,jp,je2,jcoff,j,irow,ipiv,id,icoff,i,*indxr;
	double pivinv,piv,dum,big,*pscl;

	indxr=ivector(ie1,ie2);
	pscl=dvector(ie1,ie2);
	je2=je1+ie2-ie1;
	js1=je2+1;

	if (	SetOrGetDoShowMessage(false, false)){
		ShowFloat(ie1, "ie1");
		ShowFloat(ie2, "ie2");
		ShowFloat(je1, "je1");
		ShowFloat(je2, "je2");

	}
	else{
//		Alert0("ShowMessageNrUtile = false");
	}
	for (i=ie1;i<=ie2;i++) {
		big=0.0;
		for (j=je1;j<=je2;j++)
			if (fabs(s[i][j]) > big) big=fabs(s[i][j]);
		if (big == 0.0) nrerror("Singular matrix - row all 0, in pinvs");
		if (SetOrGetDoShowMessage(false, false)){
			ShowFloat(big, "big");
		}
		pscl[i]=1.0/big;
		indxr[i]=0;
	}
	for (id=ie1;id<=ie2;id++) {
		piv=0.0;
		for (i=ie1;i<=ie2;i++) {
			if (indxr[i] == 0) {
				big=0.0;
				for (j=je1;j<=je2;j++) {
					if (fabs(s[i][j]) > big) {
						jp=j;
						big=fabs(s[i][j]);
					}
				}
				if (big*pscl[i] > piv) {
					ipiv=i;
					jpiv=jp;
					piv=big*pscl[i];
				}
			}
		}
		if (s[ipiv][jpiv] == 0.0) nrerror("Singular matrix in routine pinvs");
		indxr[ipiv]=jpiv;
		pivinv=1.0/s[ipiv][jpiv];
		for (j=je1;j<=jsf;j++) s[ipiv][j] *= pivinv;
		s[ipiv][jpiv]=1.0;
		for (i=ie1;i<=ie2;i++) {
			if (indxr[i] != jpiv) {
				if (s[i][jpiv]) {
					dum=s[i][jpiv];
					for (j=je1;j<=jsf;j++)
						s[i][j] -= dum*s[ipiv][j];
					s[i][jpiv]=0.0;
				}
			}
		}
	}
	jcoff=jc1-js1;
	icoff=ie1-je1;
	for (i=ie1;i<=ie2;i++) {
		irow=indxr[i]+icoff;
		for (j=js1;j<=jsf;j++) c[irow][j+jcoff][k]=s[i][j];
	}
	free_dvector(pscl,ie1,ie2);
	free_ivector(indxr,ie1,ie2);
}

void red(int iz1, int iz2, int jz1, int jz2, int jm1, int jm2, int jmf,
	int ic1, int jc1, int jcf, int kc, double ***c, double **s)
{
	int loff,l,j,ic,i;
	double vx;

	loff=jc1-jm1;
	ic=ic1;
	for (j=jz1;j<=jz2;j++) {
		for (l=jm1;l<=jm2;l++) {
			vx=c[ic][l+loff][kc];
			for (i=iz1;i<=iz2;i++) s[i][l] -= s[i][j]*vx;
		}
		vx=c[ic][jcf][kc];
		for (i=iz1;i<=iz2;i++) s[i][jmf] -= s[i][j]*vx;
		ic += 1;
	}
}

void bksub(int ne, int nb, int jf, int k1, int k2, double ***c)
{
	int nbf,im,kp,k,j,i;
	double xx;

	nbf=ne-nb;
	im=1;
	for (k=k2;k>=k1;k--) {
		if (k == k1) im=nbf+1;
		kp=k+1;
		for (j=1;j<=nbf;j++) {
			xx=c[j][jf][kp];
			for (i=im;i<=ne;i++)
				c[i][jf][k] -= c[i][j][k]*xx;
		}
	}
	for (k=k1;k<=k2;k++) {
		kp=k+1;
		for (i=1;i<=nb;i++) c[i][1][k]=c[i+nbf][jf][k];
		for (i=1;i<=nbf;i++) c[i+nb][1][k]=c[i][jf][kp];
	}
}

bool SetOrGetDoShowMessage(bool set, bool value){
	static bool ShowMessageNrUtile = false;

	if (set)	ShowMessageNrUtile=value;
	return ShowMessageNrUtile;
}


