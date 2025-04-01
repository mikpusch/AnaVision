// This file implements the cg_descent class


#include "stdafx.h"


#include "../common/cg_descent.h"
#include "../common/nrutil.h"
//#include "../common/utils.h"

#include <math.h>


#define CON_dfridr 1.4
#define CON2_dfridr (CON_dfridr*CON_dfridr)
#define BIG_dfridr 1.0e100
#define NTAB_dfridr 10
#define SAFE_dfridr 2.0


cg_descent::cg_descent(){
	d = g = xtemp = gtemp = xp = xm = NULL;
	amatrix = NULL;
	ActualSize = 0;
	STUFF = NULL;
	InitValues();
}

void cg_descent::InitValues(){

    cgparms_1.delta = .1;
    cgparms_1.sigma = .9;
    cgparms_1.eps = 1e-6;
//    cgparms_1.eps = 1e-20;
    cgparms_1.gamma = .66;
    cgparms_1.rho = 5.;
    cgparms_1.eta = .01;
    cgparms_1.psi0 = .01;
    cgparms_1.psi1 = .1;
    cgparms_1.psi2 = 2.;
    cgparms_1.quadcutoff = 1e-12;
//    cgparms_1.quadcutoff = 1e-8;
    cgparms_1.stopfac = 0.;
    cgparms_1.awolfefac = .001;
    cgparms_1.feps = 0.;
    cgparms_1.qdecay = .7;
    cgparms_1.nexpand = 50;
    cgparms_1.nsecant = 50;

    cgparms_1.pertrule = TRUE_;
//    cgparms_1.pertrule = FALSE_;

    cgparms_1.quadstep = TRUE_;
//	cgparms_1.quadstep = FALSE_;

    cgparms_1.printlevel = TRUE_;
    cgparms_1.printlevel = FALSE_;
 
	cgparms_1.printfinal = TRUE_;
    cgparms_1.printfinal = FALSE_;
  
	cgparms_1.stoprule = TRUE_;
//    cgparms_1.stoprule = FALSE_;

    cgparms_1.awolfe = TRUE_;
//    cgparms_1.awolfe = FALSE_;

    cgparms_1.step = FALSE_;
//    cgparms_1.step = TRUE_;

    cgparms_1.debug = FALSE_;

    cgparms_1.zero = 0.;
    cgparms_1.info = 0;

	Initialized = false;
	COSTFUNCTION = NULL;
	GRADIENT = NULL;
	CalcGradientNumerically = true;
	DIMENS = 0;


}

cg_descent::~cg_descent(){
	DeleteArrays();
}

void cg_descent::DeleteArrays(){
	if (xm) delete [] xm;
	if (xp) delete [] xp;
	if (d) delete [] d;
	if (g) delete [] g;
	if (xtemp) delete [] xtemp;
	if (gtemp) delete [] gtemp;
	if (amatrix){
		free_dmatrix(amatrix,1,NTAB_dfridr,1,NTAB_dfridr);
	}
	d = g = xtemp = gtemp = xp = xm = NULL;
	amatrix = NULL;
	

}

void cg_descent::AllocArrays(int dimens){
	if (dimens<ActualSize){
		return;
	}
	DeleteArrays();
	ActualSize = dimens;
	d = new double [ActualSize];
	g =  new double [ActualSize];
	xtemp =  new double [ActualSize];
	gtemp =  new double [ActualSize];;
	xp = new double [ActualSize];
	xm = new double [ActualSize];
	amatrix=dmatrix(1,NTAB_dfridr,1,NTAB_dfridr);
}


bool cg_descent::Init(int n, // dimension
		cg_descent_costfunction_type CostFunction, // this must be supplied
		cg_descent_costfunction_gradient_type Gradient, // is this is NULL, the gradient will be computed numerically
		int * stuff
		){

	InitValues();
	if (n<1){
		return false;
	}
	if (CostFunction == NULL){
		return false;
	}
	STUFF = stuff;
	DIMENS = n;
	COSTFUNCTION = CostFunction;
	GRADIENT = Gradient;
	if (GRADIENT){
		CalcGradientNumerically = false;
	}
	else{
		CalcGradientNumerically = true;
	}
	AllocArrays(DIMENS);
	Initialized = true;
	return true;
}

bool cg_descent::MinimizeCost(double * x, // start value and on return end value
							  double * CostAtEnd){
	if (!Initialized){
		return false;
	}

	doublereal grad_tol__=1e-10;
	integer iter;
	integer nfunc;
	integer ngrad;

	cg_descent__(&grad_tol__, x, &DIMENS, NULL, NULL, &status, 
				&gnorm, CostAtEnd, &iter, &nfunc, 
				&ngrad, d, g, xtemp, gtemp);

//	ShowFloat(status, "status");

	return true;
}


void cg_descent::CalcValue(double * f, double * x){
//	double * y = x;
//	ShowFloat(x[0], "y0");
	COSTFUNCTION(f, x, STUFF);
//	ShowFloat(f[0], "f");

}
void cg_descent::CalcGradient(double * g, double * x){
//	double * y = x;
//	ShowFloat(y[0], "y0");
	if (CalcGradientNumerically){
		DoCalcGradientNumerically(g, x);
	}
	else{
		GRADIENT(g, x, STUFF);
	}
//	ShowFloat(g[0], "g");
}

void cg_descent::DoCalcGradientNumerically(double * g, double * x){
	int i;
	double err;
	for (i=0; i<DIMENS; i++){
//		g[i] =  dfridr(i, x, 1.0e-3, &err);
//		g[i] =  dfridr(i, x, 1.0e-2, &err);
		g[i] =  dfridr(i, x, 0.2, &err);
	}

}



double cg_descent::dfridr(int index, double * x, double h, double *err)
{
	int i,j;
	double errt,fac,hh, ans;

	if (h == 0.0) nrerror("h must be nonzero in dfridr.");

	for (i=0; i<DIMENS; i++){
		xp[i] = xm[i] = x[i];
	}

	hh=h;
	xp[index] = x[index]+hh;
	xm[index] = x[index]-hh;

	double vp, vm;

	CalcValue(&vp,xp); 
	CalcValue(&vm,xm); 

//	a[1][1]=((*func)(x+hh)-(*func)(x-hh))/(2.0*hh);
	amatrix[1][1]=(vp-vm)/(2.0*hh);

	*err=BIG_dfridr;
	for (i=2;i<=NTAB_dfridr;i++) {
		hh /= CON_dfridr;
		xp[index] = x[index]+hh;
		xm[index] = x[index]-hh;
		CalcValue(&vp,xp); 
		CalcValue(&vm,xm); 

//		a[1][i]=((*func)(x+hh)-(*func)(x-hh))/(2.0*hh);
		amatrix[1][i]=(vp-vm)/(2.0*hh);

		fac=CON2_dfridr;
		for (j=2;j<=i;j++) {
			amatrix[j][i]=(amatrix[j-1][i]*fac-amatrix[j-1][i-1])/(fac-1.0);
			fac=CON2_dfridr*fac;
			errt=DMAX(fabs(amatrix[j][i]-amatrix[j-1][i]),fabs(amatrix[j][i]-amatrix[j-1][i-1]));
			if (errt <= *err) {
				*err=errt;
				ans=amatrix[j][i];
			}
		}
		if (fabs(amatrix[i][i]-amatrix[i-1][i-1]) >= SAFE_dfridr*(*err)) break;
	}
	return ans;
}
#undef CON_dfridr
#undef CON2_dfridr
#undef BIG_dfridr
#undef NTAB_dfridr
#undef SAFE_dfridr

/* Subroutine */ 

int cg_descent::cg_descent__(doublereal *grad_tol__, doublereal *x, 
	int *dim, S_fp cg_value__, S_fp cg_grad__, integer *status, 
	doublereal *gnorm, doublereal *f, integer *iter, integer *nfunc, 
	integer *ngrad, doublereal *d__, doublereal *g, doublereal *xtemp, 
	doublereal *gtemp)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3, d__4, d__5;

    /* Builtin functions */
///////    double sqrt(doublereal);
///////    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
///    extern /* Subroutine */ int cg_linew__(doublereal *, doublereal *, 
///	    doublereal *, doublereal *, doublereal *, doublereal *, 
///	    doublereal *, doublereal *, S_fp, S_fp);

    static integer i__, j;
    static doublereal t;
    static integer i1, i2, i3, i4;
    static doublereal t1, t2, t3, t4, qk, yk, yk2, beta, dphi, dkyk, ykgk, 
	    dphi0, alpha, denom, ftemp, xnorm, delta2, dnorm2, gnorm2;
