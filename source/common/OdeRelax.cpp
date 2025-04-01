#include "stdafx.h"
#include "../common/OdeRelax.h"
#include "../common/utils.h"
#include "../common/nrutil.h"


OdeRelax::OdeRelax(){
	Initialized = false;
}
OdeRelax::~OdeRelax(){
	if (Initialized){
		DeAlloc();
	}
}
void OdeRelax::ZeroYArrays(){
	if (!Initialized) return;
	for (int j=1; j<=ne; j++){
		for (int k=1; k<=m; k++){
			y[j][k] = 0.0;
		}
	}
}

void OdeRelax::DeAlloc(){
	free_dvector(x,1,m);
	free_dvector(ermax,1,ne);
	free_ivector(kmax,1,ne);
	free_ivector(indexv, 0, ne+1);
	free_dvector(scalv, 0, ne+1);
	free_dmatrix(y, 1, ne, 1, m);
	free_dmatrix(s, 1, ne, 1, 2*ne+1);
	free_d3tensor(c, 1, ne, 1, ne-nb+1, 1, m+1);
//	free_d3tensor(c, 1, ne, 1, ne, 1, m+1);
}

void OdeRelax::Init(int NumberOfEquations, int NumberOfPoints, int NumberOfLeftBoundaryConditions){
	if (Initialized){
		DeAlloc();
	}

	ne = NumberOfEquations;
	m = NumberOfPoints;
	nb = NumberOfLeftBoundaryConditions;

//	ShowFloat(ne, "ne");

//	ShowFloat(nb, "nb");
//	ShowFloat(m, "m");

	kmax=ivector(1,ne);
	ermax=dvector(1,ne);

	x=dvector(1, m);
	indexv = ivector(0, ne+1);
	scalv = dvector(0, ne+1);

	y=dmatrix(1, ne, 1, m);
	s=dmatrix(1, ne, 1, 2*ne+1);
	c=d3tensor(1, ne, 1, ne-nb+1, 1, m+1);
//	c=d3tensor(1, ne, 1, ne+1, 1, m+1);

	itmax=1; // default: 1
	conv=5.0e-200;// default: 5e-6
	slowc=1.0; // default 1.0
	for (int i=0; i<=(ne+1); i++){
		scalv[i]=1.0;// : 1...NE+1: default : 1.0
		indexv[i]=i;//	: 1...NE+1: default : i
	}
//	indexv[ne+1]=ne+1;


	Initialized = true;

}

void OdeRelax::DoDifeq(int k, int k1, int k2, int jsf, int is1, int isf, int * indexv, double * x, double **s, double **y, int * stuff){
	if (true){
		if (k==k1){
			int na=ne-nb;
			for (int n=(na+1); n<=ne; n++){
				for (int i=1; i<=ne; i++){
					s[n][ne+indexv[i]]=0;
				}
				s[n][jsf]=0;
			}
		}
		else{
			if (k>k2){
				int na=ne-nb;
				for (int n=1; n<=na; n++){
					for (int i=1; i<=ne; i++){
						s[n][ne+indexv[i]]=0;
					}
					s[n][jsf]=0;
				}
			}
			else{
				for (int n=1; n<=ne; n++){
					for (int i=1; i<=ne; i++){
						s[n][ne+indexv[i]]=0;
						s[n][indexv[i]]=0;
					}
					s[n][jsf]=0;
				}
			}
		}
	}

	difeq(k,k1, k2, jsf,is1,isf,indexv, x, s,y, stuff);
}


