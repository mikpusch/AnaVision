#include "stdafx.h"
#include "../common/DrawObject.h"
#include <math.h>
#include "../common/utils.h"

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
Vect::Vect(CPoint p){
	x=p.x;
	y=p.y;
}

double Vect::GetAngle(Vect a, Vect b){
	double cosalpha = Dot(a, b)/Length(a)/Length(b);
//	ShowFloat(cosalpha, "cosalpha");
	double abscosalpha = fabs(cosalpha);

	double alpha = acos(abscosalpha);

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

CPoint ObjectPoint::GetPos(){
	return pos;
}
int ObjectPoint::Getx(){
	return pos.x;
}
int ObjectPoint::Gety(){
	return pos.y;
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
	return Vect(Getx(), Gety());
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

//	Define(0, CPoint(0,0),CPoint(0,0),CPoint(0,0),CPoint(0,0)); 
}
AreaObject::~AreaObject(){
/*
	delete redPen;
	delete bluePen;
	delete blackPen;
*/
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

	CPoint diff = ObjectPoint::DiffPoints(newpos, P[index].GetPos() );

	for (int i=0; i<NPOINTSAREAOBJECT; i++){
		P[i].AddVector(diff);
	}

}

void AreaObject::DrawObject(CDC * pDC, CRect & DrawRect){
	if (!pDC) return;

	switch (ObjectType){
		case 0 : DrawRectangle(pDC, DrawRect, 0, NULL); break;// Rectangle
		case 1 : DrawConus(pDC, DrawRect, 0, NULL); break;// Conus
		default :  DrawRectangle(pDC, DrawRect, 0, NULL); break;// Rectangle;
	}
}

void AreaObject::DrawObject(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos){
	if (!pDC) return;

	switch (ObjectType){
		case 0 : DrawRectangle(pDC, DrawRect, ncursors, curspos); break;// Rectangle
		case 1 : DrawConus(pDC, DrawRect, ncursors, curspos); break;// Conus
		default :  DrawRectangle(pDC, DrawRect, ncursors, curspos); break;// Rectangle;
	}
}

void AreaObject::DrawRectangle(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos){
	pDC->MoveTo(P[3].GetPos());
	int i;
	for (i=0; i<4; i++){
		pDC->LineTo(P[i].GetPos());
	}
	for (i=0; i<4; i++){
		int x1, x2, y1, y2;
		int x0=P[i].Getx();
		int y0=P[i].Gety();

		x1=x0-P[i].GetRadius();
		x2=x0+P[i].GetRadius();
		y1=y0-P[i].GetRadius();
		y2=y0+P[i].GetRadius();
		
		pDC->Ellipse(x1, y1, x2, y2);
	}
	int x0=(P[0].Getx()+P[1].Getx())/2;
	int y0=(P[0].Gety()+P[1].Gety())/2;

	pDC->MoveTo(x0, y0);

	int x1=(P[2].Getx()+P[3].Getx())/2;
	int y1=(P[2].Gety()+P[3].Gety())/2;

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
	CPoint A = P[0].GetPos();
	CPoint B = P[1].GetPos();
	CPoint C = P[2].GetPos();
	CPoint D = P[3].GetPos();

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

	double Bx=P[1].Getx();
	double By=P[1].Gety();
	double Cx=P[2].Getx();
	double Cy=P[2].Gety();

	M=Vect( int(Bx+lambda*(Bx-Cx)), int(By+lambda*(By-Cy)));


	Vect BM = Vect::Sub(B, M);

	r1 = BM.Length();

	Vect CM = Vect::Sub(C, M);

	r2 = CM.Length();

	return true;
}


void AreaObject::DrawConus(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos){

	AdjustConusIndexes();

	Vect M;
	double r1, r2;

	if (!GetMidPointAndRadius( M, r1, r2)){
		return;
	}

	CPoint A = P[0].GetPos();
	CPoint B = P[1].GetPos();
	CPoint C = P[2].GetPos();
	CPoint D = P[3].GetPos();

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
		int x0=P[i].Getx();
		int y0=P[i].Gety();

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

	if (P[index].Getx() == newpos.x ){
		if (P[index].Gety() == newpos.y ){
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
			if (newpos.x==P[jj].Getx()){
				if (newpos.y==P[jj].Gety()){
					return;
				}
			}
//		}
	}

	Vect A = Vect(P[i0].GetPos());
	Vect B = Vect(P[i1].GetPos());
	Vect C = Vect(P[i2].GetPos());
	Vect D = Vect(P[i3].GetPos());
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

	Vect A = Vect(P[i1].GetPos());
	Vect E = Vect(P[i3].GetPos());
	Vect C = Vect(P[i2].GetPos());

	Vect M = Vect::Add(A, E);
	M.Scale(0.5);

	Vect AM = Vect::Sub(A, M);
	Vect CM = Vect::Sub(C, M);

	CM.Scale(AM.Length()/CM.Length());
	P[i2].SetPos(Vect::Add(M, CM).MakePoint());

	AdjustI0(i1, i2,i3, i0);
}

void AreaObject::AdjustI0(int i1, int i2, int i3, int i0){

	Vect A = Vect(P[i1].GetPos());
	Vect E = Vect(P[i3].GetPos());
	Vect C = Vect(P[i2].GetPos());

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


	Vect A = Vect(P[iA].GetPos()); 
	Vect B = Vect(P[iB].GetPos()); 
	Vect C = Vect(P[iC].GetPos()); 
	Vect D = Vect(P[iD].GetPos()); 
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


	Vect A = Vect(P[iA].GetPos()); 
	Vect B = Vect(P[iB].GetPos()); 
	Vect C = Vect(P[iC].GetPos()); 
	Vect D = Vect(P[iD].GetPos()); 
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

	NewAdjustCone(P[iA].GetPos(), P[iB].GetPos(), P[iC].GetPos(), P[iD].GetPos(), pa, pb, pc, pd);

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

AdjustMidPoints(CPoint & pd, CPoint pC, CPoint pA, CPoint pB){
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
			if (newpos.x==P[jj].Getx()){
				if (newpos.y==P[jj].Gety()){
					return;
				}
			}
		}
	}

	Vect A = Vect(P[i0].GetPos());
	Vect B = Vect(P[i1].GetPos());
	Vect C = Vect(P[i2].GetPos());
	Vect D = Vect(P[i3].GetPos());
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
	Vect A = Vect(P[i0].GetPos());
	Vect B = Vect(P[i1].GetPos());
	Vect C = Vect(P[i2].GetPos());
	Vect D = Vect(P[i3].GetPos());

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

	Vect A = Vect(P[0].GetPos());
	Vect B = Vect(P[1].GetPos());
	Vect C = Vect(P[2].GetPos());
	Vect D = Vect(P[3].GetPos());

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

	if (P[index].Getx() == newpos.x ){
		if (P[index].Gety() == newpos.y ){
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
	Vect A = Vect(P[0].GetPos());
	Vect C = Vect(P[2].GetPos());
	Vect M = Vect::Add(A, C);
	M.Scale(0.5);
	return M;
}

void AreaObject::RotateRectangle(int index, CPoint newpos){
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

	Vect A = Vect(P[i0].GetPos());
	Vect B = Vect(P[i1].GetPos());
	Vect C = Vect(P[i2].GetPos());
	Vect D = Vect(P[i3].GetPos());
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
			if (newpos.x==P[jj].Getx()){
				if (newpos.y==P[jj].Gety()){
					return;
				}
			}
		}
	}

	Vect A = Vect(P[i0].GetPos());
	Vect B = Vect(P[i1].GetPos());
	Vect C = Vect(P[i2].GetPos());
	Vect D = Vect(P[i3].GetPos());
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

//	SizeConus(i1, P[i1].GetPos());
	SizeConus(i3, P[i3].GetPos());


}


double AreaObject::Length(){
	switch (ObjectType){
		case 0 : return LengthRectangle(); break;// Rectangle
		case 1 : return LengthConus(); break;// Conus
		default :  return LengthRectangle(); break;// Rectangle;
	}
	
}
double AreaObject::LengthRectangle(){
	Vect A = Vect(P[0].GetPos());
	Vect D = Vect(P[3].GetPos());
	Vect diff = Vect::Sub(A, D);
	return diff.Length();

}
double AreaObject::LengthConus(){
	Vect A = Vect(P[0].GetPos());
	Vect D = Vect(P[3].GetPos());
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
	Vect A = Vect(P[0].GetPos());
	Vect D = Vect(P[3].GetPos());
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
	Vect A = Vect(P[0].GetPos());
	Vect D = Vect(P[3].GetPos());
	Vect DA = Vect::Sub(D, A);
	double lDA = DA.Length();
	Vect eDA = DA;
	eDA.Normalize();

	Vect M = GetMidPoint();

	Vect pM = Vect::Sub(p, M);

	if (fabs(Vect::Dot(pM, eDA)*2.0)>lDA){
		return false;
	}

	Vect B = Vect(P[1].GetPos());
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

	Vect A = Vect(P[0].GetPos());
	Vect B = Vect(P[1].GetPos());

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

bool AreaObject::GeneratePoints(bool & initialize, CPoint & p, int & index){
	switch (ObjectType){
		case 0 : return GeneratePointsRectangle(initialize, p, index);// Rectangle
		case 1 : return GeneratePointsConus(initialize, p, index);// Conus
		default :  return GeneratePointsRectangle(initialize, p, index); // Rectangle;
	}


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
		A = Vect(P[0].GetPos());
		Vect D = Vect(P[3].GetPos());
		Vect DA = Vect::Sub(D, A);
		lDA = DA.Length()/2.0;
		eDA = DA;
		eDA.Normalize();

		M = GetMidPoint();
		Vect B = Vect(P[1].GetPos());
		Vect AB = Vect::Sub(B, A);
		lAB = AB.Length()/2.0;
		eAB = AB;
		eAB.Normalize();

		int Minx=P[0].Getx();
		int Maxx=Minx;
		int Miny=P[0].Gety();
		int Maxy=Miny;
		for (int i=1; i<4; i++){
			int x=P[i].Getx();
			int y=P[i].Gety();
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

		Vect A = Vect(P[0].GetPos());
		Vect B = Vect(P[1].GetPos());

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

	Vect p = Vect( SaveRotatePoint.Getx(),  SaveRotatePoint.Gety());
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