//    extern logical cg_tol__(doublereal *, doublereal *);
    static doublereal talpha, eta_sq__;
///    extern /* Subroutine */ int cg_line__(doublereal *, doublereal *, 
///	    doublereal *, doublereal *, doublereal *, doublereal *, 
///	    doublereal *, doublereal *, S_fp, S_fp), cg_init__(doublereal *, 
///	    integer *), cg_step__(doublereal *, doublereal *, doublereal *, 
///	    doublereal *);

/* initialize the parameters */
    /* Parameter adjustments */
    --gtemp;
    --xtemp;
    --g;
    --d__;
    --x;

    /* Function Body */
    cg_init__(grad_tol__, dim);
    if (cgparms_1.step) {
		alpha = *gnorm;
    }
    delta2 = cgparms_1.delta * 2 - 1;
    eta_sq__ = cgparms_1.eta * cgparms_1.eta;
    *iter = 0;
    cgparms_1.ck = 0.;
    qk = 0.;
/* initial function and gradient evaluations, initial direction */

    //(*cg_value__)(f, &x[1], &cgparms_1.n);
	CalcValue(f, &x[1]);
//	ShowFloat(x[1], "x[1]");

    ++cgparms_1.nf;

//    (*cg_grad__)(&g[1], &x[1], &cgparms_1.n);
	CalcGradient(&g[1], &x[1]);
//	ShowFloat(g[1], "g[1]");
 

	++cgparms_1.ng;
    cgparms_1.f0 = *f + *f;
    *gnorm = cgparms_1.zero;
    xnorm = cgparms_1.zero;
    gnorm2 = cgparms_1.zero;

    i__1 = cgparms_1.n5;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	d__2 = xnorm, d__3 = (d__1 = x[i__], f2c_abs(d__1));
	xnorm = max(d__2,d__3);
	t = g[i__];
	d__[i__] = -t;
/* Computing MAX */
	d__1 = *gnorm, d__2 = f2c_abs(t);
	*gnorm = max(d__1,d__2);
	gnorm2 += t * t;
    }
    i__1 = cgparms_1.n;
//	ShowFloat(cgparms_1.n6, "cgparms_1.n6");
//	ShowFloat(i__1, "i__1");
//	ShowFloat(cgparms_1.n6, "cgparms_1.n6");
    for (i__ = cgparms_1.n6; i__ <= i__1; i__ += 5) {
/* Computing MAX */
	d__2 = xnorm, d__3 = (d__1 = x[i__], f2c_abs(d__1));
	xnorm = max(d__2,d__3);
	t = g[i__];
/* Computing MAX */
	d__1 = *gnorm, d__2 = f2c_abs(t);
	*gnorm = max(d__1,d__2);
	d__[i__] = -t;
	j = i__ + 1;
	t1 = g[j];
	d__[j] = -t1;
/* Computing MAX */
	d__1 = *gnorm, d__2 = f2c_abs(t1);
	*gnorm = max(d__1,d__2);
/* Computing MAX */
	d__2 = xnorm, d__3 = (d__1 = x[j], f2c_abs(d__1));
	xnorm = max(d__2,d__3);
	j = i__ + 2;
	t2 = g[j];
	d__[j] = -t2;
/* Computing MAX */
	d__1 = *gnorm, d__2 = f2c_abs(t2);
	*gnorm = max(d__1,d__2);
/* Computing MAX */
	d__2 = xnorm, d__3 = (d__1 = x[j], f2c_abs(d__1));
	xnorm = max(d__2,d__3);
	j = i__ + 3;
//	ShowFloat(j, "j");
	t3 = g[j];
	d__[j] = -t3;
/* Computing MAX */
	d__1 = *gnorm, d__2 = f2c_abs(t3);
	*gnorm = max(d__1,d__2);
/* Computing MAX */
	d__2 = xnorm, d__3 = (d__1 = x[j], f2c_abs(d__1));
	xnorm = max(d__2,d__3);
	j = i__ + 4;
	t4 = g[j];
	d__[j] = -t4;
/* Computing MAX */
	d__1 = *gnorm, d__2 = f2c_abs(t4);
	*gnorm = max(d__1,d__2);
/* Computing MAX */
	d__2 = xnorm, d__3 = (d__1 = x[j], f2c_abs(d__1));
	xnorm = max(d__2,d__3);
	gnorm2 = gnorm2 + t * t + t1 * t1 + t2 * t2 + t3 * t3 + t4 * t4;
    }
    if (cgparms_1.stoprule) {
/* Computing MAX */
	d__1 = *gnorm * cgparms_1.stopfac;
	cgparms_1.tol = max(d__1,cgparms_1.tol);
    }
/*       if ( PrintLevel ) then */
/*          write (*, 10) iter, f, gnorm, AWolfe */
/* 10        format ('iter: ', i5, ' f= ', e14.6, */
/*     &            ' gnorm= ', e14.6, ' AWolfe= ', l2) */
/*      endif */
    if (cg_tol__(f, gnorm)) {
	goto L100;
    }
    dphi0 = -gnorm2;
    if (! cgparms_1.step) {
	alpha = cgparms_1.psi0 * xnorm / *gnorm;
	if (xnorm == cgparms_1.zero) {
	    if (*f != cgparms_1.zero) {
		alpha = cgparms_1.psi0 * f2c_abs(*f) / gnorm2;
	    } else {
		alpha = 1.;
	    }
	}
    }
/* start the conjugate gradient iteration */

/*   alpha starts as old step, ends as initial step for next iteration */
/*   f is function value for alpha = 0 */
/*   QuadOK = .true. means that a quadratic step was taken */

    i__1 = cgparms_1.maxit;
