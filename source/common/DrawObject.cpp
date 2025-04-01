#include "stdafx.h"
#include "../common/DrawObject.h"
#include <math.h>
#include "../common/utils.h"
#include "../common/MyPolygon.h"
#include "../common/Simplex.h"
#include "../common/ConnectedComponent.h"

/*
int MultZoom(int x, int Zoom, double RealZoom){
	if (Zoom!=0){
		return (x*Zoom);
	}
	return int (double(x)*RealZoom);
}
int DivZoom(int x, int Zoom, double RealZoom){
	if (Zoom!=0){
		return (x/Zoom);
	}
	return int (double(x)/RealZoom);
}
*/
Vect::Vect(){
}

Vect::Vect(double xx, double yy){
	x=xx;
	y=yy;
}
Vect::Vect(int xx, int yy){
	x=xx;
	y=yy;
}
double Vect::Cross(Vect a, Vect b){
	return (a.x*b.y-a.y*b.x);
}


CPoint GetInterSection(CPoint P1, CPoint P2, CPoint P3, CPoint P4){
	//  lambda*(P2-P1) + P1 = mu *(P4-P3) + P3
	// l*(P2.x-P1.x) + m*(p3.x-P4.x) = P3.x-P1.x
	// l*(P2.y-P1.y) + m*(p3.y-P4.y) = P3.y-P1.y
	// 


	CPoint P(0,0);

/*	if (P1.x == P2.x){
		if (P1.y == P2.y){
			return P;
		}
		return GetInterSection(P3, P4, P1, P2);
	}
	double tp1 = double(P2.x-P1.x);
	double tp2 = double(P3.x-P4.x);
	double tp3 = double(P3.x-P1.x);
	double tp4 = double(P2.y-P1.y);
	double tp5 = double(P3.y-P4.y);
	double tp6 = double(P3.y-P1.y);

	double tp7 = tp5-tp2*tp4/tp1;
	double tp8 = tp6 - tp3*tp4/tp1;
	double mu = tp8/tp7;

//	ShowFloat(mu, "mu");

	P.x = int ( -mu*tp2 ) + P3.x;
	P.y = int ( -mu*tp5 ) + P3.y;

//	ShowFloat(P.x, "x");
//	ShowFloat(P.y, "y");

	double l= (tp3-mu*tp2)/tp1;

//	ShowFloat(l, "l");

	CPoint Q;
	Q.x = int ( l*tp1 ) + P1.x;
	Q.y = int ( l*tp4 ) + P1.y;

//	ShowFloat(Q.x, "x");
//	ShowFloat(Q.y, "y");

	return P;
*/
	double ** matrix = dmatrix(0,1,0,1);
	matrix[0][0] = double(P2.x-P1.x);
	matrix[0][1] = double(P3.x-P4.x);

	matrix[1][0] = double(P2.y-P1.y);
	matrix[1][1] = double(P3.y-P4.y);

	int result = InvertDoubleMat (matrix, matrix, 2);
	// >0: OK; <0 : singular
	if (result > 0){

		double rs[2], lm[2];
		rs[0]=double(P3.x-P1.x);
		rs[1]=double(P3.y-P1.y);

		for (int i=0; i<2; i++){
			lm[i] = 0;
			for (int j=0; j<2; j++){
				lm[i] += matrix[i][j]*rs[j];
			}
		}
		free_dmatrix(matrix, 0,1,0,1);
//		ShowFloat(lm[0], "l");

//	MulMat(matrix, rs, Intersect, 2,2,1);

		P.x = int(lm[0] * double(P2.x-P1.x) + double(P1.x));
		P.y = int(lm[0] * double(P2.y-P1.y) + double(P1.y));
	}

	return P;
}

CPoint Vect::MakePoint(Vect v){
	return CPoint(int(v.x), int(v.y));
}

Vect::Vect(CPoint p){
	x=p.x;
	y=p.y;
}

double Vect::GetAngle(Vect a, Vect b){
	double cosalpha = Dot(a, b)/Length(a)/Length(b);
//	ShowFloat(cosalpha, "cosalpha");
	double abscosalpha = fabs(cosalpha);

	double alpha = acos(abscosalpha);
//	ShowFloat(alpha, "alpha");

	double det = -(a.x*b.y-a.y*b.x);

//	ShowFloat(det, "det");

	if (det>0.0){
		if (cosalpha>0.0){
			return alpha;
		}
		else{
			return (PIGRECO-alpha);
		}
	}
	else{
		if (cosalpha>0.0){
			return (2.0*PIGRECO-alpha);
		}
		else{
			return (PIGRECO+alpha);
		}
	}
}
double Vect::GetAngle(Vect a){  // Gets angle with the positive x axis
	return GetAngle(a, Vect(1.0, 0.0));
}


double Vect::SquareDistance(Vect v){
	double tp1=x-v.x;
	double tp2=y-v.y;
	return (tp1*tp1+tp2*tp2);

};
double Vect::SquareDistance(CPoint p){
	return SquareDistance(Vect(p));
};

double Vect::SquareLength(){
	return x*x+y*y;
};

double Vect::Length(){
	return sqrt(SquareLength());
};
double Vect::SquareDistance(Vect v1, Vect v2){
	return v1.SquareDistance(v2);
};
double Vect::SquareDistance(CPoint p1, CPoint p2){
	return Vect(p1).SquareDistance(p2);
};
double Vect::SquareDistance(Vect v1, CPoint p2){
	return v1.SquareDistance(p2);
};
double Vect::SquareDistance(CPoint p1, Vect v2){
	return v2.SquareDistance(p1);
};
double Vect::Length(Vect v){
	return v.Length();
};

Vect Vect::Add(Vect v1, Vect v2){
	return Vect(v1.x+v2.x, v1.y+v2.y);
}

Vect Vect::Sub(Vect v1, Vect v2){
	return Vect(v1.x-v2.x, v1.y-v2.y);
}

Vect Vect::DiffPoints(CPoint p1, CPoint p2){
	return Sub(Vect(p1), Vect(p2));
};
Vect Vect::DiffPoints(Vect v1, CPoint p2){
	return Sub(v1, Vect(p2));
};
Vect Vect::DiffPoints(CPoint p1, Vect v2){
	return Sub(Vect(p1), v2);
};
Vect Vect::AddPoints(CPoint p1, CPoint p2){
	return Add(Vect(p1), Vect(p2));
}; 
Vect Vect::AddPoints(Vect v1, CPoint p2){
	return Add(v1, Vect(p2));
};
Vect Vect::AddPoints(CPoint p1, Vect v2){
	return Add(Vect(p1), v2);
};
double Vect::Dot(CPoint p1, CPoint p2){
	return Dot(Vect(p1), Vect(p2));
};
double Vect::Dot(Vect p1, Vect p2){
	return (p1.x*p2.x+p1.y*p2.y);
};
double Vect::Dot(CPoint p){
	return Vect::Dot(*this, Vect(p));
};
double Vect::Dot(Vect v){
	return Vect::Dot(*this, v);
};
Vect Vect::ScaleVect(double lambda, Vect v){
	v.Scale(lambda);
	return v;
};
Vect Vect::ScaleVect(double lambda){
	return ScaleVect(lambda, *this);
};
void Vect::Scale(double lambda){
	x *= lambda;
	y *= lambda;
};

void Vect::Normalize(){
	Scale(1.0/Length());
}

Vect Vect::Normalize(Vect v){
	v.Normalize();
	return v;
}

CPoint Vect::MakePoint(){
	return CPoint(int(x), int(y));
}

Vect Vect::UnitVertVect(Vect v){
	if (v.Length()<1e-300){
		return Vect(0, 0);
	}
	double a, b;
	if (v.x==0){
		a=1;
		b=0;
	}
	else{
		b=1.0;
		a=-v.y*b/v.x;
	}
	Vect result = Vect(a, b);
	result.Normalize();
	return result;
}



/********************************************************************/

ObjectPoint::ObjectPoint(){
	Init();
}

void ObjectPoint::Init(){
	radius = 4;
	UndoIndex = 0;
	pos.x = pos.y = 0;
	ResetUndo();
}


void ObjectPoint::ResetUndo(){ // sets all undo positions to current position
	for (int i=0; i<MAXUNDOOBJECTPOINT; i++){
		oldpos[i] = pos;
	}

}

void ObjectPoint::SetPos(CPoint p){
	pos = p;
	oldpos[UndoIndex]=pos;
}
void ObjectPoint::SetPos(int x, int y){
	SetPos(CPoint(x, y));
}


void ObjectPoint::SetPosWithUndo(CPoint p){
	pos = p;
	oldpos[UndoIndex]=pos;
	UndoIndex++;
	if (UndoIndex == MAXUNDOOBJECTPOINT){
		UndoIndex = 0;
	}
}
void ObjectPoint::SetPosWithUndo(int x, int y){
	SetPosWithUndo(CPoint (x, y));
}

void ObjectPoint::SetPosWithUndo(){
	SetPosWithUndo(pos);
}

void ObjectPoint::Undo(){
	UndoIndex--;
	if (UndoIndex<0){
		UndoIndex = MAXUNDOOBJECTPOINT - 1;
	}
	pos = oldpos[UndoIndex];
}
void ObjectPoint::Redo(){
	UndoIndex++;
	if (UndoIndex==MAXUNDOOBJECTPOINT){
		UndoIndex =  0;
	}
	pos = oldpos[UndoIndex];
}

CPoint ObjectPoint::GetPos(int Zoom,  bool ZoomIn){
	return CPoint(DivZoom(pos.x, Zoom, ZoomIn), DivZoom(pos.y, Zoom, ZoomIn));
}
int ObjectPoint::Getx(int Zoom,  bool ZoomIn){
	return DivZoom(pos.x, Zoom, ZoomIn);
/*	if (Zoom != 0){
		return pos.x/Zoom;
	}
	else{
		return int(double(pos.x)/RealZoom);
	}
*/
}
int ObjectPoint::Gety(int Zoom,  bool ZoomIn){
	return DivZoom(pos.y, Zoom, ZoomIn);
/*	if (Zoom != 0){
		return pos.y/Zoom;
	}
	else{
		return int(double(pos.y)/RealZoom);
	}
*/
}

double ObjectPoint::GetRadius(){
	return radius;
}

double ObjectPoint::SquareDistance(ObjectPoint p1, ObjectPoint p2){
	return SquareDistance(p1.pos, p2.pos);
}
double ObjectPoint::SquareDistance(CPoint p1, CPoint p2){
	return Vect::SquareDistance(p1, p2);
}

double ObjectPoint::SquareDistance(ObjectPoint p){
	return SquareDistance(*this, p);
}

double ObjectPoint::SquareDistance(CPoint p){
	return SquareDistance(this->pos, p);
}

bool ObjectPoint::IsClicked(CPoint p){
	if (SquareDistance(this->pos, p)<=(radius*radius)){
		return true;
	}
	else{
		return false;
	}
}

CPoint ObjectPoint::DiffPoints(CPoint p1, CPoint p2){ // return p1 - p2
	return CPoint(p1.x-p2.x, p1.y-p2.y);
}
CPoint ObjectPoint::AddPoints(CPoint p1, CPoint p2){ // return p1 - p2
	return CPoint(p1.x+p2.x, p1.y+p2.y);
}

void ObjectPoint::AddVector(CPoint vector){
	SetPos( CPoint(pos.x+vector.x, pos.y+vector.y) );
}


double ObjectPoint::LengthVector(CPoint p){
	return Vect(p).Length();
/*	double x=p.x;
	double y = p.y;

	return sqrt(x*x+y*y);
*/
}


double ObjectPoint::Dot(CPoint p1, CPoint p2){ // return p1 . p2
	return Vect::Dot(p1, p2);
//	return p1.x*p2.x + p1.y*p2.y;
}

CPoint ObjectPoint::ScaleVect(double lambda, CPoint p){ // return lambda*p1
	return Vect::ScaleVect(lambda, Vect(p)).MakePoint();
//	return CPoint(int (lambda*double(p.x)), int(lambda*double(p.y)));
}

Vect ObjectPoint::MakeVect(){
	return Vect(Getx(1,1), Gety(1,1));
}

/************************************************************************/


AreaObject::AreaObject(){
/*
	redPen = new CPen(PS_SOLID, 1, RGB(255,0,0));
	bluePen = new CPen(PS_SOLID, 1, RGB(0,0,255));
	blackPen= new CPen(PS_SOLID, 1, RGB(0,0,0));

	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
	blackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
*/
	RectangleGeneratorInitialized = false;
	ConusGeneratorInitialized = false;;
	SetSize(640, 480);

/*
	SizeGeneratorList = 0;
	BufferSizeGeneratorList = 0;
	pointlist = NULL;
//	Define(0, CPoint(0,0),CPoint(0,0),CPoint(0,0),CPoint(0,0)); 
	this->UpdateSizeGeneratorList(1);
*/
	pointlist.UpdateBuffer(1);
}
AreaObject::~AreaObject(){
/*
	delete redPen;
	delete bluePen;
	delete blackPen;
*/
//	DeallocPointList();
}

void AreaObject::SetSize(int xsize, int ysize){
	XSIZE = xsize;
	YSIZE = ysize;
}
void AreaObject::Define(int type, CPoint p[NPOINTSAREAOBJECT], int xsize, int ysize){

	ObjectType = type;
	if (ObjectType<0) ObjectType = 0;  // 0: Rectangle
	if (ObjectType>1) ObjectType = 1;  // 1: Conus

	int i;

	for (i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].SetPos(p[i]);
	}
	for (i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].ResetUndo();
	}
	SetSize(xsize, ysize);
	this->CalcGeneratorList();

};

void AreaObject::Define(int type, CPoint p0, CPoint p1, CPoint p2, CPoint p3, int xsize, int ysize){

	ObjectType = type;
	if (ObjectType<0) ObjectType = 0;  // 0: Rectangle
	if (ObjectType>1) ObjectType = 1;  // 1: Conus

	P[0].SetPos(p0);

//	ShowFloat(p0.x, "x");
//	ShowFloat(P[0].Getx(), "x");

	P[1].SetPos(p1);
	P[2].SetPos(p2);
	P[3].SetPos(p3);
	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].ResetUndo();
//		ShowFloat(P[i].Getx(), "x");
	}
	SetSize(xsize, ysize);
	this->CalcGeneratorList();
};

int AreaObject::IsClicked(CPoint p){ 
		// returns -1 if not clicked; otherwise the index of the clicked point
	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		if (P[i].IsClicked(p)) return i;
	}
	return -1;
}


void AreaObject::Undo(){
	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].Undo();
	}
}

void AreaObject::Redo(){
	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].Redo();
	}
}


void AreaObject::MovePoint(int index, CPoint newpos){
	// recalculates the position of all other points based on the new coordinates of 
	// this point
	if (index<0){
		return;
	}
	if (index>=NPOINTSAREAOBJECT){
		return;
	}

	CPoint diff = ObjectPoint::DiffPoints(newpos, P[index].GetPos(1,1) );

	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].AddVector(diff);
	}

}

void AreaObject::DrawObject(CDC * pDC, CRect & DrawRect, int Zoom,  bool ZoomIn){
	if (!pDC) return;

	switch (ObjectType){
		case 0 : DrawRectangle(pDC, DrawRect, 0, NULL, Zoom, ZoomIn); break;// Rectangle
		case 1 : DrawConus(pDC, DrawRect, 0, NULL, Zoom, ZoomIn); break;// Conus
		default :  DrawRectangle(pDC, DrawRect, 0, NULL, Zoom, ZoomIn); break;// Rectangle;
	}
}

void AreaObject::DrawObject(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos, int Zoom,  bool ZoomIn){
	if (!pDC) return;

	switch (ObjectType){
		case 0 : DrawRectangle(pDC, DrawRect, ncursors, curspos, Zoom, ZoomIn); break;// Rectangle
		case 1 : DrawConus(pDC, DrawRect, ncursors, curspos, Zoom, ZoomIn); break;// Conus
		default :  DrawRectangle(pDC, DrawRect, ncursors, curspos, Zoom, ZoomIn); break;// Rectangle;
	}
}

void AreaObject::DrawRectangle(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos, int Zoom,  bool ZoomIn){
	pDC->MoveTo(P[3].GetPos(Zoom, ZoomIn));
	int i;
	for (i=0; i<4; i++){
		pDC->LineTo(P[i].GetPos(Zoom, ZoomIn));
	}
	for (i=0; i<4; i++){
		int x1, x2, y1, y2;
		int x0=P[i].Getx(Zoom, ZoomIn);
		int y0=P[i].Gety(Zoom, ZoomIn);

		x1=x0-P[i].GetRadius();
		x2=x0+P[i].GetRadius();
		y1=y0-P[i].GetRadius();
		y2=y0+P[i].GetRadius();
		
		pDC->Ellipse(x1, y1, x2, y2);
	}
	int x0=(P[0].Getx(Zoom, ZoomIn)+P[1].Getx(Zoom, ZoomIn))/2;
	int y0=(P[0].Gety(Zoom, ZoomIn)+P[1].Gety(Zoom, ZoomIn))/2;

	pDC->MoveTo(x0, y0);

	int x1=(P[2].Getx(Zoom, ZoomIn)+P[3].Getx(Zoom, ZoomIn))/2;
	int y1=(P[2].Gety(Zoom, ZoomIn)+P[3].Gety(Zoom, ZoomIn))/2;

	pDC->LineTo(x1, y1);

	int r=P[0].GetRadius();
	x1 = x0+ r;
	x0 -= r;
	y1 = y0+ r;
	y0 -= r;
	pDC->Ellipse(x0, y0, x1, y1);

}

