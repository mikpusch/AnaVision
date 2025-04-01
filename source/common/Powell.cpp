// This file implements the Powell class


#include "stdafx.h"

#include "../common/Powell.h"
#include "../common/nrutil.h"
//#include "../common/utils.h"

#include <math.h>

Powell::Powell(){
	STUFF = NULL;
	InitValues();

	xicom = pcom = xt = NULL;
	pt = ptt = xit = NULL;
	xi=NULL;
	ActualSize = 0;
}

void Powell::InitValues(){

	Initialized = false;
	COSTFUNCTION = NULL;
	DIMENS = 0;

	ftol = 1e-7;
	ITMAX = 200;
	ITMAX = 100;
}

Powell::~Powell(){
	DeleteArrays();
}

void Powell::DeleteArrays(){
	if (xicom) free_dvector(xicom,1,ActualSize);
	if (pcom) free_dvector(pcom,1,ActualSize);
	if (xt) free_dvector(xt,1,ActualSize);
	if (pt) free_dvector(pt,1,ActualSize);
	if (ptt) free_dvector(ptt,1,ActualSize);
	if (xit) free_dvector(xit,1,ActualSize);
	if (xi) free_dmatrix(xi,1, ActualSize, 1, ActualSize);

	xicom = pcom = xt = NULL;
	pt = ptt = xit = NULL;
	xi=NULL;

}

void Powell::AllocArrays(int dimens){
	if (dimens<ActualSize){
		return;
	}
	DeleteArrays();
	ActualSize = dimens;

	pcom=dvector(1,dimens);
	xicom=dvector(1,dimens);
	xt=dvector(1,dimens);

	pt=dvector(1,dimens);
	ptt=dvector(1,dimens);
	xit=dvector(1,dimens);

	xi=dmatrix(1, dimens, 1, dimens);
}


bool Powell::Init(int n, // dimension
		powell_costfunction_type CostFunction, // this must be supplied
		int * stuff		){

	if (n<1){
		return false;
	}
	if (CostFunction == NULL){
		return false;
	}

	InitValues();

	STUFF = stuff;
	DIMENS = n;
	COSTFUNCTION = CostFunction;
	AllocArrays(DIMENS);
	Initialized = true;
	return true;
}

bool Powell::MinimizeCost(double * x, // start value and on return end value
							  double * CostAtEnd){
	if (!Initialized){
		return false;
	}
	int i, j;

	for (i=1; i<=DIMENS; i++){
		for (j=1; j<=DIMENS; j++){
			xi[i][j] = 0;
			if (i==j){
				xi[i][j]=1.0;
			}
		}
	}
	double * p = x-1;
	int iter;
	powell(p, DIMENS, ftol, &iter, CostAtEnd);
	return true;
}


double Powell::CalcValue(double * x){
	double f;
	double * xx = x+1;
	COSTFUNCTION(&f, xx, STUFF);
	return f;
}

/*
double Powell::f1dim(double x){
	int j;
//	double *xt;

//	double f;

//	xt=dvector(1,ncom);
	for (j=1;j<=DIMENS;j++){
		xt[j] = pcom[j] + x*xicom[j];
	}
	return CalcValue(xt);
//	COSTFUNCTION(&f, xt, STUFF);
//	free_dvector(xt,1,ncom);
//	return f;
}
*/

#define TOL_linmin 2.0e-8
void Powell::linmin(double p[], double xi[], int n, double *fret){//, double (*func)(double [])){
/*
	double brent(double ax, double bx, double cx,
		double (*f)(double), double tol, double *xmin);
	double f1dim(double x);
	void mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb,
		double *fc, double (*func)(double));
*/
	int j;
	double xx,xmin,fx,fb,fa,bx,ax;

//	ncom=n;
//	pcom=dvector(1,n);
//	xicom=dvector(1,n);
//	nrfunc=func;
	for (j=1;j<=n;j++) {
		pcom[j]=p[j];
		xicom[j]=xi[j];
	}
	ax=0.0;
	xx=1.0;
	mnbrak(&ax,&xx,&bx,&fa,&fx,&fb); //,f1dim);
//xxx	*fret=brent(ax,xx,bx,f1dim,TOL,&xmin);
	*fret=brent(ax,xx,bx,TOL_linmin,&xmin);
	for (j=1;j<=n;j++) {
		xi[j] *= xmin;
		p[j] += xi[j];
	}
//	free_dvector(xicom,1,n);
//	free_dvector(pcom,1,n);
}
#undef TOL_linmin

#define GOLD_mnbrak 1.618034
#define GLIMIT_mnbrak 100.0
#define TINY_mnbrak 1.0e-20
#define SHFT_mnbrak(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);