//	ShowFloat(i__1, "i__1");
//	CWaitCursor dummy;

    for (*iter = 1; *iter <= i__1; ++(*iter)) {
	cgparms_1.quadok = FALSE_;
	alpha = cgparms_1.psi2 * alpha;
	if (cgparms_1.quadstep) {
	    if (*f != cgparms_1.zero) {
		t = (d__1 = (*f - cgparms_1.f0) / *f, f2c_abs(d__1));
	    } else {
		t = 1.;
	    }
	    if (t > cgparms_1.quadcutoff) {
		talpha = cgparms_1.psi1 * alpha;
		cg_step__(&xtemp[1], &x[1], &d__[1], &talpha);

		//(*cg_value__)(&ftemp, &xtemp[1], &cgparms_1.n);
		CalcValue(&ftemp, &xtemp[1]);

		++cgparms_1.nf;
		if (ftemp < *f) {
		    denom = ((ftemp - *f) / talpha - dphi0) * 2.;
		    if (denom > cgparms_1.zero) {
			cgparms_1.quadok = TRUE_;
			alpha = -dphi0 * talpha / denom;
		    }
		}
	    }
	}
	cgparms_1.f0 = *f;
/*          if ( PrintLevel ) then */
/*              write (*, 20) QuadOK, alpha, f0, dphi0 */
/* 20            format ('QuadOK:', l2, ' initial a:', */
/*     &                 e14.6,' f0:', e14.6, ' dphi', e14.6) */
/*          endif */
/* parameters in Wolfe and approximiate Wolfe conditions, and in update */
	qk = cgparms_1.qdecay * qk + 1.f;
	cgparms_1.ck += (f2c_abs(*f) - cgparms_1.ck) / qk;
	if (cgparms_1.pertrule) {
	    cgparms_1.fpert = *f + cgparms_1.eps * cgparms_1.ck;
	} else {
	    cgparms_1.fpert = *f + cgparms_1.eps;
	}
	cgparms_1.wolfe_hi__ = cgparms_1.delta * dphi0;
	cgparms_1.wolfe_lo__ = cgparms_1.sigma * dphi0;
	cgparms_1.awolfe_hi__ = delta2 * dphi0;
	if (cgparms_1.awolfe) {
	    cg_line__(&alpha, f, &dphi, &dphi0, &x[1], &xtemp[1], &d__[1], &
		    gtemp[1], (S_fp)cg_value__, (S_fp)cg_grad__);
	} else {
	    cg_linew__(&alpha, f, &dphi, &dphi0, &x[1], &xtemp[1], &d__[1], &
		    gtemp[1], (S_fp)cg_value__, (S_fp)cg_grad__);
	}
	if (cgparms_1.info > 0) {
	    goto L100;
	}

/* Test for convergence to within machine epsilon */
/* (set feps to zero to remove this test) */

	if (-alpha * dphi0 <= cgparms_1.feps * f2c_abs(*f)) {
	    cgparms_1.info = 1;
	    goto L100;
	}
/* compute beta, yk2, gnorm, gnorm2, dnorm2, update x and g, */
	if (*iter % cgparms_1.nrestart != 0) {
	    *gnorm = cgparms_1.zero;
	    dnorm2 = cgparms_1.zero;
	    yk2 = cgparms_1.zero;
	    ykgk = cgparms_1.zero;
	    i__2 = cgparms_1.n5;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		x[i__] = xtemp[i__];
		t = gtemp[i__];
		yk = t - g[i__];
/* Computing 2nd power */
		d__1 = yk;
		yk2 += d__1 * d__1;
		ykgk += yk * t;
		g[i__] = t;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t);
		*gnorm = max(d__1,d__2);
/* Computing 2nd power */
		d__1 = d__[i__];
		dnorm2 += d__1 * d__1;
	    }
	    i__2 = cgparms_1.n;
	    for (i__ = cgparms_1.n6; i__ <= i__2; i__ += 5) {
		x[i__] = xtemp[i__];
		t = gtemp[i__];
		yk = t - g[i__];
/* Computing 2nd power */
		d__1 = yk;
		yk2 += d__1 * d__1;
		ykgk += yk * t;
		i1 = i__ + 1;
		x[i1] = xtemp[i1];
		t1 = gtemp[i1];
		i2 = i__ + 2;
		x[i2] = xtemp[i2];
		t2 = gtemp[i2];
		i3 = i__ + 3;
		x[i3] = xtemp[i3];
		t3 = gtemp[i3];
		i4 = i__ + 4;
		x[i4] = xtemp[i4];
		t4 = gtemp[i4];
/* Computing 2nd power */
		d__1 = t1 - g[i1];
/* Computing 2nd power */
		d__2 = t2 - g[i2];
/* Computing 2nd power */
		d__3 = t3 - g[i3];
/* Computing 2nd power */
		d__4 = t4 - g[i4];
		yk2 = yk2 + d__1 * d__1 + d__2 * d__2 + d__3 * d__3 + d__4 * 
			d__4;
		ykgk = ykgk + (t1 - g[i1]) * t1 + (t2 - g[i2]) * t2 + (t3 - g[
			i3]) * t3 + (t4 - g[i4]) * t4;
		g[i__] = t;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t);
		*gnorm = max(d__1,d__2);
		g[i1] = t1;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t1);
		*gnorm = max(d__1,d__2);
		g[i2] = t2;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t2);
		*gnorm = max(d__1,d__2);
		g[i3] = t3;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t3);
		*gnorm = max(d__1,d__2);
		g[i4] = t4;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t4);
		*gnorm = max(d__1,d__2);
/* Computing 2nd power */
		d__1 = d__[i__];
/* Computing 2nd power */
		d__2 = d__[i1];
/* Computing 2nd power */
		d__3 = d__[i2];
/* Computing 2nd power */
		d__4 = d__[i3];
/* Computing 2nd power */
		d__5 = d__[i4];
		dnorm2 = dnorm2 + d__1 * d__1 + d__2 * d__2 + d__3 * d__3 + 
			d__4 * d__4 + d__5 * d__5;
	    }
	    if (cg_tol__(f, gnorm)) {
		goto L100;
	    }
	    dkyk = dphi - dphi0;
	    beta = (ykgk - dphi * 2. * yk2 / dkyk) / dkyk;
/*   faster: initialize dnorm2 = gnorm2 at start, then */
/*             dnorm2 = gnorm2 + beta**2*dnorm2 - 2.d0*beta*dphi */
/*             gnorm2 = ||g_{k+1}||^2 */
/*             dnorm2 = ||d_{k+1}||^2 */
/*             dpi = g_{k+1}' d_k */
/* Computing MAX */
	    d__1 = beta, d__2 = -1. / sqrt(min(eta_sq__,gnorm2) * dnorm2);
	    beta = max(d__1,d__2);
/*     update search direction d = -g + beta*dold */
	    gnorm2 = cgparms_1.zero;
	    i__2 = cgparms_1.n5;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		t = g[i__];
		d__[i__] = -t + beta * d__[i__];
		gnorm2 += t * t;
	    }
	    i__2 = cgparms_1.n;
	    for (i__ = cgparms_1.n6; i__ <= i__2; i__ += 5) {
		d__[i__] = -g[i__] + beta * d__[i__];
		i1 = i__ + 1;
		d__[i1] = -g[i1] + beta * d__[i1];
		i2 = i__ + 2;
		d__[i2] = -g[i2] + beta * d__[i2];
		i3 = i__ + 3;
		d__[i3] = -g[i3] + beta * d__[i3];
		i4 = i__ + 4;
		d__[i4] = -g[i4] + beta * d__[i4];
/* Computing 2nd power */
		d__1 = g[i__];
/* Computing 2nd power */
		d__2 = g[i1];
/* Computing 2nd power */
		d__3 = g[i2];
/* Computing 2nd power */
		d__4 = g[i3];
/* Computing 2nd power */
		d__5 = g[i4];
		gnorm2 = gnorm2 + d__1 * d__1 + d__2 * d__2 + d__3 * d__3 + 
			d__4 * d__4 + d__5 * d__5;
	    }
	    dphi0 = -gnorm2 + beta * dphi;
	} else {
/*     search direction d = -g */
/*              if ( PrintLevel ) then */
/*                  write (*, *) "RESTART CG" */
/*              endif */
	    *gnorm = cgparms_1.zero;
	    gnorm2 = cgparms_1.zero;
	    i__2 = cgparms_1.n5;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		x[i__] = xtemp[i__];
		t = gtemp[i__];
		g[i__] = t;
		d__[i__] = -t;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t);
		*gnorm = max(d__1,d__2);
		gnorm2 += t * t;
	    }
	    i__2 = cgparms_1.n;
	    for (i__ = cgparms_1.n6; i__ <= i__2; i__ += 5) {
		x[i__] = xtemp[i__];
		t = gtemp[i__];
		g[i__] = t;
		d__[i__] = -t;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t);
		*gnorm = max(d__1,d__2);
		j = i__ + 1;
		x[j] = xtemp[j];
		t1 = gtemp[j];
		g[j] = t1;
		d__[j] = -t1;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t1);
		*gnorm = max(d__1,d__2);
		j = i__ + 2;
		x[j] = xtemp[j];
		t2 = gtemp[j];
		g[j] = t2;
		d__[j] = -t2;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t2);
		*gnorm = max(d__1,d__2);
		j = i__ + 3;
		x[j] = xtemp[j];
		t3 = gtemp[j];
		g[j] = t3;
		d__[j] = -t3;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t3);
		*gnorm = max(d__1,d__2);
		j = i__ + 4;
		x[j] = xtemp[j];
		t4 = gtemp[j];
		g[j] = t4;
		d__[j] = -t4;
/* Computing MAX */
		d__1 = *gnorm, d__2 = f2c_abs(t4);
		*gnorm = max(d__1,d__2);
		gnorm2 = gnorm2 + t * t + t1 * t1 + t2 * t2 + t3 * t3 + t4 * 
			t4;
	    }
	    if (cg_tol__(f, gnorm)) {
		goto L100;
	    }
	    dphi0 = -gnorm2;
	}
	if (! cgparms_1.awolfe) {
	    if ((d__1 = *f - cgparms_1.f0, f2c_abs(d__1)) < cgparms_1.awolfefac * 
		    cgparms_1.ck) {
		cgparms_1.awolfe = TRUE_;
	    }
	}
/*          if ( PrintLevel ) then */
/*              write (*, 10) iter, f, gnorm, AWolfe */
/*          endif */
	if (cgparms_1.debug) {
	    if (*f > cgparms_1.f0 + cgparms_1.ck * 1e-10f) {
/*                  write (*, 270) */
/*                  write (*, 50) f, f0 */
/* 50                format (' new value:', e30.16, 'old value:', e30.16) */
//		s_stop("", (ftnlen)0);
	    }
	}
	if (dphi0 > cgparms_1.zero) {
	    cgparms_1.info = 5;
	    goto L100;
	}
    }
    cgparms_1.info = 2;