bool AreaObject::GetMidPointAndRadius(Vect & M, double & r1, double & r2){
	if (this->ObjectType==0){ // Rectangle, and thus Radius makes no sense, but return 
								// gracefully.
		r1 = 0;
		r2 = Length();
		M = GetMidPoint();
		return false;
	}
	CPoint A = P[0].GetPos(1,1);
	CPoint B = P[1].GetPos(1,1);
	CPoint C = P[2].GetPos(1,1);
	CPoint D = P[3].GetPos(1,1);

	CPoint BA = ObjectPoint::DiffPoints(B, A );
	CPoint CD = ObjectPoint::DiffPoints(C, D );

	double lBA = ObjectPoint::LengthVector(BA);
	double lCD = ObjectPoint::LengthVector(CD);

	if (fabs(lBA-lCD)<0.001){
		//Alert0("return");
		ObjectType = 0;
		GetMidPointAndRadius(M, r1, r2);
		ObjectType = 1;
		return false;
	}

	double lambda = lBA/(lCD-lBA);

	double Bx=P[1].Getx(1,1);
	double By=P[1].Gety(1,1);
	double Cx=P[2].Getx(1,1);
	double Cy=P[2].Gety(1,1);

	M=Vect( int(Bx+lambda*(Bx-Cx)), int(By+lambda*(By-Cy)));


	Vect BM = Vect::Sub(B, M);

	r1 = BM.Length();

	Vect CM = Vect::Sub(C, M);

	r2 = CM.Length();

	return true;
}


void AreaObject::DrawConus(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos, int Zoom,  bool ZoomIn){

	AdjustConusIndexes();

	Vect M;
	double r1, r2;

	if (!GetMidPointAndRadius( M, r1, r2)){
		return;
	}

	M.x = DivZoom(M.x, Zoom, ZoomIn);
	M.y = DivZoom(M.y, Zoom, ZoomIn);
	r1 = DivZoom(r1, Zoom, ZoomIn);
	r2 = DivZoom(r2, Zoom, ZoomIn);
/*
if (Zoom != 0){
		M.x /= Zoom;
		M.y /= Zoom;
		r1 /= Zoom;
		r2 /= Zoom;
	}
	else{
		M.x = int(double(M.x)/RealZoom);
		M.y = int(double(M.y)/RealZoom);
		r1 = int(double(r1)/RealZoom);
		r2 = int(double(r2)/RealZoom);
	}
*/
	CPoint A = P[0].GetPos(Zoom, ZoomIn);
	CPoint B = P[1].GetPos(Zoom, ZoomIn);
	CPoint C = P[2].GetPos(Zoom, ZoomIn);
	CPoint D = P[3].GetPos(Zoom, ZoomIn);

	CRect rect;

	rect.top = M.y - r1;
	rect.bottom = M.y + r1;
	rect.left = M.x - r1;
	rect.right = M.x + r1;

	pDC->Arc(rect, B, A);

	rect.top = M.y - r2;
	rect.bottom = M.y + r2;
	rect.left = M.x - r2;
	rect.right = M.x + r2;

	pDC->Arc(rect, C, D);

	pDC->MoveTo(B);
	pDC->LineTo(C);

	pDC->MoveTo(A);
	pDC->LineTo(D);

	int i;
	for (i=0; i<4; i++){
		int x1, x2, y1, y2;
		int x0=P[i].Getx(Zoom, ZoomIn);
		int y0=P[i].Gety(Zoom, ZoomIn);

		x1=x0-P[i].GetRadius();
		x2=x0+P[i].GetRadius();
		y1=y0-P[i].GetRadius();
		y2=y0+P[i].GetRadius();
		
		pDC->Ellipse(x1, y1, x2, y2);
	}
	if (curspos){
		CPen * redPen = new CPen(PS_SOLID, 1, RGB(255,0,0));
		CPen * bluePen = new CPen(PS_SOLID, 1, RGB(0,0,255));

		CPen * OldPen = pDC->SelectObject(redPen);

		for (i=0; i<ncursors; i++){
			double ri=r1+(r2-r1)*curspos[i];
			rect.top = M.y - ri;
			rect.bottom = M.y + ri;
			rect.left = M.x - ri;
			rect.right = M.x + ri;
			Vect d1=Vect::Sub(D, A);
			Vect d2=Vect::Sub(C, B);
			d1.Scale(curspos[i]);
			d2.Scale(curspos[i]);
			Vect ap=Vect::Add(A, d1);
			Vect bp=Vect::Add(B, d2);
			CPoint Ap = ap.MakePoint();
			CPoint Bp = bp.MakePoint();
			if (i>1){
				pDC->SelectObject(bluePen);
			}
			pDC->Arc(rect, Bp, Ap);

		}
		pDC->SelectObject(OldPen);

		bluePen->DeleteObject();
		redPen->DeleteObject();
		delete bluePen;
		delete redPen;
	}
}


void AreaObject::SizePoint(int index, CPoint newpos){
	// recalculates the position of all other points based on the new coordinates of 
	// this point
	if (index<0){
		return;
	}
	if (index>=NPOINTSAREAOBJECT){
		return;
	}

	if (P[index].Getx(1,1) == newpos.x ){
		if (P[index].Gety(1,1) == newpos.y ){
			return ; // Old pos = new pos
		}
	}


	switch (ObjectType){
		case 0 : SizeRectangle(index, newpos); break;// Rectangle
		case 1 : SizeConus(index, newpos); break;// Conus
		default :  SizeRectangle(index, newpos); break;// Rectangle;
	}
}

void AreaObject::SizeRectangle(int index, CPoint newpos){
	// The diagonal point remains fixed, while the other two change in dependence of the 
	// new coordinates
	// 0 - 2 and 1 - 3 are diagonal pairs


	int i0, i1, i2, i3;

	// i3: the one moved; i1= the one fixed; i0, i2, the ones to be calculated;

	i3=index;

	switch (i3){
		case 0 : i1 = 2; i0= 1; i2 = 3; break;
//		case 1 : i1 = 3; i0= 0; i2 = 2; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
//		case 2 : i1 = 0; i0= 3; i2 = 1; break;
		case 2 : i1 = 0; i0= 3; i2 = 1; break;
//		case 3 : i1 = 1; i0= 2; i2 = 0; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}

	for (int jj=0; jj<4; jj++){
//		if (jj!=i3){
			if (newpos.x==P[jj].Getx(1,1)){
				if (newpos.y==P[jj].Gety(1,1)){
					return;
				}
			}
//		}
	}

	Vect A = Vect(P[i0].GetPos(1,1));
	Vect B = Vect(P[i1].GetPos(1,1));
	Vect C = Vect(P[i2].GetPos(1,1));
	Vect D = Vect(P[i3].GetPos(1,1));
	Vect d = Vect(newpos);

	Vect diff = Vect::Sub(d, D);
	Vect CB = Vect::Sub(C, B);
	Vect UnitCB = Vect::Normalize(CB);
	double p1 = Vect::Dot(UnitCB, diff);
	Vect AddCB = Vect::Add(CB, Vect::ScaleVect(p1, UnitCB));
	Vect c = Vect::Add(AddCB, B);
	P[i0].SetPos(c.MakePoint());

	Vect AB = Vect::Sub(A, B);
	Vect UnitAB = Vect::Normalize(AB);
	double p2 = Vect::Dot(UnitAB, diff);
	Vect AddAB = Vect::Add(AB, Vect::ScaleVect(p2, UnitAB));
	Vect a = Vect::Add(AddAB, B);
	P[i2].SetPos(a.MakePoint());

	P[i3].SetPos(newpos);

	AdjustI2(i1, i2, i3, i0);

}

void AreaObject::AdjustI2(int i1, int i2, int i3, int i0){

	Vect A = Vect(P[i1].GetPos(1,1));
	Vect E = Vect(P[i3].GetPos(1,1));
	Vect C = Vect(P[i2].GetPos(1,1));

	Vect M = Vect::Add(A, E);
	M.Scale(0.5);

	Vect AM = Vect::Sub(A, M);
	Vect CM = Vect::Sub(C, M);

	CM.Scale(AM.Length()/CM.Length());
	P[i2].SetPos(Vect::Add(M, CM).MakePoint());

	AdjustI0(i1, i2,i3, i0);
}

void AreaObject::AdjustI0(int i1, int i2, int i3, int i0){

	Vect A = Vect(P[i1].GetPos(1,1));
	Vect E = Vect(P[i3].GetPos(1,1));
	Vect C = Vect(P[i2].GetPos(1,1));

	Vect v = Vect::Add(A, E);
	v= Vect::Sub(v, C);
	P[i0].SetPos(v.MakePoint());
}

void ShowVect (Vect v, CString text){
	return;
	char s[20];
	_gcvt(v.x, 5, s);
	text += CString(": x= ")+CString(s)+CString("; y =");
	_gcvt(v.y, 5, s);
	text += CString(s);
	AfxGetMainWnd()->MessageBox(text);
}
void MyShowFloat(double v, CString text){
	return;
	char s[20];
	_gcvt(v, 5, s);
	text += CString(" = ")+CString(s);
	AfxGetMainWnd()->MessageBox(text);
}

void AreaObject::SizeConus(int index, CPoint newpos){
	int iA, iB, iC, iD;
	switch (index){
		case 0 : iB=0; iC=3; iD=2; iA = 1; break;
		case 1 : iB=1; iC=2; iD=3; iA = 0; break;
		case 2 : iB=2; iC=1; iD=0; iA = 3; break;
		case 3 : iB=3; iC=0; iD=1; iA = 2; break;
		default : return;
	}

	Vect M = SaveSizeMidPoint.MakeVect();
	Vect NewDiff = Vect::DiffPoints(newpos, M);
	double r = SaveSizeri;
	double newr = NewDiff.Length();
	if (this->IsInnerRadiusOnSize){ 	// if move A or B, new radius r1 cannot be larger than r2
		if (newr<=r){
//			Beep(2000,10);
//			Alert0("newr <= r");
			return;
		}
	}
	else{
		if (newr>=r){
//			Beep(1000,10);
//			Alert0("newr >= r");
			return;
		}
	}

	Vect NewC = NewDiff;
	NewC.Normalize();
	NewC.Scale(r);
	Vect NewA = Vect::Sub(SaveSizePoint.MakeVect(), M);
	NewA.Normalize();
	NewA.Scale(newr);

	NewC = Vect::Add(M, NewC);
	NewA = Vect::Add(M, NewA);

	P[iB].SetPos(newpos);
	P[iA].SetPos(NewA.MakePoint());
	P[iC].SetPos(NewC.MakePoint());


}
void AreaObject::NotSoOldSizeConus(int index, CPoint newpos){
	OldSizeConus(index, newpos);
	//VeryOldSizeConus(index, newpos);
	return;
	int iA, iB, iC, iD;
	switch (index){
		case 0 : iA=0; iB=1; iC=2; iD = 3; break;
		case 1 : iA=1; iB=0; iC=3; iD = 2; break;
		case 2 : iA=2; iB=3; iC=0; iD = 1; break;
		case 3 : iA=3; iB=2; iC=1; iD = 0; break;
		default : return;
	}


	Vect A = Vect(P[iA].GetPos(1,1)); 
	Vect B = Vect(P[iB].GetPos(1,1)); 
	Vect C = Vect(P[iC].GetPos(1,1)); 
	Vect D = Vect(P[iD].GetPos(1,1)); 
	Vect a = Vect(newpos);			

	Vect aA = Vect::Sub(a, A);
	if (aA.Length()<1.5) return;

	Vect M = GetMidPoint();	

	Vect aM = Vect::Sub(a, M);
	Vect CM = Vect::Sub(C, M);

	double lam = aM.Length();
	double lcm = CM.Length();

	Vect eCM = CM;
	eCM.Normalize();

	Vect eaM = aM;
	eaM.Normalize();

	Vect d1=eCM;
	d1.Scale(lam);
	Vect b = Vect::Add(M, d1);

	Vect d2=eaM;
	d2.Scale(lcm);
	Vect d =Vect::Add(M, d2);

	P[iA].SetPos(newpos);
	P[iB].SetPos(b.MakePoint());
	P[iD].SetPos(d.MakePoint());


}

   

void AreaObject::VeryOldSizeConus(int index, CPoint newpos){
	int iA, iB, iC, iD;
	switch (index){
		case 0 : iA=0; iB=1; iC=2; iD = 3; break;
		case 1 : iA=1; iB=0; iC=3; iD = 2; break;
		case 2 : iA=2; iB=3; iC=0; iD = 1; break;
		case 3 : iA=3; iB=2; iC=1; iD = 0; break;
		default : return;
	}


	Vect A = Vect(P[iA].GetPos(1,1)); 
	Vect B = Vect(P[iB].GetPos(1,1)); 
	Vect C = Vect(P[iC].GetPos(1,1)); 
	Vect D = Vect(P[iD].GetPos(1,1)); 
	Vect a = Vect(newpos);			

	Vect aA = Vect::Sub(a, A);
	if (aA.Length()<3) return;

	Vect M = GetMidPoint();		   

/*
	A.y = -A.y;
	B.y = -B.y;
	C.y = -C.y;
	D.y = -D.y;
	a.y = -a.y;
	M.y = -M.y;
*/
	Vect BM = Vect::Sub(B, M);
	Vect AB = Vect::Sub(A, B);
	Vect aM = Vect::Sub(a, M);
	Vect DC = Vect::Sub(D, C);
	Vect CM = Vect::Sub(C, M);


	Vect eBM = BM; eBM.Normalize();
	Vect eAB = AB; eAB.Normalize();
	Vect eaM = aM; eaM.Normalize();
	Vect eDC = DC; eDC.Normalize();


	ShowVect(A, "A");
	ShowVect(B, "B");
	ShowVect(C, "C");
	ShowVect(D, "D");
	ShowVect(a, "a");
	ShowVect(M, "M");

	ShowVect(BM, "BM");
	ShowVect(AB, "AB");
	ShowVect(aM, "aM");
	ShowVect(DC, "DC");
	ShowVect(CM, "CM");

	ShowVect(eBM, "eBM");
	ShowVect(eAB, "eAB");
	ShowVect(eaM, "eaM");
	ShowVect(eDC, "eDC");



	double det1 = (-eBM.x*eAB.y+eBM.y*eAB.x);

	double lambda = (-aM.x*eAB.y + aM.y*eAB.x)/det1;

	MyShowFloat(det1, "det1");
	MyShowFloat(lambda, "lambda");

	Vect temp1 = eBM;
	temp1.Scale(lambda);
	Vect b = Vect::Add(M, temp1);
	ShowVect(b, "b");

	double det2 = (-eaM.x*eDC.y + eaM.y*eDC.x);
	double alpha = (-CM.x*eDC.y+CM.y*eDC.x)/det2;

	MyShowFloat(det2, "det2");
	MyShowFloat(alpha, "alpha");

	Vect temp2 = eaM;
	temp2.Scale(alpha);

	Vect d = Vect::Add(M, temp2);
	ShowVect(d, "d");


/*
	a.y = -a.y;
	b.y = -b.y;
	d.y = -d.y;
*/


	CPoint pa = newpos;
	CPoint pb = b.MakePoint();
	CPoint pc = C.MakePoint();
	CPoint pd = d.MakePoint();

	NewAdjustCone(P[iA].GetPos(1,1), P[iB].GetPos(1,1), P[iC].GetPos(1,1), P[iD].GetPos(1,1), pa, pb, pc, pd);

	P[iA].SetPos(pa);
	P[iB].SetPos(pb);
	P[iC].SetPos(pc);
	P[iD].SetPos(pd);

/*
	if (!AdjustConus(iA, iB, iC, iD)){
		P[iA].SetPos(A.MakePoint());
		P[iB].SetPos(B.MakePoint());
		P[iC].SetPos(C.MakePoint());
		P[iD].SetPos(D.MakePoint());
	}
*/
}


void AdjustParalell(CPoint & pb, CPoint pa, CPoint pB, CPoint pA){
	Vect A = Vect(pA); 
	Vect B = Vect(pB); 
	Vect a = Vect(pa);	
	Vect b = Vect(pb);	

	Vect AB = Vect::Sub(A, B);
	Vect eAB = AB;
	eAB.Normalize();

	int itest=1;

	int minx=-itest;
	int miny=-itest;
	double MinD = 1e300;
	for (int ix=-itest; ix<(itest+1); ix++){
		for (int iy=-itest; iy<(itest+1); iy++){
			Vect bb=b;
			bb.x += ix;
			bb.y += iy;
			Vect diff = Vect::Sub(a, bb);
			diff.Normalize();
			diff = Vect::Sub(diff, eAB);
			double l=diff.Length();
			if (l<MinD){
				minx=ix;
				miny=iy;
				MinD=l;
			}

		}
	}
	pb.x += minx;
	pb.y += miny;
}

void AdjustMidPoints(CPoint & pd, CPoint pC, CPoint pA, CPoint pB){
	Vect A = Vect(pA); 
	Vect B = Vect(pB); 
	Vect C = Vect(pC);	
	Vect d = Vect(pd);	

	Vect M1 = Vect::Add(A, B);
	M1.Scale(0.5);

	Vect AB = Vect::Sub(A, B);
	Vect eAB = AB;
	eAB.Normalize();



	int itest=2;

	int minx=-itest;
	int miny=-itest;
	double MinD = 1e300;

	for (int ix=-itest; ix<(itest+1); ix++){
		for (int iy=-itest; iy<(itest+1); iy++){
			Vect dd=d;
			dd.x += ix;
			dd.y += iy;
			Vect M2 = Vect::Add(d, C);
			M2.Scale(0.5);
			Vect diff = Vect::Sub(M1, M2);
			diff.Normalize();

			double l=fabs(Vect::Dot(eAB, diff));
			if (l<MinD){
				minx=ix;
				miny=iy;
				MinD=l;
			}

		}
	}
	pd.x += minx;
	pd.y += miny;


}


void AreaObject::NewAdjustCone(CPoint pA, CPoint pB ,CPoint pC, CPoint  pD, CPoint & pa, CPoint & pb, CPoint & pc, CPoint & pd) {
	Vect A = Vect(pA); 
	Vect B = Vect(pB); 
	Vect C = Vect(pC); 
	Vect D = Vect(pD); 
	Vect a = Vect(pa);	
	Vect b = Vect(pb);	
	Vect d = Vect(pd);	

	
//	AdjustParalell(pb, pa, pB, pA);
//	AdjustParalell(pd, pC, pD, pC);
	AdjustParalell(pa, pb, pd, pC);

//	AdjustMidPoints(pd, pC, pa, pb);


}

void AreaObject::OldSizeConus(int index, CPoint newpos){
	
	int i0, i1, i2, i3;

	// i3: the one moved; i0= the one on the same side
	// i1: opposite
	i3=index;

	switch (i3){
		case 0 : i1 = 2; i0= 3; i2 = 1; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
		case 2 : i1 = 0; i0= 1; i2 = 3; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}

	for (int jj=0; jj<4; jj++){
		if (jj!=i3){
			if (newpos.x==P[jj].Getx(1,1)){
				if (newpos.y==P[jj].Gety(1,1)){
					return;
				}
			}
		}
	}

	Vect A = Vect(P[i0].GetPos(1,1));
	Vect B = Vect(P[i1].GetPos(1,1));
	Vect C = Vect(P[i2].GetPos(1,1));
	Vect D = Vect(P[i3].GetPos(1,1));
	Vect d = Vect(newpos);


	Vect BA = Vect::Sub(B, A);
	Vect Cd = Vect::Sub(C, d );

	double lBA = BA.Length();
	double lCd = Cd.Length();

	if (fabs(lBA-lCd)<0.3){
		//Alert0("return");
		return;
	}

	Vect dA = Vect::Sub(d, A );
//	Vect CD = Vect::Sub(C, D);

	Vect eBA = Vect::Normalize(BA);

	double pdA = Vect::Dot(dA, eBA);
	if (fabs(pdA)<0.2){
		return;
	}

//	c = d - DC/2 - eBA*pDd

	Vect temp = Vect::Add(d, BA);

	Vect add = eBA;
	add.Scale(-2.0*pdA);
//	add.Scale(-pDd);

	Vect c = Vect::Add(temp, add);

	Vect cd =Vect::Sub(c, d);

	if (fabs(cd.Length()-lBA)<0.01){
		return;
	}

	P[i2].SetPos(c.MakePoint());
	P[i3].SetPos(newpos);

	if (!AdjustConus(i0, i1, i2, i3)){
		P[i0].SetPos(A.MakePoint());
		P[i1].SetPos(B.MakePoint());
		P[i2].SetPos(C.MakePoint());
		P[i3].SetPos(D.MakePoint());
	}

//	AdjustConusIndexes();

}

bool AreaObject::AdjustConus(int i0, int i1, int i2, int i3){
		// adjust i2 and i3 such that they are parallel to i0,i1
	Vect A = Vect(P[i0].GetPos(1,1));
	Vect B = Vect(P[i1].GetPos(1,1));
	Vect C = Vect(P[i2].GetPos(1,1));
	Vect D = Vect(P[i3].GetPos(1,1));

	Vect BA = Vect::Sub(B, A);
	double lBA = BA.Length();
	Vect DC = Vect::Sub(D, C);
	double lDC = DC.Length();

	if (fabs(lBA-lDC)<0.9){
		return false;
	}

	if (lDC<lBA){
		P[i0].SetPos(C.MakePoint());
		P[i1].SetPos(D.MakePoint());
		P[i2].SetPos(A.MakePoint());
		P[i3].SetPos(B.MakePoint());
		return AdjustConus(i0, i1, i2, i3);
	}


	double Min = 1e300;
	Vect MinC = C;
	Vect MinD = D;

	for (int icx=-1; icx<2; icx++){
		for (int icy=-1; icy<2; icy++){
			Vect CC = Vect::Add(C, Vect(icx, icy));
			for (int idx=-1; idx<2; idx++){
				for (int idy=-1; idy<2; idy++){
					Vect DD = Vect::Add(D, Vect(idx, idy));
					Vect CD = Vect::Sub(CC, DD);
					double DELTA = lBA*CD.Length()/Vect::Dot(BA, CD);
					if (DELTA <Min){
						Min = DELTA;
//						ShowFloat(Min, "Min");
						MinC = CC;
						MinD = DD;
					}
				}
			}
		}
	}



	P[i2].SetPos(MinC.MakePoint());
	P[i3].SetPos(MinD.MakePoint());


	return true;
}


void AreaObject::AdjustConusIndexes(){

	Vect A = Vect(P[0].GetPos(1,1));
	Vect B = Vect(P[1].GetPos(1,1));
	Vect C = Vect(P[2].GetPos(1,1));
	Vect D = Vect(P[3].GetPos(1,1));

	Vect BA = Vect::Sub(B, A);
	double lBA = BA.Length();

	Vect CD = Vect::Sub(C, D);
	double lCD = CD.Length();

	if (lCD<lBA){
		P[0].SetPos(C.MakePoint());
		P[1].SetPos(D.MakePoint());
		P[2].SetPos(A.MakePoint());
		P[3].SetPos(B.MakePoint());
	}


}



void AreaObject::SetChanged(){

	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].SetPosWithUndo();
	}
	this->CalcGeneratorList();


}