void OdeRelax::solvde(int MaxIter, OdeRelax_DiffEqFunction_type equation, int * stuff){
	itmax = MaxIter;
	difeq =equation;
	STUFF = stuff;
//	for (int j=1; j<=8; j++){
//		ShowFloat(j, "j");
//		ShowFloat(y[j][1], "yj1");
//	}


//void solvde(int itmax, float conv, float slowc, float scalv[], int indexv[],
//	int ne, int nb, int m, float **y, float ***c, float **s)

/*
void bksub(int ne, int nb, int jf, int k1, int k2, float ***c);
	void difeq(int k, int k1, int k2, int jsf, int is1, int isf,
		int indexv[], int ne, float **s, float **y);
	void pinvs(int ie1, int ie2, int je1, int jsf, int jc1, int k,
		float ***c, float **s);
	void red(int iz1, int iz2, int jz1, int jz2, int jm1, int jm2, int jmf,
		int ic1, int jc1, int jcf, int kc, float ***c, float **s);
*/
	int ic1,ic2,ic3,ic4,it,j,j1,j2,j3,j4,j5,j6,j7,j8,j9;
	int jc1,jcf,jv,k,k1,k2,km,kp,nvars;
	double err,errj,fac,vmax,vz;

//	kmax=ivector(1,ne);
//	ermax=vector(1,ne);
	k1=1;
	k2=m;
	nvars=ne*m;
	j1=1;
	j2=nb;
	j3=nb+1;
	j4=ne;
	j5=j4+j1;
	j6=j4+j2;
	j7=j4+j3;
	j8=j4+j4;
	j9=j8+j1;
	ic1=1;
	ic2=ne-nb;
	ic3=ic2+1;
	ic4=ne;
	jc1=1;
	jcf=ic3;
	for (it=1;it<=itmax;it++) {
		k=k1;
//		difeq(k,k1,k2,j9,ic3,ic4,indexv,ne,s,y);

//		difeq(k,k1, k2, j9,ic3,ic4,indexv, x, s,y, STUFF);
		DoDifeq(k,k1, k2, j9,ic3,ic4,indexv, x, s,y, STUFF);
		pinvs(ic3,ic4,j5,j9,jc1,k1,c,s);
		for (k=k1+1;k<=k2;k++) {
			kp=k-1;
//			difeq(k,k1,k2,j9,ic1,ic4,indexv,ne,s,y);
//			difeq(k,k1, k2, j9,ic1,ic4,indexv, x, s,y, STUFF);
			DoDifeq(k,k1, k2, j9,ic1,ic4,indexv, x, s,y, STUFF);
//			ShowFloat(k,"k a");
			red(ic1,ic4,j1,j2,j3,j4,j9,ic3,jc1,jcf,kp,c,s);
//			ShowFloat(k,"k b");
			pinvs(ic1,ic4,j3,j9,jc1,k,c,s);
//			ShowFloat(k,"k c");
//		Alert0("hi");
		}
		k=k2+1;
//		difeq(k,k1,k2,j9,ic1,ic2,indexv,ne,s,y);
//		difeq(k,k1, k2, j9,ic1,ic2,indexv, x, s,y, STUFF);
		DoDifeq(k,k1, k2, j9,ic1,ic2,indexv, x, s,y, STUFF);
		red(ic1,ic2,j5,j6,j7,j8,j9,ic3,jc1,jcf,k2,c,s);
		pinvs(ic1,ic2,j7,j9,jcf,k2+1,c,s);
		bksub(ne,nb,jcf,k1,k2,c);
		err=0.0;
		for (j=1;j<=ne;j++) {
			jv=indexv[j];
			errj=vmax=0.0;
			km=0;
			for (k=k1;k<=k2;k++) {
				vz=fabs(c[jv][1][k]);
				if (vz > vmax) {
					vmax=vz;
					km=k;
				}
				errj += vz;
			}
			err += errj/scalv[j];
			ermax[j]=c[jv][1][km]/scalv[j];
			kmax[j]=km;
		}
		err /= nvars;
		fac=(err > slowc ? slowc/err : 1.0);
		for (j=1;j<=ne;j++) {
			jv=indexv[j];
			for (k=k1;k<=k2;k++)
				y[j][k] -= fac*c[jv][1][k];
		}
//		printf("\n%8s %9s %9s\n","Iter.","Error","FAC");
//		printf("%6d %12.6f %11.6f\n",it,err,fac);
		if (err < conv) {
//			free_vector(ermax,1,ne);
//			free_ivector(kmax,1,ne);
			return;
		}
	}
//	Alert0("Too many iterations in solvde");
	//	nrerror("Too many iterations in solvde");
}