L100:
    *nfunc = cgparms_1.nf;
    *ngrad = cgparms_1.ng;
    *status = cgparms_1.info;
    if (cgparms_1.info > 2) {
	*gnorm = cgparms_1.zero;
	i__1 = cgparms_1.n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = xtemp[i__];
	    g[i__] = gtemp[i__];
/* Computing MAX */
	    d__2 = *gnorm, d__3 = (d__1 = g[i__], f2c_abs(d__1));
	    *gnorm = max(d__2,d__3);
	}
    }
/*       if ( PrintFinal ) then */
/*          write (6, *) 'Termination status:', status */
/*          if ( status .eq. 0 ) then */
/*              write (6, 200) */
/*          else if ( status .eq. 1 ) then */
/*              write (6, 210) */
/*          else if ( status .eq. 2 ) then */
/*              write (6, 220) maxit */
/*              write (6, 300) */
/*              write (6, 400) grad_tol */
/*          else if ( status .eq. 3 ) then */
/*              write (6, 230) */
/*              write (6, 300) */
/*              write (6, 430) */
/*              write (6, 410) */
/*          else if ( status .eq. 4 ) then */
/*              write (6, 240) */
/*              write (6, 300) */
/*              write (6, 400) grad_tol */
/*          else if ( status .eq. 5 ) then */
/*              write (6, 250) */
/*          else if ( status .eq. 6 ) then */
/*              write (6, 260) */
/*              write (6, 300) */
/*              write (6, 400) grad_tol */
/*              write (6, 410) */
/*              write (6, 420) */
/*          else if ( status .eq. 7 ) then */
/*              write (6, 260) */
/*              write (6, 300) */
/*              write (6, 400) grad_tol */
/*          else if ( status .eq. 8 ) then */
/*              write (6, 260) */
/*              write (6, 300) */
/*              write (6, 400) grad_tol */
/*              write (6, 410) */
/*              write (6, 420) */
/*          endif */
/*         write (6, 500) gnorm */
/*          write (6, *) 'function value:', f */
/*          write (6, *) 'cg iterations:', iter */
/*          write (6, *) 'function evaluations:', nfunc */
/*          write (6, *) 'gradient evaluations:', ngrad */
/*      endif */
    return 0;
/* 200   format (' Convergence tolerance for gradient satisfied') */
/* 210   format (' Terminating since change in function value <= feps*|f|') */
/* 220   format (' Total number of iterations exceed max allow:', i10) */
/* 230   format (' Slope always negative in line search') */
/* 240   format (' Line search fails, too many secant steps') */
/* 250   format (' Search direction not a descent direction') */
/* 260   format (' Line search fails') */
/* 270   format (' Debugger is on, function value does not improve') */
/* 300   format (' Possible causes of this error message:') */
/* 400   format ('   - your tolerance (grad_tol = ', d10.4, */
/*     &        ') may be too strict') */
/* 410   format ('   - your gradient routine has an error') */
/* 420   format ('   - parameter epsilon in cg_descent.parm is too small') */
/* 430   format ('   - your cost function has an error') */
/* 500   format (' absolute largest component of gradient: ', d10.4) */
} /* cg_descent__ */

/*     PARAMETERS: */

/*     delta - range (0, .5), used in the Wolfe conditions */
/*     sigma - range [delta, 1), used in the Wolfe conditions */
/*     eps - range [0, infty), used to compute line search perturbation */
/*     gamma - range (0,1), determines when to perform bisection step */
/*     rho   - range (1, infty), growth factor when finding initial interval */
/*     eta   - range (0, infty), used in lower bound for beta */
/*     psi0  - range (0, 1), factor used in very initial starting guess */
/*     psi1  - range (0, 1), factor previous step multiplied by in QuadStep */
/*     psi2  - range (1, infty), factor previous step is multipled by for startup */
/*     QuadCutOff - perform QuadStep if relative change in f > QuadCutOff */
/*     StopFac - used in StopRule */
/*     AWolfeFac - used to decide when to switch from Wolfe to AWolfe */
/*     restart_fac - range (0, infty) restart cg when iter = n*restart */
/*     maxit_fac - range (0, infty) terminate in maxit = maxit_fac*n iterations */
/*     feps - stop when -alpha*dphi0 (est. change in value) <= feps*|f| */
/*            (feps = 0 removes this test, example: feps = eps*1.e-5 */
/*             where eps is machine epsilon) */
/*     tol   - range (0, infty), convergence tolerance */
/*     nexpand - range [0, infty), number of grow/shrink allowed in bracket */
/*     nsecant - range [0, infty), maximum number of secant steps */
/*     PertRule - gives the rule used for the perturbation in f */
/*                 F => fpert = eps */
/*                 T => fpert = eps*Ck, Ck is an average of prior |f| */
/*                             Ck is an average of prior |f| */
/*     QuadStep- .true. (use quadratic step) .false. (no quadratic step) */
/*     PrintLevel- .false. (no printout) .true. (print intermediate results) */
/*     PrintFinal- .false. (no printout) .true. (print messages, final error) */
/*     StopRule - .true. (max abs grad <= max (tol, StopFac*initial abs grad)) */
/*                .false. (... <= tol*(1+|f|)) */
/*     AWolfe - .false. (use standard Wolfe initially) */
/*            - .true. (use approximate + standard Wolfe) */
/*     Step - .false. (program computing starting step at iteration 0) */
/*          - .true. (user provides starting step in gnorm argument of cg_descent */
/*     debug - .false. (no debugging) */
/*           - .true. (check that function values do not increase) */
/*     info  - same as status */

/*     DEFAULT PARAMETER VALUES: */

/*         delta : 0.1 */
/*         sigma : 0.9 */
/*         eps : 1.e-6 */
/*         gamma : 0.66 */
/*         rho   : 5.0 */
/*         restart: 1.0 */
/*         eta   : 0.01 */
/*         psi0  : 0.01 */
/*         psi1  : 0.1 */
/*         psi2  : 2.0 */
/*         QuadCutOff: 1.d-12 */
/*         StopFac: 0.d0 */
/*         AWolfeFac: 1.d-3 */
/*         tol   : grad_tol */
/*         nrestart: n (restart_fac = 1) */
/*         maxit : 500*n (maxit_fac = 500) */
/*         feps : 0.0 */
/*         Qdecay : 0.7 */
/*         nexpand: 50 */
/*         nsecant: 50 */
/*         PertRule: .true. */
/*         QuadStep: .true. */
/*         PrintLevel: .false. */
/*         PrintFinal: .true. */
/*         StopRule: .true. */
/*         AWolfe: .true. */
/*         Step: .false. */
/*         debug: .false. */
/*         info  : 0 */
/*         feps  : 0.0 */