bool AreaObject::FixMouse(){
	return DoFixMouse;
}
void AreaObject::RotatePoint(int index, CPoint newpos){
	// recalculates the position of all other points based on the new coordinates of 
	// this point
	if (index<0){
		return;
	}
	if (index>=NPOINTSAREAOBJECT){
		return;
	}

	if (P[index].Getx(1,1) == newpos.x ){
		if (P[index].Gety(1,1) == newpos.y ){
			return ; // Old pos = new pos
		}
	}
	switch (ObjectType){
		case 0 : RotateRectangle(index, newpos); DoFixMouse = true; break;// Rectangle
		case 1 : RotateConus(index, newpos); DoFixMouse = false; break;// Conus
		default :  RotateRectangle(index, newpos); DoFixMouse = true; break;// Rectangle;
	}

}

Vect AreaObject::GetMidPoint(){
	if (ObjectType == 1){ // Conus
		double r1, r2;
		Vect V;
		GetMidPointAndRadius(V, r1, r2);
		return V;
	}
	Vect A = Vect(P[0].GetPos(1,1));
	Vect C = Vect(P[2].GetPos(1,1));
	Vect M = Vect::Add(A, C);
	M.Scale(0.5);
	return M;
}

void AreaObject::RotateRectangle(int index, CPoint newpos){
	// The diagonal point remains fixed, while the other two change in dependence of the 
	// new coordinates
	// 0 - 2 and 1 - 3 are diagonal pairs

	NewNewRotateRectangle(index, newpos);
	return;

	int i0, i1, i2, i3;

	// i3: the one moved; i1= the one fixed; i0, i2, the ones to be calculated;

	i3=index;

	switch (i3){
		case 0 : i1 = 2; i0= 1; i2 = 3; break;
//		case 1 : i1 = 3; i0= 0; i2 = 2; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
//		case 2 : i1 = 0; i0= 3; i2 = 1; break;
		case 2 : i1 = 0; i0= 3; i2 = 1; break;
//		case 3 : i1 = 1; i0= 2; i2 = 0; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}
	switch (i3){
		case 0 : i1 = 2; i0= 3; i2 = 1; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
		case 2 : i1 = 0; i0= 3; i2 = 1; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}
	
	Vect A = Vect(P[i0].GetPos(1,1)); // moved
	Vect B = Vect(P[i1].GetPos(1,1)); // fixed
	Vect C = Vect(P[i2].GetPos(1,1)); // moved
	Vect D = Vect(P[i3].GetPos(1,1)); // guide
	Vect NewD = Vect(newpos);

	Vect DB = Vect::Sub(D, B);
	Vect AB = Vect::Sub(A, B);
	Vect CB = Vect::Sub(C, B);

	Vect NewDB = Vect::Sub(NewD, B);

	Vect uDB = Vect::Normalize(DB);
	Vect udB = Vect::Normalize(NewDB);

	double cosphi = 1.0 - 10.0*(1.0-Vect::Dot(uDB, udB));
	if (fabs(cosphi)>1.0){
		cosphi = 0.5;
	}

	double sinphi = sqrt(1.0-cosphi*cosphi);
//	double sinphi = -Vect::Dot(uDB, udB);
//	double cosphi = sqrt(1.0-cosphi*cosphi);
	if ((uDB.x*udB.y-uDB.y*udB.x)>0.0){
		sinphi = - sinphi;
		//cosphi = - cosphi;
	}



	double dx = DB.x*cosphi + DB.y*sinphi;
	double dy = -DB.x*sinphi + DB.y*cosphi;

	double ax = AB.x*cosphi + AB.y*sinphi;
	double ay = -AB.x*sinphi + AB.y*cosphi;

	double cx = CB.x*cosphi + CB.y*sinphi;
	double cy = -CB.x*sinphi + CB.y*cosphi;

	
	Vect aB = Vect(ax, ay) ;
	Vect cB = Vect(cx, cy) ;
	Vect dB = Vect(dx, dy) ;

	aB.Scale(AB.Length()/aB.Length());
	cB.Scale(CB.Length()/cB.Length());
	dB.Scale(DB.Length()/dB.Length());

	P[i3].SetPos( Vect::Add(B, dB).MakePoint());
	P[i0].SetPos( Vect::Add(B, aB).MakePoint());
	P[i2].SetPos( Vect::Add(B, cB).MakePoint());

	AdjustI2(i1, i2, i3, i0);

}
void AreaObject::NewRotateRectangle(int index, CPoint newpos){
	// The diagonal point remains fixed, while the other two change in dependence of the 
	// new coordinates
	// 0 - 2 and 1 - 3 are diagonal pairs

	int i0, i1, i2, i3;

	// i3: the one moved; i1= the one fixed; i0, i2, the ones to be calculated;

	i3=index;

	switch (i3){
		case 0 : i1 = 2; i0= 3; i2 = 1; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
		case 2 : i1 = 0; i0= 3; i2 = 1; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}
	Vect A = Vect(P[i0].GetPos(1,1)); // moved
	Vect B = Vect(P[i1].GetPos(1,1)); // fixed
	Vect C = Vect(P[i2].GetPos(1,1)); // moved
	Vect D = Vect(P[i3].GetPos(1,1)); // guide

	Vect DPrime = Vect(newpos);

	Vect OldDiagonal = Vect::Sub(D, B);
	double LengthOldDiagonal = Vect::Length(OldDiagonal);

	Vect NewDiagonal = Vect::Sub(DPrime, B);
	double LengthNewDiagonal = Vect::Length(NewDiagonal);

	Vect NewVertex = Vect::Add(B, Vect::ScaleVect(LengthOldDiagonal/LengthNewDiagonal, NewDiagonal));
	
////XXXXXXXXXXX

	Vect FixedPoint = Vect(P[i1].GetPos(1,1));
	Vect PreviousMovedPoint = Vect(P[i3].GetPos(1,1));
	Vect MovedPoint = Vect(newpos);

	Vect OldDirection = Vect::Sub(PreviousMovedPoint, FixedPoint);
	Vect NewDirection = Vect::Sub(MovedPoint, FixedPoint);
	Vect UnitNewDirection = NewDirection;
	UnitNewDirection.Normalize();
	UnitNewDirection.Scale(OldDirection.Length());

	Vect NewNewD = Vect::Add(FixedPoint, UnitNewDirection);

	Vect VerticalVect = Vect::UnitVertVect(UnitNewDirection);
	VerticalVect.Scale(Vect::Length(Vect::Sub(A, B)));

	Vect TryNewA = Vect::Add(FixedPoint, VerticalVect);
	Vect TryNewC = Vect::Sub(NewNewD, VerticalVect);


	Vect OldDiffAB = Vect::Sub(A, B);
	Vect OldDiffCB = Vect::Sub(C, B);

	Vect NewDiffAB = Vect::Sub(TryNewA, B);
	Vect NewDiffCB = Vect::Sub(TryNewC, B);

	double OldCross = Vect::Cross(OldDiffAB, OldDiffCB);
	double NewCross = Vect::Cross(NewDiffAB, NewDiffCB);
	if (OldCross*NewCross < 0.0){
		TryNewA = Vect::Sub(FixedPoint, VerticalVect);
		TryNewC = Vect::Add(NewNewD, VerticalVect);
	}

	P[i3].SetPos( NewNewD.MakePoint());
	P[i0].SetPos( TryNewA.MakePoint());
	P[i2].SetPos( TryNewC.MakePoint());
//	AdjustI2(i1, i2, i3, i0);
	
//	AdjustI2(i1, i3, i2, i0);
//	AdjustI2(i0, i1, i2, i3);
//	AdjustI2(i1, i0, i2, i3);
	return;


//	double angle = Vect::GetAngle(OldDirection, NewDirection);

//	double newcosphi=cos(angle);
//	double newsinphi = sin(angle);




	Vect NewD = Vect(newpos);

	Vect DB = Vect::Sub(D, B);
	Vect AB = Vect::Sub(A, B);
	Vect CB = Vect::Sub(C, B);

	Vect NewDB = Vect::Sub(NewD, B);

	Vect uDB = Vect::Normalize(DB);
	Vect udB = Vect::Normalize(NewDB);

	double cosphi = 1.0 - 10.0*(1.0-Vect::Dot(uDB, udB));
	if (fabs(cosphi)>1.0){
		cosphi = 0.5;
	}

	double sinphi = sqrt(1.0-cosphi*cosphi);
//	double sinphi = -Vect::Dot(uDB, udB);
//	double cosphi = sqrt(1.0-cosphi*cosphi);
	if ((uDB.x*udB.y-uDB.y*udB.x)>0.0){
		sinphi = - sinphi;
		//cosphi = - cosphi;
	}



	double dx = DB.x*cosphi + DB.y*sinphi;
	double dy = -DB.x*sinphi + DB.y*cosphi;

	double ax = AB.x*cosphi + AB.y*sinphi;
	double ay = -AB.x*sinphi + AB.y*cosphi;

	double cx = CB.x*cosphi + CB.y*sinphi;
	double cy = -CB.x*sinphi + CB.y*cosphi;

	
	Vect aB = Vect(ax, ay) ;
	Vect cB = Vect(cx, cy) ;
	Vect dB = Vect(dx, dy) ;

	aB.Scale(AB.Length()/aB.Length());
	cB.Scale(CB.Length()/cB.Length());
	dB.Scale(DB.Length()/dB.Length());

	P[i3].SetPos( Vect::Add(B, dB).MakePoint());
	P[i0].SetPos( Vect::Add(B, aB).MakePoint());
	P[i2].SetPos( Vect::Add(B, cB).MakePoint());

	AdjustI2(i1, i2, i3, i0);
}
void AreaObject::LastOldRotateConus(int index, CPoint newpos){
//void AreaObject::RotateConus(int index, CPoint newpos){
	
	int i0, i1, i2, i3;

	// i3: the one moved; i0= the one on the same side
	// i1: opposite
	i3=index;

	switch (i3){
		case 0 : i1 = 2; i0= 3; i2 = 1; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
		case 2 : i1 = 0; i0= 1; i2 = 3; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}

	for (int jj=0; jj<4; jj++){
		if (jj!=i3){
			if (newpos.x==P[jj].Getx(1,1)){
				if (newpos.y==P[jj].Gety(1,1)){
					return;
				}
			}
		}
	}

	Vect A = Vect(P[i0].GetPos(1,1));
	Vect B = Vect(P[i1].GetPos(1,1));
	Vect C = Vect(P[i2].GetPos(1,1));
	Vect D = Vect(P[i3].GetPos(1,1));
	Vect F = Vect(newpos);


	Vect d = Vect::Sub(D, B);
	Vect FB = Vect::Sub(F, B);

	Vect eDB = Vect::Normalize(d);
	Vect eFB = Vect::Normalize(FB);

	double cosphi = 1.0 - 10.0*(1.0-Vect::Dot(eDB, eFB));
	if (fabs(cosphi)>1.0){
		cosphi = 0.5;
	}

	double sinphi = -sqrt(1.0 -  cosphi*cosphi);
	if ((eDB.x*eFB.y-eDB.y*eFB.x)<0.0){
		sinphi = -sinphi;
	}
	Vect a = Vect::Sub(A, B);
	Vect c = Vect::Sub(C, B);
	
	double Ax=B.x+a.x*cosphi+a.y*sinphi;
	double Ay=B.y-a.x*sinphi+a.y*cosphi;
	A = Vect(Ax, Ay);


	double Cx=B.x+c.x*cosphi+c.y*sinphi;
	double Cy=B.y-c.x*sinphi+c.y*cosphi;
	C = Vect(Cx, Cy);

	double Dx=B.x+d.x*cosphi+d.y*sinphi;
	double Dy=B.y-d.x*sinphi+d.y*cosphi;
	D = Vect(Dx,Dy);
	

	P[i0].SetPos(A.MakePoint());
	P[i1].SetPos(B.MakePoint());
	P[i2].SetPos(C.MakePoint());
	P[i3].SetPos(D.MakePoint());

//	SizeConus(i1, P[i1].GetPos(1,1));
	SizeConus(i3, P[i3].GetPos(1,1));


}


double AreaObject::Length(){
	switch (ObjectType){
		case 0 : return LengthRectangle(); break;// Rectangle
		case 1 : return LengthConus(); break;// Conus
		default :  return LengthRectangle(); break;// Rectangle;
	}
	
}
double AreaObject::LengthRectangle(){
	Vect A = Vect(P[0].GetPos(1,1));
	Vect D = Vect(P[3].GetPos(1,1));
	Vect diff = Vect::Sub(A, D);
	return diff.Length();

}
double AreaObject::LengthConus(){
	Vect A = Vect(P[0].GetPos(1,1));
	Vect D = Vect(P[3].GetPos(1,1));
	Vect diff = Vect::Sub(A, D);
	return diff.Length();
}

int AreaObject::GetRelevantIndex(CPoint p){
	switch (ObjectType){
		case 0 : return GetIndexRectangle(p); break;// Rectangle
		case 1 : return GetIndexConus(p); break;// Conus
		default :  return GetIndexRectangle(p); break;// Rectangle;
	}
}

int AreaObject::GetIndexRectangle(CPoint p){
	Vect A = Vect(P[0].GetPos(1,1));
	Vect D = Vect(P[3].GetPos(1,1));
	Vect DA = Vect::Sub(D, A);
	DA.Normalize();
	Vect pA = Vect::Sub(p, A);
	return Vect::Dot(pA, DA);
}

int AreaObject::GetIndexConus(CPoint p){
	Vect M;
	double r1, r2;

	if (!GetMidPointAndRadius( M, r1,r2)){
		return 0;
	}

	Vect PM = Vect::Sub(p, M );

	return (PM.Length()-r1);
}


bool AreaObject::PointIsInside(CPoint p){
	switch (ObjectType){
		case 0 : return PointIsInsideRectangle(p); break;// Rectangle
		case 1 : return PointIsInsideConus(p); break;// Conus
		default :  return PointIsInsideRectangle(p); break;// Rectangle;
	}

}

bool AreaObject::PointIsInsideRectangle(CPoint p){
	Vect A = Vect(P[0].GetPos(1,1));
	Vect D = Vect(P[3].GetPos(1,1));
	Vect DA = Vect::Sub(D, A);
	double lDA = DA.Length();
	Vect eDA = DA;
	eDA.Normalize();

	Vect M = GetMidPoint();

	Vect pM = Vect::Sub(p, M);

	if (fabs(Vect::Dot(pM, eDA)*2.0)>lDA){
		return false;
	}

	Vect B = Vect(P[1].GetPos(1,1));
	Vect AB = Vect::Sub(B, A);
	double lAB = AB.Length();
	Vect eAB = AB;
	eAB.Normalize();

	if (fabs(Vect::Dot(pM, eAB)*2.0)>lAB){
		return false;
	}

	return true;

}

bool AreaObject::PointIsInsideConus(CPoint p){
	Vect M;
	double r1, r2;

	if (!GetMidPointAndRadius( M, r1,r2)){
		return false;
	}

	Vect PM = Vect::Sub(p, M);

	double r = PM.Length();

	if (r<r1) return false;
	if (r>r2) return false;

	Vect A = Vect(P[0].GetPos(1,1));
	Vect B = Vect(P[1].GetPos(1,1));

	Vect AM = Vect::Sub(A, M);
	Vect BM = Vect::Sub(B, M);


	double cross = AM.x*BM.y-AM.y*BM.x;
	double crossA = AM.x*PM.y-AM.y*PM.x;
	double crossB = BM.x*PM.y-BM.y*PM.x;
//	ShowFloat(cross, " cross");
//	ShowFloat(crossA, " crossA");
//	ShowFloat(crossB, " crossB");

	if (cross>0){
		if (crossA<0){
			return false;
		}
		if (crossB>0){
			return false;
		}
		return true;
	}
	else{
		if (crossA>0){
			return true;
		}
		else{
			if (crossB<0){
				return true;
			}
			else{
				return false;
			}
		}
	}
}

bool AreaObject::GeneratePointsOld(bool & initialize, CPoint & p, int & index){
	switch (ObjectType){
		case 0 : return GeneratePointsRectangle(initialize, p, index);// Rectangle
		case 1 : return GeneratePointsConus(initialize, p, index);// Conus
		default :  return GeneratePointsRectangle(initialize, p, index); // Rectangle;
	}
}

bool AreaObject::GeneratePoints(bool & initialize, CPoint & p, int & index){
	static int iter;

//	return GeneratePointsOld(initialize,  p, index);


	if (initialize){
		iter = 0;
		initialize = false;
	}
//	if (iter>=(SizeGeneratorList)){
	if (iter>=(pointlist.GetSize())){
		return false;
	}
	p = pointlist.buff[iter].p;
	index = pointlist.buff[iter].index;
//	ShowFloat(index, "index");
	iter++;
	return true;
}

bool AreaObject::GeneratePointsRectangle(bool & initialize, CPoint & p, int & index){
	static double lDA;
	static Vect A;
	static Vect eDA;
	static Vect M;
	static Vect eAB;
	static double lAB;

	static CRect rect;

	static int ix;
	static int iy;

	if (!RectangleGeneratorInitialized){
		if (!initialize){
			return false;
		}
	}
	if (initialize){
		A = Vect(P[0].GetPos(1,1));
		Vect D = Vect(P[3].GetPos(1,1));
		Vect DA = Vect::Sub(D, A);
		lDA = DA.Length()/2.0;
		eDA = DA;
		eDA.Normalize();

		M = GetMidPoint();
		Vect B = Vect(P[1].GetPos(1,1));
		Vect AB = Vect::Sub(B, A);
		lAB = AB.Length()/2.0;
		eAB = AB;
		eAB.Normalize();

		int Minx=P[0].Getx(1,1);
		int Maxx=Minx;
		int Miny=P[0].Gety(1,1);
		int Maxy=Miny;
		for (int i=1; i<4; i++){
			int x=P[i].Getx(1,1);
			int y=P[i].Gety(1,1);
			if (x<Minx) Minx=x;
			if (x>Maxx) Maxx=x;
			if (y<Miny) Miny=y;
			if (y>Maxy) Maxy=y;
		}
		rect.left = Minx;
		rect.right = Maxx;
		rect.top = Miny;
		rect.bottom = Maxy;

		if (!CheckRect(rect)){
			return false;
		}

		ix=rect.left-1;
		iy=rect.top;

		RectangleGeneratorInitialized = true;
		initialize = false;
	}
	while (true){
		ix++;
		if (ix>rect.right){
			ix=rect.left;
			iy++;
			if (iy>rect.bottom){
				return false;
			}
		}
		p=CPoint(ix, iy);
		Vect pM = Vect::Sub(p, M);
		if (fabs(Vect::Dot(pM, eDA))<=lDA){
			if (fabs(Vect::Dot(pM, eAB))<=lAB){
				Vect pA = Vect::Sub(p, A);
				index =  Vect::Dot(pA, eDA);
				return true;
			}
		}
	}
}

bool AreaObject::CheckRect(CRect & rect){
		if (rect.left>=XSIZE){
			return false;
		}
		if (rect.right<0){
			return false;
		}
		if (rect.top>=YSIZE){
			return false;
		}
		if (rect.bottom<0){
			return false;
		}

		if (rect.left<0){
			rect.left = 0;
		}
		if (rect.right>=XSIZE){
			rect.right = XSIZE-1;
		}
		if (rect.top<0){
			rect.top=0;
		}
		if (rect.bottom>=YSIZE){
			rect.bottom = YSIZE-1;
		}

		if (rect.left>rect.right){
			rect.left = rect.right;
		}
		if (rect.top>rect.bottom){
			rect.top = rect.bottom;
		}
		return true;
}


bool AreaObject::GeneratePointsConus(bool & initialize, CPoint & p, int & index){
	static Vect M;
	static Vect AM;
	static Vect BM;
	static double r1square;
	static double r2square;
	static double r1;
//	static double r2;
	static CRect rect;
	static double cross;
	static int ix;
	static int iy;

	if (!ConusGeneratorInitialized){
		if (!initialize){
			return false;
		}
	}
	if (initialize){
		double r2;
		if (!GetMidPointAndRadius( M, r1,r2)){
			return false;
		}
		r1square = r1*r1;
		r2square = r2*r2;

		Vect A = Vect(P[0].GetPos(1,1));
		Vect B = Vect(P[1].GetPos(1,1));

		AM = Vect::Sub(A, M);
		BM = Vect::Sub(B, M);
		cross = AM.x*BM.y-AM.y*BM.x;


		rect.top = M.y - r2;
		rect.bottom = M.y + r2;
		rect.left = M.x - r2;
		rect.right = M.x + r2;


		if (!CheckRect(rect)){
			return false;
		}
		ix=rect.left-1;
		iy=rect.top;

		ConusGeneratorInitialized = true;
		initialize = false;
	}
	while (true){
//		NTotal++;
		ix++;
		if (ix>=rect.right){
			ix=rect.left;
			iy++;
			if (iy>=rect.bottom){
				return false;
			}
		}
		p = CPoint(ix, iy);
		Vect PM = Vect::Sub(p, M);
		double rsquare = PM.SquareLength();

		bool IsOk = true;

		if (rsquare<r1square){
			IsOk = false;
		}
		else{
			if (rsquare>r2square){
				IsOk = false;
			}
			else{
	
				double crossA = AM.x*PM.y-AM.y*PM.x;

				if (cross>0){
					if (crossA<0){
						IsOk =  false;
					}
					else{
//						double crossB = BM.x*PM.y-BM.y*PM.x;
//						if (crossB>0){
						if (BM.x*PM.y>BM.y*PM.x){
							IsOk =  false;
						}
					}
				}
				else{
					if (crossA<=0){
//						double crossB = BM.x*PM.y-BM.y*PM.x;
//						if (crossB>=0){
						if (BM.x*PM.y>=BM.y*PM.x){
							IsOk =  false;
						}
					}
				}
			}
		}

		if (IsOk){
			index = PM.Length()-r1;
//			NPlus++;
			return true;
		}

	}

}


void AreaObject::StartRotate(int index){
	if (ObjectType==1) {  // Conus
		StartRotateConus(index);
	}

}
void AreaObject::StartSize(int index){
	if (ObjectType==1) {  // Conus
		StartSizeConus(index);
	}
}

double AreaObject::GetConusAngle(){

	Vect M;
	double r1, r2;

	this->GetMidPointAndRadius(M, r1, r2);
	Vect MB = Vect::Sub(P[2].MakeVect(), M);
	Vect MC = Vect::Sub(P[3].MakeVect(), M);
	return Vect::GetAngle(MB, MC);

}
void AreaObject::StartRotateConus(int index){
	SaveRotateSizeIndex = index;

	Vect M;

	this->GetMidPointAndRadius(M, SaveRotater1, SaveRotater2);
	SaveRotateMidPoint.SetPos(M.MakePoint());
	int iA, iB, iC, iD;

	switch (index){
		case 0 : iB=0; iC=3; iD=2; iA = 1; break;
		case 1 : iB=1; iC=2; iD=3; iA = 0; break;
		case 2 : iB=2; iC=1; iD=0; iA = 3; break;
		case 3 : iB=3; iC=0; iD=1; iA = 2; break;
		default : return;
	}

	Vect A = P[iA].MakeVect();
	Vect B = P[iB].MakeVect();
	Vect C = P[iC].MakeVect();
	Vect D = P[iD].MakeVect();

	Vect AM = Vect::Sub(A, M);
	Vect BM = Vect::Sub(B, M);
	Vect CM = Vect::Sub(C, M);
	Vect DM = Vect::Sub(D, M);

	SaveRotatAlpha1 = Vect::GetAngle(CM);
	SaveRotatAlpha2 = Vect::GetAngle(DM);

//	ShowFloat(M.x, "Mx");
//	ShowFloat(M.y, "My");
//	ShowFloat(SaveRotater1, "SaveRotater1");
//	ShowFloat(SaveRotater2, "SaveRotater2");
	
	SaveRotatealpha = GetConusAngle();
//	ShowFloat(SaveRotatealpha*360.0/2.0/3.1415, "alpah");

	switch (index){
		case 0 : SaveRotatePoint = this->P[2]; break;
		case 1 : SaveRotatePoint = this->P[3]; break;
		case 2 : SaveRotatePoint = this->P[0]; break;
		case 3 : SaveRotatePoint = this->P[1]; break;
		default: Alert0("ERROR IN CONUS START ROTATE"); break;
	}

//	ShowFloat(P[2].Getx(), "rx");

//	ShowFloat(P[2].Gety(), "ry");
//	ShowFloat(SaveRotatePoint.Getx(), "p.x");
//	ShowFloat(SaveRotatePoint.Gety(), "p.y");

}

void AreaObject::StartSizeConus(int index){
	SaveRotateSizeIndex = index;

	IsInnerRadiusOnSize = true;

	if ((index==0) || (index==1)){
		IsInnerRadiusOnSize = false;
	}

	double r1, r2;
	Vect M;

	this->GetMidPointAndRadius(M, r1, r2);

	SaveSizeMidPoint.SetPos(M.MakePoint());

	if (IsInnerRadiusOnSize){
		SaveSizeri = r1;
	}
	else{
		SaveSizeri = r2;
	}

	switch (index){
		case 0 : SaveSizePoint = this->P[2]; break;
		case 1 : SaveSizePoint = this->P[3]; break;
		case 2 : SaveSizePoint = this->P[0]; break;
		case 3 : SaveSizePoint = this->P[1]; break;
		default: Alert0("ERROR IN CONUS START SIZE"); break;
	}


}

void AreaObject::RotateConus(int index, CPoint newpos){

	Vect NewB(newpos);
	Vect M = SaveRotateMidPoint.MakeVect();

	Vect NewBM = Vect::Sub(NewB, M);

	if (NewBM.Length()<1.0){
		return;
	}

	int iA, iB, iC, iD;
	double r1, r2;

	switch (index){
		case 0 : iB=0; iC=3; iD=2; iA = 1; 	r1 = SaveRotater1; r2 = SaveRotater2; break;
		case 1 : iB=1; iC=2; iD=3; iA = 0; 	r1 = SaveRotater1; r2 = SaveRotater2; break;
		case 2 : iB=2; iC=1; iD=0; iA = 3; 	r1 = SaveRotater2; r2 = SaveRotater1; break;
		case 3 : iB=3; iC=0; iD=1; iA = 2; 	r1 = SaveRotater2; r2 = SaveRotater1; break;
		default : return;
	}

	double alpha1 = Vect::GetAngle(NewBM);
	double alpha2 = alpha1-SaveRotatAlpha1+SaveRotatAlpha2;

	double s1 = sin(alpha1); double c1 = cos(alpha1);
	double s2 = sin(alpha2); double c2 = cos(alpha2);


	P[iA].SetPos(Vect::Add(M, Vect(r1*c2, r1*s2)).MakePoint());
	P[iB].SetPos(Vect::Add(M, Vect(r1*c1, r1*s1)).MakePoint());
	P[iC].SetPos(Vect::Add(M, Vect(r2*c1, r2*s1)).MakePoint());
	P[iD].SetPos(Vect::Add(M, Vect(r2*c2, r2*s2)).MakePoint());
	
}

void AreaObject::LastNewRotateConus(int index, CPoint newpos){

	CDC * pDC = AfxGetMainWnd()->GetDC();

	if (SaveRotateSizeIndex != index){
		Alert0("ERROR");
		return;
	}
//	ShowFloat(index, "index");

	// We know one point, the opposite new point, the angle and the two radii
		

	// Let P be the known point; Q the new point

//	double a = newpos.x - SaveRotatePoint.Getx();
//	double b = newpos.y - SaveRotatePoint.Gety();

	double r1, r2;

	char s[20];

	_gcvt(newpos.x, 6, s);
	CString c = "nx "+CString(s);
	_gcvt(newpos.y, 6, s);
	c += CString(" ny ")+CString(s);
	pDC->TextOut(100,100, c);

	switch (index) {
		case 0 : r1= SaveRotater2; r2 = SaveRotater1;break;
		case 1 : r1= SaveRotater2; r2 = SaveRotater1;break;
		case 2 : r1= SaveRotater1; r2 = SaveRotater2;break;
		case 3 : r1= SaveRotater1; r2 = SaveRotater2;break;
		default: Alert0("ERROR IN CONUS ROTATE"); break;
	}

//	ShowFloat(r1, "r1");
//	ShowFloat(r2, "r2");

	Vect p = Vect( SaveRotatePoint.Getx(1,1),  SaveRotatePoint.Gety(1,1));
//	ShowFloat(p.x, "p.x");
//	ShowFloat(p.y, "p.y");

//	Vect q = Vect( P[index].Getx(), P[index].Gety());
	Vect q = Vect( newpos.x, newpos.y);
//	ShowFloat(q.x, "q.x");
//	ShowFloat(q.y, "q.y");

	double p2=p.SquareLength();
	double q2=q.SquareLength();

	double a=(r2*r2-r1*r1+p2-q2)/2/(p.y-q.y);
	double b=(q.x-p.x)/(p.y-q.y);    // My = a + b*Mx

	double a0=1.0 + b*b;
	double a1=2.0*a*b-2.0*p.x-2.0*b*p.y;
	double a2 = p2+a*a-r1*r1-2.0*p.y*a;

	double p0=-a1/a0/2.0;
	double q0=a2/a0;

	double rootsquare=p0*p0-q0;
	if (rootsquare<0){
		Alert0("rootsqaure <0");
		return;
	}
	double rroot = sqrt(rootsquare);

	double Mxa=p0+rroot;
	double Mxb=p0-rroot;

	double Mya = a+b*Mxa;
	double Myb = a+b*Mxb;

	Vect VMa = Vect(Mxa, Mya);
	Vect VMb = Vect(Mxb, Myb);

	double anglea = Vect::GetAngle(Vect::Sub(p, VMa), Vect::Sub(q, VMa));
	double angleb = Vect::GetAngle(Vect::Sub(p, VMb), Vect::Sub(q, VMb));

	double Mx, My;
	if (fabs(anglea-SaveRotatealpha)<fabs(angleb-SaveRotatealpha)){
		Mx=Mxa;
		My=Mya;
	}
	else{
		Mx=Mxb;
		My=Myb;
	}


	Vect M = Vect(Mx, My);


	_gcvt(Mx, 6, s);
	c = "Mx "+CString(s);
	_gcvt(My, 6, s);
	c += CString(" My ")+CString(s);
	pDC->TextOut(100,140, c);

//	ShowFloat(M.x, "Mx");
//	ShowFloat(M.y, "My");


	Vect PM = Vect::Sub(p, M);
	PM.Scale(r2/r1);
	Vect R = Vect::Add(M, PM);

//	ShowFloat(PM.Length(), "PMle");
//	ShowFloat(r1, "r1");

	Vect QM = Vect::Sub(q, M);

	QM.Normalize();

	Vect NS = QM;

	NS.Scale(r1);

	Vect S = Vect::Add(M, NS);

	QM.Scale(r2);

	Vect nq=Vect::Add(M, QM);

//	ShowFloat(R.x, "Rx");
//	ShowFloat(R.y, "Ry");


	int i0, i1, i2, i3;

	switch (index) {
		case 0 : i0=1; i1=2; i2=3; i3=0; break;
		case 1 : i0=2; i1=3; i2=0; i3=1; break;
		case 2 : i0=3; i1=0; i2=1; i3=2; break;
		case 3 : i0=0; i1=1; i2=2; i3=3; break;
		default: Alert0("ERROR IN CONUS ROTATE"); break;
	}


	this->P[i1].SetPos(p.MakePoint());
	this->P[i2].SetPos(R.MakePoint());
	this->P[i3].SetPos(nq.MakePoint());
	this->P[i0].SetPos(S.MakePoint());

//	Beep(1000,10);
//	StartRotate(index);

}


void AreaObject::CalcGeneratorList(){
	switch (ObjectType){
		case 0 : CalcGeneratorListRectangle(); break;// Rectangle
		case 1 : CalcGeneratorListConus(); break;// Conus
		default :  CalcGeneratorListConus(); break;// Rectangle;
	}
}


void AreaObject::CalcGeneratorListRectangle(){
	int NPoints = 0;
	bool initialize = true;
	CPoint p;
	int index;
	while (GeneratePointsRectangle(initialize, p, index)){
		NPoints++;
	}
//	NPoints--;
//	ShowFloat(NPoints,"NP");
//	if (NPoints<1) NPoints=1;
//	Beep(2000,10);
//	UpdateSizeGeneratorList(NPoints);
	pointlist.UpdateBuffer(NPoints);
	if (NPoints<1){
		return;
	}
	initialize = true;
	NPoints = 0;
	while (GeneratePointsRectangle(initialize, p, index)){
		pointlist.buff[NPoints].index = index;
		pointlist.buff[NPoints].p = p;
		NPoints++;
	}
}



void AreaObject::CalcGeneratorListConus(){
	int NPoints = 0;
	bool initialize = true;
	CPoint p;
	int index;
	while (GeneratePointsConus(initialize, p, index)){
		NPoints++;
	}
//	NPoints--;
//	Beep(1000,10);
//	ShowFloat(NPoints,"NP");
//	if (NPoints<1) NPoints=1;
//	UpdateSizeGeneratorList(NPoints);
	pointlist.UpdateBuffer(NPoints);
	if (NPoints<1){
		return;
	}
	initialize = true;
	NPoints = 0;

	while (GeneratePointsConus(initialize, p, index)){
		pointlist.buff[NPoints].index = index;
		pointlist.buff[NPoints].p = p;
		NPoints++;
	}
}

/*
void AreaObject::DeallocPointList(){
	if (pointlist){
		delete [] pointlist;
		pointlist = NULL;
	}
}
	

void AreaObject::UpdateSizeGeneratorList(int length){
	if (length>BufferSizeGeneratorList){
		DeallocPointList();
		pointlist = new PointList[length+10];
		BufferSizeGeneratorList = length+10;
	}
	SizeGeneratorList = length;
}
*/


// Define assignment operator.
AreaObject &AreaObject::operator=( AreaObject &ptRHS )
{
    ObjectType = ptRHS.ObjectType;
	int i;
    for (i=0; i<NPOINTSAREAOBJECT; i++){
		P[i] = ptRHS.P[i];
	}
     XSIZE= ptRHS.XSIZE;
     YSIZE= ptRHS.YSIZE;
     DoFixMouse= ptRHS.DoFixMouse;
     SaveRotateSizeIndex= ptRHS.SaveRotateSizeIndex;
 
	 SaveRotater1= ptRHS.SaveRotater1;
     SaveRotater2= ptRHS.SaveRotater2;
     SaveRotatealpha= ptRHS.SaveRotatealpha;
     SaveRotatAlpha1= ptRHS.SaveRotatAlpha1;
     SaveRotatAlpha2= ptRHS.SaveRotatAlpha2;
     SaveRotatePoint= ptRHS.SaveRotatePoint;

	
     SaveRotateMidPoint= ptRHS.SaveRotateMidPoint;
     SaveSizeri= ptRHS.SaveSizeri;
     IsInnerRadiusOnSize= ptRHS.IsInnerRadiusOnSize;
     SaveSizePoint= ptRHS.SaveSizePoint;
     SaveSizeMidPoint= ptRHS.SaveSizeMidPoint;
//     SizeGeneratorList= ptRHS.SizeGeneratorList;
//     BufferSizeGeneratorList= ptRHS.BufferSizeGeneratorList;
//	BufferSizeGeneratorList = 0;
//	pointlist = 0;

	 pointlist = ptRHS.pointlist;

//	 pointlist.UpdateBuffer(ptRHS.pointlist.GetSize());
//	UpdateSizeGeneratorList(SizeGeneratorList);

//	if (SizeGeneratorList>0){
/*
	if (ptRHS.pointlist.GetSize()>0){
		for (int i=0; i<SizeGeneratorList; i++){
			pointlist.buff[i] = ptRHS.pointlist.buff[i];
		}
	}

*/

    return *this;  // Assignment operator returns left side.
}

PolygonObject::PolygonObject(){
//	pointlist.UpdateBuffer(4);
//	pointlist.UpdateBuffer(0);
	pointlist.resize(4);
	pointlist.resize(0);
	IsLine = false;

}
PolygonObject::~PolygonObject(){
}

double PolygonObject::CalcLength(){
	double l=0.0;
	if (this->pointlist.size() < 2 ){
		return 0.0;
	}
	int x0=this->pointlist[0].x;
	int y0=this->pointlist[0].y;
	for (int i=1; i<this->pointlist.size(); i++){
		int x1 = this->pointlist[i].x;
		int y1 = this->pointlist[i].y;
		double dx = x0-x1;
		double dy = y0-y1;
		//ShowFloat(dx, "dx");
		double d = sqrt(dx*dx+dy*dy);

		l += d;
		x0=x1;
		y0=y1;
	}
	if (!this->IsLine && (this->pointlist.size()>2)){	
		int x1 = this->pointlist[0].x;
		int y1 = this->pointlist[0].y;
		double dx = x0-x1;
		double dy = y0-y1;
		double d = sqrt(dx*dx+dy*dy);
		l += d;
	}


	return l;
}
void PolygonObject::AddPoint(CPoint p){
	pointlist.push_back(p);
}

int PolygonObject::NPoints(){
	return pointlist.size();
}

void PolygonObject::ShiftPoints(int dx, int dy){
	for (int i=0; i<NPoints(); i++){
		pointlist[i].x +=dx;
		pointlist[i].y +=dy;
	}
}


void PolygonObject::UpdateDataPoints(){
	DataPoints.resize(0);
//	int NP=pointlist.GetSize();
	int NP=pointlist.size();
	if (NP<1) return;
//	CPoint * buff =(CPoint *)(pointlist.buff);

//	MinX = MaxX=buff[0].x;
//	MinY = MaxY=buff[0].y;
	MinX = MaxX=pointlist[0].x;
	MinY = MaxY=pointlist[0].y;
	for (int i=1; i<NP; i++){
		int x = pointlist[i].x;
		if (x<MinX) MinX=x;
		if (x>MaxX) MaxX=x;
		int y = pointlist[i].y;
		if (y<MinY) MinY=y;
		if (y>MaxY) MaxY=y;
	}
	for (int x=MinX; x<=MaxX; x++){
		for (int y=MinY; y<=MaxY; y++){
			CPoint p = CPoint(x,y);
			if (MyPolygon::pointInPolygon(NP, p, pointlist ) ){
				DataPoints.push_back(p);
			}
		}
	}
//	Alert0("precalc");
}
int PolygonObject::GetNumberOfDataPoints(){
	return DataPoints.size();
}

/*
void PolygonObject::UpdateInsidePrecalc(){
	int NP=pointlist.GetSize();
	if (NP<1) return;
	CPoint * buff =(CPoint *)(pointlist.buff);


	MinX=MaxX=buff[0].x;
	MinY=MaxY=buff[0].y;
	for (int i=1; i<NP; i++){
		int x = buff[i].x;
		if (x<MinX) MinX=x;
		if (x>MaxX) MaxX=x;
		int y = buff[i].y;
		if (y<MinY) MinY=y;
		if (y>MaxY) MaxY=y;
	}
	InsidePrecalc.resize((MaxX-MinX+1)*(MaxY-MinY+1));
//	for (int i=0; i<InsidePrecalc.size(); i++){
//		InsidePrecalc[i]=false;
//	}
	for (int x=MinX; x<=MaxX; x++){
		for (int y=MinY; y<=MaxY; y++){
			InsidePrecalc[x-MinX+(y-MinY)*(MaxX-MinX+1)] = 	MyPolygon::pointInPolygon(NP, CPoint(x,y), buff ) ;
		}
	}
//	Alert0("precalc");
}
*/

bool PolygonObject::IsInside(CPoint p){
	if (IsLine){
		return false;
	}
	int NP=pointlist.size();
	if (NP<2) return false;
// NEW STUFF:

/*
int x = p.x;
	int y = p.y;

	if (x<MinX) return false;
	if (x>MaxX) return false;
	if (y<MinY) return false;
	if (y>MaxY) return false;

	return InsidePrecalc[x-MinX+(y-MinY)*(MaxX-MinX+1)];
// end new stuff
*/
//	CPoint * buff =(CPoint *)(pointlist.buff);
	return MyPolygon::pointInPolygon(NP, p, pointlist ) ;
//	return MyPolygon::inpoly(NP, p, buff ) ;
}

bool PolygonObject::IsInside(int x, int y){
	return IsInside(CPoint(x,y));

}

void PolygonObject::Draw(CDC * pDC, int offsetx, int offsety, int index, int Zoom,  bool ZoomIn, bool PreliminaryDraw){
	// PrelininaryDraw is used to not connect last and first point for construction.
//	void Draw(CDC * pDC, int index = -1, int Zoom = 1, int offset x = 0, int offsety = 0);
//	ShowFloat(Zoom, "Zoom");
//	ShowFloat(offsetx, "offsetx");
	int NP=pointlist.size();
	if (NP<1) return;
//	CPoint * buff =(CPoint *)(pointlist.buff);
	pDC->MoveTo(DivZoom(pointlist[0].x+offsetx, Zoom, ZoomIn),
				DivZoom(pointlist[0].y+offsety, Zoom, ZoomIn));
	for (int i=1; i<NP; i++){
//			pDC->LineTo( (pointlist[i].x+offsetx)/Zoom, (pointlist[i].y+offsety)/Zoom);
		pDC->LineTo(DivZoom(pointlist[i].x+offsetx, Zoom, ZoomIn),
				DivZoom(pointlist[i].y+offsety, Zoom, ZoomIn));
	}
	if ((!PreliminaryDraw) && (!IsLine)){
		pDC->LineTo(DivZoom(pointlist[0].x+offsetx, Zoom, ZoomIn),
				DivZoom((pointlist[0].y+offsety), Zoom, ZoomIn));
//			pDC->LineTo((pointlist[0].x+offsetx)/Zoom, (pointlist[0].y+offsety)/Zoom);
	}
	for (int i=0; i<NP; i++){
		int xx, yy;
		xx= DivZoom(pointlist[i].x+offsetx, Zoom, ZoomIn);
		yy= DivZoom(pointlist[i].y+offsety, Zoom, ZoomIn);
/*
if (Zoom!=0){
			xx = (pointlist[i].x+offsetx)/Zoom;
			yy = (pointlist[i].y+offsety)/Zoom;
		}
		else{
			xx = int(double(pointlist[i].x+offsetx)/RealZoom);
			yy = int(double(pointlist[i].y+offsety)/RealZoom);
		}
*/
		pDC->Ellipse( xx-4, yy-4, xx+4, yy+4); 
	}
//	if (index>=0){
	char s[20];
	_itoa(index, s, 10);
//		int x, y;
//		if (Zoom!=0){
		int x = DivZoom(pointlist[0].x+offsetx, Zoom, ZoomIn);
		int y = DivZoom(pointlist[0].y+offsety, Zoom, ZoomIn);
//		}
//		pDC->TextOutA( (pointlist[0].x+offsetx)/Zoom, (pointlist[0].y+offsety)/Zoom, CString(s));
		pDC->TextOutA( x, y, CString(s));
}

int PolygonObject::ClickedOnAPoint(CPoint p){
	int NP=pointlist.size();
	if (NP<1) return false;
	//CPoint * buff =(CPoint *)(pointlist.buff);
	double x=p.x;
	double y=p.y;
	for (int i=0; i<NP; i++){
		CPoint pp=pointlist[i];
		double xx=pp.x;
		double yy=pp.y;
		if ( ((x-xx)*(x-xx)+(y-yy)*(y-yy))<=16) return i;
	}
	return (-1);
}


/*
PolygonObject &PolygonObject::operator=( PolygonObject &pP )
{
	pointlist = pP.pointlist;
	IsLine = pP.IsLine;
//	UpdateInsidePrecalc();
	UpdateDataPoints();
    return *this;  // Assignment operator returns left side.
}
*/
PolygonObject &PolygonObject::operator=( PolygonObject pP )
{
	pointlist = pP.pointlist;
	IsLine = pP.IsLine;
//	UpdateInsidePrecalc();
	UpdateDataPoints();
    return *this;  // Assignment operator returns left side.
}

void PolygonObject::Copy (PolygonObject & CopyTo){
	CopyTo.pointlist.resize(0);
	CopyTo.IsLine = IsLine;
	for (int i=0; i<this->NPoints(); i++){
		CopyTo.pointlist.push_back(this->pointlist[i]);
	}
	CopyTo.DataPoints.resize(0);
}

double PolygonObject::CalcMeanValue(double Back, double ** image, int xsize, int ysize,
									int offsetx, int offsety, int CalcType){
	if (!image){
		return 0.0;
	}
	if (IsLine){
		return 0.0;
	}
	ImageType = 0;
	imageptr = (char *)image;
	int NData=0;
	return DoCalcMeanValue(Back, xsize, ysize, offsetx, offsety, NData, CalcType);
}
double PolygonObject::CalcMeanValue(int Back, int ** image, int xsize, int ysize,
									int offsetx, int offsety, int CalcType){
	if (!image){
		return 0.0;
	}
	if (IsLine){
		return 0.0;
	}
	ImageType = 2;
	imageptr = (char *)image;
	int NData=0;
	return DoCalcMeanValue(double(Back), xsize, ysize, offsetx, offsety, NData, CalcType);
}
double PolygonObject::CalcMeanValue(float Back, float ** image, int xsize, int ysize,
									int offsetx, int offsety, int CalcType){
	if (!image){
		return 0.0;
	}
	if (IsLine){
		return 0.0;
	}
	ImageType = 1;
	imageptr = (char *)image;
	int NData=0;
	return DoCalcMeanValue(double(Back), xsize, ysize, offsetx, offsety, NData, CalcType);
}
double PolygonObject::GetImageValue(int x, int y, char * imageptr){
	if (ImageType == 1){ // float
		float ** d = (float **) imageptr;
		return d[x][y];
	}
	else{
		if (ImageType==0){ // double
			double ** d = (double **) imageptr;
			return d[x][y];
		}
		else{ // int
			int  ** d = (int **) imageptr;
			return d[x][y];
		}
	}

}
double PolygonObject::OldDoCalcMeanValue(int xsize, int ysize, int offsetx, int offsety){
/*
if (!image){
		return 0.0;
	}
*/
	if (xsize<1) return 0.0;
	if (ysize<1) return 0.0;
	int NP=pointlist.size();
	if (NP<1) return 0.0;
	
//	CPoint * buff =(CPoint *)(pointlist.buff);
	CPoint P=pointlist[0];
//	CPoint * po = &P;
	int MinY=P.y+offsety;
	int MaxY=MinY;
	int MinX=P.x+offsetx;
	int MaxX=MinX;
	for (int i=1; i<NP; i++){
		P=pointlist[i];
		int xx = P.x + offsetx;
		int yy = P.y + offsety;
		if (xx>MaxX) MaxX=xx;
		if (xx<MinX) MinX=xx;
		if (yy>MaxY) MaxY=yy;
		if (yy<MinY) MinY=yy;
	}
	if (MinX>=xsize){
		return 0.0;
	}
	if (MinY>=ysize){
		return 0.0;
	}
	if (MaxX<0) return 0.0;
	if (MaxY<0) return 0.0;
	if (MaxX>=xsize){
		MaxX=xsize-1;
	}
	if (MaxY>=ysize){
		MaxY=ysize-1;
	}
	if (MinY<0) MinY=0;
	if (MinX<0) MinX=0;

	int NData = 0;
	double sum=0.0;
//
//	float ** d = (float **) imageptr;
//	return d[x][y];
	for (int x=MinX; x<=MaxX; x++){
		int xx = x - offsetx;
		for (int y=MinY; y<=MaxY; y++){
			int yy = y- offsety;
			if (IsInside(xx, yy)){//
//				sum += image[x][y];
				sum += GetImageValue(x, y, imageptr);
//				sum += d[x][y];
				NData++;
			}
		}
	}
	if (NData<1) return 0.0;

	return sum/double(NData);

}
double PolygonObject::DoCalcMeanValue(double Back, int xsize, int ysize, int offsetx,
										int offsety, int & NData, int CalcValue){
	if (xsize<1) return 0.0;
	if (ysize<1) return 0.0;

	NData = 0;
	double sum=0.0;
	for (int i=0; i<this->GetNumberOfDataPoints(); i++){
		CPoint p = this->DataPoints[i];
		int x=p.x+offsetx; 
		int y=p.y+offsety;
		if ((x>=0) && (x<xsize) && (y>=0) && (y<ysize)){
			sum += double(GetImageValue(x, y, imageptr))-Back;
			NData++;
		}
	}
	if (NData<1) return 0.0;

	if (CalcValue == 0){
		return sum/double(NData);
	}
	if (CalcValue == 1){
		return sum;
	}
	if (CalcValue == 2){
		return NData;
	}
	return 0.0;

}

double PolygonObject::CalcMembraneValue(float Back, float ** image, int xsize, int ysize, int offsetx, int offsety,
										int MembraneSize, bool IsCircular, int CalcMode){
	if (!image){
		return 0.0;
	}
	ImageType = 1;
	imageptr = (char *)image;
	return DoCalcMembraneValue(Back, xsize, ysize, offsetx, offsety, MembraneSize, IsCircular, CalcMode);
}
double PolygonObject::CalcMembraneValue(int Back, int ** image, int xsize, int ysize, int offsetx, int offsety,
										int MembraneSize, bool IsCircular, int CalcMode){
	if (!image){
		return 0.0;
	}
	ImageType = 2;
	imageptr = (char *)image;
	return DoCalcMembraneValue(double(Back), xsize, ysize, offsetx, offsety,
						MembraneSize, IsCircular, CalcMode);
}
double PolygonObject::CalcMembraneValue(double Back, double ** image, int xsize, int ysize,
										int offsetx, int offsety,
										int MembraneSize, bool IsCircular, int CalcMode){
	if (!image){
		return 0.0;
	}
	ImageType = 0;
	imageptr = (char *)image;
	return DoCalcMembraneValue(Back, xsize, ysize, offsetx, offsety, MembraneSize, IsCircular, CalcMode);
}




ArrayOfPolygons::ArrayOfPolygons(){
	AreLines = false;
}
ArrayOfPolygons::~ArrayOfPolygons(){
}
void ArrayOfPolygons::AddPolygon(PolygonObject poly){
	int NP=NPoly();
//	polygons.UpdateBuffer(NP+1);
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	buff[NP]=poly;
	polygons.push_back(poly);
}
PolygonObject * ArrayOfPolygons::GetPoly(int index){
	int NP=NPoly();
	if (index<0){
		return NULL;
	}
	if (index>=NP){
		return NULL;
	}
	PolygonObject * buff = &polygons[0]; 
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
	return (buff+index);
}
int ArrayOfPolygons::NPoly(){
//	return polygons.GetSize();
	return polygons.size();
}

bool ArrayOfPolygons::SaveOnFile(CFile * fp, int & byteswritten, int Version){
	MyWriteVariable(AreLines);
	int NP = NPoly();
	MyWriteVariable(NP);
	if (NP<1){
//		Beep(100,10);

		return true;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
	for (int i=0; i<NP; i++){
//		Beep(1000,10);
//		PolygonObject & p = buff[i];
		PolygonObject & p = polygons[i];
		int NEdge = p.NPoints();
		MyWriteVariable(NEdge);
		if (NEdge>0){
//			CPoint * points = (CPoint *)(p.pointlist.buff);
			for (int j=0; j<NEdge; j++){
				MyWriteVariable(p.pointlist[j].x);
				MyWriteVariable(p.pointlist[j].y);
				//ShowFloat(p.pointlist[j].x, "j.x");
				//ShowFloat(p.pointlist[j].y, "j.y");
			}
		}
	}
	return true;
}
bool ArrayOfPolygons::LoadFromFile(CFile * fp, int & bytesavailable, int Version){
	AreLines = false;
//	ShowFloat(Version, "Version");
	if (Version>10){
			MyReadVariable(AreLines);
	}
	int NP;
//	ReByNS(NP);
	MyReadVariable(NP);
//	ShowFloat(NP, "NP");
	if (NP<0){
		//Beep(1000,10);
		NP=0;
	}
//	polygons.UpdateBuffer(NP);
	polygons.resize(NP);
	if (NP<1){
		//Beep(100,10);

		return true;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
	for (int i=0; i<NP; i++){
//		PolygonObject & p = buff[i];
		PolygonObject & p = polygons[i];
		p.IsLine = AreLines;
		int NEdge;
		MyReadVariable(NEdge);
		if (NEdge<1) NEdge=0;
		p.pointlist.resize(NEdge);
//		p.pointlist.UpdateBuffer(NEdge);
		if (NEdge>0){
//			CPoint * points = (CPoint *)(p.pointlist.buff);
			for (int j=0; j<NEdge; j++){
				MyReadVariable(p.pointlist[j].x);
				MyReadVariable(p.pointlist[j].y);
				//ShowFloat(p.pointlist[j].x, "j.x");
				//ShowFloat(p.pointlist[j].y, "j.y");
			}
		}
//		p.UpdateInsidePrecalc();
		p.UpdateDataPoints();
	}
	return true;

}
void ArrayOfPolygons::DrawPolys(CDC * pDC, int selectedpolygon, int Zoom, int offsetx, int offsety,  bool ZoomIn){
	if (!pDC) return;
	int NP=NPoly();
	if (NP<1) return;

	CPen redPen;
	redPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	CPen blackPen;
	blackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	CPen * oldpen= pDC->SelectObject(&(blackPen));

//	PolygonObject * buff = (PolygonObject *)polygons.buff;
	for (int i=0; i<NP; i++){
		if (i==selectedpolygon){
			pDC->SelectObject(&(redPen));
		}
		else{
			pDC->SelectObject(&(blackPen));
		}
//		buff[i].Draw(pDC, offsetx, offsety, i, Zoom, ZoomIn);
		polygons[i].Draw(pDC, offsetx, offsety, i, Zoom, ZoomIn);
	}
	pDC->SelectObject(oldpen);

}
int  ArrayOfPolygons::ClickedOnAPoint(CPoint p, int & corner){
	int NP=NPoly();
	if (NP<1) return -1;
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
	for (int i=0; i<NP; i++){
//		corner = buff[i].ClickedOnAPoint(p);
		corner = polygons[i].ClickedOnAPoint(p);
		if (corner>=0){
			return i;
		}
	}
	return -1;
}

void ArrayOfPolygons::MovePolygon(int poly, int corner, CPoint p, CDC * pDC, int Zoom, int offsetx, int offsety,  bool ZoomIn){
	if (!pDC) return;
	if (poly<0) return;
	int NP=NPoly();
	if (NP<1) return ;
	if (poly>=NP) return;
	if (corner<0) return;

//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	PolygonObject & po = buff[poly];
	PolygonObject & po = polygons[poly];
	int NC = po.NPoints();
	if (NC<1) return;
	if (corner>=NC) return;
//	CPoint * points = (CPoint *)(po.pointlist.buff);
	po.pointlist[corner]=p;
	po.Draw(pDC, offsetx, offsety, poly, Zoom, ZoomIn);
}



void ArrayOfPolygons::DeletePolygon(int index){
	if (index<0){
		return;
	}
	int NP=NPoly();
	if (NP<1){
		return;
	}
	if (index>=NP){
		return;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;

	for (int i=index; i<(NP-1); i++){
///		buff[i]=buff[i+1];
		polygons[i]=polygons[i+1];
	}
//	buff[NP-1].pointlist.resize(0);
	polygons[NP-1].pointlist.resize(0);
//	polygons.UpdateBuffer(NP-1);
	polygons.resize(NP-1);

}
bool ArrayOfPolygons::IsInside(CPoint p, int index){
	if (index<0){
		return false;
	}
	if (AreLines){
		return false;
	}
	int NP=NPoly();
	if (NP<1){
		return false;
	}
	if (index>=NP){
		return false;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	return buff[index].IsInside(p);
	return polygons[index].IsInside(p);

}
double ArrayOfPolygons::CalcMeanValue(double Back, int index, double ** image,
										int xsize, int ysize, int offsetx, int offsety,
										int CalcMode){
	if (index<0){
		return 0.0;
	}
	int NP=NPoly();
	if (NP<1){
		return 0.0;
	}
	if (index>=NP){
		return 0.0;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	return buff[index].CalcMeanValue(Back, image, xsize, ysize, offsetx, offsety, CalcMode);
	return polygons[index].CalcMeanValue(Back, image, xsize, ysize, offsetx, offsety, CalcMode);
}
double ArrayOfPolygons::CalcMeanValue(int Back, int index, int ** image, int xsize,
									  int ysize, int offsetx, int offsety, int CalcMode){
	if (index<0){
		return 0.0;
	}
	int NP=NPoly();
	if (NP<1){
		return 0.0;
	}
	if (index>=NP){
		return 0.0;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	return buff[index].CalcMeanValue(Back, image, xsize, ysize, offsetx, offsety, CalcMode);
	return polygons[index].CalcMeanValue(Back, image, xsize, ysize, offsetx, offsety, CalcMode);
}
double ArrayOfPolygons::CalcMeanValue(float Back, int index, float ** image, int xsize,
									  int ysize, int offsetx, int offsety, int CalcMode){
	if (index<0){
		return 0.0;
	}
	int NP=NPoly();
	if (NP<1){
		return 0.0;
	}
	if (index>=NP){
		return 0.0;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	return buff[index].CalcMeanValue(Back, image, xsize, ysize, offsetx, offsety, CalcMode);
	return polygons[index].CalcMeanValue(Back, image, xsize, ysize, offsetx, offsety, CalcMode);
}


void  ArrayOfPolygons::Track(int TrackMode,
							 int index, AnaVisionImageDataType ** RefImage, AnaVisionImageDataType ** Image,
								int xsize, int ysize,
								int offsetrefx, int offsetrefy, int & offsetx, int & offsety){
	if (index<0){
		return ;
	}
	int NP=NPoly();
	if (NP<1){
		return ;
	}
	if (index>=NP){
		return ;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	buff[index].Track(TrackMode, RefImage, Image, xsize, ysize, offsetrefx, offsetrefy, offsetx, offsety);
	polygons[index].Track(TrackMode, RefImage, Image, xsize, ysize, offsetrefx, offsetrefy, offsetx, offsety);
}

int PolygonObject::GetNumberOfPointsInPolygon(){ // irrespective of where it is in space!
	return this->DataPoints.size();
	int NP=pointlist.size();
	if (NP<1) return 0;
	
//	CPoint * buff =(CPoint *)(pointlist.buff);
//	CPoint * po = buff;
	CPoint P = pointlist[0];
	int MinY=P.y;
	int MaxY=MinY;
	int MinX=P.x;
	int MaxX=MinX;
	for (int i=1; i<NP; i++){
		P=pointlist[i];
		int xx = P.x;
		int yy = P.y;
		if (xx>MaxX) MaxX=xx;
		if (xx<MinX) MinX=xx;
		if (yy>MaxY) MaxY=yy;
		if (yy<MinY) MinY=yy;
	}

	int NData = 0;
	for (int x=MinX; x<=MaxX; x++){
		int xx = x;
		for (int y=MinY; y<=MaxY; y++){
			int yy = y;
			if (IsInside(xx, yy)){
				NData++;
			}
		}
	}
	return NData;

}
void PolygonObject::GetArrayOfPointsInPolygon( PolygonDataPoint * & ArrayOfPoints){
	for (int i=0; i<DataPoints.size(); i++){
		ArrayOfPoints[i].x=DataPoints[i].x;
		ArrayOfPoints[i].y=DataPoints[i].y;
	}
	return;

	int NP=pointlist.size();
	if (NP<1) return ;
	
//	CPoint * buff =(CPoint *)(pointlist.buff);
//	CPoint * po = buff;
	CPoint p = pointlist[0];
/*
int MinY=po->y;
	int MaxY=MinY;
	int MinX=po->x;
	int MaxX=MinX;
	for (int i=1; i<NP; i++){
		po=&(buff[i]);
		int xx = po->x;
		int yy = po->y;
		if (xx>MaxX) MaxX=xx;
		if (xx<MinX) MinX=xx;
		if (yy>MaxY) MaxY=yy;
		if (yy<MinY) MinY=yy;
	}
*/
	
	
	int NData = 0;
	for (int x=MinX; x<=MaxX; x++){
		int xx = x;
		for (int y=MinY; y<=MaxY; y++){
			int yy = y;
			if (IsInside(xx, yy)){
				ArrayOfPoints[NData].x = xx;
				ArrayOfPoints[NData].y = yy;
				NData++;
			}
		}
	}
}

double PolygonObject::CalcDiffImage(int offx, int offy, int NP,  PolygonDataPoint *  RefPoints,
									AnaVisionImageDataType ** TrackImage, int xsize, int ysize){
	int NUsefulPoints = 0;
	double diff = 0;
	for (int i=0; i<NP; i++){
		if (RefPoints[i].InsideRect){
			int x=RefPoints[i].x+offx;
			int y=RefPoints[i].y+offy;
			if ((x<0) || (x>=xsize) || (y<0) || (y>=ysize) ){
			}
			else{
				NUsefulPoints++;
				double d = RefPoints[i].value - TrackImage[x][y];
				diff += d*d;
			}
		}
	}

	if (NUsefulPoints == 0){
		return 1e300;
	}
	else{
		return (diff/double(NUsefulPoints));
	}
}

double PolygonObject::CalcCrossCorrel(int offx, int offy, int NP, PolygonDataPoint *  RefPoints, 
							PolygonDataPoint *  TrackPoints,
					   AnaVisionImageDataType ** TrackImage,int xsize, int ysize){


	for (int i=0; i<NP; i++){
		TrackPoints[i] = RefPoints[i];
	}

	int NUsefulPoints = 0;
	double diff = 0;
	double meanref = 0;
	double meantrack = 0;
	for (int i=0; i<NP; i++){
		if (RefPoints[i].InsideRect){
			int x=RefPoints[i].x+offx;
			int y=RefPoints[i].y+offy;
			if ((x<0) || (x>=xsize) || (y<0) || (y>=ysize) ){
				TrackPoints[i].InsideRect = false;
			}
			else{
				NUsefulPoints++;
				TrackPoints[i].InsideRect = true;
				TrackPoints[i].x = x;
				TrackPoints[i].y = y;
				TrackPoints[i].value = TrackImage[x][y];
				meanref += RefPoints[i].value;
				meantrack += TrackPoints[i].value;
			}
		}
	}

	if (NUsefulPoints == 0){
		return 1e300;
	}
	meanref /= double(NUsefulPoints);
	meantrack /= double(NUsefulPoints);

	double sref = 0.0;
	double strack = 0.0;
	double cross = 0.0;
	for (int i=0; i<NP; i++){
		if (TrackPoints[i].InsideRect){
			double diffref = RefPoints[i].value - meanref;
			double difftrack  = TrackPoints[i].value - meantrack;

			sref += diffref*diffref;
			strack += difftrack*difftrack;
			cross += diffref*difftrack;
		}
	}
	sref = sqrt(sref/double(NUsefulPoints));
	strack = sqrt(strack/double(NUsefulPoints));

	cross = cross/sref/strack/double(NUsefulPoints);

	return 1.0-cross;
}


double PolygonObject::CalcDiffMean(int offx, int offy, int NP, PolygonDataPoint *  RefPoints, 
							PolygonDataPoint *  TrackPoints,
					   AnaVisionImageDataType ** TrackImage,int xsize, int ysize){


	for (int i=0; i<NP; i++){
		TrackPoints[i] = RefPoints[i];
	}

	int NUsefulPoints = 0;
	double diff = 0;
	double meanref = 0;
	double meantrack = 0;
	for (int i=0; i<NP; i++){
		if (RefPoints[i].InsideRect){
			int x=RefPoints[i].x+offx;
			int y=RefPoints[i].y+offy;
			if ((x<0) || (x>=xsize) || (y<0) || (y>=ysize) ){
				TrackPoints[i].InsideRect = false;
			}
			else{
				NUsefulPoints++;
				TrackPoints[i].InsideRect = true;
				TrackPoints[i].x = x;
				TrackPoints[i].y = y;
				TrackPoints[i].value = TrackImage[x][y];
				meanref += RefPoints[i].value;
				meantrack += TrackPoints[i].value;
			}
		}
	}

	if (NUsefulPoints == 0){
		return 1e300;
	}
	meanref /= double(NUsefulPoints);
	meantrack /= double(NUsefulPoints);

	double temp = (meanref-meantrack);

	return temp*temp;

}

void PolygonObject::Track(int TrackMode,
						  AnaVisionImageDataType ** RefImage, AnaVisionImageDataType ** Image, int xsize, int ysize,
						  int offsetrefx, int offsetrefy, int & offsetx, int & offsety){

	// GetNumberOfPointsInPolygon
	int NPoints = GetNumberOfPointsInPolygon();
	if (NPoints<=0){
		Alert0("<= 0 ponts in poly");
		return;
	}
	
//	ShowFloat(offsetrefx, "offsetxref");
//	ShowFloat(offsetrefy, "offsetyref");
//	ShowFloat(offsetx, "offsetx");
//	ShowFloat(offsety, "offsety");

	//	ShowFloat(NPoints, "NP");
	PolygonDataPoint * RefPolyPoints = new PolygonDataPoint [NPoints];
	PolygonDataPoint * TrackPolyPoints = new PolygonDataPoint [NPoints];

	GetArrayOfPointsInPolygon( RefPolyPoints);  // contains all points

	int NUseful = 0;
	for (int i=0; i<NPoints; i++){
		RefPolyPoints[i].x += offsetrefx;
		RefPolyPoints[i].y += offsetrefy;
		int x=RefPolyPoints[i].x;
		int y=RefPolyPoints[i].y;

		if ((x<0) || (x>=xsize) || (y<0) || (y>=ysize) ){
			RefPolyPoints[i].InsideRect = false;
		}
		else{
			RefPolyPoints[i].InsideRect = true;
			NUseful++;
			RefPolyPoints[i].value = RefImage[x][y];
		}
	}

	if (NUseful==0){
		Alert0("no points in poly in image");
		delete [] TrackPolyPoints;
		delete [] RefPolyPoints;
		return;
	}

	Simplex simp;

	double Params[2];
	double BestParams[2];
	BestParams[0]=BestParams[1]=0;
	offsetx -= offsetrefx;
	offsety -= offsetrefy;

	double BestSD =  CalcTrackSD(TrackMode, offsetx, offsety, NPoints,  RefPolyPoints, TrackPolyPoints, Image, xsize, ysize);

//	ShowFloat(BestSD, "BestSD");

	for (int ii=0; ii<4; ii++){
		Params[0]=Params[1]=20.0;
		if (ii==1){
			Params[0]=-20.0;
		}
		if (ii==2){
			Params[1]=-20.0;
		}
		if (ii==3){
			Params[0]=-20.0; Params[1]=-20.0;
		}
		simp.InitSimplex(0.8, Params, 2);
		for (int iter = 0; iter<50; iter++){
			double SD =  CalcTrackSD(TrackMode, int(Params[0])+offsetx, int(Params[1])+offsety, 
							NPoints,  RefPolyPoints, TrackPolyPoints, Image, xsize, ysize);

			if (SD<BestSD){
//				ShowFloat(SD, "SD");
//				ShowFloat(int(Params[0]), "ox");
//				ShowFloat(int(Params[1]), "oy");

				BestSD =SD;
				simp.CopyParams(Params, BestParams);
			}
			simp.NextSimp(Params, SD);
		}
	}
	double SD =  CalcTrackSD(TrackMode, int(BestParams[0])+offsetx, int(BestParams[1])+offsety, 
							NPoints,  RefPolyPoints, TrackPolyPoints, Image, xsize, ysize);

//	ShowFloat(BestSD, "SD");
//	ShowFloat(int(BestParams[0]), "ox");
//	ShowFloat(int(BestParams[1]), "oy");
	offsetx = int(BestParams[0]);
	offsety = int(BestParams[1]);

	delete [] TrackPolyPoints;
	delete [] RefPolyPoints;

}



double PolygonObject::CalcTrackSD(int TrackMode, 
						int offx, int offy, int NP, PolygonDataPoint *  RefPoints, 
							PolygonDataPoint *  TrackPoints,
							AnaVisionImageDataType ** TrackImage,int xsize, int ysize){
	double result = 1e300;
	switch (TrackMode){
		case 0 : result = CalcDiffImage(offx, offy, NP, RefPoints, TrackImage, xsize, ysize);
				 break;
		case 1 : result = CalcCrossCorrel(offx, offy, NP,  RefPoints, TrackPoints, TrackImage, xsize, ysize);
				 break;
		case 2 : result = CalcDiffMean(offx, offy, NP,  RefPoints, TrackPoints, TrackImage, xsize, ysize);
				 break;
		default : break;
	}
	return result;
}

void PolygonObject::GetPoint(int index , CPoint & p){
	if (index<0) return;
	if (index>=this->GetNumberOfVertices()) return;
//	ShowFloat(this->pointlist.GetSize(), "buffsize");;
	p = this->pointlist[index];
}


int PolygonObject::GetNumberOfVertices(){
	return pointlist.size();
}
void PolygonObject::GetVerticalPoints(CPoint P1, CPoint P2, CPoint & POut, CPoint & PIn,
									  int MembraneSize){
	double dx = P2.x-P1.x;
	double dy = P2.y-P1.y;
	double length = sqrt(dx*dx+dy*dy);
	double Scale = double(MembraneSize)/length;
	POut.x = floor(double(P1.x)+0.5 - dy*Scale);
	POut.y = floor(double(P1.y) + 0.5 +dx*Scale);

//	ShowFloat(P1Out.x, "P1Out.x");
//	ShowFloat(P1Out.y, "P1Out.y");


	PIn.x = floor(double(P1.x)+0.5 + dy*Scale);
	PIn.y = floor(double(P1.y) + 0.5 - dx*Scale);


}


void PolygonObject::GetNewPoints(CPoint P1, CPoint P2, int MembraneSize,
									CPoint & P1Out, 
									CPoint & P1In,
									CPoint & P2Out,
									CPoint & P2In){

//	ShowFloat(P1.x, "P1.x");
//	ShowFloat(P1.y, "P1.y");
//	ShowFloat(P2.x, "P2.x");
//	ShowFloat(P2.y, "P2.y");
	double dx = double(P2.x-P1.x);
	double dy = double(P2.y-P1.y);
	double length = sqrt(dx*dx+dy*dy);
	double Scale = double(MembraneSize)/length;

//	ShowFloat(length, "length");
//	ShowFloat(Scale, "Scale");

//	double x1out = P1.x-dy;
	P1Out.x = int(floor(double(P1.x)+0.5 - dy*Scale));
//	double y1out = P1.y+dx;
	P1Out.y = int(floor(double(P1.y) + 0.5 +dx*Scale));

//	ShowFloat(P1Out.x, "P1Out.x");
//	ShowFloat(P1Out.y, "P1Out.y");


//	double x1in  = P1.x+dy;
	P1In.x = int(floor(double(P1.x)+0.5 + dy*Scale));
//	double y1in  = P1.y-dx;
	P1In.y = int(floor(double(P1.y) + 0.5 - dx*Scale));

//	double x2out = P2.x-dy;
	P2Out.x = int(floor(double(P2.x)+0.5 - dy*Scale));
//	double y2out = P2.y+dx;
	P2Out.y = int(floor(double(P2.y) + 0.5 +dx*Scale));

//	double x2in  = P2.x+dy;
	P2In.x = int(floor(double(P2.x)+0.5 + dy*Scale));
//	double y2in  = P2.y-dx;
	P2In.y = int(floor(double(P2.y) + 0.5 - dx*Scale));
}
void PolygonObject::MakeMembranePolygons(PolygonObject *POut1,
						  PolygonObject *POut2,
						  PolygonObject *PIn1,
						  PolygonObject *PIn2,
						  int MembraneSize, bool IsCircular){


	int NVertices = GetNumberOfVertices();

	POut1->pointlist.resize(NVertices);
	POut2->pointlist.resize(NVertices);
	PIn1->pointlist.resize(NVertices);
	PIn2->pointlist.resize(NVertices);

//		ShowFloat(i, "i");
//		ShowFloat(points[i].x, "x");
//		ShowFloat(points[i].y, "y");
//	}

	vector<CPoint> & pointsOut1 = POut1->pointlist; 
	vector<CPoint> & pointsOut2 = POut2->pointlist;
	vector<CPoint> & pointsIn1 = PIn1->pointlist;
	vector<CPoint> & pointsIn2 = PIn2->pointlist;

	for (int i=0; i<NVertices; i++){
		CPoint P1=pointlist[i];
//		ShowFloat(P1.x, "P1.x");
//		ShowFloat(P1.y, "P1.y");
		
		CPoint P2;
		bool DoThis = true;
		if (i<(NVertices-1)){
			P2 = pointlist[i+1];
//			Alert0("P2");
//			ShowFloat(P2.x, "P2.x");
//			ShowFloat(P2.y, "P2.y");

		}
		else{
			if (IsCircular){
				P2 = pointlist[0];
			}
			else{
				DoThis = false;
			}
		}

		if (DoThis){
			CPoint P1Out, P1In, P2Out, P2In;
			GetNewPoints(P1, P2, MembraneSize, P1Out, P1In, P2Out, P2In);

			pointsOut1[i] = P1Out;
			pointsIn1[i] = P1In;

			if (i<(NVertices-1)){
				pointsOut2[i+1] = P2Out;
				pointsIn2[i+1] = P2In;
			}
			else{
				// i.e. circular
				pointsOut2[0] = P2Out;
				pointsIn2[0] = P2In;
			}
			if (!IsCircular){
				if (i==0){
					pointsOut2[0] = P1Out;
					pointsIn2[0] = P1In;
				}
				if (i==(NVertices-2)){
					pointsOut1[i+1] = P2Out;
					pointsIn1[i+1] = P2In;
				}
			}

		}
	}
	for (int i=0; i<NVertices; i++){
/*		ShowFloat(i, "i");
		ShowFloat(pointsOut1[i].x, "pointsOut1[i].x");
		ShowFloat(pointsOut1[i].y, "pointsOut1[i].y");
		ShowFloat(pointsOut2[i].x, "pointsOut2[i].x");
		ShowFloat(pointsOut2[i].y, "pointsOut2[i].y");
		ShowFloat(pointsIn1[i].x, "pointsIn1[i].x");
		ShowFloat(pointsIn1[i].y, "pointsIn1[i].y");
		ShowFloat(pointsIn2[i].x, "pointsIn2[i].x");
		ShowFloat(pointsIn2[i].y, "pointsIn2[i].y");
	*/
		CPoint P1, P2;
		P1.x = (pointsOut1[i].x + pointsOut2[i].x)/2;
		P1.y = (pointsOut1[i].y + pointsOut2[i].y)/2;
		P2.x = (pointsIn1[i].x + pointsIn2[i].x)/2;
		P2.y = (pointsIn1[i].y + pointsIn2[i].y)/2;

		pointsOut1[i] = P1;
		pointsIn1[i] = P2;
	}

}

CPoint PolygonObject::CalcCenter(){
	CPoint result(0,0);
	int NV = this->GetNumberOfVertices();
//	ShowFloat(NV, "NV");
	if (NV>0){
		for (int i=0; i<NV; i++){
			CPoint p;
			this->GetPoint(i, p);
			result.x += p.x;
			result.y += p.y;
		}
		result.x /= NV;
		result.y /= NV;
	}
	return result;
}

double PolygonObject::DoCalcMembraneValue(double Back, int xsize, int ysize, int offsetx, int offsety,
											int MembraneSize, bool IsCircular, int CalcMode){
	if (xsize<1) return 0.0;
	if (ysize<1) return 0.0;

	if (GetNumberOfDataPoints()<2){
		return 0.0;
	}
	PolygonObject  POut1;
	PolygonObject * POut2 = new PolygonObject;
	PolygonObject * PIn1 = new PolygonObject;
	PolygonObject * PIn2 = new PolygonObject;
	MakeMembranePolygons(&POut1, POut2, PIn1, PIn2, MembraneSize, IsCircular);

	double value = 0.0;
	if (!IsCircular){
		for (int i=0; i<this->GetNumberOfDataPoints(); i++){
			POut1.AddPoint(PIn1->DataPoints[this->GetNumberOfDataPoints()-i-1]);
		}
		int NData=0;
		double value = 
			POut1.DoCalcMeanValue(Back, xsize, ysize, offsetx, offsety, NData, CalcMode);
	}
	if (IsCircular){
		int NDataOut=0;
		double valueOut = POut1.DoCalcMeanValue(Back, xsize, ysize, offsetx, offsety, NDataOut, CalcMode);
		int NDataIn=0;
		double valueIn = PIn1->DoCalcMeanValue(Back, xsize, ysize, offsetx, offsety, NDataIn, CalcMode);

		value=valueOut-valueIn;
		if (NDataIn>NDataOut){
			value *= -1.0;
		}
	}
	delete 	PIn2;
	delete 	PIn1;
	delete  POut2;
//	delete 	POut1;

	return value;


}

double ArrayOfPolygons::CalcMembraneValue(float Back, int index, float ** image, int xsize,
										  int ysize, int offsetx, int offsety,
										  int MembraneSize, bool RoiIsCircular,
										  int CalcMode){
	if (index<0){
		return 0.0;
	}
	int NP=NPoly();
	if (NP<1){
		return 0.0;
	}
	if (index>=NP){
		return 0.0;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;

//	int NVertices = buff[index].GetNumberOfVertices();
	int NVertices = polygons[index].GetNumberOfVertices();
//	ShowFloat(NVertices, "NVERT");

	PolygonObject POut1;
	PolygonObject POut2;
	PolygonObject PIn1; 
	PolygonObject PIn2; 

//	buff[index].MakeMembranePolygons(&POut1, &POut2, &PIn1, &PIn2, MembraneSize, RoiIsCircular);
	polygons[index].MakeMembranePolygons(&POut1, &POut2, &PIn1, &PIn2, MembraneSize, RoiIsCircular);

//	Alert0("before add");
	if (!RoiIsCircular){
		int NV = POut1.GetNumberOfVertices();
		for (int i=0; i<NV; i++){
			POut1.AddPoint(PIn1.pointlist[NV-i-1]);
		}
	}

//	this->polygons.UpdateBuffer(NP+1, true);
	this->polygons.resize(NP+1);
//	buff = (PolygonObject *)polygons.buff;

//	PolygonObject & newpoly = buff[NP];
	PolygonObject & newpoly = polygons[NP];

	vector <CPoint> & newlist = POut1.pointlist;
	vector <CPoint> & newpolylist = newpoly.pointlist;

	newpolylist.resize(POut1.GetNumberOfVertices());

	for (int i=0; i<POut1.GetNumberOfVertices(); i++){
		newpolylist[i] = newlist[i];
	}

//	buff[NP].UpdateDataPoints();
	polygons[NP].UpdateDataPoints();

	if (RoiIsCircular){ // add also the other 

		NP=NPoly();
//		this->polygons.UpdateBuffer(NP+1, true);
		this->polygons.resize(NP+1);
//		buff = (PolygonObject *)polygons.buff;

//		PolygonObject & newpolyin = buff[NP];
		PolygonObject & newpolyin = polygons[NP];

		newpolyin.pointlist.resize(POut1.GetNumberOfVertices());

		for (int i=0; i<POut1.GetNumberOfVertices(); i++){
			newpolyin.pointlist[i] = PIn1.pointlist[i];
		}
//		buff[NP].UpdateDataPoints();
		polygons[NP].UpdateDataPoints();
	}


//	Alert0("after add");

//	return buff[index].CalcMembraneValue(image, xsize, ysize, offsetx, offsety, MembraneSize, RoiIsCircular);

}
void ArrayOfPolygons::MakeGradientPolysAssumePolyIsaLine(int poly, int MembraneSize, bool RoiIsCircular){
	MakeMembranePolys(poly, MembraneSize, true, true);
}
void ArrayOfPolygons::Createnpolygonsinside(int index, int NPolyToAdd){
	if (NPolyToAdd<1) return;

	if (index<0){
		return ;
	}
	int NP=NPoly();
	if (NP<1){
		return ;
	}
	if (index>=NP){
		return ;
	}
//	PolygonObject * buff = (PolygonObject *)polygons.buff;
//	PolygonObject & poly = buff[index];
	PolygonObject & poly = polygons[index];
	int NVertices = poly.GetNumberOfVertices();
//	ShowFloat(NVertices, "NVERT");

	CPoint Center = poly.CalcCenter();
	//ShowFloat(Center.x, "center.x");

	for (int i=0; i<NPolyToAdd; i++){
//		PolygonObject * buff = (PolygonObject *)polygons.buff;
//		PolygonObject & poly = buff[index];
		PolygonObject & poly = polygons[index];
		double scale = double(i+1)/double(NPolyToAdd+1);
//		ShowFloat(scale, "scale");
		PolygonObject p;
		p.IsLine = false;
		for (int v=0; v<NVertices; v++){

			CPoint vert;
			poly.GetPoint(v, vert);

			CPoint NewP;
			NewP.x = vert.x + int (scale*double(Center.x-vert.x));
			NewP.y = vert.y + int (scale*double(Center.y-vert.y));
			p.AddPoint(NewP);

/*			Vect OldEdge = Vect(vert);

			Vect delta = Vect::Sub(Center, vert);
			delta = Vect::ScaleVect(scale, delta);
			Vect NewEdge = Vect::Add(OldEdge, delta);
			CPoint NewVert = Vect::MakePoint(NewEdge);

//			p.AddPoint(NewVert);
*/
		}
		this->AddPolygon(p);
	}
}

void ArrayOfPolygons::MakeMembranePolys(int index, int MembraneSize, bool RoiIsCircular,
										bool AssumeIsALine){
	if (index<0){
		return ;
	}
	int NP=NPoly();
	if (NP<1){
		return ;
	}
	if (index>=NP){
		return ;
	}
//	RoiIsCircular = false;
//	AssumeIsALine = true;
//	PolygonObject * buff = (PolygonObject *)polygons.buff;

//	int NVertices = buff[index].GetNumberOfVertices();
	int NVertices = polygons[index].GetNumberOfVertices();
//	ShowFloat(NVertices, "NVERT");

	PolygonObject POut1;
	PolygonObject POut2;
	PolygonObject PIn1; 
	PolygonObject PIn2; 

	polygons[index].MakeMembranePolygons(&POut1, &POut2, &PIn1, &PIn2, MembraneSize, RoiIsCircular);

//	Alert0("before add");
	if ((!RoiIsCircular) && (!AssumeIsALine)){
		int NV = POut1.GetNumberOfVertices();
		for (int i=0; i<NV; i++){
			POut1.AddPoint(PIn1.pointlist[NV-i-1]);
		}
	}
	if (AssumeIsALine){
		int NV = POut1.GetNumberOfVertices();
		for (int i=0; i<NV; i++){
//			POut1.AddPoint(buff[index].pointlist[NV-i-1]);
			POut1.AddPoint(polygons[index].pointlist[NV-i-1]);
		}
		for (int i=0; i<NV; i++){
//			PIn1.AddPoint(buff[index].pointlist[NV-i-1]);
			PIn1.AddPoint(polygons[index].pointlist[NV-i-1]);
		}
	}


//	this->polygons.UpdateBuffer(NP+1, true);
	this->polygons.resize(NP+1);
//	buff = (PolygonObject *)polygons.buff;

//	PolygonObject & newpoly = buff[NP];
	PolygonObject & newpoly = polygons[NP];

	vector <CPoint> & newlist = POut1.pointlist;
	vector <CPoint> & newpolylist = newpoly.pointlist;

	newpolylist.resize(POut1.GetNumberOfVertices());

	for (int i=0; i<POut1.GetNumberOfVertices(); i++){
		newpolylist[i] = newlist[i];
	}

//	buff[NP].UpdateDataPoints();
	polygons[NP].UpdateDataPoints();

	if (RoiIsCircular || AssumeIsALine){ // add also the other 

		NP=NPoly();
//		this->polygons.UpdateBuffer(NP+1, true);
		this->polygons.resize(NP+1);
//		buff = (PolygonObject *)polygons.buff;

//		PolygonObject & newpolyin = buff[NP];
		PolygonObject & newpolyin = polygons[NP];

		newpolyin.pointlist.resize(POut1.GetNumberOfVertices());

		for (int i=0; i<POut1.GetNumberOfVertices(); i++){
			newpolyin.pointlist[i] = PIn1.pointlist[i];
		}
//		buff[NP].UpdateDataPoints();
		polygons[NP].UpdateDataPoints();
	}

}

void ArrayOfPolygons::MakePolygonAroundLine(int index, int MembraneSize,
					   ArrayOfPolygons & WhereToAppend){
	if (index<0){
		return ;
	}
	int NP=NPoly();
	if (NP<1){
		return ;
	}
	if (index>=NP){
		return ;
	}

//	PolygonObject * buff = (PolygonObject *)polygons.buff;

	PolygonObject POut;

//	if (buff[index].MakePoygonAroundLine(POut, MembraneSize)){
	if (polygons[index].MakePoygonAroundLine(POut, MembraneSize)){
		WhereToAppend.AddPolygon(POut);
	}

}

//bool GetLineNewPoints(CPoint P1, CPoint P2, CPoint P3int size
bool PolygonObject::MakePoygonAroundLine(PolygonObject & POut,
						  int MembraneSize){

	int NVertices = GetNumberOfVertices();
	if (NVertices<2){
		return false;
	}

	PolygonObject POut1, POut2, PIn1, PIn2;
	PolygonObject OutPoints, InPoints;
	POut1.pointlist.resize(NVertices);
	PIn1.pointlist.resize(NVertices);
	POut2.pointlist.resize(NVertices);
	PIn2.pointlist.resize(NVertices);
	OutPoints.pointlist.resize(NVertices);
	InPoints.pointlist.resize(NVertices);


	vector<CPoint> & pointsOut1 = POut1.pointlist; 
	vector<CPoint> & pointsIn1 = PIn1.pointlist;
	vector<CPoint> & pointsOut2 = POut2.pointlist; 
	vector<CPoint> & pointsIn2 = PIn2.pointlist;
	for (int i=0; i<(NVertices-1); i++){
		CPoint P1, P2;
//		CPoint P1Out, P1In, P2Out, P2In;
		P1=pointlist[i];
		P2=pointlist[i+1];
		GetVerticalPoints(P1, P2, POut1.pointlist[i], PIn1.pointlist[i], MembraneSize);
		if (i==0){
			PIn2.pointlist[i]=PIn1.pointlist[i];
			POut2.pointlist[i]=POut1.pointlist[i];
		}
		GetVerticalPoints(P2, P1, PIn2.pointlist[i+1], POut2.pointlist[i+1], MembraneSize);
		if (i==(NVertices-2)){
			PIn1.pointlist[i+1]=PIn2.pointlist[i+1];
			POut1.pointlist[i+1]=POut2.pointlist[i+1];
		}

	}

	/*
//				POut1.AddPoint( Vect::MakePoint(Vect::ScaleVect(0.5, Vect::AddPoints(P1Out, P2Out))));
//				PIn1.AddPoint( Vect::MakePoint(Vect::ScaleVect(0.5, Vect::AddPoints(P1In, P2In))));
				POut1.AddPoint( Vect::MakePoint(Vect::ScaleVect(0.5, Vect::AddPoints(P1Out, P2))));
				PIn1.AddPoint( Vect::MakePoint(Vect::ScaleVect(0.5, Vect::AddPoints(P2Out, P2In))));
*/

	for (int i=0; i<NVertices; i++){
/*		ShowFloat(i, "i");
		ShowFloat(pointsOut1[i].x, "pointsOut1[i].x");
		ShowFloat(pointsOut1[i].y, "pointsOut1[i].y");
		ShowFloat(pointsOut2[i].x, "pointsOut2[i].x");
		ShowFloat(pointsOut2[i].y, "pointsOut2[i].y");
		ShowFloat(pointsIn1[i].x, "pointsIn1[i].x");
		ShowFloat(pointsIn1[i].y, "pointsIn1[i].y");
		ShowFloat(pointsIn2[i].x, "pointsIn2[i].x");
		ShowFloat(pointsIn2[i].y, "pointsIn2[i].y");
	*/
		CPoint P1, P2;
		if (i==0){
//			P2.x = (pointsOut1[i].x + pointsOut2[i].x)/2;
//			P2.y = (pointsOut1[i].y + pointsOut2[i].y)/2;
//			P1.x = (pointsIn1[i].x + pointsIn2[i].x)/2;
//			P1.y = (pointsIn1[i].y + pointsIn2[i].y)/2;
			P1=pointsOut1[0];
			P2=pointsIn1[0];
		}
		else{
			if (i==(NVertices-1)){
				P1=pointsOut2[i];
				P2=pointsIn2[i];
			}
			else{
				//P1 = Vect::MakePoint(Vect::ScaleVect(0.5, Vect::AddPoints(pointsOut1[i],pointsOut2[i])));
				if ( (pointsOut1[i].x==pointsOut2[i].x) && (pointsOut1[i].y==pointsOut2[i].y)){
//					Alert0("hi");
					P1 = pointsOut1[i];
				}
				else{
//					P1 = GetInterSection(pointsOut2[i-1], pointsOut1[i], pointsOut2[i], pointsOut1[i+1]);
					P1 = GetInterSection(pointsOut1[i-1], pointsOut2[i], pointsOut1[i], pointsOut2[i+1]);
				}

//				Vect Sum = Vect::AddPoints(pointsOut1[i],pointsOut2[i]);
//				Vect Subb = Vect::Sub(Sum, Vect(pointlist[i]));
//				P1 = Vect::MakePoint(Subb);

				//P2 = Vect::MakePoint(Vect::ScaleVect(0.5, Vect::AddPoints(pointsIn1[i],pointsIn2[i])));
//				Sum = Vect::AddPoints(pointsIn1[i],pointsIn2[i]);
//				Subb = Vect::Sub(Sum, Vect(pointlist[i]));
//				P2 = Vect::MakePoint(Subb);

				if ( (pointsIn1[i].x==pointsIn2[i].x) && (pointsIn1[i].y==pointsIn2[i].y)){
					P2 = pointsIn1[i];
				}
				else{
//					P2= GetInterSection(pointsIn2[i-1], pointsIn1[i], pointsIn2[i], pointsIn1[i+1]);
					P2= GetInterSection(pointsIn1[i-1], pointsIn2[i], pointsIn1[i], pointsIn2[i+1]);
				}

			}
		}
		OutPoints.pointlist[i] = P1;
		InPoints.pointlist[i] = P2;
	}

	for (int i=0; i<NVertices; i++){
		POut.AddPoint(OutPoints.pointlist[i]);
//		POut.AddPoint(pointsOut1[i]);
//		POut.AddPoint(pointsOut2[i]);
	}
	for (int i=0; i<NVertices; i++){
		POut.AddPoint(InPoints.pointlist[NVertices-i-1]);
//		POut.AddPoint(pointsIn1[NVertices-i-1]);
//		POut.AddPoint(pointsIn2[NVertices-i-1]);
	}

}


void AreaObject::NewNewRotateRectangle(int index, CPoint newpos){
	// The diagonal point remains fixed, while the other two change in dependence of the 
	// new coordinates
	// 0 - 2 and 1 - 3 are diagonal pairs

//	NewRotateRectangle(index, newpos);
//	return;

	int i0, i1, i2, i3;

	// i3: the one moved; i1= the one fixed; i0, i2, the ones to be calculated;

	i3=index;

	switch (i3){
		case 0 : i1 = 2; i0= 1; i2 = 3; break;
//		case 1 : i1 = 3; i0= 0; i2 = 2; break;
		case 1 : i1 = 3; i0= 2; i2 = 0; break;
//		case 2 : i1 = 0; i0= 3; i2 = 1; break;
		case 2 : i1 = 0; i0= 3; i2 = 1; break;
//		case 3 : i1 = 1; i0= 2; i2 = 0; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}

/*	switch (i3){
		case 0 : i1 = 2; i0= 1; i2 = 3; break;
		case 1 : i1 = 3; i0= 0; i2 = 2; break;
		case 2 : i1 = 0; i0= 3; i2 = 1; break;
		case 3 : i1 = 1; i0= 0; i2 = 2; break;
	}
*/	
	Vect A = Vect(P[i0].GetPos(1,1)); // moved
	Vect B = Vect(P[i1].GetPos(1,1)); // fixed
	Vect C = Vect(P[i2].GetPos(1,1)); // moved
	Vect D = Vect(P[i3].GetPos(1,1)); // guide
	Vect NewDPrime = Vect(newpos);

	Vect DB = Vect::Sub(D, B);
	Vect AB = Vect::Sub(A, B);
	Vect CB = Vect::Sub(C, B);

	Vect NewDB = Vect::Sub(NewDPrime, B);

	Vect uDB = Vect::Normalize(DB);
	Vect udB = Vect::Normalize(NewDB);

	Vect NewDiagonal = udB;
	NewDiagonal.Scale(DB.Length());

	Vect NewD = Vect::Add(B, NewDiagonal);

//	double alpha1 = Vect::GetAngle(Vect::Sub(NewDiagonal, DB));
//	double alpha1 = Vect::GetAngle(Vect::Sub(DB, NewDiagonal));
	double alpha1 = Vect::GetAngle(uDB, udB);
///	double alpha2 = alpha1-SaveRotatAlpha1+SaveRotatAlpha2;


	double cosphi = cos(alpha1);
	double sinphi = sin(alpha1);
	
/*	sqrt(1.0-cosphi*cosphi);

	if ((uDB.x*udB.y-uDB.y*udB.x)>0.0){
		sinphi = - sinphi;
	}
*/

	double ax = AB.x*cosphi + AB.y*sinphi;
	double ay = -AB.x*sinphi + AB.y*cosphi;

	double cx = CB.x*cosphi + CB.y*sinphi;
	double cy = -CB.x*sinphi + CB.y*cosphi;
	
	Vect aB = Vect(ax, ay) ;
	Vect cB = Vect(cx, cy) ;
//	Vect dB = Vect(dx, dy) ;

	aB.Scale(AB.Length()/aB.Length());
	cB.Scale(CB.Length()/cB.Length());
//	dB.Scale(DB.Length()/dB.Length());

//	P[i3].SetPos( Vect::Add(B, dB).MakePoint());
	P[i3].SetPos( NewD.MakePoint());
	P[i0].SetPos( Vect::Add(B, aB).MakePoint());
	P[i2].SetPos( Vect::Add(B, cB).MakePoint());

	AdjustI2(i1, i2, i3, i0);

}


void DrawFilledGradientArrow(
					CDC * pDC,
					CPoint StartPoint,
					CPoint EndPoint,
					int rs, int gs, int bs,
					int re, int ge, int be,
					int WidthLine,
					int WidthArrow,
					int SizeTip){

	if (pDC==NULL){
		return;
	}
//	pDC->MoveTo(StartPoint);
//	pDC->LineTo(EndPoint);

//	Beep(1000,10);
	Vect StartVect = Vect(StartPoint);
	Vect EndVect = Vect(EndPoint);

	Vect LineVect = Vect::Sub(EndVect, StartVect);
	double L = LineVect.Length();
//	ShowFloat(L, "L");
	if (fabs(L)<1.1){
		return;
	}
	int x0p=0;
	int x1p=int (L-double(SizeTip));
	
	if (x1p<x0p){
		x1p=x0p+1;
	}

	int NRgn = 20;
	if (NRgn >= (L-1)){
		NRgn = L-1;
	}
	if (NRgn <1) {
		NRgn = 1;
	}

	int NRgnLeft = int (double(x1p)/double(L)*double(NRgn));
//	ShowFloat(NRgnLeft, "NRgnLeft");
	if (NRgnLeft<1){
		NRgnLeft =1;
	}
	int NRgnRight = NRgn-NRgnLeft;
	if (NRgnRight<1){
		NRgnRight = 1;
	}
//	ShowFloat(NRgnRight, "NRgnr");

	vector <int> Xvect;
	for (int i=0; i<=NRgnLeft; i++){
		Xvect.push_back( int (double(i)*double(x1p)/double(NRgnLeft)) );
//		ShowFloat(Xvect[i], "xi");
	}
	for (int i=0; i<=NRgnRight; i++){
		Xvect.push_back( x1p + int (double(i)*(L-double(x1p))/double(NRgnRight)));
//		ShowFloat(Xvect[i+NRgnLeft+1], "xi r");
	}

	pDC->MoveTo(Xvect[0]+StartPoint.x, StartPoint.y);
	for (int i=0; i<Xvect.size(); i++){
//		pDC->LineTo(Xvect[i]+StartPoint.x, StartPoint.y);
	}

	vector <VierEck> Regions;

	VierEck ve;
	ve.p[0]=CPoint(Xvect[0], WidthLine);
	ve.p[1]=CPoint(Xvect[0], -WidthLine);
	for (int i=0; i<NRgnLeft; i++){
		ve.p[2]=CPoint(Xvect[i+1], -WidthLine);
		ve.p[3]=CPoint(Xvect[i+1], WidthLine);
		Regions.push_back(ve);
		ve.p[0]=ve.p[3];
		ve.p[1]=ve.p[2];
	}

	ve.p[0]=CPoint(Xvect[NRgnLeft], WidthArrow);
	ve.p[1]=CPoint(Xvect[NRgnLeft], -WidthArrow);
	for (int i=0; i<=NRgnRight; i++){
		int x=Xvect[NRgnLeft+i+1];
		int dx=x-Xvect[NRgnLeft];
		int dy = int (double(dx)/(L-double(x1p))*double(WidthArrow));
		int y = WidthArrow - dy;
		//ShowFloat(x, "x");
		//ShowFloat(y, "y");
		ve.p[2]=CPoint(x, -y);
		ve.p[3]=CPoint(x, y);
		Regions.push_back(ve);
		ve.p[0]=ve.p[3];
		ve.p[1]=ve.p[2];
	}
 //   double alpha = Vect::GetAngle(EndVect, StartVect);
	Vect DiffVect = Vect::Sub(EndVect, StartVect);
	double alpha = -atan2(DiffVect.y, DiffVect.x);
	
//	alpha = 2.0*PIGRECO - alpha; 
//	ShowFloat(angle,  "angle");
	for (int i=0; i<Regions.size(); i++){
		VierEck & ve = Regions[i];
		ve.Rotate(alpha);
	}


	for (int i=0; i<Regions.size(); i++){
		VierEck & ve = Regions[i];
		for (int k=0; k<4; k++){
//			ShowPoint(ve.p[k]);

			ve.p[k].x += StartPoint.x;
			ve.p[k].y = StartPoint.y - ve.p[k].y;
//			ShowPoint(ve.p[k]);

		}
//		ve.DrawLine(pDC);
	}
	int N = Regions.size();

	for (int i=0; i<N; i++){
		int r = rs + int (double(i*(re-rs))/double(N));
		int g = gs + int (double(i*(ge-gs))/double(N));
		int b = bs + int (double(i*(be-bs))/double(N));
		CRgn rgn;
		rgn.CreatePolygonRgn(&Regions[i].p[0], 4, WINDING);
		CBrush brush;
		brush.CreateSolidBrush(RGB(r,g,b));
		pDC->FillRgn(&rgn, &brush);
	}
}
void ShowPoint ( CPoint p){
	char s[20];
	_itoa(p.x, s, 10);
	CString c="x: "+CString(s);
	_itoa(p.y, s, 10);
	c +="; y: "+CString(s);
	Alert0(c);

}
void VierEck::DrawLine(CDC * pDC){
	if (pDC == NULL){
		return;
	}

	pDC->MoveTo(p[0]);
	
	for (int k=1; k<=4; k++){
//		ShowPoint(p[k%4]);
		pDC->LineTo(p[k%4]);
	}

}

void VierEck::Rotate(double alpha){
	for (int i=0; i<4; i++){
		Vect v = Vect::RotateVect(Vect(p[i]), alpha);
		p[i] = Vect::MakePoint(v);
	}
}



Vect Vect::RotateVect(Vect v1, double alpha){
	double sina = sin(alpha);
	double cosa = cos(alpha);
	Vect v2;
	v2.x = int(  double(v1.x)*cosa  + double(v1.y)*sina);
	v2.y = int( double(v1.x)*sina   - double(v1.y)*cosa);
	return v2;
}
void Vect::RotateVects(vector<Vect> * vects, double alpha) {
}


double ArrayOfPolygons::CalcLocalizationCoefficient(int index, double * Background, float ** I1, float ** I2, int xsize, int ysize,
								   int offsetx, int offsety, int CalcType, double Threshold){
	if (index<0){
		return 0.0;
	}
	int NP=NPoly();
	if (NP<1){
		return 0.0;
	}
	if (index>=NP){
		return 0.0;
	}
	return polygons[index].CalcLocalizationCoefficient(
		Background, I1, I2, xsize, ysize, offsetx, offsety, CalcType, Threshold);

}
double PolygonObject::CalcLocalizationCoefficient(double * Background, float ** I1, float ** I2, int xsize, int ysize,
		int offsetx, int offsety, int CalcType, double Threshold){
	if (!I1){
		return 0.0;
	}
	if (!I2){
		return 0.0;
	}
	ImageType = 1; // float
	imageptr = (char *)I1;
	imageptr2 = (char *)I2;
	int NData=0;
	return DoCalcLocalizationCoefficient(Background, xsize, ysize, offsetx, offsety, NData, CalcType, Threshold);
}

double PolygonObject::DoCalcLocalizationCoefficient(double * Background, int xsize, int ysize,
					int offsetx, int offsety, int & NData, int CalcType, double Threshold){

	if (xsize<1) return 0.0;
	if (ysize<1) return 0.0;

	char * imgs[2];
	imgs[0]=imageptr;
	imgs[1]=imageptr2;
	NData = 0;
	double SumEach[2]; // Sum red, green
	double SumColoc[2]; // 
	for (int k=0; k<2; k++){
		SumEach[k]=0;
		SumColoc[k]=0;
	}

	for (int i=0; i<this->GetNumberOfDataPoints(); i++){
		CPoint p = this->DataPoints[i];
		int x=p.x+offsetx; 
		int y=p.y+offsety;
		if ((x>=0) && (x<xsize) && (y>=0) && (y<ysize)){
			double Values[2];
			for (int ii=0; ii<2; ii++){
				Values[ii] = double(GetImageValue(x, y, imgs[ii]))-Background[ii];
				SumEach[ii] += Values[ii];
			}
			if (Values[0]>Threshold){
				SumColoc[1] += Values[1];
			}
			if (Values[1]>Threshold){
				SumColoc[0] += Values[0];
			}

			NData++;
		}
	}
	if (NData<1) return 0.0;

	double SumSquare[2];
	double M[2]; // Manders
	double Mean[2];

	for (int k=0; k<2; k++){
		Mean[k] = SumEach[k]/double(NData);
		M[k]=SumColoc[k]/SumEach[k];
		SumSquare[k]=0;
	}
	double SumCross=0;

	for (int i=0; i<this->GetNumberOfDataPoints(); i++){
		double Values[2];
		CPoint p = this->DataPoints[i];
		int x=p.x+offsetx; 
		int y=p.y+offsety;
		if ((x>=0) && (x<xsize) && (y>=0) && (y<ysize)){
			for (int ii=0; ii<2; ii++){
				Values[ii] = double(GetImageValue(x, y, imgs[ii]))-Background[ii]-Mean[ii];
//				Values[ii] = double(GetImageValue(x, y, imgs[ii]))-Background[ii];
				SumSquare[ii] += Values[ii]*Values[ii];
			}
			SumCross += Values[0]*Values[1];
		}
	}
	double Pearson = SumCross/sqrt(SumSquare[0]*SumSquare[1]);

	if (CalcType == 0){
		//ShowFloat(Pearson, "P");
		return Pearson;
	}
	if (CalcType == 1){
		return M[0];
	}
	if (CalcType == 2){
		return M[1];
	}
	return 0.0;

}