void Powell::mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb, double *fc){
//	double (*func)(double))
	double ulim,u,r,q,fu,dum;

	*fa=f1dim(*ax);
	*fb=f1dim(*bx);
	if (*fb > *fa) {
		SHFT_mnbrak(dum,*ax,*bx,dum)
		SHFT_mnbrak(dum,*fb,*fa,dum)
	}
	*cx=(*bx)+GOLD_mnbrak*(*bx-*ax);
	*fc=f1dim(*cx);
	while (*fb > *fc) {
		r=(*bx-*ax)*(*fb-*fc);
		q=(*bx-*cx)*(*fb-*fa);
//		u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/(2.0*SIGN(FMAX(fabs(q-r),TINY_mnbrak),q-r));
		u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/(2.0*SIGN(DMAX(fabs(q-r),TINY_mnbrak),q-r));
		ulim=(*bx)+GLIMIT_mnbrak*(*cx-*bx);
		if ((*bx-u)*(u-*cx) > 0.0) {
			fu=f1dim(u);
			if (fu < *fc) {
				*ax=(*bx);
				*bx=u;
				*fa=(*fb);
				*fb=fu;
				return;
			} else if (fu > *fb) {
				*cx=u;
				*fc=fu;
				return;
			}
			u=(*cx)+GOLD_mnbrak*(*cx-*bx);
			fu=f1dim(u);
		} else if ((*cx-u)*(u-ulim) > 0.0) {
			fu=f1dim(u);
			if (fu < *fc) {
				SHFT_mnbrak(*bx,*cx,u,*cx+GOLD_mnbrak*(*cx-*bx))
				SHFT_mnbrak(*fb,*fc,fu,f1dim(u))
			}
		} else if ((u-ulim)*(ulim-*cx) >= 0.0) {
			u=ulim;
			fu=f1dim(u);
		} else {
			u=(*cx)+GOLD_mnbrak*(*cx-*bx);
			fu=f1dim(u);
		}
		SHFT_mnbrak(*ax,*bx,*cx,u)
		SHFT_mnbrak(*fa,*fb,*fc,fu)
	}
}
#undef GOLD_mnbrak
#undef GLIMIT_mnbrak
#undef TINY_mnbrak
#undef SHFT_mnbrak



//#define ITMAX_brent 100
#define ITMAX_brent 50
#define CGOLD_brent 0.3819660
//#define ZEPS_brent 1.0e-10
//#define ZEPS_brent 1.0e-20
#define ZEPS_brent 1.0e-10
#define SHFT_brent(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);

double Powell::brent(double ax, double bx, double cx, double tol, double *xmin){
	int iter;
	double a,b,d,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
	double e=0.0;

	a=(ax < cx ? ax : cx);
	b=(ax > cx ? ax : cx);
	x=w=v=bx;
//	fw=fv=fx=(*f)(x);
	fw=fv=fx=f1dim(x);
	for (iter=1;iter<=ITMAX_brent;iter++) {
		xm=0.5*(a+b);
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS_brent);
		if (fabs(x-xm) <= (tol2-0.5*(b-a))) {
			*xmin=x;
			return fx;
		}
		if (iter==ITMAX_brent){
			//Beep(1000,10);
			*xmin=x;
			return fx;
		}
		if (fabs(e) > tol1) {
			r=(x-w)*(fx-fv);
			q=(x-v)*(fx-fw);
			p=(x-v)*q-(x-w)*r;
			q=2.0*(q-r);
			if (q > 0.0) p = -p;
			q=fabs(q);
			etemp=e;
			e=d;
			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
				d=CGOLD_brent*(e=(x >= xm ? a-x : b-x));
			else {
				d=p/q;
				u=x+d;
				if (u-a < tol2 || b-u < tol2)
					d=SIGN(tol1,xm-x);
			}
		} else {
			d=CGOLD_brent*(e=(x >= xm ? a-x : b-x));
		}
		u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
//		fu=(*f)(u);
		fu=f1dim(u);
		if (fu <= fx) {
			if (u >= x) a=x; else b=x;
			SHFT_brent(v,w,x,u)
			SHFT_brent(fv,fw,fx,fu)
		} else {
			if (u < x) a=u; else b=u;
			if (fu <= fw || w == x) {
				v=w;
				w=u;
				fv=fw;
				fw=fu;
			} else if (fu <= fv || v == x || v == w) {
				v=u;
				fv=fu;
			}
		}
	}
//	nrerror("Too many iterations in brent");
	return 0;
}
#undef ITMAX_brent
#undef CGOLD_brent
#undef ZEPS_brent
#undef SHFT_brent




void Powell::powell(double p[], int n, double ftol, int *iter, double *fret){
//		void linmin(float p[], float xi[], int n, float *fret,
//		float (*func)(float []));

	int i,ibig,j;
	double del,fp,fptt,t;
//	,*pt,*ptt,*xit;

//	pt=vector(1,n);
//	ptt=vector(1,n);
//	xit=vector(1,n);

//	*fret=(*func)(p);
	*fret=CalcValue(p);
	for (j=1;j<=n;j++) pt[j]=p[j];
	for (*iter=1;;++(*iter)) {
		fp=(*fret);
		ibig=0;
		del=0.0;
		for (i=1;i<=n;i++) {
			for (j=1;j<=n;j++) xit[j]=xi[j][i];
			fptt=(*fret);
//			linmin(p,xit,n,fret,func);
			linmin(p,xit,n,fret);
			if (fabs(fptt-(*fret)) > del) {
				del=fabs(fptt-(*fret));
				ibig=i;
			}
		}
		if (2.0*fabs(fp-(*fret)) <= ftol*(fabs(fp)+fabs(*fret))) {
//			free_vector(xit,1,n);
//			free_vector(ptt,1,n);
//			free_vector(pt,1,n);
			return;
		}
		if (*iter == ITMAX){
			nrerror("powell exceeding maximum iterations.");
			return;
		}
		for (j=1;j<=n;j++) {
			ptt[j]=2.0*p[j]-pt[j];
			xit[j]=p[j]-pt[j];
			pt[j]=p[j];
		}
//		fptt=(*func)(ptt);
		fptt=CalcValue(ptt);
		if (fptt < fp) {
			t=2.0*(fp-2.0*(*fret)+fptt)*DSQR(fp-(*fret)-del)-del*DSQR(fp-fptt);
			if (t < 0.0) {
//				linmin(p,xit,n,fret,func);
				linmin(p,xit,n,fret);
				for (j=1;j<=n;j++) {
					xi[j][ibig]=xi[j][n];
					xi[j][n]=xit[j];
				}
			}
		}
	}
}