/*      (double) grad_tol-- used in stopping rule */
/*      (int)    dim     --problem dimension (also denoted n) */
/* Subroutine */ 
int cg_descent::cg_init__(doublereal *grad_tol__, int *dim)
{
    static doublereal maxit_fac__, restart_fac__;
    static float theta;

    cgparms_1.n = *dim;
    cgparms_1.tol = *grad_tol__;
/* .1d0      delta        (Wolfe line search parameter) */
/* .9d0      sigma        (Wolfe line search parameter) */
/* 1.d-6     eps          (perturbation parameter for computing fpert) */
/* .5d0      theta        (parameter used in update routine) */
/* .66d0     gamma        (required decay factor in interval) */
/* 5.0d0     rho          (interval growth factor used to get bracketting interval) */
/* .01d0     eta          (lower bound for cg's beta_k) */
/* .01d0     psi0         (factor used in starting guess for iteration 1) */
/* .1d0      psi1         (factor previous step multiplied by in QuadStep) */
/* 2.d0      psi2         (factor previous step is multipled by for startup) */
/* 1.d-12    QuadCutOff   (QuadStep if relative change in f > QuadCutOff) */
/* 0.d-12    StopFact     (factor multiplying starting |grad|_infty in StopRule) */
/* 1.d-3     AWolfeFac    (AWolfe = F => set AWolfe = T if |f-f0| < Awolfe_fac*Ck) */
/* 1.0d0     restart_fac  (restart cg in restart_fac*n iterations) */
/* 500.d0    maxit_fac    (terminate in maxit_fac*n iterations) */
/* 0.d0      feps         (stop when value change <= feps*|f|) */
/* .7d0      Qdecay       (used in Qk update: Qk = Qdecay*Qk + 1) */
/* 50        nexpand      (number of grow/shrink allowed in bracket) */
/* 50        nsecant      (number of secant steps allowed in line search) */
/* .true.    PertRule     (F => eps, T => eps*Ck) */
/* .true.    QuadStep     (use initial quad interpolation in line search) */
/* .false.   PrintLevel   F (no print) T (intermediate results) */
/* .true.    PrintFinal   F (no print) T (print error messages, final error) */
/* .true.    StopRule     T (|grad|_infty <= max(tol,|grad|_0*StopFact) F (... <= tol*(1+|f|)) */
/* .true.    AWolfe       F (Wolfe) T (approx Wolfe) */
/* .false.   Step         F (no initial line search guess) T (guess in gnorm) */
/* .false.   debug        F (no debugging) T (check for no increase in f) */
/* !     open (10, file='cg_parm') */
/**************************
    cgparms_1.delta = .1;
    cgparms_1.sigma = .9;
    cgparms_1.eps = 1e-6f;
    cgparms_1.gamma = .66;
    cgparms_1.rho = 5.;
    cgparms_1.eta = .01;
    cgparms_1.psi0 = .01;
    cgparms_1.psi1 = .1;
    cgparms_1.psi2 = 2.;
    cgparms_1.quadcutoff = 1e-12;
    cgparms_1.stopfac = 0.;
    cgparms_1.awolfefac = .001;
    cgparms_1.feps = 0.;
    cgparms_1.qdecay = .7;
    cgparms_1.nexpand = 50;
    cgparms_1.nsecant = 50;
    cgparms_1.pertrule = TRUE_;
    cgparms_1.quadstep = TRUE_;
    cgparms_1.printlevel = TRUE_;
    cgparms_1.printfinal = TRUE_;
    cgparms_1.stoprule = TRUE_;
    cgparms_1.awolfe = TRUE_;
    cgparms_1.step = FALSE_;
    cgparms_1.debug = FALSE_;


	*///////////////////////
/*      close (10) */
    restart_fac__ = 1.;
    cgparms_1.zero = 0.;
    cgparms_1.info = 0;
    cgparms_1.n5 = cgparms_1.n % 5;
    cgparms_1.n6 = cgparms_1.n5 + 1;
    cgparms_1.nf = 0;
    cgparms_1.ng = 0;
    cgparms_1.nrestart = (integer) (cgparms_1.n * restart_fac__);
    cgparms_1.maxit = (integer) (cgparms_1.n * maxit_fac__);
    theta = .5;
    restart_fac__ = 1.;
    maxit_fac__ = 500.;
    cgparms_1.maxit = (integer) (cgparms_1.n * maxit_fac__);

    return 0;
} /* cg_init__ */

/* check whether the Wolfe or the approximate Wolfe conditions */
/*     are satisfied */
/*      (double) alpha   -- stepsize */
/*      (double) f       -- function value associated with stepsize alpha */
/*      (double) dphi    -- derivative value associated with stepsize alpha */
logical cg_descent::cg_wolfe__(doublereal *alpha, doublereal *f, doublereal *dphi)
{
    /* System generated locals */
    logical ret_val;

    if (*dphi >= cgparms_1.wolfe_lo__) {
/* test original Wolfe conditions */
	if (*f - cgparms_1.f0 <= *alpha * cgparms_1.wolfe_hi__) {
	    ret_val = TRUE_;
/*              if ( PrintLevel ) then */
/*                  write (*, 10) f, f0, alpha*wolfe_hi, dphi */
/* 10                format (' wolfe f:', e14.6, ' f0: ', */
/*     &                    e14.6, e14.6, ' dphi:', e14.6) */
/*              endif */
	    return ret_val;
/* test approximate Wolfe conditions */
	} else if (cgparms_1.awolfe) {
	    if (*f <= cgparms_1.fpert && *dphi <= cgparms_1.awolfe_hi__) {
		ret_val = TRUE_;
/*                  if ( PrintLevel ) then */
/*                      write (*, 20) f, fpert, dphi, awolfe_hi */
/* 20                        format ('f:', e14.6, ' fpert:', e14.6, */
/*     &                            ' dphi: ', e14.6, ' fappx:', e14.6) */
/*                  endif */
		return ret_val;
	    }
	}
    }
    ret_val = FALSE_;
    return ret_val;
} /* cg_wolfe__ */

/* check for convergence of the cg iterations */
/*      (double) f       -- function value associated with stepsize */
/*      (double) gnorm   -- gradient (infinity) norm */
logical cg_descent::cg_tol__(doublereal *f, doublereal *gnorm)
{
    /* System generated locals */
    logical ret_val;

    if (cgparms_1.stoprule) {
	if (*gnorm <= cgparms_1.tol) {
	    ret_val = TRUE_;
	    return ret_val;
	}
    } else {
	if (*gnorm <= cgparms_1.tol * (f2c_abs(*f) + 1.f)) {
	    ret_val = TRUE_;
	    return ret_val;
	}
    }
    ret_val = FALSE_;
    return ret_val;
} /* cg_tol__ */

/* compute dot product of x and y, vectors of length n */
/*      (double) x       -- first vector */
/*      (double) y       -- second vector */
doublereal cg_descent::cg_dot__(doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Local variables */
    static integer i__;
    static doublereal t;

    /* Parameter adjustments */
    --y;
    --x;

    /* Function Body */
    t = cgparms_1.zero;
    i__1 = cgparms_1.n5;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t += x[i__] * y[i__];
    }
    i__1 = cgparms_1.n;
    for (i__ = cgparms_1.n6; i__ <= i__1; i__ += 5) {
	t = t + x[i__] * y[i__] + x[i__ + 1] * y[i__ + 1] + x[i__ + 2] * y[
		i__ + 2] + x[i__ + 3] * y[i__ + 3] + x[i__ + 4] * y[i__ + 4];
    }
    ret_val = t;
    return ret_val;
} /* cg_dot__ */


/*  compute xtemp = x + alpha d */

/*      (double) xtemp   -- output vector */
/*      (double) x       -- initial vector */
/*      (double) d       -- search direction vector */
/*      (double) alpha   -- stepsize along search direction vector */
/* Subroutine */ 

int cg_descent::cg_step__(doublereal *xtemp, doublereal *x, doublereal *
	d__, doublereal *alpha)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    --d__;
    --x;
    --xtemp;

    /* Function Body */
    i__1 = cgparms_1.n5;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xtemp[i__] = x[i__] + *alpha * d__[i__];
    }
    i__1 = cgparms_1.n;
    for (i__ = cgparms_1.n6; i__ <= i__1; i__ += 5) {
	xtemp[i__] = x[i__] + *alpha * d__[i__];
	j = i__ + 1;
	xtemp[j] = x[j] + *alpha * d__[j];
	j = i__ + 2;
	xtemp[j] = x[j] + *alpha * d__[j];
	j = i__ + 3;
	xtemp[j] = x[j] + *alpha * d__[j];
	j = i__ + 4;
	xtemp[j] = x[j] + *alpha * d__[j];
    }
    return 0;
} /* cg_step__ */

/*      (double) alpha   -- stepsize along search direction vector */
/*      (double) phi     -- function value for step alpha */
/*      (double) dphi    -- function derivative for step alpha */
/*      (double) dphi0   -- function derivative at starting point (alpha = 0) */
/*      (double) x       -- current iterate */
/*      (double) xtemp   -- x + alpha*d */
/*      (double) d       -- current search direction */
/*      (double) gtemp   -- gradient at x + alpha*d */
/*      (external) cg_value -- routine to evaluate function value */
/*      (external) cg_grad  -- routine to evaluate function gradient */
/* Subroutine */ 
int cg_descent::cg_line__(doublereal *alpha, doublereal *phi, doublereal 
	*dphi, doublereal *dphi0, doublereal *x, doublereal *xtemp, 
	doublereal *d__, doublereal *gtemp, S_fp cg_value__, S_fp cg_grad__)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
//    extern logical cg_wolfe__(doublereal *, doublereal *, doublereal *);
    static doublereal a, b, c__;
//    extern integer cg_update__(doublereal *, doublereal *, doublereal *, 
//	    doublereal *, doublereal *, doublereal *, doublereal *, 
//	    doublereal *, doublereal *, doublereal *, doublereal *, S_fp, 
//	    S_fp);
    static doublereal a0, b0, da0, db0;
    static integer flag__, iter;
    static doublereal dphia, dphib, fquad, width;
    static integer ngrow;
//   extern doublereal cg_dot__(doublereal *, doublereal *);
///    extern /* Subroutine */ int cg_step__(doublereal *, doublereal *, 
///	    doublereal *, doublereal *);
    static integer nshrink;

    /* Parameter adjustments */
    --gtemp;
    --d__;
    --xtemp;
    --x;

    /* Function Body */
    cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//    (*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);

    ++cgparms_1.ng;
    *dphi = cg_dot__(&gtemp[1], &d__[1]);

/* Find initial interval [a,b] such that dphia < 0, dphib >= 0, */
/*        and phia <= phi0 + feps*dabs (phi0) */

    a = cgparms_1.zero;
    dphia = *dphi0;
    ngrow = 0;
    nshrink = 0;
    while(*dphi < cgparms_1.zero) {

//	(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);

	++cgparms_1.nf;

/* if quadstep in effect and quadratic conditions hold, check wolfe condition */

	if (cgparms_1.quadok) {
	    if (ngrow == 0) {
		fquad = min(*phi,cgparms_1.f0);
	    }
	    if (*phi <= fquad) {
/*                  if ( PrintLevel ) then */
/*                      write (*, 10) alpha, phi, fquad */
/* 10                    format ('alpha:', e14.6, ' phi:', e14.6, */
/*     &                        ' fquad:', e14.6) */
/*                  endif */
		if (cg_wolfe__(alpha, phi, dphi)) {
		    return 0;
		}
	    }
	}
	if (*phi <= cgparms_1.fpert) {
	    a = *alpha;
	    dphia = *dphi;
	} else {

/* contraction phase */

	    b = *alpha;
	    while(TRUE_) {
		*alpha = (a + b) * .5;
		++nshrink;
		if (nshrink > cgparms_1.nexpand) {
		    cgparms_1.info = 6;
		    return 0;
		}
		cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//		(*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
		CalcGradient(&gtemp[1], &xtemp[1]);

		++cgparms_1.ng;
		*dphi = cg_dot__(&gtemp[1], &d__[1]);
		if (*dphi >= cgparms_1.zero) {
		    goto L100;
		}

//		(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
		CalcValue(phi, &xtemp[1]);

		++cgparms_1.nf;
/*                  if ( PrintLevel ) then */
/*                      write (6, 20) a, b, alpha, phi, dphi */
/* 20                    format ('contract, a:', e14.6, */
/*     &                        ' b:', e14.6, ' alpha:', e14.6, */
/*     &                        ' phi:', e14.6, ' dphi:', e14.6) */
/*                  endif */
		if (cgparms_1.quadok && *phi <= fquad) {
		    if (cg_wolfe__(alpha, phi, dphi)) {
			return 0;
		    }
		}
		if (*phi <= cgparms_1.fpert) {
		    a = *alpha;
		    dphia = *dphi;
		} else {
		    b = *alpha;
		}
	    }
	}

/* expansion phase */

	++ngrow;
	if (ngrow > cgparms_1.nexpand) {
	    cgparms_1.info = 3;
	    return 0;
	}
	*alpha = cgparms_1.rho * *alpha;
	cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//	(*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);


	++cgparms_1.ng;
	*dphi = cg_dot__(&gtemp[1], &d__[1]);
/*          if ( PrintLevel ) then */
/*              write (*, 30) a, alpha, phi, dphi */
/* 30            format ('expand,   a:', e14.6, ' alpha:', e14.6, */
/*     &                 ' phi:', e14.6, ' dphi:', e14.6) */
/*          endif */
    }
L100:
    b = *alpha;
    dphib = *dphi;
    if (cgparms_1.quadok) {

//	(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);

	
	++cgparms_1.nf;
	if (ngrow + nshrink == 0) {
	    fquad = min(*phi,cgparms_1.f0);
	}
	if (*phi <= fquad) {
	    if (cg_wolfe__(alpha, phi, dphi)) {
		return 0;
	    }
	}
    }
    i__1 = cgparms_1.nsecant;
    for (iter = 1; iter <= i__1; ++iter) {
/*          if ( PrintLevel ) then */
/*              write (*, 40) a, b, dphia, dphib */
/* 40            format ('secant, a:', e14.6, ' b:', e14.6, */
/*     &                 ' da:', e14.6, ' db:', e14.6) */
/*          endif */
	width = cgparms_1.gamma * (b - a);
	if (-dphia <= dphib) {
	    *alpha = a - (a - b) * (dphia / (dphia - dphib));
	} else {
	    *alpha = b - (a - b) * (dphib / (dphia - dphib));
	}
	c__ = *alpha;
	a0 = a;
	b0 = b;
	da0 = dphia;
	db0 = dphib;
	flag__ = cg_update__(&a, &dphia, &b, &dphib, alpha, phi, dphi, &x[1], 
		&xtemp[1], &d__[1], &gtemp[1], (S_fp)cg_value__, (S_fp)
		cg_grad__);
	if (flag__ > 0) {
	    return 0;
	} else if (flag__ == 0) {
	    if (c__ == a) {
		if (*dphi > da0) {
		    *alpha = c__ - (c__ - a0) * (*dphi / (*dphi - da0));
		} else {
		    *alpha = a;
		}
	    } else {
		if (*dphi < db0) {
		    *alpha = c__ - (c__ - b0) * (*dphi / (*dphi - db0));
		} else {
		    *alpha = b;
		}
	    }
	    if (*alpha > a && *alpha < b) {
/*                  if ( PrintLevel ) write (*, *) "2nd secant" */
		flag__ = cg_update__(&a, &dphia, &b, &dphib, alpha, phi, dphi,
			 &x[1], &xtemp[1], &d__[1], &gtemp[1], (S_fp)
			cg_value__, (S_fp)cg_grad__);
		if (flag__ > 0) {
		    return 0;
		}
	    }
	}

/*    bisection iteration */

	if (b - a >= width) {
	    *alpha = (b + a) * .5;
/*              if ( PrintLevel ) write (*, *) "bisection" */
	    flag__ = cg_update__(&a, &dphia, &b, &dphib, alpha, phi, dphi, &x[
		    1], &xtemp[1], &d__[1], &gtemp[1], (S_fp)cg_value__, (
		    S_fp)cg_grad__);
	    if (flag__ > 0) {
		return 0;
	    }
	} else {
	    if (b <= a) {
		cgparms_1.info = 7;
		return 0;
	    }
	}
    }
    cgparms_1.info = 4;
    return 0;
} /* cg_line__ */

/*  This routine is identical to cg_line except that the function */
/*  psi (a) = phi (a) - phi (0) - a*delta*dphi (0) is miniminized instead of */
/*  the function phi */
/*      (double) alpha   -- stepsize along search direction vector */
/*      (double) phi     -- function value for step alpha */
/*      (double) dphi    -- function derivative for step alpha */
/*      (double) dphi0   -- function derivative at starting point (alpha = 0) */
/*      (double) x       -- current iterate */
/*      (double) xtemp   -- x + alpha*d */
/*      (double) d       -- current search direction */
/*      (double) gtemp   -- gradient at x + alpha*d */
/*      (external) cg_value -- routine to evaluate function value */
/*      (external) cg_grad  -- routine to evaluate function gradient */
/* Subroutine */ 
int cg_descent::cg_linew__(doublereal *alpha, doublereal *phi, 
	doublereal *dphi, doublereal *dphi0, doublereal *x, doublereal *xtemp,
	 doublereal *d__, doublereal *gtemp, S_fp cg_value__, S_fp cg_grad__)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
//    extern logical cg_wolfe__(doublereal *, doublereal *, doublereal *);
    static doublereal a, b, c__, a0, b0, da0, db0;
//    extern integer cg_updatew__(doublereal *, doublereal *, doublereal *, 
//	    doublereal *, doublereal *, doublereal *, doublereal *, 
//	    doublereal *, doublereal *, doublereal *, doublereal *, 
//	    doublereal *, S_fp, S_fp);
    static doublereal psi;
    static integer flag__;
    static doublereal dpsi;
    static integer iter;
    static doublereal dpsia, dpsib, fquad, width;
    static integer ngrow;
//    extern doublereal cg_dot__(doublereal *, doublereal *);
///    extern /* Subroutine */ int cg_step__(doublereal *, doublereal *, 
///	    doublereal *, doublereal *);
    static integer nshrink;

    /* Parameter adjustments */
    --gtemp;
    --d__;
    --xtemp;
    --x;

    /* Function Body */
    cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//    (*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);


    ++cgparms_1.ng;
    *dphi = cg_dot__(&gtemp[1], &d__[1]);
    dpsi = *dphi - cgparms_1.wolfe_hi__;

/* Find initial interval [a,b] such that dpsia < 0, dpsib >= 0, */
/*        and psia <= phi0 + feps*dabs (phi0) */

    a = cgparms_1.zero;
    dpsia = *dphi0 - cgparms_1.wolfe_hi__;
    ngrow = 0;
    nshrink = 0;
    while(dpsi < cgparms_1.zero) {

//	(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);

	
	psi = *phi - *alpha * cgparms_1.wolfe_hi__;
	++cgparms_1.nf;

/* if quadstep in effect and quadratic conditions hold, check wolfe condition */

	if (cgparms_1.quadok) {
	    if (ngrow == 0) {
		fquad = min(*phi,cgparms_1.f0);
	    }
	    if (*phi <= fquad) {
/*                  if ( PrintLevel ) then */
/*                      write (*, 10) alpha, phi, fquad */
/* 10                    format ('alpha:', e14.6, ' phi:', e14.6, */
/*     &                        ' fquad:', e14.6) */
/*                  endif */
		if (cg_wolfe__(alpha, phi, dphi)) {
		    return 0;
		}
	    }
	}
	if (psi <= cgparms_1.fpert) {
	    a = *alpha;
	    dpsia = dpsi;
	} else {

/* contraction phase */

	    b = *alpha;
	    while(TRUE_) {
		*alpha = (a + b) * .5;
		++nshrink;
		if (nshrink > cgparms_1.nexpand) {
		    cgparms_1.info = 6;
		    return 0;
		}
		cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//		(*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
		CalcGradient(&gtemp[1], &xtemp[1]);

		++cgparms_1.ng;
		*dphi = cg_dot__(&gtemp[1], &d__[1]);
		dpsi = *dphi - cgparms_1.wolfe_hi__;
		if (dpsi >= cgparms_1.zero) {
		    goto L100;
		}

//		(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
		CalcValue(phi, &xtemp[1]);

		psi = *phi - *alpha * cgparms_1.wolfe_hi__;
		++cgparms_1.nf;
/*                  if ( PrintLevel ) then */
/*                      write (6, 20) a, b, alpha, phi, dphi */
/* 20                    format ('contract, a:', e14.6, */
/*     &                        ' b:', e14.6, ' alpha:', e14.6, */
/*     &                        ' phi:', e14.6, ' dphi:', e14.6) */
/*                  endif */
		if (cgparms_1.quadok && *phi <= fquad) {
		    if (cg_wolfe__(alpha, phi, dphi)) {
			return 0;
		    }
		}
		if (psi <= cgparms_1.fpert) {
		    a = *alpha;
		    dpsia = dpsi;
		} else {
		    b = *alpha;
		}
	    }
	}

/* expansion phase */

	++ngrow;
	if (ngrow > cgparms_1.nexpand) {
	    cgparms_1.info = 3;
	    return 0;
	}
	*alpha = cgparms_1.rho * *alpha;
	cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//	(*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);


	++cgparms_1.ng;
	*dphi = cg_dot__(&gtemp[1], &d__[1]);
	dpsi = *dphi - cgparms_1.wolfe_hi__;
/*          if ( PrintLevel ) then */
/*              write (*, 30) a, alpha, phi, dphi */
/* 30            format ('expand,   a:', e14.6, ' alpha:', e14.6, */
/*     &                 ' phi:', e14.6, ' dphi:', e14.6) */
/*              write (6, *) "expand, alpha:", alpha, "dphi:", dphi */
/*          endif */
    }
L100:
    b = *alpha;
    dpsib = dpsi;
    if (cgparms_1.quadok) {

//	(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);


	++cgparms_1.nf;
	if (ngrow + nshrink == 0) {
	    fquad = min(*phi,cgparms_1.f0);
	}
	if (*phi <= fquad) {
	    if (cg_wolfe__(alpha, phi, dphi)) {
		return 0;
	    }
	}
    }
    i__1 = cgparms_1.nsecant;
    for (iter = 1; iter <= i__1; ++iter) {
/*          if ( PrintLevel ) then */
/*              write (*, 40) a, b, dpsia, dpsib */
/* 40            format ('secant, a:', e14.6, ' b:', e14.6, */
/*     &                 ' da:', e14.6, ' db:', e14.6) */
/*          endif */
	width = cgparms_1.gamma * (b - a);
	if (-dpsia <= dpsib) {
	    *alpha = a - (a - b) * (dpsia / (dpsia - dpsib));
	} else {
	    *alpha = b - (a - b) * (dpsib / (dpsia - dpsib));
	}
	c__ = *alpha;
	a0 = a;
	b0 = b;
	da0 = dpsia;
	db0 = dpsib;
	flag__ = cg_updatew__(&a, &dpsia, &b, &dpsib, alpha, phi, dphi, &dpsi,
		 &x[1], &xtemp[1], &d__[1], &gtemp[1], (S_fp)cg_value__, (
		S_fp)cg_grad__);
	if (flag__ > 0) {
	    return 0;
	} else if (flag__ == 0) {
	    if (c__ == a) {
		if (dpsi > da0) {
		    *alpha = c__ - (c__ - a0) * (dpsi / (dpsi - da0));
		} else {
		    *alpha = a;
		}
	    } else {
		if (dpsi < db0) {
		    *alpha = c__ - (c__ - b0) * (dpsi / (dpsi - db0));
		} else {
		    *alpha = b;
		}
	    }
	    if (*alpha > a && *alpha < b) {
/*                  if ( PrintLevel ) write (*, *) "2nd secant" */
		flag__ = cg_updatew__(&a, &dpsia, &b, &dpsib, alpha, phi, 
			dphi, &dpsi, &x[1], &xtemp[1], &d__[1], &gtemp[1], (
			S_fp)cg_value__, (S_fp)cg_grad__);
		if (flag__ > 0) {
		    return 0;
		}
	    }
	}

/*    bisection iteration */

	if (b - a >= width) {
	    *alpha = (b + a) * .5;
/*              if ( PrintLevel ) write (*, *) "bisection" */
	    flag__ = cg_updatew__(&a, &dpsia, &b, &dpsib, alpha, phi, dphi, &
		    dpsi, &x[1], &xtemp[1], &d__[1], &gtemp[1], (S_fp)
		    cg_value__, (S_fp)cg_grad__);
	    if (flag__ > 0) {
		return 0;
	    }
	} else {
	    if (b <= a) {
		cgparms_1.info = 7;
		return 0;
	    }
	}
    }
    cgparms_1.info = 4;
    return 0;
} /* cg_linew__ */


/* update returns 1 if Wolfe condition is satisfied or too many iterations */
/*        returns  0 if the interval updated successfully */
/*        returns -1 if search done */

/*      (double) a       -- left side of bracketting interval */
/*      (double) dphia   -- derivative at a */
/*      (double) b       -- right side of bracketting interval */
/*      (double) dphib   -- derivative at b */
/*      (double) alpha   -- trial step (between a and b) */
/*      (double) phi     -- function value at alpha (returned) */
/*      (double) dphi    -- function derivative at alpha (returned) */
/*      (double) x       -- current iterate */
/*      (double) xtemp   -- x + alpha*d */
/*      (double) d       -- current search direction */
/*      (double) gtemp   -- gradient at x + alpha*d */
/*      (external) cg_value -- routine to evaluate function value */
/*      (external) cg_grad  -- routine to evaluate function gradient */
integer cg_descent::cg_update__(doublereal *a, doublereal *dphia, doublereal *b, 
	doublereal *dphib, doublereal *alpha, doublereal *phi, doublereal *
	dphi, doublereal *x, doublereal *xtemp, doublereal *d__, doublereal *
	gtemp, S_fp cg_value__, S_fp cg_grad__)
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
//    extern logical cg_wolfe__(doublereal *, doublereal *, doublereal *);
//    extern doublereal cg_dot__(doublereal *, doublereal *);
///    extern /* Subroutine */ int cg_step__(doublereal *, doublereal *, 
///	    doublereal *, doublereal *);
    static integer nshrink;

    /* Parameter adjustments */
    --gtemp;
    --d__;
    --xtemp;
    --x;

    /* Function Body */
    cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//    (*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);

    ++cgparms_1.nf;

//    (*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);


    ++cgparms_1.ng;
    *dphi = cg_dot__(&gtemp[1], &d__[1]);
/*      if ( PrintLevel ) then */
/*          write (*, 10) alpha, phi, dphi */
/* 10        format ('update alpha:', e14.6, ' phi:', e14.6, */
/*     &            ' dphi:', e14.6) */
/*      endif */
    ret_val = 0;
    if (cg_wolfe__(alpha, phi, dphi)) {
	ret_val = 1;
	goto L110;
    }
    if (*dphi >= cgparms_1.zero) {
	*b = *alpha;
	*dphib = *dphi;
	goto L110;
    } else {
	if (*phi <= cgparms_1.fpert) {
	    *a = *alpha;
	    *dphia = *dphi;
	    goto L110;
	}
    }
    nshrink = 0;
    *b = *alpha;
    while(TRUE_) {
	*alpha = (*a + *b) * .5;
	++nshrink;
	if (nshrink > cgparms_1.nexpand) {
	    cgparms_1.info = 8;
	    ret_val = 1;
	    goto L110;
	}
	cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//	(*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);

	++cgparms_1.ng;
	*dphi = cg_dot__(&gtemp[1], &d__[1]);

//	(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);

	
	++cgparms_1.nf;
/*          if ( PrintLevel ) then */
/*              write (6, 20) a, alpha, phi, dphi */
/* 20            format ('contract, a:', e14.6, ' alpha:', e14.6, */
/*     &                 ' phi:', e14.6, ' dphi:', e14.6) */
/*          endif */
	if (cg_wolfe__(alpha, phi, dphi)) {
	    ret_val = 1;
	    goto L110;
	}
	if (*dphi >= cgparms_1.zero) {
	    *b = *alpha;
	    *dphib = *dphi;
	    goto L100;
	}
	if (*phi <= cgparms_1.fpert) {
/*              if ( PrintLevel ) then */
/*                  write (6, *) "update a:", alpha, "dphia:", dphi */
/*              endif */
	    *a = *alpha;
	    *dphia = *dphi;
	} else {
	    *b = *alpha;
	}
    }
L100:
    ret_val = -1;
L110:
/*      if ( PrintLevel ) then */
/*          write (*, 200) a, b, dphia, dphib, cg_update */
/* 200       format ('UP a:', e14.6, ' b:', e14.6, */
/*     &             ' da:', e14.6, ' db:', e14.6, ' up:', i2) */
/*      endif */
    return ret_val;
} /* cg_update__ */

/*  This routine is identical to cg_update except that the function */
/*  psi (a) = phi (a) - phi (0) - a*delta*dphi (0) is miniminized instead of */
/*  the function phi */

/* update returns 1 if Wolfe condition is satisfied or too many iterations */
/*        returns  0 if the interval updated successfully */
/*        returns -1 if search done */

/*      (double) a       -- left side of bracketting interval */
/*      (double) dpsia   -- derivative at a */
/*      (double) b       -- right side of bracketting interval */
/*      (double) dpsib   -- derivative at b */
/*      (double) alpha   -- trial step (between a and b) */
/*      (double) phi     -- function value at alpha (returned) */
/*      (double) dphi    -- derivative of phi at alpha (returned) */
/*      (double) dpsi    -- derivative of psi at alpha (returned) */
/*      (double) x       -- current iterate */
/*      (double) xtemp   -- x + alpha*d */
/*      (double) d       -- current search direction */
/*      (double) gtemp   -- gradient at x + alpha*d */
/*      (external) cg_value -- routine to evaluate function value */
/*      (external) cg_grad  -- routine to evaluate function gradient */
integer cg_descent::cg_updatew__(doublereal *a, doublereal *dpsia, doublereal *b, 
	doublereal *dpsib, doublereal *alpha, doublereal *phi, doublereal *
	dphi, doublereal *dpsi, doublereal *x, doublereal *xtemp, doublereal *
	d__, doublereal *gtemp, S_fp cg_value__, S_fp cg_grad__)
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
//    extern logical cg_wolfe__(doublereal *, doublereal *, doublereal *);
    static doublereal psi;
//    extern doublereal cg_dot__(doublereal *, doublereal *);
///    extern /* Subroutine */ int cg_step__(doublereal *, doublereal *, 
///	    doublereal *, doublereal *);
    static integer nshrink;

    /* Parameter adjustments */
    --gtemp;
    --d__;
    --xtemp;
    --x;

    /* Function Body */
    cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//    (*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);

    psi = *phi - *alpha * cgparms_1.wolfe_hi__;
    ++cgparms_1.nf;

//    (*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);

    ++cgparms_1.ng;
    *dphi = cg_dot__(&gtemp[1], &d__[1]);
    *dpsi = *dphi - cgparms_1.wolfe_hi__;
/*      if ( PrintLevel ) then */
/*          write (*, 10) alpha, psi, dpsi */
/* 10        format ('update alpha:', e14.6, ' psi:', e14.6, */
/*     &            ' dpsi:', e14.6) */
/*      endif */
    ret_val = 0;
    if (cg_wolfe__(alpha, phi, dphi)) {
	ret_val = 1;
	goto L110;
    }
    if (*dpsi >= cgparms_1.zero) {
	*b = *alpha;
	*dpsib = *dpsi;
	goto L110;
    } else {
	if (psi <= cgparms_1.fpert) {
	    *a = *alpha;
	    *dpsia = *dpsi;
	    goto L110;
	}
    }
    nshrink = 0;
    *b = *alpha;
    while(TRUE_) {
	*alpha = (*a + *b) * .5;
	++nshrink;
	if (nshrink > cgparms_1.nexpand) {
	    cgparms_1.info = 8;
	    ret_val = 1;
	    goto L110;
	}
	cg_step__(&xtemp[1], &x[1], &d__[1], alpha);

//	(*cg_grad__)(&gtemp[1], &xtemp[1], &cgparms_1.n);
	CalcGradient(&gtemp[1], &xtemp[1]);

	++cgparms_1.ng;
	*dphi = cg_dot__(&gtemp[1], &d__[1]);
	*dpsi = *dphi - cgparms_1.wolfe_hi__;

//	(*cg_value__)(phi, &xtemp[1], &cgparms_1.n);
	CalcValue(phi, &xtemp[1]);


	psi = *phi - *alpha * cgparms_1.wolfe_hi__;
	++cgparms_1.nf;
/*          if ( PrintLevel ) then */
/*              write (6, 20) a, alpha, phi, dphi */
/* 20            format ('contract, a:', e14.6, ' alpha:', e14.6, */
/*     &                 ' phi:', e14.6, ' dphi:', e14.6) */
/*          endif */
	if (cg_wolfe__(alpha, phi, dphi)) {
	    ret_val = 1;
	    goto L110;
	}
	if (*dpsi >= cgparms_1.zero) {
	    *b = *alpha;
	    *dpsib = *dpsi;
	    goto L100;
	}
	if (psi <= cgparms_1.fpert) {
/*              if ( PrintLevel ) then */
/*                  write (6, *) "update a:", alpha, "dpsia:", dpsi */
/*              endif */
	    *a = *alpha;
	    *dpsia = *dpsi;
	} else {
	    *b = *alpha;
	}
    }
L100:
    ret_val = -1;
L110:
/*      if ( PrintLevel ) then */
/*          write (*, 200) a, b, dpsia, dpsib, cg_updateW */
/* 200       format ('UP a:', e14.6, ' b:', e14.6, */
/*     &             ' da:', e14.6, ' db:', e14.6, ' up:', i2) */
/*      endif */
    return ret_val;
} /* cg_updatew__ */

